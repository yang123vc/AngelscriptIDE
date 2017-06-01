#ifndef CCONFIGURATIONSWIDGET_H
#define CCONFIGURATIONSWIDGET_H

#include "Angelscript/IConfigurationEventListener.h"

#include "CBaseOptionsWidget.h"

namespace Ui
{
class CConfigurationsWidget;
}

class CASIDEApp;
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

	void ConfigurationAdded( const std::string& szName ) override;
	void ConfigurationRemoved( const std::string& szName ) override;
	void ConfigurationRenamed( const std::string& szOldName, const std::string& szNewName ) override;

protected:

	bool SetActiveConfiguration( const QString& szActiveConfig );
	bool SetActiveConfiguration( int iIndex );

	void SaveActiveConfiguration();

	void SetFieldsEnabled( bool fState );

	static void AddStringToList( QListWidget* pList, const std::string& szString );

	static void AddStringsToList( QListWidget* pList, const std::vector<std::string>& strings );

protected slots:

	void OpenEditConfigs();

	void ConfigurationSelected( int iIndex );

	void NameChanged( const QString& szText );

	void ConfigFileChanged( const QString& szText );

	void OpenSelectConfigFileDialog();

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
	bool m_fChangingContents; //Set to true if we're currently changing ui contents
	QString m_szConfigName;
};

#endif // CCONFIGURATIONSWIDGET_H
