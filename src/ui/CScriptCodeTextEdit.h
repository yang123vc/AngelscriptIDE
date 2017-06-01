#ifndef CSCRIPTCODETEXTEDIT_H
#define CSCRIPTCODETEXTEDIT_H

#include <memory>
#include <string>

#include "CCodeTextEdit.h"

class CScriptFile;

class CASIDEApp;

class CScriptCodeTextEdit : public CCodeTextEdit
{
	Q_OBJECT

public:
	enum SaveMode
	{
		Save_IfNeeded	= 0,
		Save_Always,
	};

	enum PromptMode
	{
		Prompt_IfNeeded		= 0,
		Prompt_Always,
		Prompt_Never,
	};

	enum FileSelectMode
	{
		FileSelect_IfNeeded	= 0,
		FileSelect_Always,
		FileSelect_Never,
	};

	enum class SaveResult
	{
		Saved,
		Discarded,
		Canceled
	};

public:

	CScriptCodeTextEdit( const std::string& szFilename, std::shared_ptr<CASIDEApp> app, QWidget* pParent = nullptr );
	CScriptCodeTextEdit( const std::string& szName, const std::string& szFilename, std::shared_ptr<CASIDEApp> app, QWidget* pParent = nullptr );
	virtual ~CScriptCodeTextEdit();

	const std::string& GetName() const { return m_szName; }

	std::shared_ptr<CScriptFile> GetScriptFile() const { return m_pScriptFile; }

	bool HasFile() const { return m_pScriptFile != nullptr; }

	bool UnsavedChangedMade() const;

	void SetUnsavedChangesMade( bool fState );

	SaveResult Save( int iSaveMode = Save_IfNeeded, int iPromptMode = Prompt_IfNeeded, int iFileSelect = FileSelect_IfNeeded );

protected:

	bool OpenSaveDialog();

signals:

	void NameChanged( const std::string& szName );

protected slots:

	void ContentChanged();

	void UndoStateChanged( bool fState );

private:
	std::string m_szName;
	std::shared_ptr<CScriptFile> m_pScriptFile;
	std::shared_ptr<CASIDEApp> m_App;
};

#endif // CSCRIPTCODETEXTEDIT_H
