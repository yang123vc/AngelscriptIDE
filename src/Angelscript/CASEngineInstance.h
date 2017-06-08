#ifndef ANGELSCRIPT_CASENGINEINSTANCE_H
#define ANGELSCRIPT_CASENGINEINSTANCE_H

#include <memory>
#include <stdexcept>
#include <string>

#include <QObject>
#include <QString>

#include <angelscript.h>

class asIScriptEngine;
class CConfiguration;

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
class CASEngineInstance final : public QObject
{
	Q_OBJECT

public:
	/**
	*	Message used for threaded callbacks
	*/
	struct CMessageInfo
	{
		QString section;
		bool bHasSection;

		int         row;
		int         col;

		asEMsgType  type;

		QString message;
		bool bHasMessage;
	};

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
	*	Loads an API saved with WriteConfigToFile or WriteConfigToStream
	*/
	bool LoadAPIFromFile( const std::string& szFilename );

	/**
	*	Compiles a given script
	*/
	bool CompileScript( const std::string& szScriptFilename, const std::shared_ptr<const CConfiguration>& config );

	void Error( const char* pszSection, int row, int col, const char* pszFormat, ... );

signals:
	void EngineMessage( const CMessageInfo& msg );

private:
	void MessageCallback( asSMessageInfo* pMsg );

private:
	asIScriptEngine* m_pScriptEngine = nullptr;

private:
	CASEngineInstance( const CASEngineInstance& ) = delete;
	CASEngineInstance& operator=( const CASEngineInstance& ) = delete;
};

#endif //ANGELSCRIPT_CASENGINEINSTANCE_H
