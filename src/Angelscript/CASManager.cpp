#include <cassert>
#include <iostream>
#include <stdexcept>

#include "angelscript.h"

#include <AngelscriptUtils/CASModule.h>
#include <AngelscriptUtils/wrapper/ASCallable.h>

#include "CASEngineInstance.h"
#include "IASEventListener.h"
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
	connect( m_ConfigurationManager.get(), &CConfigurationManager::ActiveConfigurationChanged, this, &CASManager::OnActiveConfigurationChanged );

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

void CASManager::MessageCallback( const asSMessageInfo* pMsg )
{
	CompilerMessage( *pMsg );
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

void CASManager::ActiveConfigSet( const std::shared_ptr<CConfiguration>& config )
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
			event.create.bHasConfig = config != nullptr;

			NotifyEventListeners( event );
		}

		if( config )
		{
			ASEvent configEvent( ASEventType::CONFIG_CHANGE );

			configEvent.configChange.changeType = ASConfigChangeType::SET;
			configEvent.configChange.pszName = &config->GetName();

			NotifyEventListeners( configEvent );

			if( !config->GetConfigScriptFilename().empty() )
			{
				CASConfigModuleBuilder builder( config, m_Context );

				m_pConfigModule = m_ModuleManager->BuildModule( "Config", "Config", builder );

				if( m_pConfigModule )
				{
					m_pConfigModule->AddRef();
				}

				m_ConfigurationObject = builder.GetConfigurationObject();

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
			}

			ASEvent regEvent( ASEventType::API_REGISTERED );

			regEvent.apiRegistration.pszConfigFilename = &config->GetConfigFilename();
			regEvent.apiRegistration.bSuccess = m_Instance->LoadAPIFromFile( config->GetConfigFilename() );

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

void CASManager::OnActiveConfigurationChanged( const std::shared_ptr<CConfiguration>& oldConfig, const std::shared_ptr<CConfiguration>& newConfig )
{
	ActiveConfigSet( newConfig );
}
