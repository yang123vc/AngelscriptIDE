#ifndef CINFORMATIONOUTPUTWIDGET_H
#define CINFORMATIONOUTPUTWIDGET_H

#include <chrono>

#include "Angelscript/IASCompilerListener.h"
#include "Angelscript/IASEventListener.h"

#include "CUI.h"

#include "COutputWidget.h"

class asSMessageInfo;

class CInformationOutputWidget :
		public COutputWidget,
		public IASEventListener,
		public IASCompilerListener,
		public IUIEventListener
{
protected:

	static const char DEFAULT_COMPILE_SEPARATOR_CHAR = '-';
	static const unsigned int DEFAULT_COMPILE_SEPARATOR_WIDTH = 40;

public:
	CInformationOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent = nullptr );
	~CInformationOutputWidget();

	void WriteString( const char* pszString );
	void WriteString( const std::string& szString );
	void WriteString( const QString& szString );

	void AngelscriptEventOccured( ASEvent event, const void* pArg ) override;

	void CompilerMessage( const asSMessageInfo* pMsg ) override;

	void ReceiveUIMessage( const char* pszString, UIMessageType type ) override;

protected:

	void WriteCompileSeparator( char cChar = DEFAULT_COMPILE_SEPARATOR_CHAR, unsigned int uiWidth = DEFAULT_COMPILE_SEPARATOR_WIDTH );

private:
	std::chrono::high_resolution_clock::time_point m_CompileStartTime;
	unsigned int m_uiErrors;
	unsigned int m_uiWarnings;
};

#endif // CINFORMATIONOUTPUTWIDGET_H
