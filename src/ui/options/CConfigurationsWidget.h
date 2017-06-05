#ifndef IDE_UI_OPTIONS_CCONFIGURATIONSWIDGET_H
#define IDE_UI_OPTIONS_CCONFIGURATIONSWIDGET_H

#include <memory>

#include "Angelscript/IConfigurationEventListener.h"

#include "CBaseOptionsWidget.h"

namespace Ui
{
class CConfigurationsWidget;
}

class CASIDEApp;
class CConfiguration;
class CUI;
class QListWidget;

class CConfigurationsWidget : public CBaseOptionsWidget, public IConfigurationEventListener
{
	Q_OBJECT

public:
	explicit CConfigurationsWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent );
	~CConfigurationsWidget();

	bool CanSave( QString& szReason ) override;

	void ApplyChanges() override;

	void ConfigEventOccurred( const ConfigEvent& event ) override;

protected:

	bool SetCurrentConfiguration( const QString& szActiveConfig );
	bool SetCurrentConfiguration( int iIndex );

	void SaveCurrentConfiguration();

	void SetFieldsEnabled( bool bState );

	static void AddStringToList( QListWidget* pList, const std::string& szString );

	static void AddStringsToList( QListWidget* pList, const std::vector<std::string>& strings );

protected slots:

	void OpenEditConfigs();

	void ConfigurationSelected( int iIndex );

	void NameChanged( const QString& szText );

	void ConfigFileChanged( const QString& szText );

	void OpenSelectConfigFileDialog();

	void ConfigScriptFileChanged( const QString& szText );

	void OpenSelectConfigScriptFileDialog();

	void IncludeFilenameChanged( const QString& szText );

	void OpenIncludeFilenameFileDialog();

	void FallbackExtensionsChanged( const QString& szText );

	void AddWord();

	void RemoveWord();

	void AddIncludePath();

	void RemoveIncludePath();

	void AddExtension();

	void RemoveExtension();

private:
	std::unique_ptr<Ui::CConfigurationsWidget> m_WidgetUI;

	std::shared_ptr<CConfiguration> m_CurrentConfiguration;
};

#endif //IDE_UI_OPTIONS_CCONFIGURATIONSWIDGET_H
