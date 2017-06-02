#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QWindowStateChangeEvent>
#include <QAction>

#include <iostream>
#include <chrono>
#include <initializer_list>

#include "util/CDispatchingOutStream.h"

#include "Angelscript/CScript.h"
#include "Angelscript/CConfiguration.h"

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "options/COptionsDialog.h"

#include "util/UIUtils.h"

#include "CUI.h"

#include "CAbout.h"
#include "CScriptCodeTextEdit.h"

#include "CCompileOutputWidget.h"
#include "CInformationOutputWidget.h"

#include "CFindDialog.h"

#include "ui_CASMainWindow.h"
#include "CASMainWindow.h"

const char* CASMainWindow::OUTPUT_WINDOW_PROPERTY = "OutputWindowType";

CASMainWindow::CASMainWindow( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* parent )
	: QMainWindow( parent )
	, m_WidgetUI( new Ui::CASMainWindow )
	, m_App( app )
	, m_UI( ui )
{
	m_WidgetUI->setupUi( this );

	m_App->AddAppListener( this );

	//File menu
	connect( m_WidgetUI->actionNew, SIGNAL( triggered() ), this, SLOT( NewFile() ) );
	connect( m_WidgetUI->actionOpen, SIGNAL( triggered() ), this, SLOT( OpenFile() ) );
	connect( m_WidgetUI->actionSave, SIGNAL( triggered() ), this, SLOT( Save() ) );
	connect( m_WidgetUI->actionSave_As, SIGNAL( triggered() ), this, SLOT( SaveAs() ) );
	connect( m_WidgetUI->actionClose, SIGNAL( triggered() ), this, SLOT( CloseFile() ) );
	connect( m_WidgetUI->actionClose_All, SIGNAL( triggered() ), this, SLOT( CloseAllFiles() ) );
	connect( m_WidgetUI->actionExit, SIGNAL( triggered() ), this, SLOT( OnExit() ) );

	//Edit menu
	connect( m_WidgetUI->actionUndo, SIGNAL( triggered() ), this, SLOT( Undo() ) );
	connect( m_WidgetUI->actionRedo, SIGNAL( triggered() ), this, SLOT( Redo() ) );

	connect( m_WidgetUI->actionCut, SIGNAL( triggered() ), this, SLOT( Cut() ) );
	connect( m_WidgetUI->actionCopy, SIGNAL( triggered() ), this, SLOT( Copy() ) );
	connect( m_WidgetUI->actionPaste, SIGNAL( triggered() ), this, SLOT( Paste() ) );

	connect( m_WidgetUI->actionSelect_All, SIGNAL( triggered() ), this, SLOT( SelectAll() ) );

	connect( m_WidgetUI->actionFind, SIGNAL( triggered() ), this, SLOT( OpenFindDialog() ) );

	//Script menu
	connect( m_WidgetUI->actionCompile, SIGNAL( triggered() ), this, SLOT( CompileScript() ) );
	connect( m_WidgetUI->actionReload_configuration, SIGNAL( triggered() ), this, SLOT( ReloadConfiguration() ) );

	//View menu
	connect( m_WidgetUI->actionOutput, SIGNAL( triggered() ), this, SLOT( AddOutputWindow() ) );
	connect( m_WidgetUI->actionInformation, SIGNAL( triggered() ), this, SLOT( AddInformationWindow() ) );

	//Tools menu
	connect( m_WidgetUI->actionClear_Recent_Files_List, SIGNAL( triggered() ), this, SLOT( ClearRecentFiles() ) );
	connect( m_WidgetUI->actionOptions, SIGNAL( triggered() ), this, SLOT( ShowOptions() ) );

	//Help menu
	connect( m_WidgetUI->actionAbout, SIGNAL( triggered() ), this, SLOT( ShowAbout() ) );

	//Set up responses to ui events
	connect( m_WidgetUI->m_pFiles, SIGNAL( tabCloseRequested( int ) ), this, SLOT( CloseFile( int ) ) );
	connect( m_WidgetUI->m_pFiles, SIGNAL( currentChanged( int ) ), this, SLOT( TabChanged( int ) ) );

	connect( m_WidgetUI->m_pOutputWindows, SIGNAL( tabCloseRequested( int ) ), this, SLOT( CloseOutputWindow( int ) ) );

	//Disable buttons that require an open script
	SetDependentActionsState( false );

	//Set window maximized state
	auto options = m_App->GetOptions();

	if( options->StartMaximized() )
		setWindowState( windowState() | Qt::WindowMaximized );
	else
		setWindowState( windowState() & ~Qt::WindowMaximized );

	m_pNoRecentFilesAction = new QAction( "No recent files", nullptr );
	m_pNoRecentFilesAction->setEnabled( false );

	const auto& recentFiles = options->GetRecentFiles();

	if( !recentFiles.empty() )
	{
		for( const auto& recentFile : recentFiles )
			AddRecentFile( recentFile );
	}
	else
		m_WidgetUI->menuRecent_Files->addAction( m_pNoRecentFilesAction );

	//Open output windows
	AddOutputWindow( OutputWindow::OUTPUT );
	AddOutputWindow( OutputWindow::INFORMATION );

	m_App->LoadActiveConfiguration();
}

CASMainWindow::~CASMainWindow()
{
	m_App->RemoveAppListener( this );

	ClearRecentFilesActions();

	delete m_pNoRecentFilesAction;
}

void CASMainWindow::AppStartedUp()
{
}

void CASMainWindow::AppShutdown()
{
	//This is failsafe to let users save their work in case of unexpected shutdown
	CloseAllFiles( true );
}

void CASMainWindow::closeEvent( QCloseEvent* pEvent )
{
	//User saved or discarded all modified open files; close window
	if( CloseAllFiles( false, true ) )
		QMainWindow::closeEvent( pEvent );
	else //User canceled at least once
		pEvent->ignore();
}

void CASMainWindow::changeEvent( QEvent* pEvent )
{
	if( pEvent->type() == QEvent::WindowStateChange )
	{
		auto options = m_App->GetOptions();

		options->SetStartMaximized( windowState() & Qt::WindowMaximized );
	}

	QMainWindow::changeEvent( pEvent );
}

QWidget* CASMainWindow::GetOutputWindow( OutputWindow window ) const
{
	for( int iIndex = 0; iIndex < m_WidgetUI->m_pOutputWindows->count(); ++iIndex )
	{
		QWidget* pWidget = m_WidgetUI->m_pOutputWindows->widget( iIndex );

		QVariant value = pWidget->property( OUTPUT_WINDOW_PROPERTY );

		bool fResult;

		if( value.isValid() && value.toInt( &fResult ) == static_cast<int>( window ) && fResult )
			return pWidget;
	}

	return nullptr;
}

void CASMainWindow::AddOutputWindow( OutputWindow window )
{
	//Already present; activate window
	if( QWidget* pWidget = GetOutputWindow( window ) )
	{
		m_WidgetUI->m_pOutputWindows->setCurrentWidget( pWidget );
		return;
	}

	CBaseOutputWidget* pWidget = nullptr;

	switch( window )
	{
	//case OutputWindow::ErrorList: pWidget = new CErrorListWidget( m_App ); break;
	case OutputWindow::OUTPUT: pWidget = new CCompileOutputWidget( m_App, m_UI ); break;
	case OutputWindow::INFORMATION: pWidget = new CInformationOutputWidget( m_App, m_UI ); break;

	default: std::cout << "Attempted to instance unknown output window '" << static_cast<int>( window ) << "'!" << std::endl;
	}

	if( pWidget )
	{
		pWidget->setProperty( OUTPUT_WINDOW_PROPERTY, static_cast<int>( window ) );

		int iIndex = m_WidgetUI->m_pOutputWindows->addTab( pWidget, pWidget->GetName() );

		m_WidgetUI->m_pOutputWindows->setCurrentIndex( iIndex );
	}
}

CScriptCodeTextEdit* CASMainWindow::GetActiveScript() const
{
	QWidget* pActiveFile = m_WidgetUI->m_pFiles->currentWidget();

	return pActiveFile ? static_cast<CScriptCodeTextEdit*>( pActiveFile ) : nullptr;
}

/*
 * Sets the state of all actions dependent on the state of the current active script
*/
void CASMainWindow::SetDependentActionsState( bool fState )
{
	auto list =
	{
		m_WidgetUI->actionSave,
		m_WidgetUI->actionSave_As,
		m_WidgetUI->actionClose,
		m_WidgetUI->actionClose_All,
		m_WidgetUI->actionUndo,
		m_WidgetUI->actionRedo,
		m_WidgetUI->actionCut,
		m_WidgetUI->actionCopy,
		m_WidgetUI->actionPaste,
		m_WidgetUI->actionSelect_All,
		m_WidgetUI->actionFind,
		m_WidgetUI->actionCompile
	};

	for( auto action : list )
		action->setEnabled( fState );
}

CScriptCodeTextEdit* CASMainWindow::FindOpenFile( const QString& szFilename, int* pIndex )
{
	if( pIndex )
		*pIndex = -1;

	if( szFilename.isEmpty() )
		return nullptr;

	const QString szAbsFilename = QFileInfo( szFilename ).absoluteFilePath();

	QTabWidget* pTabs = m_WidgetUI->m_pFiles;

	const auto iSize = pTabs->count();

	CScriptCodeTextEdit* pEdit;

	for( decltype( pTabs->count() ) iIndex = 0; iIndex < iSize; ++iIndex )
	{
		pEdit = static_cast<CScriptCodeTextEdit*>( pTabs->widget( iIndex ) );

		if( !pEdit->HasFile() )
			continue;

		if( QFileInfo( pEdit->GetName().c_str() ).absoluteFilePath() == szAbsFilename )
		{
			if( pIndex )
				*pIndex = iIndex;

			return pEdit;
		}
	}

	return nullptr;
}

void CASMainWindow::AddFile( CScriptCodeTextEdit* pCodeEdit )
{
	const int iIndex = m_WidgetUI->m_pFiles->addTab( pCodeEdit, pCodeEdit->GetName().c_str() );

	connect( pCodeEdit, SIGNAL( NameChanged( std::string ) ), this, SLOT( ScriptNameChanged( std::string ) ) );

	m_WidgetUI->m_pFiles->setCurrentIndex( iIndex );
}

void CASMainWindow::OpenFile( const QString& szFilename )
{
	if( szFilename.isEmpty() )
		return;

	AddRecentFileToOptions( szFilename.toStdString() );

	int iIndex;

	//If it's already open, make active
	if( FindOpenFile( szFilename, &iIndex ) )
	{
		m_WidgetUI->m_pFiles->setCurrentIndex( iIndex );
		return;
	}

	QFileInfo file( szFilename );

	AddFile( new CScriptCodeTextEdit( file.fileName().toStdString(), szFilename.toStdString(), m_App ) );
}


/*
 * Returns true if the user either saved or discarded the contents
*/
bool CASMainWindow::CloseFile( CScriptCodeTextEdit* pCodeEdit, bool fForceClose )
{
	bool fCanceled = false;

	if( pCodeEdit )
	{
		if( pCodeEdit->UnsavedChangedMade() )
		{
			if( pCodeEdit->Save() == CScriptCodeTextEdit::SaveResult::CANCELED )
				fCanceled = true;
		}

		if( fForceClose || !fCanceled )
		{
			m_WidgetUI->m_pFiles->removeTab( m_WidgetUI->m_pFiles->indexOf( pCodeEdit ) );

			delete pCodeEdit;
		}
	}

	return !fCanceled;
}

void CASMainWindow::AddRecentFile( const std::string& szFilename )
{
	QMenu* pMenu = m_WidgetUI->menuRecent_Files;

	QList<QAction*> actions = pMenu->actions();

	auto it = std::find( actions.begin(), actions.end(), szFilename );

	if( it != actions.end() )
	{
		delete *it;
		actions.erase( it );
	}

	QAction* pAction = new QAction( szFilename.c_str(), nullptr );

	connect( pAction, SIGNAL( triggered() ), this, SLOT( OpenRecentFile() ) );

	if( actions.size() == 1 && actions[ 0 ] == m_pNoRecentFilesAction )
	{
		pMenu->removeAction( m_pNoRecentFilesAction );

		actions.removeFirst();
	}

	pMenu->insertAction( !actions.isEmpty() ? actions[ 0 ] : nullptr, pAction );

	actions.insert( actions.begin(), pAction );

	if( static_cast<size_t>( actions.size() ) > COptions::MAX_RECENT_FILES )
	{
		auto it = actions.begin() + COptions::MAX_RECENT_FILES;

		auto end = actions.end();

		actions.erase( it, end );

		for( ; it != end; ++it )
			delete *it;
	}
}

void CASMainWindow::AddRecentFileToOptions( const std::string& szFilename )
{
	QFileInfo file( szFilename.c_str() );

	const std::string szAbsolutePath = file.absoluteFilePath().toStdString();

	m_App->GetOptions()->AddRecentFile( szAbsolutePath );

	AddRecentFile( szAbsolutePath );
}

void CASMainWindow::ClearRecentFilesActions()
{
	QList<QAction*> actions = m_WidgetUI->menuRecent_Files->actions();

	m_WidgetUI->menuRecent_Files->clear();

	for( auto pAction : actions )
	{
		//Don't delete this particular action; we need it
		if( pAction != m_pNoRecentFilesAction )
			delete pAction;
	}

	m_WidgetUI->menuRecent_Files->addAction( m_pNoRecentFilesAction );
}

void CASMainWindow::NewFile()
{
	AddFile( new CScriptCodeTextEdit( "New File", m_App ) );
}

void CASMainWindow::OpenFile()
{
	auto options = m_App->GetOptions();

	const QString szDir = options->GetCurrentDirectory().c_str();

	auto config = m_App->GetActiveConfiguration();

	QString szExtensions = config ? ui::FormatExtensions( config->GetExtensions() ) : ui::ANY_EXTENSION;

	QString szFilename = QFileDialog::getOpenFileName(
				this, tr( "Select script file" ), szDir,
				QString( tr( "Angelscript script files" ) + "(%1)" ).arg( szExtensions ) ).trimmed();

	if( szFilename.isEmpty() )
		return;

	QFileInfo file( szFilename );

	options->SetCurrentDirectory( file.canonicalPath().toStdString() );

	OpenFile( szFilename );
}

void CASMainWindow::Save()
{
	CScriptCodeTextEdit* pCodeEdit = GetActiveScript();

	if( pCodeEdit )
	{
		const bool fHadFile = pCodeEdit->HasFile();

		if( pCodeEdit->Save( CScriptCodeTextEdit::SaveMode::ALWAYS, CScriptCodeTextEdit::PromptMode::NEVER ) == CScriptCodeTextEdit::SaveResult::SAVED )
		{
			if( !fHadFile )
				AddRecentFileToOptions( pCodeEdit->GetName() );
		}
	}
}

void CASMainWindow::SaveAs()
{
	CScriptCodeTextEdit* pCodeEdit = GetActiveScript();

	if( pCodeEdit )
	{
		if( pCodeEdit->Save(
					CScriptCodeTextEdit::SaveMode::ALWAYS,
					CScriptCodeTextEdit::PromptMode::NEVER,
					CScriptCodeTextEdit::FileSelectMode::ALWAYS ) == CScriptCodeTextEdit::SaveResult::SAVED )
		{
			AddRecentFileToOptions( pCodeEdit->GetName() );
		}
	}
}

void CASMainWindow::CloseFile()
{
	CloseFile( GetActiveScript() );
}

/*
 * Returns true if the user saved or discarded the contents of every open file
 * If fForceCloseAllOpenFiles is false, will stop as soon as the user cancels once
*/
bool CASMainWindow::CloseAllFiles( bool fForceCloseAllOpenFiles, bool fContinueAfterCancel )
{
	bool fResult = true;

	for( auto iIndex = m_WidgetUI->m_pFiles->count(); ( iIndex >= 0 ) && ( fForceCloseAllOpenFiles || fContinueAfterCancel || fResult ); --iIndex )
		fResult = CloseFile( static_cast<CScriptCodeTextEdit*>( m_WidgetUI->m_pFiles->widget( iIndex ) ), fForceCloseAllOpenFiles ) && fResult;

	return fResult;
}

bool CASMainWindow::CloseAllFiles()
{
	return CloseAllFiles( false, true );
}

void CASMainWindow::OpenRecentFile()
{
	QAction* pAction = static_cast<QAction*>( sender() );

	OpenFile( pAction->text() );
}

void CASMainWindow::OnExit()
{
	QApplication::instance()->exit();
}

void CASMainWindow::Undo()
{
	auto script = GetActiveScript();

	if( script )
		script->undo();
}

void CASMainWindow::Redo()
{
	auto script = GetActiveScript();

	if( script )
		script->redo();
}

void CASMainWindow::Cut()
{
	auto script = GetActiveScript();

	if( script )
		script->cut();
}

void CASMainWindow::Copy()
{
	auto script = GetActiveScript();

	if( script )
		script->copy();
}

void CASMainWindow::Paste()
{
	auto script = GetActiveScript();

	if( script )
		script->paste();
}

void CASMainWindow::SelectAll()
{
	auto script = GetActiveScript();

	if( script )
		script->selectAll();
}

void CASMainWindow::OpenFindDialog()
{
	CFindDialog find( this );

	connect( &find, SIGNAL( Find( QString ) ), this, SLOT( Find( QString ) ) );

	find.exec();
}

void CASMainWindow::Find( const QString& szString )
{
	auto script = GetActiveScript();

	if( script )
	{
		CFindDialog* pFind = static_cast<CFindDialog*>( sender() );

		QTextDocument::FindFlags flags;

		if( pFind->GetSearchDirection() == CFindDialog::SearchDirection::BACKWARD )
			flags |= QTextDocument::FindBackward;

		if( pFind->MatchCase() )
			flags |= QTextDocument::FindCaseSensitively;

		if( pFind->MatchWholeWord() )
			flags |= QTextDocument::FindWholeWords;

		script->find( szString, flags );
	}
}

void CASMainWindow::CompileScript()
{
	CScriptCodeTextEdit* pCodeEdit = GetActiveScript();

	if( pCodeEdit )
	{
		auto script = pCodeEdit->GetScriptFile();

		if( !script )
		{
			//User didn't want to save file or save failed; cancel compilation
			if( pCodeEdit->Save( CScriptCodeTextEdit::SaveMode::ALWAYS ) != CScriptCodeTextEdit::SaveResult::SAVED )
				return;
		}
		else
			pCodeEdit->Save();

		script = pCodeEdit->GetScriptFile();

		//TODO: can pass the script directly? - Solokiller
		m_App->CompileScript(
					pCodeEdit->GetName(),
					pCodeEdit->toPlainText().toStdString() );
	}
}

void CASMainWindow::ReloadConfiguration()
{
	m_App->SetActiveConfiguration( m_App->GetOptions()->GetActiveConfigurationName() );
}

void CASMainWindow::AddOutputWindow()
{
	AddOutputWindow( OutputWindow::OUTPUT );
}

void CASMainWindow::AddInformationWindow()
{
	AddOutputWindow( OutputWindow::INFORMATION );
}

void CASMainWindow::ClearRecentFiles()
{
	ClearRecentFilesActions();

	m_App->GetOptions()->ClearRecentFiles();
}

void CASMainWindow::ShowOptions()
{
	COptionsDialog options( m_App, m_UI, this );

	options.exec();
}

void CASMainWindow::ShowAbout()
{
	CAbout about( this );

	about.exec();
}

void CASMainWindow::CloseFile( int iIndex )
{
	CScriptCodeTextEdit* pWidget = static_cast<CScriptCodeTextEdit*>( m_WidgetUI->m_pFiles->widget( iIndex ) );

	CScriptCodeTextEdit::SaveResult result = pWidget->Save();

	if( result != CScriptCodeTextEdit::SaveResult::CANCELED )
	{
		m_WidgetUI->m_pFiles->removeTab( iIndex );

		delete pWidget;
	}
}

void CASMainWindow::ScriptNameChanged( const std::string& szName )
{
	CScriptCodeTextEdit* pSender = static_cast<CScriptCodeTextEdit*>( sender() );

	int iIndex = m_WidgetUI->m_pFiles->indexOf( pSender );

	if( iIndex != -1 )
		m_WidgetUI->m_pFiles->setTabText( iIndex, szName.c_str() );
}

void CASMainWindow::TabChanged( int iIndex )
{
	SetDependentActionsState( iIndex != -1 );
}

void CASMainWindow::CloseOutputWindow( int iIndex )
{
	m_WidgetUI->m_pOutputWindows->removeTab( iIndex );
}
