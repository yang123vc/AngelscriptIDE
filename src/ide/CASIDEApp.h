#ifndef IDE_CASIDEAPP_H
#define IDE_CASIDEAPP_H

#include <memory>
#include <string>
#include <vector>

#include "util/CBaseApp.h"
#include "util/CListenerManager.h"

#include "Angelscript/IConfigurationEventListener.h"

class CASMainWindow;
class CASManager;
class CConfiguration;
class COptions;
class IASCompilerListener;
class IASEventListener;
class IUI;

/**
*	Angelscript IDE app
*/
class CASIDEApp final : public CBaseApp, public std::enable_shared_from_this<CASIDEApp>, public IConfigurationEventListener
{
public:
	CASIDEApp( std::shared_ptr<IUI> ui );
	~CASIDEApp();

	//IOutStreamListener
	void WriteString( const char* pszString ) override;
	void WriteString( const std::string& szString );

	//CBaseApp
	void Startup() override;
	void OnBeforeRun() override;
	void Shutdown() override;

	std::shared_ptr<CASManager> GetAngelscriptManager() const { return m_ASManager; }

	std::shared_ptr<COptions> GetOptions() const { return m_Options; }

	std::shared_ptr<CConfiguration> GetActiveConfiguration() const;

	void SetMainWindow( const std::shared_ptr<CASMainWindow>& window );

	//ASManager passthrough
	void AddASEventListener( IASEventListener* pListener );

	void RemoveASEventListener( IASEventListener* pListener );

	void AddCompilerListener( IASCompilerListener* pListener );

	void RemoveCompilerListener( IASCompilerListener* pListener );

	bool CompileScript( const std::string& szSectionName, const std::string& szScriptContents );

	void RefreshSyntaxHighlights();

	void LoadSettings();
	void SaveSettings();

	//IConfigurationEventListener
	void ConfigEventOccurred( const ConfigEvent& event ) override;

private:
	std::shared_ptr<IUI>			m_UI;
	std::shared_ptr<CASManager>		m_ASManager;
	std::shared_ptr<COptions>		m_Options;
	std::shared_ptr<CASMainWindow>	m_MainWindow;
};

#endif //IDE_CASIDEAPP_H
