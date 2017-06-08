#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSyntaxHighlighter>

#include "Angelscript/CConfiguration.h"
#include "Angelscript/CASDevEnvironment.h"

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "util/UIUtils.h"

#include "CAngelscriptSyntaxHighlighter.h"
#include "CScriptCodeTextEdit.h"

CScriptCodeTextEdit::CScriptCodeTextEdit( const QString& szTitle, std::shared_ptr<CASIDEApp> app, QWidget* pParent )
	: CCodeTextEdit( pParent )
	, m_App( app )
{
	setTabStopWidth( m_App->GetOptions()->GetTabWidth() );

	m_SyntaxHighlighter = std::make_unique<CAngelscriptSyntaxHighlighter>( document(), m_App->GetOptions() );

	connect( this, SIGNAL( textChanged() ), this, SLOT( ContentChanged() ) );
	connect( this, SIGNAL( undoAvailable( bool ) ), this, SLOT( UndoStateChanged( bool ) ) );

	connect( m_App->GetOptions().get(), &COptions::OptionsLoaded, this, &CScriptCodeTextEdit::OnOptionsChanged );
	connect( m_App->GetOptions().get(), &COptions::OptionsSaved, this, &CScriptCodeTextEdit::OnOptionsChanged );

	NameChanged( m_szFilename );
}

CScriptCodeTextEdit::CScriptCodeTextEdit( const QString& szFilename, const IsFilenameTag_t& isFilename, std::shared_ptr<CASIDEApp> app,  QWidget* pParent )
	: CScriptCodeTextEdit( szFilename, app, pParent )
{
	m_szFilename = szFilename;

	m_bIsNewFile = false;

	//TODO: report file load failures - Solokiller
	auto szContents = m_App->GetDevEnvironment()->LoadScript( szFilename );

	appendPlainText( szContents );

	SetUnsavedChangesMade( false );
}

CScriptCodeTextEdit::~CScriptCodeTextEdit()
{
}

bool CScriptCodeTextEdit::UnsavedChangedMade() const
{
	return document()->isModified();
}

void CScriptCodeTextEdit::SetUnsavedChangesMade( bool fState )
{
	document()->setModified( fState );

	if( fState )
		NameChanged( m_szFilename + '*' );
	else
	{
		document()->clearUndoRedoStacks();
		NameChanged( m_szFilename );
	}
}

CScriptCodeTextEdit::SaveResult CScriptCodeTextEdit::Save( SaveMode saveMode, PromptMode promptMode, FileSelectMode fileSelect )
{
	SaveResult result = SaveResult::CANCELED;

	if(
			( saveMode == SaveMode::ALWAYS ) ||
			UnsavedChangedMade() ||
			( IsNewFile() && !this->document()->isEmpty() ) )
	{
		int iRet = QMessageBox::Save;

		if( ( promptMode != PromptMode::NEVER ) && ( UnsavedChangedMade() || ( promptMode == PromptMode::ALWAYS ) ) )
		{
			QMessageBox messageBox;
			messageBox.setText( "This document has unsaved changes." );
			messageBox.setInformativeText( "Do you want to save the changes?" );
			messageBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
			messageBox.setDefaultButton( QMessageBox::Save );

			iRet = messageBox.exec();
		}

		switch( iRet )
		{
		case QMessageBox::Save:
			{
				if( ( fileSelect != FileSelectMode::NEVER ) && ( IsNewFile() || ( fileSelect == FileSelectMode::ALWAYS ) ) )
				{
					OpenSaveDialog();
				}

				auto szPath = QDir( m_App->GetOptions()->GetCurrentDirectory().c_str() ).filePath( m_szFilename ).toStdString();

				const bool bResult = m_App->GetDevEnvironment()->SaveScript( m_szFilename, toPlainText() );

				if( bResult )
				{
					SetUnsavedChangesMade( false );
					result = SaveResult::SAVED;

					if( m_bIsNewFile )
						m_bIsNewFile = false;
				}
				else
				{
					QMessageBox errorBox;

					errorBox.setText( "An error occured." );
					errorBox.setInformativeText( "An error occured while trying to save the file." );
					errorBox.exec();

					result = SaveResult::CANCELED;
				}

				break;
			}

		case QMessageBox::Discard: result = SaveResult::DISCARED; break;
		case QMessageBox::Cancel: result = SaveResult::CANCELED; break;
		default: break;
		}
	}
	else
		result = SaveResult::SAVED;

	return result;
}

void CScriptCodeTextEdit::RefreshSyntaxHighlights()
{
	m_SyntaxHighlighter->rehighlight();
}

bool CScriptCodeTextEdit::OpenSaveDialog()
{
	auto options = m_App->GetOptions();

	const QString szDir = options->GetCurrentDirectory().c_str();

	auto config = m_App->GetActiveConfiguration();

	QString szExtensions = config ? ui::FormatExtensions( config->GetExtensions() ) : ui::ANY_EXTENSION;

	QString szFilename = QFileDialog::getSaveFileName(
				this, tr( "Save script" ), szDir,
				QString( tr( "Angelscript script files" ) + "(%1)" ).arg( szExtensions ) );

	bool fResult = !szFilename.isEmpty();

	if( fResult )
	{
		QFileInfo file( szFilename );

		options->SetCurrentDirectory( file.absolutePath().toStdString() );

		m_szFilename = file.absoluteFilePath();

		NameChanged( m_szFilename );
	}

	return fResult;
}

void CScriptCodeTextEdit::ContentChanged()
{
	NameChanged( m_szFilename + '*' );
}

void CScriptCodeTextEdit::UndoStateChanged( bool fState )
{
	if( !fState )
		SetUnsavedChangesMade( false );
}

void CScriptCodeTextEdit::OnOptionsChanged()
{
	setTabStopWidth( m_App->GetOptions()->GetTabWidth() );

	m_SyntaxHighlighter->rehighlight();
}
