#ifndef IDE_UI_CASMAINWINDOW_H
#define IDE_UI_CASMAINWINDOW_H

#include <memory>
#include <vector>

#include <QMainWindow>

#include "util/CDispatchingOutStream.h"

class CScriptCodeTextEdit;

namespace Ui
{
class CASMainWindow;
}

class CASIDEApp;
class CUI;

class QAction;

class CASMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	enum class OutputWindow
	{
		OUTPUT,
		INFORMATION
	};

private:

	static const char* OUTPUT_WINDOW_PROPERTY;

public:
	explicit CASMainWindow( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* parent = nullptr );
	~CASMainWindow();

	void RefreshSyntaxHighlights();

protected:

	virtual void closeEvent( QCloseEvent* pEvent ) override;
	virtual void changeEvent( QEvent* pEvent ) override;

	QWidget* GetOutputWindow( OutputWindow window ) const;

	void AddOutputWindow( OutputWindow window );

	CScriptCodeTextEdit* GetActiveScript() const;

	void SetDependentActionsState( bool fState );

	CScriptCodeTextEdit* FindOpenFile( const QString& szFilename, int* pIndex = nullptr );

	void AddFile( CScriptCodeTextEdit* pCodeEdit );

	void OpenFile( const QString& szFilename );

	bool CloseFile( CScriptCodeTextEdit* pCodeEdit, bool fForceClose = false );

	void AddRecentFile( const std::string& szFilename );

	void AddRecentFileToOptions( const QString& szFilename );

	void ClearRecentFilesActions();

public slots:

	void OnAppBeforeRun();
	void OnAppShutdown();

	//File menu
	void NewFile();

	void OpenFile();

	void Save();

	void SaveAs();

	void CloseFile();

	bool CloseAllFiles( bool fForceCloseAllOpenFiles, bool fContinueAfterCancel = false );

	bool CloseAllFiles();

	void OpenRecentFile();

	void OnExit();

	//Edit menu
	void Undo();
	void Redo();

	void Cut();
	void Copy();
	void Paste();

	void SelectAll();

	void OpenFindDialog();

	//Find dialog slots
	void Find( const QString& szString );

	//Script menu
	void CompileScript();

	//View menu
	void AddOutputWindow();

	void AddInformationWindow();

	//Tools menu
	void ClearRecentFiles();

	void ShowOptions();

	//Help menu
	void ShowAbout();

	//Callbacks
	void CloseFile( int iIndex );

	void ScriptNameChanged( const std::string& szName );

	void TabChanged( int iIndex );

	void CloseOutputWindow( int iIndex );

private:
	std::unique_ptr<Ui::CASMainWindow> m_WidgetUI;
	std::shared_ptr<CASIDEApp> m_App;
	std::shared_ptr<CUI> m_UI;

	QAction* m_pNoRecentFilesAction;
};

#endif //IDE_UI_CASMAINWINDOW_H
