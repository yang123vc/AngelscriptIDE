#ifndef IDE_UI_CANGELSCRIPTSYNTAXHIGHLIGHTER_H
#define IDE_UI_CANGELSCRIPTSYNTAXHIGHLIGHTER_H

#include <memory>

#include <QSyntaxHighlighter>

class COptions;

class CAngelscriptSyntaxHighlighter : public QSyntaxHighlighter
{
public:
	explicit CAngelscriptSyntaxHighlighter( QTextDocument* parent, std::shared_ptr<COptions> options );

	void highlightBlock( const QString& text ) override;

private:
	std::shared_ptr<COptions> m_Options;
};

#endif //IDE_UI_CANGELSCRIPTSYNTAXHIGHLIGHTER_H
