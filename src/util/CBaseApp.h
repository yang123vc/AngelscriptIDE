#ifndef UTIL_CBASEAPP_H
#define UTIL_CBASEAPP_H

#include <memory>

#include <QObject>

#include "CDispatchingOutStream.h"

/**
*	Base class for apps
*/
class CBaseApp : public QObject
{
	Q_OBJECT

public:
	CBaseApp();
	~CBaseApp();

	/**
	*	Starts up the app
	*/
	virtual void Startup();

	/**
	*	Called right before the app starts running the main loop
	*/
	virtual void OnBeforeRun();

	/**
	*	Shuts down the app
	*/
	virtual void Shutdown();

signals:
	void AppStartup();

	void AppBeforeRun();

	void AppShutdown();

public slots:
	/**
	*	Writes a string to be displayed
	*/
	virtual void WriteString( const char* pszString ) = 0;

protected:
	//Redirect cout & cerr
	std::unique_ptr<CDispatchingOutStream> m_OutStream;
	std::unique_ptr<CDispatchingOutStream> m_ErrStream;
};

#endif //UTIL_CBASEAPP_H
