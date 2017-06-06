#ifndef IDE_UI_CINFORMATIONOUTPUTWIDGET_H
#define IDE_UI_CINFORMATIONOUTPUTWIDGET_H

#include <chrono>

#include "Angelscript/CASDevEnvironment.h"
#include "COutputWidget.h"
#include "CUI.h"

struct asSMessageInfo;

class CInformationOutputWidget :
		public COutputWidget
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

protected:

	void WriteCompileSeparator( char cChar = DEFAULT_COMPILE_SEPARATOR_CHAR, unsigned int uiWidth = DEFAULT_COMPILE_SEPARATOR_WIDTH );

private slots:
	void OnEngineCreated( const std::string& szVersion, bool bHasConfig );

	void OnEngineDestroyed();

	void OnAPIRegistered( const std::string& szConfigFilename, bool bSuccess );

	void OnConfigChanged( ConfigChangeType changeType, const std::string& szName );

	void OnCompilationStarted( const std::shared_ptr<const CScript>& script );

	void OnCompilationEnded( const std::shared_ptr<const CScript>& script, bool bSuccess );

	void OnCompilerMessage( const asSMessageInfo& msg );

	void OnUIMessage( const char* pszString, UIMessageType type );

private:
	std::chrono::high_resolution_clock::time_point m_CompileStartTime;
	unsigned int m_uiErrors;
	unsigned int m_uiWarnings;
};

#endif //IDE_UI_CINFORMATIONOUTPUTWIDGET_H
