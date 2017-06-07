#include <cassert>

#include "CASCompilerThread.h"
#include "CASEngineInstance.h"
#include "CScript.h"

CASCompilerThread::CASCompilerThread( const std::shared_ptr<CASEngineInstance>& instance, const std::shared_ptr<const CConfiguration>& config,
									  std::string&& szSectionName, std::string&& szContents )
	: m_Instance( instance )
	, m_Config( config )
{
	assert( instance );
	assert( config );

	m_Parameters.m_szSectionName = std::move( szSectionName );
	m_Parameters.m_szContents = std::move( szContents );
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
	auto script = std::make_shared<const CScript>( std::move( m_Parameters.m_szSectionName ), std::move( m_Parameters.m_szContents ) );

	CompilationStart( script );

	const bool bResult = m_Instance->CompileScript( script, m_Config );

	CompilationEnd( script, bResult );
}
