#ifndef CLINENUMBERAREA_H
#define CLINENUMBERAREA_H

#include <QWidget>

class CCodeTextEdit;

class CLineNumberArea : public QWidget
{
	Q_OBJECT
public:
	explicit CLineNumberArea( CCodeTextEdit* pParent );
	~CLineNumberArea();

	QSize sizeHint() const;

protected:
	void paintEvent( QPaintEvent *event );

private:
	CCodeTextEdit* m_pCodeEditor;
};

#endif // CLINENUMBERAREA_H
