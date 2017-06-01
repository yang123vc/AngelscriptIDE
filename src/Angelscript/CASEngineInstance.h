#ifndef CASENGINEINSTANCE_H
#define CASENGINEINSTANCE_H

#include <memory>
#include <string>

class asIScriptEngine;
struct asSMessageInfo;

class CScript;

class CASEngineInstance
{
public:
	enum StartupResult_t
	{
		STARTUP_SUCCESS = 0,
		STARTUP_ALREADYINITIALIZED,
		STARTUP_FAILED
	};

public:

	CASEngineInstance();
	~CASEngineInstance();

	asIScriptEngine* GetScriptEngine() const { return m_pScriptEngine; }

	bool IsInitialized() const { return m_pScriptEngine != nullptr; }

	const char* GetVersion() const;

	void SetMessageCallback( const asSFuncPtr& callback, void* pObj, asDWORD callConv );

	StartupResult_t Startup();
	void Shutdown();

	bool LoadAPIFromFile( const std::string& szFilename );

	bool CompileScript( std::shared_ptr<const CScript> script );

private:
	asIScriptEngine* m_pScriptEngine;

private:
	CASEngineInstance( const CASEngineInstance& ) = delete;
	CASEngineInstance& operator=( const CASEngineInstance& ) = delete;
};

#endif // CASENGINEINSTANCE_H
