#include <QRegularExpression>

#include "ide/COptions.h"

#include "CAngelscriptSyntaxHighlighter.h"

CAngelscriptSyntaxHighlighter::CAngelscriptSyntaxHighlighter( QTextDocument* parent, std::shared_ptr<COptions> options )
	: QSyntaxHighlighter( parent )
	, m_Options( options )
{
}

void CAngelscriptSyntaxHighlighter::highlightBlock( const QString& text )
{
	for( const auto& pattern : m_Options->GetPatterns() )
	{
		//Skip empty patterns
		if( pattern.m_szPattern.isEmpty() )
			continue;

		QTextCharFormat format;
		format.setForeground( pattern.m_FGColor );
		format.setBackground( pattern.m_BGColor );
		format.setFontWeight( pattern.m_bBold ? QFont::Bold : QFont::Normal );
		format.setFontUnderline( pattern.m_bUnderline );

		QRegularExpression expression( pattern.m_szPattern );

		auto it = expression.globalMatch( text );

		while( it.hasNext() )
		{
			auto match = it.next();

			setFormat( match.capturedStart(), match.capturedLength(), format );
		}
	}
}
