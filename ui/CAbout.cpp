#include "CAbout.h"
#include "ui_CAbout.h"

CAbout::CAbout(QWidget* pParent)
	: QDialog(pParent)
	, ui( new Ui::CAbout )
{
	ui->setupUi( this );

	connect( ui->OkButton, SIGNAL( clicked() ), this, SLOT( Close() ) );
}

CAbout::~CAbout()
{
	delete ui;
}

void CAbout::Close()
{
	this->accept();
}
