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

void CASManager::NotifyEventListeners( const ASEvent& event )
{
	m_EventListeners.NotifyListeners( &IASEventListener::AngelscriptEventOccured, event );
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

void CASManager::SetActiveConfiguration( const std::string& szName )
{
	if( m_Instance )
		ClearActiveConfiguration();

	try
	{
		m_Instance = std::make_unique<CASEngineInstance>();

		m_Instance->SetMessageCallback( asMETHOD( CASManager, MessageCallback ), this, asCALL_THISCALL );

		const std::string szVersion = m_Instance->GetVersion();

		{
			ASEvent event( ASEventType::CREATED );

			event.create.pszVersion = &szVersion;
			event.create.bHasConfig = !szName.empty();

			NotifyEventListeners( event );
		}


		if( !szName.empty() )
		{
			auto config = CConfiguration::Load( GetConfigurationManager(), szName );

			if( config )
			{
				m_ActiveConfiguration = config;
			}

			ASEvent configEvent( ASEventType::CONFIG_CHANGE );

			configEvent.configChange.changeType = config ? ASConfigChangeType::SET : ASConfigChangeType::FAILED_TO_LOAD;
			configEvent.configChange.pszName = &szName;

			NotifyEventListeners( configEvent );

			if( config )
			{
				ASEvent regEvent( ASEventType::API_REGISTERED );

				regEvent.apiRegistration.pszConfigFilename = &config->GetConfigFilename();
				regEvent.apiRegistration.bSuccess = m_Instance->LoadAPIFromFile( config->GetConfigFilename() );

				NotifyEventListeners( regEvent );
			}
		}
	}
	catch( const CASEngineException& e )
	{
		std::cerr << e.what() << std::endl;
	}
}

void CASManager::ClearActiveConfiguration()
{
	if( m_Instance )
	{
		ASEvent destroyEvent( ASEventType::DESTROYED );

		NotifyEventListeners( destroyEvent );
		m_Instance.reset();

		if( m_ActiveConfiguration )
		{
			m_ActiveConfiguration.reset();
		}

		ASEvent configEvent( ASEventType::CONFIG_CHANGE );

		configEvent.configChange.changeType = ASConfigChangeType::CLEARED;
		std::string szName;
		configEvent.configChange.pszName = &szName;

		NotifyEventListeners( configEvent );
	}
}

void CASManager::ReloadActiveConfiguration()
{
	if( m_ActiveConfiguration )
		SetActiveConfiguration( m_ActiveConfiguration->GetName() );
}

bool CASManager::CompileScript( const std::string& szSectionName, const std::string& szScriptContents )
{
	auto script = std::make_shared<const CScript>( std::string( szSectionName ), std::string( szScriptContents ), m_ActiveConfiguration );

	ASEvent startEvent( ASEventType::COMPILATION_STARTED );

	startEvent.compilationStart.pScript = script.get();

	NotifyEventListeners( startEvent );
	const bool bResult = m_Instance->CompileScript( script );

	ASEvent endEvent( ASEventType::COMPILATION_ENDED );

	endEvent.compilationEnd.pScript = script.get();
	endEvent.compilationEnd.bSuccess = bResult;

	NotifyEventListeners( endEvent );

	return bResult;
}
