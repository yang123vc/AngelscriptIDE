#ifndef CABOUT_H
#define CABOUT_H

#include <QDialog>

namespace Ui {
class CAbout;
}

class CAbout : public QDialog
{
	Q_OBJECT

public:
	explicit CAbout( QWidget* pParent = nullptr );
	~CAbout();

public slots:

	void Close();

private:
	Ui::CAbout *ui;
};

#endif // CABOUT_H
