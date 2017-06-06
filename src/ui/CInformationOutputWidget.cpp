#include "angelscript.h"

#include "Angelscript/CASManager.h"
#include "Angelscript/CScript.h"

#include "ide/CASIDEApp.h"

#include "CInformationOutputWidget.h"
#include "ui_COutputWidget.h"

CInformationOutputWidget::CInformationOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: COutputWidget( app, "Information", ui, pParent )
	, m_uiErrors( 0 )
	, m_uiWarnings( 0 )
{
	auto manager = m_App->GetAngelscriptManager();

	connect( manager.get(), &CASManager::EngineCreated, this, &CInformationOutputWidget::OnEngineCreated );
	connect( manager.get(), &CASManager::EngineDestroyed, this, &CInformationOutputWidget::OnEngineDestroyed );
	connect( manager.get(), &CASManager::APIRegistered, this, &CInformationOutputWidget::OnAPIRegistered );
	connect( manager.get(), &CASManager::ConfigChanged, this, &CInformationOutputWidget::OnConfigChanged );
	connect( manager.get(), &CASManager::CompilationStarted, this, &CInformationOutputWidget::OnCompilationStarted );
	connect( manager.get(), &CASManager::CompilationEnded, this, &CInformationOutputWidget::OnCompilationEnded );
	connect( manager.get(), &CASManager::CompilerMessage, this, &CInformationOutputWidget::OnCompilerMessage );

	connect( m_UI.get(), &CUI::UIMessage, this, &CInformationOutputWidget::OnUIMessage );
}

CInformationOutputWidget::~CInformationOutputWidget()
{
}

void CInformationOutputWidget::WriteString( const char* pszString )
{
	m_WidgetUi->m_pOutput->moveCursor( QTextCursor::End );
	m_WidgetUi->m_pOutput->insertPlainText( pszString );
}

void CInformationOutputWidget::WriteString( const std::string& szString )
{
	WriteString( szString.c_str() );
}

void CInformationOutputWidget::WriteString( const QString& szString )
{
	WriteString( szString.toStdString() );
}

void CInformationOutputWidget::WriteCompileSeparator( char cChar, unsigned int uiWidth )
{
	m_WidgetUi->m_pOutput->moveCursor( QTextCursor::End );

	char szChar[] = { cChar, '\0' };

	for( unsigned int uiIndex = 0; uiIndex < uiWidth; ++uiIndex )
		m_WidgetUi->m_pOutput->insertPlainText( szChar );


	szChar[ 0 ] = '\n';

	m_WidgetUi->m_pOutput->insertPlainText( szChar );
}

void CInformationOutputWidget::OnEngineCreated( const std::string& szVersion, bool bHasConfig )
{
	WriteString( QString( "Angelscript initialized\nVersion: %1\n" ).arg( szVersion.c_str() ) );

	if( !bHasConfig )
		WriteString( "No active configuration\n" );
}

void CInformationOutputWidget::OnEngineDestroyed()
{
	WriteString( "Angelscript shut down\n" );
}

void CInformationOutputWidget::OnAPIRegistered( const std::string& szConfigFilename, bool bSuccess )
{
	if( bSuccess )
		OnUIMessage( ( std::string( "API Configuration \"" ) + szConfigFilename + "\" loaded\n" ).c_str(), UIMessageType::INFO );
	else
		OnUIMessage( ( std::string( "Failed to load API configuration \"" ) + szConfigFilename + "\"!\n" ).c_str(), UIMessageType::WARNING );
}

void CInformationOutputWidget::OnConfigChanged( ConfigChangeType changeType, const std::string& szName )
{
	switch( changeType )
	{
	case ConfigChangeType::SET:
		{
			WriteString( std::string( "Configuration \"" ) + szName + "\" loaded\n" );
			break;
		}

	default: break;
	}
}

void CInformationOutputWidget::OnCompilationStarted( const std::shared_ptr<const CScript>& script )
{
	WriteCompileSeparator();
	WriteString( QString( "Starting compilation of script '%1'\n" ).arg( script->GetSectionName().c_str() ) );

	m_CompileStartTime = std::chrono::high_resolution_clock::now();

	m_uiErrors = 0;
	m_uiWarnings = 0;
}

void CInformationOutputWidget::OnCompilationEnded( const std::shared_ptr<const CScript>& script, bool bSuccess )
{
	WriteString( bSuccess ? "Compilation succeeded\n" : "Compilation failed\n" );

	if( !bSuccess )
		WriteString( QString( "%1 errors, %2 warnings\n" ).arg( m_uiErrors ).arg( m_uiWarnings ).toStdString().c_str() );

	auto endTime = std::chrono::high_resolution_clock::now();

	auto time = endTime - m_CompileStartTime;

	auto hours = std::chrono::duration_cast<std::chrono::hours>( time );

	time -= hours;

	auto minutes = std::chrono::duration_cast<std::chrono::minutes>( time );

	time -= minutes;

	auto seconds = std::chrono::duration_cast<std::chrono::seconds>( time );

	time -= seconds;

	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>( time );

	WriteString(
		QString( "Compilation time: %1:%2:%3.%4\n" )
		.arg( hours.count(), 2, 10, QLatin1Char( '0' ) )
		.arg( minutes.count(), 2, 10, QLatin1Char( '0' ) )
		.arg( seconds.count(), 2, 10, QLatin1Char( '0' ) )
		.arg( milliseconds.count(), 3, 10, QLatin1Char( '0' ) )
		.toStdString().c_str()
	);

	WriteCompileSeparator();
}

void CInformationOutputWidget::OnCompilerMessage( const asSMessageInfo& msg )
{
	if( msg.type == asMSGTYPE_ERROR )
		++m_uiErrors;
	else if( msg.type == asMSGTYPE_WARNING )
		++m_uiWarnings;
}

void  CInformationOutputWidget::OnUIMessage( const char* pszString, UIMessageType type )
{
	QString szMessage;

	switch( type )
	{
	default:
	case UIMessageType::INFO: szMessage = pszString; break;
	case UIMessageType::WARNING: szMessage = QString( "Warning: %1" ).arg( pszString ); break;
	case UIMessageType::ERROR: szMessage = QString( "Error: %1" ).arg( pszString ); break;
	}

	WriteString( szMessage.toStdString().c_str() );
}
