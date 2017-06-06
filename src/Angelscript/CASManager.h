#ifndef ANGELSCRIPT_CASMANAGER_H
#define ANGELSCRIPT_CASMANAGER_H

#include <memory>

#include <QObject>

#include <AngelscriptUtils/CASModuleManager.h>
#include <AngelscriptUtils/util/CASObjPtr.h>
#include <AngelscriptUtils/util/CASRefPtr.h>

class asIScriptContext;
class asIScriptEngine;
class CASEngineInstance;
class CConfiguration;
class CConfigurationManager;
class CScript;
struct asSMessageInfo;
struct ASEvent;

enum class ConfigChangeType
{
	SET = 0,
	CLEARED
};

/**
*	Manages the Angelscript engine and handles compilation
*/
class CASManager : public QObject
{
	Q_OBJECT

public:
	/**
	*	Creates an Angelscript context manager
	*	@throws std::runtime_error If the IDE script engine could not be created
	*/
	CASManager( std::shared_ptr<CConfigurationManager> configurationManager );
	~CASManager();

	std::shared_ptr<CConfigurationManager> GetConfigurationManager() { return m_ConfigurationManager; }

	void MessageCallback( const asSMessageInfo* pMsg );

	/**
	*	Compiles a script
	*/
	bool CompileScript( const std::string& szSectionName, const std::string& szScriptContents );

private:
	void ActiveConfigSet( const std::shared_ptr<CConfiguration>& config );

	void ClearConfigurationScript();

signals:
	/**
	*	An engine has been created
	*	@param szVersion The Angelscript version as a string
	*	@param Whether a configuration exists for this instance
	*/
	void EngineCreated( const std::string& szVersion, bool bHasConfig );

	/**
	*	An engine has been destroyed
	*/
	void EngineDestroyed();

	/**
	*	The API for an engine has been registered
	*	@param szConfigFilename Name of the config file that was loaded
	*	@param Whether registration succeded
	*/
	void APIRegistered( const std::string& szConfigFilename, bool bSuccess );

	/**
	*	The active configuration was changed to another
	*	@param changeType Whether the config was set to a new configuration, or set to none
	*	@param szName Name of the new configuration, if any
	*/
	void ConfigChanged( ConfigChangeType changeType, const std::string& szName );

	/**
	*	Compilation has started
	*	@param script The script currently being compiled
	*/
	void CompilationStarted( const std::shared_ptr<const CScript>& script );

	/**
	*	Compilation has ended
	*	@param script The script currently being compiled
	*	@param bSuccess Whether compilation succeeded or failed
	*/
	void CompilationEnded( const std::shared_ptr<const CScript>& script, bool bSuccess );

	void CompilerMessage( const asSMessageInfo& msg );

private slots:
	void OnActiveConfigurationChanged( const std::shared_ptr<CConfiguration>& oldConfig, const std::shared_ptr<CConfiguration>& newConfig );

private:
	std::unique_ptr<CASEngineInstance>		m_Instance;
	std::shared_ptr<CConfigurationManager>	m_ConfigurationManager;

	//This engine is used for scripts used to configure and control the IDE itself
	asIScriptEngine*						m_pIDEEngine = nullptr;

	CASRefPtr<asIScriptContext>				m_Context;

	std::unique_ptr<CASModuleManager>		m_ModuleManager;

	CASModule* m_pConfigModule = nullptr;

	CASObjPtr m_ConfigurationObject;

private:
	CASManager( const CASManager& ) = delete;
	CASManager& operator=( const CASManager& ) = delete;
};

#endif //ANGELSCRIPT_CASMANAGER_H
