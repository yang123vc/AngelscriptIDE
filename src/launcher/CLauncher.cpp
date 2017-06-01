#include <iostream>

#include "ui/CASMainWindow.h"
#include <QApplication>

#include "util/CBaseApp.h"

#include "LauncherException.h"

#include "CLauncher.h"

CLauncher::CLauncher( UIFactory uiFactory, AppFactory appFactory, MainWindowFactory mainWindowFactory, QObject* parent )
	: QObject( parent )
	, m_UIFactory( uiFactory )
	, m_AppFactory( appFactory )
	, m_MainWindowFactory( mainWindowFactory )
{
	if( !m_UIFactory )
		throw LauncherException( "UI factory must be valid!" );

	if( !m_AppFactory )
		throw LauncherException( "App factory must be valid!" );

	if( !m_MainWindowFactory )
		throw LauncherException( "Main window factory must be valid!" );
}

CLauncher::~CLauncher()
{
}

int CLauncher::Run( int iArgc, char** argv )
{
	auto ui = m_UIFactory();
	m_App = m_AppFactory( ui );

	m_App->Startup();

	QApplication qApplication( iArgc, argv );

	qApplication.connect( &qApplication, SIGNAL( aboutToQuit() ), this, SLOT( OnQuit() ) );

	auto m_MainWindow = m_MainWindowFactory( m_App, ui );
	m_MainWindow->show();

	return qApplication.exec();
}

void CLauncher::OnQuit()
{
	m_App->Shutdown();
}
