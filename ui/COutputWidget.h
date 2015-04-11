#ifndef COUTPUTWIDGET_H
#define COUTPUTWIDGET_H

#include <memory>

#include "CBaseOutputWidget.h"

namespace Ui
{
class COutputWidget;
}

class CASIDEApp;

class COutputWidget : public CBaseOutputWidget
{
	Q_OBJECT

public:
	explicit COutputWidget( std::shared_ptr<CASIDEApp> app, const QString szName, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	virtual ~COutputWidget();

public slots:

	void Clear();

protected:
	std::unique_ptr<Ui::COutputWidget> m_WidgetUi;
	std::shared_ptr<CASIDEApp> m_App;
};

#endif // COUTPUTWIDGET_H
