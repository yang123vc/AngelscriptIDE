#ifndef IDE_UI_OPTIONS_COPTIONSDIALOG_H
#define IDE_UI_OPTIONS_COPTIONSDIALOG_H

#include <memory>
#include <vector>

#include <QDialog>

class CASIDEApp;
class COptions;
class CBaseOptionsWidget;
class QPushButton;
class QLabel;
class CUI;

namespace Ui
{
class COptionsDialog;
}

class COptionsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit COptionsDialog( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	~COptionsDialog();

protected:

	void ApplyChanges();

public slots:

	//Buttons
	void Ok();

	void Cancel();

	void Apply();

	//Input from child widgets
	//A child was changed from saved settings
	void ChangesMade( bool bState );

private:
	std::shared_ptr<CASIDEApp> m_App;
	std::shared_ptr<CUI> m_UI;
	std::unique_ptr<Ui::COptionsDialog> m_WidgetUI;
	std::vector<CBaseOptionsWidget*> m_Widgets;

	bool m_bInitialized = false;
};

#endif //IDE_UI_OPTIONS_COPTIONSDIALOG_H
