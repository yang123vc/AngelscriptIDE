#ifndef IDE_UI_CSCRIPTCODETEXTEDIT_H
#define IDE_UI_CSCRIPTCODETEXTEDIT_H

#include <memory>
#include <string>

#include "CCodeTextEdit.h"

class CScript;

class CASIDEApp;
class CAngelscriptSyntaxHighlighter;

class CScriptCodeTextEdit : public CCodeTextEdit
{
	Q_OBJECT

public:
	enum class SaveMode
	{
		IF_NEEDED = 0,
		ALWAYS,
	};

	enum class PromptMode
	{
		IF_NEEDED = 0,
		ALWAYS,
		NEVER,
	};

	enum class FileSelectMode
	{
		IF_NEEDED = 0,
		ALWAYS,
		NEVER,
	};

	enum class SaveResult
	{
		SAVED,
		DISCARED,
		CANCELED
	};

public:

	CScriptCodeTextEdit( const std::string& szFilename, std::shared_ptr<CASIDEApp> app, QWidget* pParent = nullptr );
	CScriptCodeTextEdit( const std::string& szName, const std::string& szFilename, std::shared_ptr<CASIDEApp> app, QWidget* pParent = nullptr );
	virtual ~CScriptCodeTextEdit();

	const std::string& GetName() const { return m_szName; }

	std::shared_ptr<CScript> GetScriptFile() const { return m_pScriptFile; }

	bool HasFile() const { return m_pScriptFile != nullptr; }

	bool UnsavedChangedMade() const;

	void SetUnsavedChangesMade( bool fState );

	SaveResult Save( SaveMode saveMode = SaveMode::IF_NEEDED, PromptMode promptMode = PromptMode::IF_NEEDED, FileSelectMode fileSelect = FileSelectMode::IF_NEEDED );

	void RefreshSyntaxHighlights();

protected:

	bool OpenSaveDialog();

signals:

	void NameChanged( const std::string& szName );

protected slots:

	void ContentChanged();

	void UndoStateChanged( bool fState );

private:
	std::string m_szName;
	std::shared_ptr<CScript> m_pScriptFile;
	std::shared_ptr<CASIDEApp> m_App;

	std::unique_ptr<CAngelscriptSyntaxHighlighter> m_SyntaxHighlighter;
};

#endif //IDE_UI_CSCRIPTCODETEXTEDIT_H
