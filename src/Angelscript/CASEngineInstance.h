#ifndef ANGELSCRIPT_CASENGINEINSTANCE_H
#define ANGELSCRIPT_CASENGINEINSTANCE_H

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

	/**
	*	@return The engine version as a string
	*/
	const char* GetVersion() const;

	/**
	*	Sets the message callback to use for this engine
	*/
	void SetMessageCallback( const asSFuncPtr& callback, void* pObj, asDWORD callConv );

	/**
	*	Loads an API saved with WriteConfigToFile or WriteConfigToStream
	*/
	bool LoadAPIFromFile( const std::string& szFilename );

	/**
	*	Compiles a given script
	*/
	bool CompileScript( std::shared_ptr<const CScript> script );

private:
	asIScriptEngine* m_pScriptEngine = nullptr;

private:
	CASEngineInstance( const CASEngineInstance& ) = delete;
	CASEngineInstance& operator=( const CASEngineInstance& ) = delete;
};

#endif //ANGELSCRIPT_CASENGINEINSTANCE_H
