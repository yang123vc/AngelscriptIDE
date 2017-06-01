#include <QMessageBox>
#include <QFileDialog>

#include "Angelscript/CScriptFile.h"
#include "Angelscript/CConfiguration.h"

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "util/UIUtils.h"

#include "CScriptCodeTextEdit.h"

CScriptCodeTextEdit::CScriptCodeTextEdit( const std::string& szName, std::shared_ptr<CASIDEApp> app,  QWidget* pParent )
	: CCodeTextEdit( pParent )
	, m_szName( szName )
	, m_App( app )
{
	setTabStopWidth( m_App->GetOptions()->GetTabWidth() );

	connect( this, SIGNAL( textChanged() ), this, SLOT( ContentChanged() ) );
	connect( this, SIGNAL( undoAvailable( bool ) ), this, SLOT( UndoStateChanged( bool ) ) );
}

CScriptCodeTextEdit::CScriptCodeTextEdit( const std::string& szName, const std::string& szFilename, std::shared_ptr<CASIDEApp> app,  QWidget* pParent )
	: CScriptCodeTextEdit( szName, app, pParent )
{
	m_pScriptFile = std::make_shared<CScriptFile>( szFilename );

	auto buffer = m_pScriptFile->GetScriptContents();

	if( buffer )
		this->appendPlainText( buffer->c_str() );

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
		NameChanged( m_szName + '*' );
	else
	{
		document()->clearUndoRedoStacks();
		NameChanged( m_szName );
	}
}

CScriptCodeTextEdit::SaveResult CScriptCodeTextEdit::Save( int iSaveMode, int iPromptMode, int iFileSelect )
{
	SaveResult result = SaveResult::Canceled;

	if(
			( iSaveMode == Save_Always ) ||
			UnsavedChangedMade() ||
			( !m_pScriptFile && !this->document()->isEmpty() ) )
	{
		int iRet = QMessageBox::Save;

		if( ( iPromptMode != Prompt_Never ) && ( UnsavedChangedMade() || ( iPromptMode == Prompt_Always ) ) )
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
				if( ( iFileSelect != FileSelect_Never ) && ( !m_pScriptFile || ( iFileSelect == FileSelect_Always ) ) )
				{
					OpenSaveDialog();
				}

				if( m_pScriptFile )
				{
					const bool fResult = m_pScriptFile->SetScriptContents( this->toPlainText().toStdString() );

					if( fResult )
					{
						SetUnsavedChangesMade( false );
						result = SaveResult::Saved;
					}
					else
					{
						QMessageBox errorBox;

						errorBox.setText( "An error occured." );
						errorBox.setInformativeText( "An error occured while trying to save the file." );
						errorBox.exec();

						result = SaveResult::Canceled;
					}
				}

				break;
			}

		case QMessageBox::Discard: result = SaveResult::Discarded; break;
		case QMessageBox::Cancel: result = SaveResult::Canceled; break;
		default: break;
		}
	}
	else
		result = SaveResult::Saved;

	return result;
}

bool CScriptCodeTextEdit::OpenSaveDialog()
{
	auto options = m_App->GetOptions();

	const QString szDir = options->GetCurrentDirectory().c_str();

	auto config = m_App->GetActiveConfiguration();

	QString szExtensions = config ? ui::FormatExtensions( config->GetExtensions() ) : ui::ANY_EXTENSION;

	std::string szFilename = QFileDialog::getSaveFileName(
				this, tr( "Save script" ), szDir,
				QString( tr( "Angelscript script files" ) + "(%1)" ).arg( szExtensions ) ).toStdString();

	bool fResult = !szFilename.empty();

	if( fResult )
	{
		m_pScriptFile = std::make_shared<CScriptFile>( szFilename );

		QFileInfo file( szFilename.c_str() );

		options->SetCurrentDirectory( file.canonicalPath().toStdString() );

		m_szName = file.fileName().toStdString();

		NameChanged( m_szName );
	}

	return fResult;
}

void CScriptCodeTextEdit::ContentChanged()
{
	NameChanged( m_szName + '*' );
}

void CScriptCodeTextEdit::UndoStateChanged( bool fState )
{
	if( !fState )
		SetUnsavedChangesMade( false );
}
