#include <iostream>

#include "CBaseApp.h"

CBaseApp::CBaseApp()
{
}

CBaseApp::~CBaseApp()
{
}

void CBaseApp::Startup()
{
	m_OutStream.reset( new CDispatchingOutStream( std::cout, this ) );
	m_ErrStream.reset( new CDispatchingOutStream( std::cerr, this ) );

	AppStartup();
}

void CBaseApp::OnBeforeRun()
{
	AppBeforeRun();
}

void CBaseApp::Shutdown()
{
	AppShutdown();

	m_ErrStream.reset();
	m_OutStream.reset();
}
