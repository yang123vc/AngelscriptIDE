#include <iostream>
#include <fstream>

 #include "util/IUI.h"

#include "angelscript.h"

#include "Angelscript/CASManager.h"
#include "Angelscript/CScript.h"
#include "Angelscript/CScriptFile.h"
#include "Angelscript/IConfigurationEventListener.h"
#include "Angelscript/CConfiguration.h"

#include "CConfigurationManager.h"
#include "COptions.h"

#include "CASIDEApp.h"

CASIDEApp::CASIDEApp( std::shared_ptr<IUI> ui )
	: m_UI( ui )
	, m_ConfigurationManager()
	, m_ASManager()
{
}

CASIDEApp::~CASIDEApp()
{
}

void CASIDEApp::WriteString( const char* pszString )
{
	printf( "%s", pszString );

	if( strstr( pszString, "\n" ) )
		fflush( stdout );

	m_UI->SendMessage( pszString );
}

void CASIDEApp::WriteString( const std::string& szString )
{
	WriteString( szString.c_str() );
}

void CASIDEApp::Startup()
{
	CBaseApp::Startup();

	m_ConfigurationManager = std::make_shared<CConfigurationManager>( shared_from_this() );
	m_ASManager = std::make_shared<CASManager>( m_ConfigurationManager );

	bool fOptionsLoaded = false;
	m_Options = COptions::Load( "options.properties", &fOptionsLoaded );

	m_ConfigurationManager->AddConfigurationEventListener( this );
}

void CASIDEApp::Shutdown()
{
	m_ConfigurationManager->RemoveConfigurationEventListener( this );

	m_Options->Save();

	ClearActiveConfiguration( true );

	m_ASManager.reset();
	m_ConfigurationManager.reset();

	CBaseApp::Shutdown();
}

std::shared_ptr<CConfiguration> CASIDEApp::GetActiveConfiguration() const
{
	return m_ASManager->GetActiveConfiguration();
}

void CASIDEApp::AddASEventListener( IASEventListener* pListener )
{
	if( m_ASManager )
		m_ASManager->AddEventListener( pListener );
}

void CASIDEApp::RemoveASEventListener( IASEventListener* pListener )
{
	if( m_ASManager )
		m_ASManager->RemoveEventListener( pListener );
}

void CASIDEApp::AddCompilerListener( IASCompilerListener* pListener )
{
	if( m_ASManager )
		m_ASManager->AddCompilerListener( pListener );
}

void CASIDEApp::RemoveCompilerListener( IASCompilerListener* pListener )
{
	if( m_ASManager )
		m_ASManager->RemoveCompilerListener( pListener );
}

bool CASIDEApp::CompileScript( const std::string& szSectionName, const std::string& szScriptContents )
{
	return m_ASManager->CompileScript( szSectionName, szScriptContents );
}

void CASIDEApp::LoadActiveConfiguration()
{
	SetActiveConfiguration( m_Options->GetActiveConfigurationName() );
}

void CASIDEApp::SetActiveConfiguration( const std::string& szName, bool fSaveOldConfig )
{
	m_ASManager->SetActiveConfiguration( szName, fSaveOldConfig );
}

void CASIDEApp::ClearActiveConfiguration( bool fSave )
{
	m_ASManager->ClearActiveConfiguration( fSave );
}

void CASIDEApp::ConfigurationRemoved( const std::string& szName )
{
	if( m_Options->GetActiveConfigurationName() == szName )
	{
		WriteString( "Active configuration removed\n" );
		SetActiveConfiguration( "" );
	}
}

void CASIDEApp::ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName )
{
	if( szOldName == m_Options->GetActiveConfigurationName() )
	{
		m_Options->SetActiveConfigurationName( szNewName );
		SetActiveConfiguration( szNewName );
	}
}

void CASIDEApp::ConfigurationSaved( const std::string& szName )
{
	auto activeConfig = m_ASManager->GetActiveConfiguration();

	if( activeConfig && szName == activeConfig->GetName() )
		m_ASManager->ReloadActiveConfiguration();
}

void CASIDEApp::AngelscriptEventOccured( ASEvent event, const void* pArg )
{
	switch( event )
	{
	case ASEvent::ConfigurationSet:
		{
			m_Options->SetActiveConfigurationName( *reinterpret_cast<const std::string*>( pArg ) );
			break;
		}

	case ASEvent::ConfigurationCleared:
		{
			m_Options->SetActiveConfigurationName( "" );
			break;
		}

	default: break;
	}
}
