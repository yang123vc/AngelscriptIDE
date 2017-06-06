#include "angelscript.h"

#include <iostream>
#include <algorithm>
#include <fstream>

#include <QDir>
#include <QFileInfo>

#include "AngelscriptUtils/add_on/scriptbuilder.h"
#include "AngelscriptUtils/add_on/scripthelper.h"

#include "CScript.h"
#include "CConfiguration.h"

#include "CASEngineInstance.h"

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

bool CASEngineInstance::CompileScript( std::shared_ptr<const CScript> script )
{
	CScriptBuilder builder;

	builder.SetIncludeCallback(
	[]( const char* pszInclude, const char* pszFrom, CScriptBuilder* pBuilder, void* pUserParam ) -> int
	{
		//Should never happen
		if( !pUserParam )
		{
			std::cerr << "CScriptBuilder include callback user parameter is null!" << std::endl;
			return -1;
		}

		auto script = *reinterpret_cast<std::shared_ptr<const CScript>*>( pUserParam );

		auto config = script->GetConfiguration();

		if( !config )
		{
			pBuilder->GetModule()->GetEngine()->WriteMessage(
						pszFrom, -1, -1, asMSGTYPE_ERROR, "No configuration file specified, cannot resolve include files" );
			return -1;
		}

		const QString szInclude( pszInclude );

		auto tryLoad = [ & ]( const QDir& dir ) -> bool
		{
			QFileInfo includedFile( dir.filePath( szInclude ) );

			if( includedFile.completeSuffix().isEmpty() )
				includedFile = dir.filePath( szInclude + config->GetFallbackExtension().c_str() );

			if( !includedFile.exists() )
				return false;

			return pBuilder->AddSectionFromFile( includedFile.absoluteFilePath().toStdString().c_str() ) >= 0;
		};

		//First try a relative path
		if( tryLoad( QDir( QFileInfo( pszFrom ).path() ) ) )
			return 1;

		std::string szPath;

		for( const auto& path : config->GetIncludePaths() )
		{
			if( tryLoad( QDir( path.c_str() ) ) )
				return 1;
		}

		QString szMessage = QString( "Couldn't find file \"%1\", included from \"%2\"" ).arg( pszInclude, pszFrom );

		pBuilder->GetModule()->GetEngine()->WriteMessage(
			pszFrom, -1, -1, asMSGTYPE_ERROR, szMessage.toStdString().c_str() );

		return -1;
	}
	, &script );

	bool fSuccess = builder.StartNewModule( m_pScriptEngine, "ScriptModule" ) >= 0;

	if( fSuccess )
	{
		auto config = script->GetConfiguration();

		if( config )
		{
			for( const auto& szWord : config->GetWords() )
				builder.DefineWord( szWord.c_str() );

			const auto& szIncludeFilename = config->GetIncludeFilename();

			if( !szIncludeFilename.empty() )
			{
				bool bSuccess;

				auto szContents = CScript::LoadContentsFromFile( szIncludeFilename, bSuccess );

				if( bSuccess )
					fSuccess = builder.AddSectionFromMemory( "Include", szContents.c_str() ) >= 0;
			}
		}
	}
	else
		std::cerr << "Failed to create module!" << std::endl;

	if( fSuccess )
	{
		const auto iResult = builder.AddSectionFromMemory( script->GetSectionName().c_str(), script->GetContents().c_str() );

		fSuccess = iResult >= 0;
	}
	else
		std::cerr << "Failed to initialize builder!" << std::endl;

	if( fSuccess )
		fSuccess = builder.BuildModule() >= 0;
	else
		std::cerr << "Failed to add code to builder!" << std::endl;

	asIScriptModule* pModule = builder.GetModule();

	pModule->Discard();

	//Clean up anything that isn't needed
	m_pScriptEngine->GarbageCollect();

	return fSuccess;
}
