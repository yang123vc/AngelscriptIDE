#include "angelscript.h"

#include "CASInstance.h"
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

	m_Instance.reset( new CASInstance() );

	CASInstance::StartupResult_t result = m_Instance->Startup();

	if( result == CASInstance::STARTUP_SUCCESS )
	{
		m_Instance->SetMessageCallback( asMETHOD( CASManager, MessageCallback ), this, asCALL_THISCALL );

		const std::string szVersion = m_Instance->GetVersion();

		const ASCreationResult creationResult = { szVersion, !szName.empty() };

		NotifyEventListeners( ASEvent::Created, &creationResult );

		if( creationResult.fHasConfig )
		{
			auto config = CConfiguration::Load( GetConfigurationManager(), szName );

			if( config )
			{
				m_ActiveConfiguration = config;

				NotifyEventListeners( ASEvent::ConfigurationSet, &szName );

				const ASAPIRegistrationResult registrationResult = { config->GetConfigFilename(), m_Instance->LoadAPIFromFile( config->GetConfigFilename() ) };

				NotifyEventListeners( ASEvent::APIRegistered, &registrationResult );
			}
			else
				NotifyEventListeners( ASEvent::ConfigurationNotFound, &szName );
		}
	}
}

void CASManager::ClearActiveConfiguration( bool fSave )
{
	if( m_Instance )
	{
		NotifyEventListeners( ASEvent::Destroyed );
		m_Instance->Shutdown();
		m_Instance.reset();

		if( m_ActiveConfiguration )
		{
			if( fSave )
				m_ActiveConfiguration->Save();

			m_ActiveConfiguration.reset();
		}

		NotifyEventListeners( ASEvent::ConfigurationCleared );
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

	NotifyEventListeners( ASEvent::CompilationStarted, &script );
	const bool fResult = m_Instance->CompileScript( script );
	NotifyEventListeners( ASEvent::CompilationEnded, &fResult );

	return fResult;
}
