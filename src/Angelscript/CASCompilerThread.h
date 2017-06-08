#ifndef ANGELSCRIPT_CASCOMPILERTHREAD_H
#define ANGELSCRIPT_CASCOMPILERTHREAD_H

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <QObject>
#include <QString>

class CASEngineInstance;
class CConfiguration;

/**
*	Contains the parameters for script compilation.
*/
class CASCompileParameters final
{
public:
	CASCompileParameters() = default;

public:
	QString m_szScriptFilename;

private:
	CASCompileParameters( const CASCompileParameters& ) = delete;
	CASCompileParameters& operator=( const CASCompileParameters& ) = delete;
};

class CASCompilerThread final : public QObject, public std::enable_shared_from_this<CASCompilerThread>
{
	Q_OBJECT

public:
	CASCompilerThread( const std::shared_ptr<CASEngineInstance>& instance, const std::shared_ptr<const CConfiguration>& config,
					   QString&& szScriptFilename );
	~CASCompilerThread() = default;

	void Run();

signals:
	void CompilationStart( const QString& szScriptFilename );

	void CompilationEnd( const QString& szScriptFilename, bool bSuccess );

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
