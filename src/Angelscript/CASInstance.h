#ifndef CASINSTANCE_H
#define CASINSTANCE_H

#include <memory>
#include <string>

class asIScriptEngine;
struct asSMessageInfo;

class CScript;

class CASInstance
{
public:
	enum StartupResult_t
	{
		STARTUP_SUCCESS = 0,
		STARTUP_ALREADYINITIALIZED,
		STARTUP_FAILED
	};

public:

	CASInstance();
	~CASInstance();

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
	CASInstance( const CASInstance& ) = delete;
	CASInstance& operator=( const CASInstance& ) = delete;
};

#endif // CASINSTANCE_H
