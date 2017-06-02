#ifndef IDE_UI_OPTIONS_CGENERALWIDGET_H
#define IDE_UI_OPTIONS_CGENERALWIDGET_H

#include <QWidget>

#include "CBaseOptionsWidget.h"

namespace Ui
{
class CGeneralWidget;
}

class CASIDEApp;

/*
 * This widget does not listen to changes made to configurations due to possible race conditions.
*/
class CGeneralWidget : public CBaseOptionsWidget
{
	Q_OBJECT

public:
	explicit CGeneralWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent );
	~CGeneralWidget();

	bool CanSave( QString& szReason ) override;

	void ApplyChanges() override;

	void PostApply() override;

protected:

	void LoadConfigurations();

protected slots:

	void ActiveConfigurationChanged( int iIndex );

	void TabWidthChanged( int iValue );

private:
	std::unique_ptr<Ui::CGeneralWidget> m_WidgetUI;
};

#endif //IDE_UI_OPTIONS_CGENERALWIDGET_H
