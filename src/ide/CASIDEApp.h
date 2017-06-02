#ifndef IDE_CASIDEAPP_H
#define IDE_CASIDEAPP_H

#include <memory>
#include <vector>
#include <string>

#include "util/CBaseApp.h"
#include "util/CListenerManager.h"

#include "Angelscript/IConfigurationEventListener.h"
#include "Angelscript/IASEventListener.h"

class IASCompilerListener;
class IUI;
class IConfigurationManager;
class CConfiguration;
class COptions;
class CASManager;

class CASIDEApp final : public CBaseApp, public std::enable_shared_from_this<CASIDEApp>, public IConfigurationEventListener, public IASEventListener
{
public:
	CASIDEApp( std::shared_ptr<IUI> ui );
	~CASIDEApp();

	//IOutStreamListener
	virtual void WriteString( const char* pszString ) override;
	void WriteString( const std::string& szString );

	//CBaseApp
	virtual void Startup() override;
	virtual void Shutdown() override;

	std::shared_ptr<IConfigurationManager> GetConfigurationManager() const { return m_ConfigurationManager; }

	std::shared_ptr<CASManager> GetAngelscriptManager() const { return m_ASManager; }

	std::shared_ptr<COptions> GetOptions() const { return m_Options; }

	std::shared_ptr<CConfiguration> GetActiveConfiguration() const;

	//ASManager passthrough
	void AddASEventListener( IASEventListener* pListener );

	void RemoveASEventListener( IASEventListener* pListener );

	void AddCompilerListener( IASCompilerListener* pListener );

	void RemoveCompilerListener( IASCompilerListener* pListener );

	bool CompileScript( const std::string& szSectionName, const std::string& szScriptContents );

	void LoadActiveConfiguration();

	void SetActiveConfiguration( const std::string& szName, bool fSaveOldConfig = false );

	void ClearActiveConfiguration( bool fSave = false );

	//IConfigurationEventListener
	void ConfigurationRemoved( const std::string& szName ) override;

	void ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName ) override;

	void ConfigurationSaved( const std::string& szName ) override;

	//ICompilerListener
	void AngelscriptEventOccured( ASEvent event, const void* pArg ) override;

private:
	std::shared_ptr<IUI>					m_UI;
	std::shared_ptr<IConfigurationManager>	m_ConfigurationManager;
	std::shared_ptr<CASManager>				m_ASManager;
	std::shared_ptr<COptions>				m_Options;
};

#endif //IDE_CASIDEAPP_H
