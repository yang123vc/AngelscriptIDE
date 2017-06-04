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
	std::vector<CBaseOptionsWidget*> m_Widgets;

	QLabel* m_pMessageLabel;
	QPushButton* m_pApplyButton;

	bool m_bInitialized = false;
};

#endif //IDE_UI_OPTIONS_COPTIONSDIALOG_H
