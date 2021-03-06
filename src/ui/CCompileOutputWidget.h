#ifndef IDE_UI_CCOMPILEOUTPUTWIDGET_H
#define IDE_UI_CCOMPILEOUTPUTWIDGET_H

#include <memory>
#include <string>

#include "COutputWidget.h"

struct asSMessageInfo;

class CCompileOutputWidget : public COutputWidget
{
	Q_OBJECT

public:
	CCompileOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	~CCompileOutputWidget();

private slots:
	void OnEngineCreated( const std::string& szVersion, bool bHasConfig );

	void OnCompilationStarted( const QString& szScriptFilename );

	void OnCompilerMessage( const asSMessageInfo& msg );
};

#endif //IDE_UI_CCOMPILEOUTPUTWIDGET_H
