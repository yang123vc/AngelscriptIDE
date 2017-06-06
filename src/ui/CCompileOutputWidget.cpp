#include <angelscript.h>

#include "Angelscript/CASManager.h"
#include "ide/CASIDEApp.h"

#include "ui_COutputWidget.h"
#include "CCompileOutputWidget.h"

CCompileOutputWidget::CCompileOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: COutputWidget( app, "Output", ui, pParent )
{
	connect( m_App->GetAngelscriptManager().get(), &CASManager::CompilerMessage, this, &CCompileOutputWidget::OnCompilerMessage );
	m_App->AddASEventListener( this );
}

CCompileOutputWidget::~CCompileOutputWidget()
{
	m_App->RemoveASEventListener( this );
}

void CCompileOutputWidget::AngelscriptEventOccured( const ASEvent& event )
{
	switch( event.type )
	{
	case ASEventType::CREATED: Clear(); break;
	case ASEventType::COMPILATION_STARTED: Clear(); break;

	default: break;
	}
}

void CCompileOutputWidget::OnCompilerMessage( const asSMessageInfo& msg )
{
	QString szMessage;

	if( *msg.section )
		szMessage = QString( "Section \"%1\", row %2, column %3:\n%4\n" ).arg( msg.section ).arg( msg.row ).arg( msg.col ).arg( msg.message );
	else
		szMessage = QString( "%1\n" ).arg( msg.message );

	m_WidgetUi->m_pOutput->moveCursor( QTextCursor::End );
	m_WidgetUi->m_pOutput->insertPlainText( szMessage );
}
