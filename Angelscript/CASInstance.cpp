#include "angelscript.h"

#include <iostream>
#include <algorithm>
#include <fstream>

#include <QFileInfo>

#include "IASCompilerListener.h"

#include "add_on/scriptbuilder.h"
#include "add_on/scripthelper.h"

#include "CScript.h"
#include "CScriptFile.h"
#include "CConfiguration.h"

#include "CASInstance.h"

CASInstance::CASInstance()
	: m_pScriptEngine( nullptr )
{
}

CASInstance::~CASInstance()
{
}

const char* CASInstance::GetVersion() const
{
	return ANGELSCRIPT_VERSION_STRING;
}

void CASInstance::SetMessageCallback( const asSFuncPtr& callback, void* pObj, asDWORD callConv )
{
	if( !IsInitialized() )
		return;

	m_pScriptEngine->SetMessageCallback( callback, pObj, callConv );
}

CASInstance::StartupResult_t CASInstance::Startup()
{
	if( IsInitialized() )
		return STARTUP_ALREADYINITIALIZED;

	m_pScriptEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );

	if( !m_pScriptEngine )
		return STARTUP_FAILED;

	return STARTUP_SUCCESS;
}

void CASInstance::Shutdown()
{
	if( !IsInitialized() )
		return;

	m_pScriptEngine->Release();

	m_pScriptEngine = nullptr;
}

bool CASInstance::LoadAPIFromFile( const std::string& szFilename )
{
	if( !IsInitialized() )
		return false;

	std::ifstream inStream( szFilename );

	if( !inStream.is_open() )
		return false;

	return ConfigEngineFromStream( GetScriptEngine(), inStream ) >= 0;
}

bool CASInstance::CompileScript( std::shared_ptr<const CScript> script )
{
	if( !IsInitialized() )
	{
		std::cerr << "Instance not initialized!" << std::endl;
		return false;
	}

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

		std::string szPath;

		const QString szBasePath = QFileInfo( pszFrom ).canonicalPath();

		for( const auto& path : config->GetIncludePaths() )
		{
			//If it starts with a '.', it's a relative path
			szPath = path.find( '.' ) == 0 ? QFileInfo( QString( "%1/%2" ).arg( szBasePath, path.c_str() ) ).path().toStdString() : path;

			std::string szCompletePath = szPath + '/' + pszInclude;

			QFileInfo includedFile( pszInclude );

			if( includedFile.completeSuffix().isEmpty() )
				szCompletePath += config->GetFallbackExtension();

			if( pBuilder->AddSectionFromFile( ( szCompletePath ).c_str() ) >= 0 )
				return 1;
		}

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
				auto include = std::make_shared<CScriptFile>( szIncludeFilename );

				auto includeContents = include->GetScriptContents();

				if( includeContents )
					fSuccess = builder.AddSectionFromMemory( "Include", includeContents->c_str() ) >= 0;
			}
		}
	}
	else
		std::cerr << "Failed to create module!" << std::endl;

	if( fSuccess )
		fSuccess = builder.AddSectionFromMemory( script->GetSectionName().c_str(), script->GetContents().c_str() ) >= 0;
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
