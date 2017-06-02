#ifndef ANGELSCRIPT_CASMANAGER_H
#define ANGELSCRIPT_CASMANAGER_H

#include <memory>

#include "util/CListenerManager.h"

class CASEngineInstance;
class IConfigurationManager;
class CConfiguration;
class IASEventListener;
class IASCompilerListener;
struct asSMessageInfo;
enum class ASEvent;

class CASManager
{
public:
	CASManager( std::shared_ptr<IConfigurationManager> configurationManager );
	~CASManager();

	std::shared_ptr<IConfigurationManager> GetConfigurationManager() { return m_ConfigurationManager; }

	std::shared_ptr<CConfiguration> GetActiveConfiguration() const { return m_ActiveConfiguration; }

	void AddEventListener( IASEventListener* pListener );

	void RemoveEventListener( IASEventListener* pListener );

	void AddCompilerListener( IASCompilerListener* pListener );

	void RemoveCompilerListener( IASCompilerListener* pListener );

	void MessageCallback( const asSMessageInfo* pMsg );

	void SetActiveConfiguration( const std::string& szName, bool fSaveOldConfig = false );

	void ClearActiveConfiguration( bool fSave = false );

	void ReloadActiveConfiguration();

	bool CompileScript( const std::string& szSectionName, const std::string& szScriptContents );

protected:
	void NotifyEventListeners(ASEvent event, const void* pArg = nullptr );

private:
	std::unique_ptr<CASEngineInstance>		m_Instance;
	std::shared_ptr<IConfigurationManager>	m_ConfigurationManager;
	std::shared_ptr<CConfiguration>			m_ActiveConfiguration;
	CListenerManager<IASEventListener>		m_EventListeners;
	CListenerManager<IASCompilerListener>		m_CompilerListeners;

private:
	CASManager( const CASManager& ) = delete;
	CASManager& operator=( const CASManager& ) = delete;
};

#endif //ANGELSCRIPT_CASMANAGER_H
