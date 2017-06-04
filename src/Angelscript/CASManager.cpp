#include <iostream>

#include "angelscript.h"

#include "CASEngineInstance.h"
#include "IASEventListener.h"
#include "IASCompilerListener.h"
#include "IConfigurationManager.h"
#include "CConfiguration.h"
#include "CScript.h"

#include "CASManager.h"

CASManager::CASManager( std::shared_ptr<IConfigurationManager> configurationManager )
	: m_ConfigurationManager( configurationManager )
{
	m_ConfigurationManager->AddConfigurationEventListener( this );
}

CASManager::~CASManager()
{
	m_ConfigurationManager->RemoveConfigurationEventListener( this );
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

bool CASManager::CompileScript( const std::string& szSectionName, const std::string& szScriptContents )
{
	auto script = std::make_shared<const CScript>( std::string( szSectionName ), std::string( szScriptContents ), m_ConfigurationManager->GetActiveConfiguration() );

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

void CASManager::ConfigEventOccurred( const ConfigEvent& event )
{
	switch( event.type )
	{
	case ConfigEventType::CHANGE:
		{
			ActiveConfigSet( event.change.pNewConfig );
			break;
		}
	}
}

void CASManager::ActiveConfigSet( const CConfiguration* pConfig )
{
	if( m_Instance )
	{
		ASEvent destroyEvent( ASEventType::DESTROYED );

		NotifyEventListeners( destroyEvent );
		m_Instance.reset();
	}

	try
	{
		m_Instance = std::make_unique<CASEngineInstance>();

		m_Instance->SetMessageCallback( asMETHOD( CASManager, MessageCallback ), this, asCALL_THISCALL );

		const std::string szVersion = m_Instance->GetVersion();

		{
			ASEvent event( ASEventType::CREATED );

			event.create.pszVersion = &szVersion;
			event.create.bHasConfig = pConfig != nullptr;

			NotifyEventListeners( event );
		}


		if( pConfig )
		{
			ASEvent configEvent( ASEventType::CONFIG_CHANGE );

			configEvent.configChange.changeType = ASConfigChangeType::SET;
			configEvent.configChange.pszName = &pConfig->GetName();

			NotifyEventListeners( configEvent );

			ASEvent regEvent( ASEventType::API_REGISTERED );

			regEvent.apiRegistration.pszConfigFilename = &pConfig->GetConfigFilename();
			regEvent.apiRegistration.bSuccess = m_Instance->LoadAPIFromFile( pConfig->GetConfigFilename() );

			NotifyEventListeners( regEvent );
		}
		else
		{
			ASEvent configEvent( ASEventType::CONFIG_CHANGE );

			configEvent.configChange.changeType = ASConfigChangeType::CLEARED;
			std::string szName;
			configEvent.configChange.pszName = &szName;

			NotifyEventListeners( configEvent );
		}
	}
	catch( const CASEngineException& e )
	{
		std::cerr << e.what() << std::endl;
	}
}
