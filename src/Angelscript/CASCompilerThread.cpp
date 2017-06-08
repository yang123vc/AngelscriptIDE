#include <cassert>

#include "CASCompilerThread.h"
#include "CASEngineInstance.h"

CASCompilerThread::CASCompilerThread( const std::shared_ptr<CASEngineInstance>& instance, const std::shared_ptr<const CConfiguration>& config,
									  QString&& szScriptFilename )
	: m_Instance( instance )
	, m_Config( config )
{
	assert( instance );
	assert( config );

	m_Parameters.m_szScriptFilename = std::move( szScriptFilename );
}

void CASCompilerThread::Run()
{
	m_Thread = std::thread( &CASCompilerThread::ThreadProc, shared_from_this() );

	m_Thread.detach();
}

void CASCompilerThread::ThreadProc( const std::shared_ptr<CASCompilerThread>& compilerThread )
{
	compilerThread->CompileScript();
}

void CASCompilerThread::CompileScript()
{
	CompilationStart( m_Parameters.m_szScriptFilename );

	const bool bResult = m_Instance->CompileScript( m_Parameters.m_szScriptFilename.toStdString(), m_Config );

	CompilationEnd( m_Parameters.m_szScriptFilename, bResult );
}
