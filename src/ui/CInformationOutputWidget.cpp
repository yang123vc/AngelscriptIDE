#include "angelscript.h"

#include "Angelscript/CScript.h"

#include "ide/CASIDEApp.h"

#include "ui_COutputWidget.h"
#include "CInformationOutputWidget.h"

CInformationOutputWidget::CInformationOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: COutputWidget( app, "Information", ui, pParent )
	, m_uiErrors( 0 )
	, m_uiWarnings( 0 )
{
	app->AddASEventListener( this );
	app->AddCompilerListener( this );
	m_UI->AddUIEventListener( this );
}

CInformationOutputWidget::~CInformationOutputWidget()
{
	m_UI->RemoveUIEventListener( this );
	m_App->RemoveCompilerListener( this );
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

void CInformationOutputWidget::AngelscriptEventOccured(ASEvent event, const void* pArg )
{
	switch( event )
	{
	case ASEvent::Created:
		{
			m_App->AddCompilerListener( this );

			const ASCreationResult& creationResult = *reinterpret_cast<const ASCreationResult*>( pArg );

			WriteString( QString( "Angelscript initialized\nVersion: %1\n" ).arg( creationResult.szVersion.c_str() ) );

			if( !creationResult.fHasConfig )
				WriteString( "No active configuration\n" );

			break;
		}

	case ASEvent::Destroyed:
		{
			m_App->RemoveCompilerListener( this );

			WriteString( "Angelscript shut down\n" );

			break;
		}

	case ASEvent::APIRegistered:
		{
			const ASAPIRegistrationResult& result = *reinterpret_cast<const ASAPIRegistrationResult*>( pArg );

			if( result.fSuccess )
				ReceiveUIMessage( ( std::string( "API Configuration \"" ) + result.szConfigFilename + "\" loaded\n" ).c_str(), UIMessageType::Info );
			else
				ReceiveUIMessage( ( std::string( "Failed to load API configuration \"" ) + result.szConfigFilename + "\"!\n" ).c_str(), UIMessageType::Warning );
			break;
		}

	case ASEvent::CompilationStarted:
		{
			auto script = *reinterpret_cast<const std::shared_ptr<const CScript>*>( pArg );

			WriteCompileSeparator();
			WriteString( QString( "Starting compilation of script '%1'\n" ).arg( script->GetSectionName().c_str() ) );

			m_CompileStartTime = std::chrono::high_resolution_clock::now();

			m_uiErrors = 0;
			m_uiWarnings = 0;
			break;
		}

	case ASEvent::CompilationEnded:
		{
			const bool fSuccess = *static_cast<const bool*>( pArg );

			WriteString( fSuccess ? "Compilation succeeded\n" : "Compilation failed\n" );

			if( !fSuccess )
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
						QString( "Compilation took %1:%2:%3.%4\n" )
							.arg( hours.count() )
							.arg( minutes.count() )
							.arg( seconds.count() )
							.arg( milliseconds.count() ).toStdString().c_str() );

			WriteCompileSeparator();
			break;
		}

	case ASEvent::ConfigurationSet:
		{
			const std::string& szName = *reinterpret_cast<const std::string*>( pArg );

			WriteString( std::string( "Configuration \"" ) + szName + "\" loaded\n" );

			break;
		}

	case ASEvent::ConfigurationNotFound:
		{
			const std::string& szName = *reinterpret_cast<const std::string*>( pArg );

			WriteString( std::string( "Could not find configuration \"" ) + szName + "\"!\n" );
			break;
		}

	default: break;
	}
}

void CInformationOutputWidget::CompilerMessage( const asSMessageInfo* pMsg )
{
	if( pMsg->type == asMSGTYPE_ERROR )
		++m_uiErrors;
	else if( pMsg->type == asMSGTYPE_WARNING )
		++m_uiWarnings;
}

void CInformationOutputWidget::ReceiveUIMessage( const char* pszString, UIMessageType type )
{
	QString szMessage;

	switch( type )
	{
	default:
	case UIMessageType::Info: szMessage = pszString; break;
	case UIMessageType::Warning: szMessage = QString( "Warning: %1" ).arg( pszString ); break;
	case UIMessageType::Error: szMessage = QString( "Error: %1" ).arg( pszString ); break;
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
