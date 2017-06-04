#include <iostream>
#include <fstream>

#include <QCoreApplication.h>
#include <QSettings.h>

 #include "util/IUI.h"

#include "angelscript.h"

#include "Angelscript/CASManager.h"
#include "Angelscript/CScript.h"
#include "Angelscript/IConfigurationEventListener.h"
#include "Angelscript/CConfiguration.h"
#include "ui/CASMainWindow.h"

#include "CConfigurationManager.h"
#include "COptions.h"

#include "CASIDEApp.h"

CASIDEApp::CASIDEApp( std::shared_ptr<IUI> ui )
	: m_UI( ui )
{
	//Set up the settings file data so it saves in the right place
	QCoreApplication::setOrganizationName( "Sam Vanheer" );
	QCoreApplication::setApplicationName( "Angelscript IDE" );

	QSettings::setDefaultFormat( QSettings::IniFormat );
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

	m_Options = std::make_shared<COptions>();

	m_ASManager = std::make_shared<CASManager>( m_Options->GetConfigurationManager() );

	m_Options->GetConfigurationManager()->AddConfigurationEventListener( this );
}

void CASIDEApp::OnBeforeRun()
{
	//Load settings now that everything's initialized.
	LoadSettings();

	CBaseApp::OnBeforeRun();
}

void CASIDEApp::Shutdown()
{
	SaveSettings();

	m_Options->GetConfigurationManager()->RemoveConfigurationEventListener( this );

	m_ASManager.reset();
	m_Options.reset();

	CBaseApp::Shutdown();
}

std::shared_ptr<CConfiguration> CASIDEApp::GetActiveConfiguration() const
{
	return m_Options->GetConfigurationManager()->GetActiveConfiguration();
}

void CASIDEApp::SetMainWindow( const std::shared_ptr<CASMainWindow>& window )
{
	m_MainWindow = window;
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

void CASIDEApp::RefreshSyntaxHighlights()
{
	m_MainWindow->RefreshSyntaxHighlights();
}

void CASIDEApp::LoadSettings()
{
	QSettings settings;

	m_Options->LoadOptions( settings );
}

void CASIDEApp::SaveSettings()
{
	QSettings settings;

	//Clear everything so no old stuff stays behind
	settings.clear();

	m_Options->SaveOptions( settings );

	RefreshSyntaxHighlights();
}

void CASIDEApp::ConfigEventOccurred( const ConfigEvent& event )
{
	switch( event.type )
	{
	case ConfigEventType::REMOVE:
		{
			if( event.remove.bIsActiveConfig )
			{
				WriteString( "Active configuration removed\n" );
			}
			break;
		}

	default: break;
	}
}
