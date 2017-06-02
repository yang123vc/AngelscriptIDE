#ifndef IDE_UI_OPTIONS_CEDITCONFIGURATIONSDIALOG_H
#define IDE_UI_OPTIONS_CEDITCONFIGURATIONSDIALOG_H

#include <memory>

#include <QDialog>

namespace Ui
{
class CEditConfigurationsDialog;
}

class CASIDEApp;
class CUI;

class CEditConfigurationsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CEditConfigurationsDialog( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	~CEditConfigurationsDialog();

protected slots:

	void AddConfiguraton();

	void RemoveConfiguration();

	void Ok();

private:
	std::unique_ptr<Ui::CEditConfigurationsDialog> m_WidgetUI;
	std::shared_ptr<CASIDEApp> m_App;
	std::shared_ptr<CUI> m_UI;
};

#endif //IDE_UI_OPTIONS_CEDITCONFIGURATIONSDIALOG_H
