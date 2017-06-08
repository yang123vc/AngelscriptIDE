#include <QPainter>
#include <QTextBlock>

#include "CCodeTextEdit.h"

#include "CLineNumberArea.h"

CCodeTextEdit::CCodeTextEdit( QWidget* pParent )
	: QPlainTextEdit( pParent )
{
	m_pLineNumberArea = new CLineNumberArea( this );

	connect( this, &CCodeTextEdit::blockCountChanged, this, &CCodeTextEdit::updateLineNumberAreaWidth );
	connect( this, &CCodeTextEdit::updateRequest, this, &CCodeTextEdit::updateLineNumberArea );
	connect( this, &CCodeTextEdit::cursorPositionChanged, this, &CCodeTextEdit::highlightCurrentLine );

	updateLineNumberAreaWidth( 0 );
	highlightCurrentLine();
}

CCodeTextEdit::~CCodeTextEdit()
{
	delete m_pLineNumberArea;
}

int CCodeTextEdit::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax( 1, blockCount() );

	while( max >= 10 )
	{
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().width( QLatin1Char( '9' ) ) * digits;

	return space;
}

void CCodeTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins( lineNumberAreaWidth(), 0, 0, 0 );
}

void CCodeTextEdit::updateLineNumberArea( const QRect &rect, int dy )
{
	if ( dy )
		m_pLineNumberArea->scroll( 0, dy );
	else
		m_pLineNumberArea->update( 0, rect.y(), m_pLineNumberArea->width(), rect.height() );

	if ( rect.contains( viewport()->rect() ) )
		updateLineNumberAreaWidth( 0 );
}

void CCodeTextEdit::resizeEvent( QResizeEvent *e )
{
	QPlainTextEdit::resizeEvent( e );

	QRect cr = contentsRect();
	m_pLineNumberArea->setGeometry( QRect( cr.left(), cr.top(), lineNumberAreaWidth(), cr.height() ) );
}

void CCodeTextEdit::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if( !isReadOnly() )
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor( Qt::yellow ).lighter( 160 );

		selection.format.setBackground( lineColor );
		selection.format.setProperty( QTextFormat::FullWidthSelection, true );
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append( selection );
	}

	setExtraSelections( extraSelections );
}

void CCodeTextEdit::lineNumberAreaPaintEvent( QPaintEvent *event )
{
	QPainter painter( m_pLineNumberArea );
	painter.fillRect( event->rect(), Qt::lightGray );

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while( block.isValid() && top <= event->rect().bottom() )
	{
		if( block.isVisible() && bottom >= event->rect().top() )
		{
			QString number = QString::number( blockNumber + 1 );
			painter.setPen( Qt::black );
			painter.drawText( 0, top, m_pLineNumberArea->width(), fontMetrics().height(),
				Qt::AlignRight, number );
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
}
