#include "angelscript.h"

#include "ide/CASIDEApp.h"

#include "ui_COutputWidget.h"
#include "CCompileOutputWidget.h"

CCompileOutputWidget::CCompileOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: COutputWidget( app, "Output", ui, pParent )
{
	m_App->AddCompilerListener( this );
	m_App->AddASEventListener( this );
}

CCompileOutputWidget::~CCompileOutputWidget()
{
	m_App->RemoveASEventListener( this );
	m_App->RemoveCompilerListener( this );
}

void CCompileOutputWidget::CompilerMessage( const asSMessageInfo* pMsg )
{
	QString szMessage;

	if( *pMsg->section )
		szMessage = QString( "Section \"%1\", row %2, column %3:\n%4\n" ).arg( pMsg->section ).arg( pMsg->row ).arg( pMsg->col ).arg( pMsg->message );
	else
		szMessage = QString( "%1\n" ).arg( pMsg->message );

	m_WidgetUi->m_pOutput->moveCursor( QTextCursor::End );
	m_WidgetUi->m_pOutput->insertPlainText( szMessage );
}

void CCompileOutputWidget::AngelscriptEventOccured( const ASEvent& event )
{
	switch( event.type )
	{
	case ASEventType::CREATED: Clear(); m_App->AddCompilerListener( this ); break;
	case ASEventType::DESTROYED: m_App->RemoveCompilerListener( this ); break;
	case ASEventType::COMPILATION_STARTED: Clear(); break;

	default: break;
	}
}
