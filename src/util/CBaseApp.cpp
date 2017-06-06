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
	m_OutStream = std::make_unique<CDispatchingOutStream>( std::cout );
	m_ErrStream = std::make_unique<CDispatchingOutStream>( std::cerr );

	connect( m_OutStream.get(), &CDispatchingOutStream::WriteString, this, &CBaseApp::WriteString );
	connect( m_ErrStream.get(), &CDispatchingOutStream::WriteString, this, &CBaseApp::WriteString );

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
