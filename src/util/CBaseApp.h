#ifndef UTIL_CBASEAPP_H
#define UTIL_CBASEAPP_H

#include <memory>

#include "CDispatchingOutStream.h"
#include "CListenerManager.h"

class IAppListener;

/**
*	Base class for apps
*/
class CBaseApp : public IOutStreamListener
{
public:
	CBaseApp();
	~CBaseApp();

	/**
	*	Writes a string to be displayed
	*/
	virtual void WriteString( const char* pszString ) = 0;

	void AddAppListener( IAppListener* pListener );
	void RemoveAppListener( IAppListener* pListener );

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

protected:
	//Redirect cout & cerr
	std::unique_ptr<CDispatchingOutStream> m_OutStream;
	std::unique_ptr<CDispatchingOutStream> m_ErrStream;

private:
	CListenerManager<IAppListener> m_AppListeners;
};

#endif //UTIL_CBASEAPP_H
