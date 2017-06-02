#ifndef IDE_UI_CCOMPILEOUTPUTWIDGET_H
#define IDE_UI_CCOMPILEOUTPUTWIDGET_H

#include "Angelscript/IASCompilerListener.h"
#include "Angelscript/IASEventListener.h"

#include "COutputWidget.h"

class CCompileOutputWidget : public COutputWidget, public IASCompilerListener, public IASEventListener
{
	Q_OBJECT

public:
	CCompileOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	~CCompileOutputWidget();

	void CompilerMessage( const asSMessageInfo* pMsg ) override;

	void AngelscriptEventOccured( const ASEvent& event ) override;
};

#endif //IDE_UI_CCOMPILEOUTPUTWIDGET_H
