#ifndef CASENGINEINSTANCE_H
#define CASENGINEINSTANCE_H

#include <memory>
#include <stdexcept>
#include <string>

class asIScriptEngine;
struct asSMessageInfo;

class CScript;

/**
*	Exception thrown if an Angelscript exception has occured
*/
class CASEngineException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

/**
*	Manages an Angelscript engine
*/
class CASEngineInstance
{
public:

	/**
	*	Creates an engine. 
	*	@throws CASEngineException if the engine could not be created
	*/
	CASEngineInstance();
	~CASEngineInstance();

	asIScriptEngine* GetScriptEngine() const { return m_pScriptEngine; }

	const char* GetVersion() const;

	void SetMessageCallback( const asSFuncPtr& callback, void* pObj, asDWORD callConv );

	bool LoadAPIFromFile( const std::string& szFilename );

	bool CompileScript( std::shared_ptr<const CScript> script );

private:
	asIScriptEngine* m_pScriptEngine = nullptr;

private:
	CASEngineInstance( const CASEngineInstance& ) = delete;
	CASEngineInstance& operator=( const CASEngineInstance& ) = delete;
};

#endif // CASENGINEINSTANCE_H
