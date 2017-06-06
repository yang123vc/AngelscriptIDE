#include <cassert>
#include <iostream>
#include <stdexcept>

#include "angelscript.h"

#include <AngelscriptUtils/CASModule.h>
#include <AngelscriptUtils/wrapper/ASCallable.h>

#include "CASEngineInstance.h"
#include "IASEventListener.h"
#include "IASCompilerListener.h"
#include "CConfiguration.h"
#include "CConfigurationManager.h"
#include "CScript.h"
#include "IDE_API.h"

#include "CASConfigModuleBuilder.h"
#include "CASManager.h"
#include "AngelscriptUtils/add_on/scriptbuilder.h"
#include "ScriptAPI/ASIConfiguration.h"

CASManager::CASManager( std::shared_ptr<CConfigurationManager> configurationManager )
	: m_ConfigurationManager( configurationManager )
{
	m_ConfigurationManager->AddConfigurationEventListener( this );
	m_pIDEEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );

	if( !m_pIDEEngine )
		throw std::runtime_error( "Couldn't create IDE script engine" );

	m_pIDEEngine->SetMessageCallback( asMETHOD( CASManager, MessageCallback ), this, asCALL_THISCALL );

	RegisterIDE_API( *m_pIDEEngine );

	m_Context.Set( m_pIDEEngine->CreateContext(), true );

	m_ModuleManager = std::make_unique<CASModuleManager>( *m_pIDEEngine );

	m_ModuleManager->AddDescriptor( "Config", 0xFFFFFFFF );
}

CASManager::~CASManager()
{
	m_ConfigurationManager->RemoveConfigurationEventListener( this );
	ClearConfigurationScript();

	if( m_ModuleManager )
	{
		m_ModuleManager->Clear();
		m_ModuleManager.reset();
	}

	if( m_Context )
	{
		m_Context.Reset();
	}

	if( m_pIDEEngine )
		m_pIDEEngine->ShutDownAndRelease();
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

	ClearConfigurationScript();

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

			//TODO: need to pass the smart pointer directly, needs redesign to Qt signals - Solokiller
			auto config = m_ConfigurationManager->Find( pConfig->GetName() );

			if( config )
			{
				CASConfigModuleBuilder builder( config, m_Context );

				m_pConfigModule = m_ModuleManager->BuildModule( "Config", "Config", builder );

				if( m_pConfigModule )
				{
					m_pConfigModule->AddRef();
				}

				m_ConfigurationObject = builder.GetConfigurationObject();
			}
			else
			{
				std::cerr << "Couldn't find config" << "\"" << config->GetName() << "\"" << std::endl;
			}

			if( m_ConfigurationObject )
			{
				std::cout << "Configuration object \"" <<
					m_ConfigurationObject.GetTypeInfo()->GetNamespace() << "::" << m_ConfigurationObject.GetTypeInfo()->GetName() <<
					"\" found" << std::endl;

				//Configure the engine
				auto pFunction = m_ConfigurationObject.GetTypeInfo()->GetMethodByDecl( "void ConfigureEngine(asIScriptEngine@ pScriptEngine)" );

				if( pFunction )
					as::Call( m_ConfigurationObject.Get(), m_Context, pFunction, m_Instance->GetScriptEngine() );
			}

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

void CASManager::ClearConfigurationScript()
{
	if( m_ConfigurationObject )
	{
		m_ConfigurationObject.Reset();
	}

	if( m_pConfigModule )
	{
		m_ModuleManager->RemoveModule( m_pConfigModule );
		m_pConfigModule->Release();
		m_pConfigModule = nullptr;
	}
}
