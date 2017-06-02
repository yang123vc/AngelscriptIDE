#ifndef IDE_UI_CLINENUMBERAREA_H
#define IDE_UI_CLINENUMBERAREA_H

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

#endif //IDE_UI_CLINENUMBERAREA_H
