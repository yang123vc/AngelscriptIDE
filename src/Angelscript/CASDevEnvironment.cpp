#include <cassert>
#include <iostream>
#include <stdexcept>

#include <angelscript.h>

#include <AngelscriptUtils/CASModule.h>
#include <AngelscriptUtils/add_on/scriptbuilder.h>
#include <AngelscriptUtils/wrapper/ASCallable.h>

#include "CASCompilerThread.h"
#include "CASConfigModuleBuilder.h"
#include "CASDevEnvironment.h"
#include "CASEngineInstance.h"
#include "CConfiguration.h"
#include "CConfigurationManager.h"
#include "CScript.h"
#include "IDE_API.h"

#include "ScriptAPI/ASIConfiguration.h"

//Needed for queued connections; these types are passed around
Q_DECLARE_METATYPE( std::shared_ptr<const CScript> );
Q_DECLARE_METATYPE( CASEngineInstance::CMessageInfo );

void RegisterQtMetaTypes()
{
	qRegisterMetaType<std::shared_ptr<const CScript>>();
	qRegisterMetaType<CASEngineInstance::CMessageInfo>();
}

CASDevEnvironment::CASDevEnvironment( std::shared_ptr<CConfigurationManager> configurationManager )
	: m_ConfigurationManager( configurationManager )
{
	connect( m_ConfigurationManager.get(), &CConfigurationManager::ActiveConfigurationChanged, this, &CASDevEnvironment::OnActiveConfigurationChanged );

	m_pIDEEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );

	if( !m_pIDEEngine )
		throw std::runtime_error( "Couldn't create IDE script engine" );

	m_pIDEEngine->SetMessageCallback( asMETHOD( CASDevEnvironment, MessageCallback ), this, asCALL_THISCALL );

	RegisterIDE_API( *m_pIDEEngine );

	m_Context.Set( m_pIDEEngine->CreateContext(), true );

	m_ModuleManager = std::make_unique<CASModuleManager>( *m_pIDEEngine );

	m_ModuleManager->AddDescriptor( "Config", 0xFFFFFFFF );

	RegisterQtMetaTypes();
}

CASDevEnvironment::~CASDevEnvironment()
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

bool CASDevEnvironment::CompileScript( const std::string& szSectionName, const std::string& szScriptContents )
{
	//No concurrent compilations
	//TODO: queue it up? - Solokiller
	if( m_CompilerThread )
		return false;

	m_CompilerThread = std::make_shared<CASCompilerThread>( m_Instance, m_ConfigurationManager->GetActiveConfiguration(), std::string( szSectionName ), std::string( szScriptContents ) );

	connect( m_CompilerThread.get(), &CASCompilerThread::CompilationStart, this, &CASDevEnvironment::OnCompilationStarted, Qt::QueuedConnection );
	connect( m_CompilerThread.get(), &CASCompilerThread::CompilationEnd, this, &CASDevEnvironment::OnCompilationEnded, Qt::QueuedConnection );

	m_CompilerThread->Run();

	return true;
}

bool CASDevEnvironment::IsCompiling() const
{
	//We're compiling if the thread exists
	return m_CompilerThread != nullptr;
}

void CASDevEnvironment::MessageCallback( const asSMessageInfo* pMsg )
{
	CompilerMessage( *pMsg );
}

void CASDevEnvironment::ActiveConfigSet( const std::shared_ptr<CConfiguration>& config )
{
	if( m_Instance )
	{
		m_Instance.reset();
		EngineDestroyed();
	}

	ClearConfigurationScript();

	try
	{
		m_Instance = std::make_unique<CASEngineInstance>();

		//Use a queued connection so messages are handled in a threadsafe manner
		connect( m_Instance.get(), &CASEngineInstance::EngineMessage, this, &CASDevEnvironment::OnEngineMessage, Qt::QueuedConnection );

		const std::string szVersion = m_Instance->GetVersion();

		EngineCreated( m_Instance->GetVersion(), config != nullptr );

		if( config )
		{
			ConfigChanged( ConfigChangeType::SET, config->GetName() );

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

			APIRegistered( config->GetConfigFilename(), m_Instance->LoadAPIFromFile( config->GetConfigFilename() ) );
		}
		else
		{
			ConfigChanged( ConfigChangeType::CLEARED, "" );
		}
	}
	catch( const CASEngineException& e )
	{
		std::cerr << e.what() << std::endl;
	}
}

void CASDevEnvironment::ClearConfigurationScript()
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

void CASDevEnvironment::OnEngineMessage( const CASEngineInstance::CMessageInfo& msg )
{
	//Turn it back into a regular message so we can send it to listeners
	asSMessageInfo message;

	auto szSection = msg.section.toLocal8Bit();

	message.section = msg.bHasSection ? szSection.constData() : nullptr;

	message.row = msg.row;
	message.col = msg.col;

	message.type = msg.type;

	auto szMessage = msg.message.toLocal8Bit();

	message.message = msg.bHasMessage ? szMessage.constData() : nullptr;

	CompilerMessage( message );
}

void CASDevEnvironment::OnCompilationStarted( const std::shared_ptr<const CScript>& script )
{
	CompilationStarted( script );
}

void CASDevEnvironment::OnCompilationEnded( const std::shared_ptr<const CScript>& script, bool bSuccess )
{
	CompilationEnded( script, bSuccess );

	m_CompilerThread.reset();
}

void CASDevEnvironment::OnActiveConfigurationChanged( const std::shared_ptr<CConfiguration>& oldConfig, const std::shared_ptr<CConfiguration>& newConfig )
{
	ActiveConfigSet( newConfig );
}
