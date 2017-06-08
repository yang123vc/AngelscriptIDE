#ifndef IDE_UI_CSCRIPTCODETEXTEDIT_H
#define IDE_UI_CSCRIPTCODETEXTEDIT_H

#include <memory>
#include <string>

#include <QString>

#include "CCodeTextEdit.h"

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

	//Tag used to differentiate between new files and opened files
	struct IsFilenameTag_t
	{
	};

	static constexpr IsFilenameTag_t IsFilename = {};

public:

	CScriptCodeTextEdit( const QString& szTitle, std::shared_ptr<CASIDEApp> app, QWidget* pParent = nullptr );
	CScriptCodeTextEdit( const QString& szFilename, const IsFilenameTag_t& isFilename, std::shared_ptr<CASIDEApp> app, QWidget* pParent = nullptr );
	virtual ~CScriptCodeTextEdit();

	const QString& GetFilename() const { return m_szFilename; }

	/**
	*	@return Whether this is a new file, i.e. if the file has not been saved at least once
	*/
	bool IsNewFile() const { return m_bIsNewFile; }

	bool UnsavedChangedMade() const;

	void SetUnsavedChangesMade( bool fState );

	SaveResult Save( SaveMode saveMode = SaveMode::IF_NEEDED, PromptMode promptMode = PromptMode::IF_NEEDED, FileSelectMode fileSelect = FileSelectMode::IF_NEEDED );

	void RefreshSyntaxHighlights();

protected:

	bool OpenSaveDialog();

signals:

	void NameChanged( const QString& szName );

protected slots:

	void ContentChanged();

	void UndoStateChanged( bool fState );

	void OnOptionsChanged();

private:
	QString m_szFilename;
	bool m_bIsNewFile = true;
	std::shared_ptr<CASIDEApp> m_App;

	std::unique_ptr<CAngelscriptSyntaxHighlighter> m_SyntaxHighlighter;
};

#endif //IDE_UI_CSCRIPTCODETEXTEDIT_H
