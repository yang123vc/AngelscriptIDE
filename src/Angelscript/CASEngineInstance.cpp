#include "angelscript.h"

#include <iostream>
#include <algorithm>
#include <fstream>

#include <QDir>
#include <QFileInfo>

#include <AngelscriptUtils/add_on/scriptbuilder.h>
#include <AngelscriptUtils/add_on/scripthelper.h>

#include "CASEngineInstance.h"
#include "CConfiguration.h"
#include "CScript.h"
#include "IncludeCallback.h"

CASEngineInstance::CASEngineInstance()
{
	m_pScriptEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );

	if( !m_pScriptEngine )
		throw CASEngineException( "Couldn't create Angelscript engine" );
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

void CASEngineInstance::SetMessageCallback( const asSFuncPtr& callback, void* pObj, asDWORD callConv )
{
	m_pScriptEngine->SetMessageCallback( callback, pObj, callConv );
}

bool CASEngineInstance::LoadAPIFromFile( const std::string& szFilename )
{
	std::ifstream inStream( szFilename );

	if( !inStream.is_open() )
		return false;

	return ConfigEngineFromStream( GetScriptEngine(), inStream ) >= 0;
}

bool CASEngineInstance::CompileScript( const std::shared_ptr<const CScript>& script, const std::shared_ptr<const CConfiguration>& config )
{
	CScriptBuilder builder;

	struct CallbackData_t
	{
		const decltype( config )& config;
	};

	CallbackData_t data{ config };

	builder.SetIncludeCallback(
	[]( const char* pszInclude, const char* pszFrom, CScriptBuilder* pBuilder, void* pUserParam ) -> int
	{
		//Should never happen
		if( !pUserParam )
		{
			std::cerr << "CScriptBuilder include callback user parameter is null!" << std::endl;
			return -1;
		}

		const auto& data = *reinterpret_cast<CallbackData_t*>( pUserParam );

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
				auto szContents = CScript::LoadContentsFromFile( szIncludeFilename, bSuccess );

				if( bSuccess )
					bSuccess = builder.AddSectionFromMemory( "Include", szContents.c_str() ) >= 0;
			}
		}
	}
	else
		std::cerr << "Failed to create module!" << std::endl;

	if( bSuccess )
	{
		const auto iResult = builder.AddSectionFromMemory( script->GetSectionName().c_str(), script->GetContents().c_str() );

		bSuccess = iResult >= 0;
	}
	else
		std::cerr << "Failed to initialize builder!" << std::endl;

	if( bSuccess )
		bSuccess = builder.BuildModule() >= 0;
	else
		std::cerr << "Failed to add code to builder!" << std::endl;

	asIScriptModule* pModule = builder.GetModule();

	pModule->Discard();

	//Clean up anything that isn't needed
	m_pScriptEngine->GarbageCollect();

	return bSuccess;
}
