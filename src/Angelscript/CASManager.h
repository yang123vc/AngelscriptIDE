#ifndef ANGELSCRIPT_CASMANAGER_H
#define ANGELSCRIPT_CASMANAGER_H

#include <memory>

#include "IConfigurationEventListener.h"
#include "util/CListenerManager.h"

class CASEngineInstance;
class IConfigurationManager;
class CConfiguration;
class IASEventListener;
class IASCompilerListener;
struct asSMessageInfo;
struct ASEvent;

/**
*	Manages the Angelscript engine and handles compilation
*/
class CASManager : public IConfigurationEventListener
{
public:
	CASManager( std::shared_ptr<IConfigurationManager> configurationManager );
	~CASManager();

	std::shared_ptr<IConfigurationManager> GetConfigurationManager() { return m_ConfigurationManager; }

	void AddEventListener( IASEventListener* pListener );

	void RemoveEventListener( IASEventListener* pListener );

	void AddCompilerListener( IASCompilerListener* pListener );

	void RemoveCompilerListener( IASCompilerListener* pListener );

	void MessageCallback( const asSMessageInfo* pMsg );

	/**
	*	Compiles a script
	*/
	bool CompileScript( const std::string& szSectionName, const std::string& szScriptContents );

	void ConfigEventOccurred( const ConfigEvent& event ) override;

protected:
	void NotifyEventListeners( const ASEvent& event );

private:
	void ActiveConfigSet( const CConfiguration* pConfig );

private:
	std::unique_ptr<CASEngineInstance>		m_Instance;
	std::shared_ptr<IConfigurationManager>	m_ConfigurationManager;
	CListenerManager<IASEventListener>		m_EventListeners;
	CListenerManager<IASCompilerListener>	m_CompilerListeners;

private:
	CASManager( const CASManager& ) = delete;
	CASManager& operator=( const CASManager& ) = delete;
};

#endif //ANGELSCRIPT_CASMANAGER_H
