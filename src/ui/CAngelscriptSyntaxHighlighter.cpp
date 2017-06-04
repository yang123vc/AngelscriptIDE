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
		QTextCharFormat format;
		format.setForeground( pattern.m_FGColor );
		format.setBackground( pattern.m_BGColor );
		format.setFontWeight( pattern.m_bBold ? QFont::Bold : QFont::Normal );
		format.setFontUnderline( pattern.m_bUnderline );

		QRegExp expression( pattern.m_szPattern );

		int iIndex = text.indexOf( expression );

		while( iIndex >= 0 )
		{
			int length = expression.matchedLength();
			setFormat( iIndex, length, format );
			iIndex = text.indexOf( expression, iIndex + length );
		}
	}
}
