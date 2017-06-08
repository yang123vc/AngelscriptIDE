#include <algorithm>
#include <cstdarg>
#include <fstream>
#include <iostream>

#include <QDir>
#include <QFileInfo>

#include <angelscript.h>

#include <AngelscriptUtils/add_on/scriptbuilder.h>
#include <AngelscriptUtils/add_on/scripthelper.h>

#include "CASEngineInstance.h"
#include "CConfiguration.h"
#include "IncludeCallback.h"

CASEngineInstance::CASEngineInstance()
{
	m_pScriptEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );

	if( !m_pScriptEngine )
		throw CASEngineException( "Couldn't create Angelscript engine" );

	m_pScriptEngine->SetMessageCallback( asMETHOD( CASEngineInstance, MessageCallback ), this, asCALL_THISCALL );
}

CASEngineInstance::~CASEngineInstance()
{
	if( m_pScriptEngine )
		m_pScriptEngine->ShutDownAndRelease();
}

const char* CASEngineInstance::GetVersion() const
{
	return ANGELSCRIPT_VERSION_STRING;
}

bool CASEngineInstance::LoadAPIFromFile( const std::string& szFilename )
{
	std::ifstream inStream( szFilename );

	if( !inStream.is_open() )
		return false;

	return ConfigEngineFromStream( GetScriptEngine(), inStream ) >= 0;
}

bool CASEngineInstance::CompileScript( const std::string& szScriptFilename, const std::shared_ptr<const CConfiguration>& config )
{
	CScriptBuilder builder;

	struct CallbackData_t
	{
		const decltype( config )& config;
		CASEngineInstance* const pEngineInstance;
	};

	CallbackData_t data{ config, this };

	builder.SetIncludeCallback(
	[]( const char* pszInclude, const char* pszFrom, CScriptBuilder* pBuilder, void* pUserParam ) -> int
	{
		const auto& data = *reinterpret_cast<CallbackData_t*>( pUserParam );

		//Should never happen
		if( !pUserParam )
		{
			data.pEngineInstance->Error( "Compiler", 0, 0, "CScriptBuilder include callback user parameter is null!\n" );
			return -1;
		}

		return FindIncludedFile( *pBuilder, data.config, pszInclude, pszFrom );
	}
	, &data );

	bool bSuccess = builder.StartNewModule( m_pScriptEngine, "ScriptModule" ) >= 0;

	if( bSuccess )
	{
		if( config )
		{
			for( const auto& szWord : config->GetWords() )
				builder.DefineWord( szWord.c_str() );

			const auto& szIncludeFilename = config->GetIncludeFilename();

			if( !szIncludeFilename.empty() )
			{
				bSuccess = builder.AddSectionFromFile( szIncludeFilename.c_str() ) >= 0;
			}
		}
	}
	else
		Error( "Compiler", 0, 0, "Failed to create module!\n" );

	if( bSuccess )
	{
		const auto iResult = builder.AddSectionFromFile( szScriptFilename.c_str() );

		bSuccess = iResult >= 0;
	}
	else
		Error( "Compiler", 0, 0, "Failed to initialize builder!\n" );

	if( bSuccess )
		bSuccess = builder.BuildModule() >= 0;
	else
		Error( "Compiler", 0, 0, "Failed to add code to builder!\n" );

	asIScriptModule* pModule = builder.GetModule();

	pModule->Discard();

	//Clean up anything that isn't needed
	m_pScriptEngine->GarbageCollect();

	return bSuccess;
}

void CASEngineInstance::MessageCallback( asSMessageInfo* pMsg )
{
	CMessageInfo message;

	message.bHasSection = pMsg->section != nullptr;

	if( pMsg->section )
		message.section = pMsg->section;

	message.row = pMsg->row;
	message.col = pMsg->col;

	message.type = pMsg->type;

	message.bHasMessage = pMsg->message != nullptr;

	if( pMsg->message )
		message.message = pMsg->message;

	EngineMessage( message );
}

void CASEngineInstance::Error( const char* pszSection, int row, int col, const char* pszFormat, ... )
{
	QString szMessage;

	va_list va;

	va_start( va, pszFormat );

	szMessage.vasprintf( pszFormat, va );

	va_end( va );

	m_pScriptEngine->WriteMessage( pszSection, row, col, asMSGTYPE_ERROR, qPrintable( szMessage ) );
}
