#include "angelscript.h"

#include "Angelscript/CASManager.h"
#include "Angelscript/CScript.h"

//TODO: remove Windows dependency - Solokiller
#undef ERROR

#include "ide/CASIDEApp.h"

#include "ui_COutputWidget.h"
#include "CInformationOutputWidget.h"

CInformationOutputWidget::CInformationOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: COutputWidget( app, "Information", ui, pParent )
	, m_uiErrors( 0 )
	, m_uiWarnings( 0 )
{
	app->AddASEventListener( this );
	connect( m_App->GetAngelscriptManager().get(), &CASManager::CompilerMessage, this, &CInformationOutputWidget::OnCompilerMessage );
	m_UI->AddUIEventListener( this );
}

CInformationOutputWidget::~CInformationOutputWidget()
{
	m_UI->RemoveUIEventListener( this );
	m_App->RemoveASEventListener( this );
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

void CInformationOutputWidget::AngelscriptEventOccured( const ASEvent& event )
{
	switch( event.type )
	{
	case ASEventType::CREATED:
		{
			WriteString( QString( "Angelscript initialized\nVersion: %1\n" ).arg( event.create.pszVersion->c_str() ) );

			if( !event.create.bHasConfig )
				WriteString( "No active configuration\n" );

			break;
		}

	case ASEventType::DESTROYED:
		{
			WriteString( "Angelscript shut down\n" );

			break;
		}

	case ASEventType::API_REGISTERED:
		{
			if( event.apiRegistration.bSuccess )
				ReceiveUIMessage( ( std::string( "API Configuration \"" ) + *event.apiRegistration.pszConfigFilename + "\" loaded\n" ).c_str(), UIMessageType::INFO );
			else
				ReceiveUIMessage( ( std::string( "Failed to load API configuration \"" ) + *event.apiRegistration.pszConfigFilename + "\"!\n" ).c_str(), UIMessageType::WARNING );
			break;
		}

	case ASEventType::COMPILATION_STARTED:
		{
			WriteCompileSeparator();
			WriteString( QString( "Starting compilation of script '%1'\n" ).arg( event.compilationStart.pScript->GetSectionName().c_str() ) );

			m_CompileStartTime = std::chrono::high_resolution_clock::now();

			m_uiErrors = 0;
			m_uiWarnings = 0;
			break;
		}

	case ASEventType::COMPILATION_ENDED:
		{
			WriteString( event.compilationEnd.bSuccess ? "Compilation succeeded\n" : "Compilation failed\n" );

			if( !event.compilationEnd.bSuccess )
				WriteString( QString("%1 errors, %2 warnings\n" ).arg( m_uiErrors ).arg( m_uiWarnings ).toStdString().c_str() );

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
			break;
		}

	case ASEventType::CONFIG_CHANGE:
		{
			switch( event.configChange.changeType )
			{
			case ASConfigChangeType::SET:
				{
					WriteString( std::string( "Configuration \"" ) + *event.configChange.pszName + "\" loaded\n" );
					break;
				}

			default: break;
			}
			break;
		}

	default: break;
	}
}

void CInformationOutputWidget::ReceiveUIMessage( const char* pszString, UIMessageType type )
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

void CInformationOutputWidget::WriteCompileSeparator( char cChar, unsigned int uiWidth )
{
	m_WidgetUi->m_pOutput->moveCursor( QTextCursor::End );

	char szChar[] = { cChar, '\0' };

	for( unsigned int uiIndex = 0; uiIndex < uiWidth; ++uiIndex )
		m_WidgetUi->m_pOutput->insertPlainText( szChar );


	szChar[ 0 ] = '\n';

	m_WidgetUi->m_pOutput->insertPlainText( szChar );
}

void CInformationOutputWidget::OnCompilerMessage( const asSMessageInfo& msg )
{
	if( msg.type == asMSGTYPE_ERROR )
		++m_uiErrors;
	else if( msg.type == asMSGTYPE_WARNING )
		++m_uiWarnings;
}
