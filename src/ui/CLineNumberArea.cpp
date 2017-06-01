#include "CLineNumberArea.h"

#include "CCodeTextEdit.h"

CLineNumberArea::CLineNumberArea( CCodeTextEdit* pParent )
	: QWidget( pParent )
	, m_pCodeEditor( pParent )
{

}

CLineNumberArea::~CLineNumberArea()
{

}

QSize CLineNumberArea::sizeHint() const
{
	return QSize( m_pCodeEditor->lineNumberAreaWidth(), 0 );
}

void CLineNumberArea::paintEvent( QPaintEvent* event )
{
	m_pCodeEditor->lineNumberAreaPaintEvent( event );
}
