#ifndef CLAUNCHER_H
#define CLAUNCHER_H

#include <memory>

#include <QObject>

class QMainWindow;

class IUI;
class CBaseApp;

class CLauncher : public QObject
{
	Q_OBJECT
public:

	using UIFactory = std::shared_ptr<IUI>( * ) ();
	using AppFactory = std::shared_ptr<CBaseApp> ( * )( std::shared_ptr<IUI> );
	using MainWindowFactory = std::unique_ptr<QMainWindow> ( * )( std::shared_ptr<CBaseApp>, std::shared_ptr<IUI> );

	explicit CLauncher( UIFactory uiFactory, AppFactory appFactory, MainWindowFactory mainWindowFactory, QObject* parent = nullptr );
	~CLauncher();

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

#endif // CLAUNCHER_H
