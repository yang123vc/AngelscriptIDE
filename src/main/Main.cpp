#include <QMessageBox>

#include "ide/CASIDEApp.h"
#include "ui/CASMainWindow.h"
#include "ui/CUI.h"

#include "launcher/CLauncherException.h"
#include "launcher/CLauncher.h"

std::shared_ptr<IUI> CreateUI()
{
	return std::make_shared<CUI>();
}

std::shared_ptr<CBaseApp> CreateApp( std::shared_ptr<IUI> ui )
{
	return std::make_shared<CASIDEApp>( ui );
}

std::unique_ptr<QMainWindow> CreateMainWindow( std::shared_ptr<CBaseApp> app, std::shared_ptr<IUI> ui )
{
	return std::unique_ptr<QMainWindow>( new CASMainWindow( std::static_pointer_cast<CASIDEApp>( app ), std::static_pointer_cast<CUI>( ui ) ) );
}

int main( int argc, char* argv[] )
{
	//TODO: catch exception, show message box - Solokiller
	return CLauncher( &CreateUI, &CreateApp, &CreateMainWindow ).Run( argc, argv );
}
