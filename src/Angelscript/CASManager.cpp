#include <iostream>

#include "angelscript.h"

#include "CASEngineInstance.h"
#include "IASEventListener.h"
#include "IASCompilerListener.h"
#include "CConfiguration.h"
#include "CScript.h"

#include "CASManager.h"

CASManager::CASManager( std::shared_ptr<IConfigurationManager> configurationManager )
	: m_ConfigurationManager( configurationManager )
{
}

CASManager::~CASManager()
{
}

void CASManager::AddEventListener( IASEventListener* pListener )
{
	m_EventListeners.AddListener( pListener );
}

void CASManager::RemoveEventListener( IASEventListener* pListener )
{
	m_EventListeners.RemoveListener( pListener );
}

void CASManager::NotifyEventListeners( ASEvent event, const void* pArg )
{
	m_EventListeners.NotifyListeners( &IASEventListener::AngelscriptEventOccured, event, pArg );
}

void CASManager::AddCompilerListener( IASCompilerListener* pListener )
{
	m_CompilerListeners.AddListener( pListener );
}

void CASManager::RemoveCompilerListener( IASCompilerListener* pListener )
{
	m_CompilerListeners.RemoveListener( pListener );
}

void CASManager::MessageCallback( const asSMessageInfo* pMsg )
{
	m_CompilerListeners.NotifyListeners( &IASCompilerListener::CompilerMessage, pMsg );
}

void CASManager::SetActiveConfiguration( const std::string& szName, bool fSaveOldConfig )
{
	if( m_Instance )
		ClearActiveConfiguration( fSaveOldConfig );

	try
	{
		m_Instance = std::make_unique<CASEngineInstance>();

		m_Instance->SetMessageCallback( asMETHOD( CASManager, MessageCallback ), this, asCALL_THISCALL );

		const std::string szVersion = m_Instance->GetVersion();

		const ASCreationResult creationResult = { szVersion, !szName.empty() };

		NotifyEventListeners( ASEvent::CREATED, &creationResult );

		if( creationResult.fHasConfig )
		{
			auto config = CConfiguration::Load( GetConfigurationManager(), szName );

			if( config )
			{
				m_ActiveConfiguration = config;

				NotifyEventListeners( ASEvent::CONFIG_SET, &szName );

				const ASAPIRegistrationResult registrationResult = { config->GetConfigFilename(), m_Instance->LoadAPIFromFile( config->GetConfigFilename() ) };

				NotifyEventListeners( ASEvent::API_REGISTERED, &registrationResult );
			}
			else
				NotifyEventListeners( ASEvent::CONFIG_NOT_FOUND, &szName );
		}
	}
	catch( const CASEngineException& e )
	{
		std::cerr << e.what() << std::endl;
	}
}

void CASManager::ClearActiveConfiguration( bool fSave )
{
	if( m_Instance )
	{
		NotifyEventListeners( ASEvent::DESTROYED );
		m_Instance.reset();

		if( m_ActiveConfiguration )
		{
			if( fSave )
				m_ActiveConfiguration->Save();

			m_ActiveConfiguration.reset();
		}

		NotifyEventListeners( ASEvent::CONFIG_CLEARED );
	}
}

void CASManager::ReloadActiveConfiguration()
{
	if( m_ActiveConfiguration )
		SetActiveConfiguration( m_ActiveConfiguration->GetName() );
}

bool CASManager::CompileScript( const std::string& szSectionName, const std::string& szScriptContents )
{
	auto script = std::make_shared<const CScript>( szSectionName, szScriptContents, m_ActiveConfiguration );

	NotifyEventListeners( ASEvent::COMPILATION_STARTED, &script );
	const bool fResult = m_Instance->CompileScript( script );
	NotifyEventListeners( ASEvent::COMPILATION_ENDED, &fResult );

	return fResult;
}
