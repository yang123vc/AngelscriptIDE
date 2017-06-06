#ifndef ANGELSCRIPT_CASMANAGER_H
#define ANGELSCRIPT_CASMANAGER_H

#include <memory>

#include <QObject>

#include <AngelscriptUtils/CASModuleManager.h>
#include <AngelscriptUtils/util/CASObjPtr.h>
#include <AngelscriptUtils/util/CASRefPtr.h>

#include "util/CListenerManager.h"

class asIScriptContext;
class asIScriptEngine;
class CASEngineInstance;
class CConfiguration;
class CConfigurationManager;
class IASEventListener;
class IASCompilerListener;
struct asSMessageInfo;
struct ASEvent;

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

	void AddEventListener( IASEventListener* pListener );

	void RemoveEventListener( IASEventListener* pListener );

	void AddCompilerListener( IASCompilerListener* pListener );

	void RemoveCompilerListener( IASCompilerListener* pListener );

	void MessageCallback( const asSMessageInfo* pMsg );

	/**
	*	Compiles a script
	*/
	bool CompileScript( const std::string& szSectionName, const std::string& szScriptContents );

protected:
	void NotifyEventListeners( const ASEvent& event );

private:
	void ActiveConfigSet( const std::shared_ptr<CConfiguration>& config );

	void ClearConfigurationScript();

private slots:
	void OnActiveConfigurationChanged( const std::shared_ptr<CConfiguration>& oldConfig, const std::shared_ptr<CConfiguration>& newConfig );

private:
	std::unique_ptr<CASEngineInstance>		m_Instance;
	std::shared_ptr<CConfigurationManager>	m_ConfigurationManager;
	CListenerManager<IASEventListener>		m_EventListeners;
	CListenerManager<IASCompilerListener>	m_CompilerListeners;

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
