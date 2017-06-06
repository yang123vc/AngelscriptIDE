#include <QApplication>
#include <QMessageBox>

#include "Main.h"
#include "ide/CASIDEApp.h"
#include "ui/CASMainWindow.h"
#include "ui/CUI.h"

void CLauncher::OnQuit()
{
	m_App->Shutdown();
}

int main( int argc, char* argv[] )
{
	//TODO: catch exception, show message box - Solokiller
	auto ui = std::make_shared<CUI>();

	CLauncher launcher;

	launcher.m_App = std::make_shared<CASIDEApp>( ui );

	launcher.m_App->Startup();

	QApplication qApplication( argc, argv );

	qApplication.connect( &qApplication, &QApplication::aboutToQuit, &launcher, &CLauncher::OnQuit );

	auto mainWindow = std::make_shared<CASMainWindow>( launcher.m_App, ui );

	launcher.m_App->SetMainWindow( mainWindow );

	launcher.m_App->OnBeforeRun();

	mainWindow->show();

	return qApplication.exec();
}
