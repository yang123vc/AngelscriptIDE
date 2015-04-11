#ifndef CCODETEXTEDIT_H
#define CCODETEXTEDIT_H

#include <QPlainTextEdit>

class CCodeTextEdit : public QPlainTextEdit
{
	 Q_OBJECT

public:
	CCodeTextEdit( QWidget* pParent = nullptr );
	virtual ~CCodeTextEdit();

	void lineNumberAreaPaintEvent( QPaintEvent *event );
	int lineNumberAreaWidth();

protected:
	void resizeEvent( QResizeEvent *event );

private slots:
	 void updateLineNumberAreaWidth( int newBlockCount );
	 void highlightCurrentLine();
	 void updateLineNumberArea( const QRect&, int );

private:
	QWidget* m_pLineNumberArea;
};

#endif // CCODETEXTEDIT_H
