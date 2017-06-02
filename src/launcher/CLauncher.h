#ifndef LAUNCHER_CLAUNCHER_H
#define LAUNCHER_CLAUNCHER_H

#include <memory>

#include <QObject>

class QMainWindow;

class IUI;
class CBaseApp;

/**
*	Launcher for Qt apps
*/
class CLauncher : public QObject
{
	Q_OBJECT
public:

	using UIFactory = std::shared_ptr<IUI>( * ) ();
	using AppFactory = std::shared_ptr<CBaseApp> ( * )( std::shared_ptr<IUI> );
	using MainWindowFactory = std::unique_ptr<QMainWindow> ( * )( std::shared_ptr<CBaseApp>, std::shared_ptr<IUI> );

	/**
	*	@param uiFactory Factory for UI objects
	*	@param appFactory Factory for app objects
	*	@param mainWindowFactory Factory for main windows
	*	@param parent Optional. Parent for the main window
	*/
	explicit CLauncher( UIFactory uiFactory, AppFactory appFactory, MainWindowFactory mainWindowFactory, QObject* parent = nullptr );
	~CLauncher();

	/**
	*	Runs the app
	*/
	int Run( int iArgc, char** argv );

signals:

public slots:

	void OnQuit();

private:
	UIFactory m_UIFactory;
	AppFactory m_AppFactory;
	MainWindowFactory m_MainWindowFactory;
	std::shared_ptr<CBaseApp> m_App;
};

#endif //LAUNCHER_CLAUNCHER_H
