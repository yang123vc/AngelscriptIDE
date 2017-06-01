#include <iostream>

#include "IAppListener.h"

#include "CBaseApp.h"

CBaseApp::CBaseApp()
{
}

CBaseApp::~CBaseApp()
{

}

void CBaseApp::AddAppListener( IAppListener* pListener )
{
	m_AppListeners.AddListener( pListener );
}

void CBaseApp::RemoveAppListener( IAppListener* pListener )
{
	m_AppListeners.RemoveListener( pListener );
}

void CBaseApp::Startup()
{
	m_OutStream.reset( new CDispatchingOutStream( std::cout, this ) );
	m_ErrStream.reset( new CDispatchingOutStream( std::cerr, this ) );

	m_AppListeners.NotifyListeners( &IAppListener::AppStartedUp );
}

void CBaseApp::Shutdown()
{
	m_AppListeners.NotifyListeners( &IAppListener::AppShutdown );

	m_ErrStream.reset();
	m_OutStream.reset();
}
