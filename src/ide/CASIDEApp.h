#ifndef IDE_CASIDEAPP_H
#define IDE_CASIDEAPP_H

#include <memory>
#include <string>
#include <vector>

#include <QObject>
#include <QString>

#include "util/CBaseApp.h"

class CASDevEnvironment;
class CASMainWindow;
class CConfiguration;
class COptions;
class CUI;

/**
*	Angelscript IDE app
*/
class CASIDEApp final : public CBaseApp, public std::enable_shared_from_this<CASIDEApp>
{
	Q_OBJECT

public:
	CASIDEApp( std::shared_ptr<CUI> ui );
	~CASIDEApp();

	//IOutStreamListener
	void WriteString( const char* pszString ) override;
	void WriteString( const std::string& szString );

	//CBaseApp
	void Startup() override;
	void OnBeforeRun() override;
	void Shutdown() override;

	std::shared_ptr<CASDevEnvironment> GetDevEnvironment() const { return m_ASDDevEnv; }

	std::shared_ptr<COptions> GetOptions() const { return m_Options; }

	std::shared_ptr<CConfiguration> GetActiveConfiguration() const;

	void SetMainWindow( const std::shared_ptr<CASMainWindow>& window );

	//ASManager passthrough
	bool CompileScript( QString&& szScriptFilename );

	void RefreshSyntaxHighlights();

	void LoadSettings();
	void SaveSettings();

private slots:
	void OnConfigurationRemoved( const std::shared_ptr<CConfiguration>& config, bool bIsActiveConfig );

private:
	std::shared_ptr<CUI>					m_UI;
	std::shared_ptr<CASDevEnvironment>		m_ASDDevEnv;
	std::shared_ptr<COptions>				m_Options;
	std::shared_ptr<CASMainWindow>			m_MainWindow;

	std::vector<QMetaObject::Connection> m_Connections;
};

#endif //IDE_CASIDEAPP_H
