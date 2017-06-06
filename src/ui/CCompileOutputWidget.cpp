#include <angelscript.h>

#include "Angelscript/CASDevEnvironment.h"
#include "ide/CASIDEApp.h"

#include "CCompileOutputWidget.h"
#include "ui_COutputWidget.h"

CCompileOutputWidget::CCompileOutputWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: COutputWidget( app, "Output", ui, pParent )
{
	auto devEnv = m_App->GetDevEnvironment();

	connect( devEnv.get(), &CASDevEnvironment::EngineCreated, this, &CCompileOutputWidget::OnEngineCreated );
	connect( devEnv.get(), &CASDevEnvironment::CompilationStarted, this, &CCompileOutputWidget::OnCompilationStarted );
	connect( devEnv.get(), &CASDevEnvironment::CompilerMessage, this, &CCompileOutputWidget::OnCompilerMessage );
}

CCompileOutputWidget::~CCompileOutputWidget()
{
}

void CCompileOutputWidget::OnEngineCreated( const std::string& szVersion, bool bHasConfig )
{
	Clear();
}

void CCompileOutputWidget::OnCompilationStarted( const std::shared_ptr<const CScript>& script )
{
	Clear();
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
