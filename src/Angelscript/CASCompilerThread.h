#ifndef ANGELSCRIPT_CASCOMPILERTHREAD_H
#define ANGELSCRIPT_CASCOMPILERTHREAD_H

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <QObject>

class CASEngineInstance;
class CConfiguration;
class CScript;

/**
*	Contains the parameters for script compilation.
*/
class CASCompileParameters final
{
public:
	CASCompileParameters() = default;

public:
	std::string m_szSectionName;
	std::string m_szContents;

private:
	CASCompileParameters( const CASCompileParameters& ) = delete;
	CASCompileParameters& operator=( const CASCompileParameters& ) = delete;
};

class CASCompilerThread final : public QObject, public std::enable_shared_from_this<CASCompilerThread>
{
	Q_OBJECT

public:
	CASCompilerThread( const std::shared_ptr<CASEngineInstance>& instance, const std::shared_ptr<const CConfiguration>& config,
					   std::string&& szSectionName, std::string&& szContents );
	~CASCompilerThread() = default;

	void Run();

signals:
	void CompilationStart( const std::shared_ptr<const CScript>& script );

	void CompilationEnd( const std::shared_ptr<const CScript>& script, bool bSuccess );

private slots:


private:
	//Takes a shared ptr so our lifetime doesn't end while we're still in our own method calls
	static void ThreadProc( const std::shared_ptr<CASCompilerThread>& compilerThread );

	void CompileScript();

private:
	std::thread m_Thread;

	std::shared_ptr<CASEngineInstance> m_Instance;
	std::shared_ptr<const CConfiguration> m_Config;

	CASCompileParameters m_Parameters;

private:
	CASCompilerThread( const CASCompilerThread& ) = delete;
	CASCompilerThread& operator=( const CASCompilerThread& ) = delete;
};

#endif //ANGELSCRIPT_CASCOMPILERTHREAD_H
