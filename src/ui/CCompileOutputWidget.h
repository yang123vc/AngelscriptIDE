#ifndef IDE_UI_CCOMPILEOUTPUTWIDGET_H
#define IDE_UI_CCOMPILEOUTPUTWIDGET_H

#include "Angelscript/IASEventListener.h"

#include "COutputWidget.h"

struct asSMessageInfo;

class CCompileOutputWidget : public COutputWidget, public IASEventListener
{
	Q_OBJECT

public:
	CCompileOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	~CCompileOutputWidget();

	void AngelscriptEventOccured( const ASEvent& event ) override;

private slots:
	void OnCompilerMessage( const asSMessageInfo& msg );
};

#endif //IDE_UI_CCOMPILEOUTPUTWIDGET_H
