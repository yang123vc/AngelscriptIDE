#ifndef UTIL_CBASEAPP_H
#define UTIL_CBASEAPP_H

#include <memory>

#include "CDispatchingOutStream.h"
#include "CListenerManager.h"

class IAppListener;

class CBaseApp : public IOutStreamListener
{
public:
	CBaseApp();
	~CBaseApp();

	virtual void WriteString( const char* pszString ) = 0;

	void AddAppListener( IAppListener* pListener );
	void RemoveAppListener( IAppListener* pListener );

	virtual void Startup();
	virtual void Shutdown();

protected:
	std::unique_ptr<CDispatchingOutStream> m_OutStream;
	std::unique_ptr<CDispatchingOutStream> m_ErrStream;

private:
	CListenerManager<IAppListener> m_AppListeners;
};

#endif //UTIL_CBASEAPP_H
