#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "ui/CUI.h"

#include "CBaseOptionsWidget.h"
#include "CGeneralWidget.h"
#include "CConfigurationsWidget.h"

#include "COptionsDialog.h"

COptionsDialog::COptionsDialog( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: QDialog( pParent )
	, m_App( app )
	, m_UI( ui )
{
	setModal( true );

	QTabWidget* pTabs = new QTabWidget();

	CGeneralWidget* pGeneral		= new CGeneralWidget( m_App, ui, this );
	CConfigurationsWidget* pConfig	= new CConfigurationsWidget( m_App, ui, this );

	m_Widgets.push_back( pGeneral );
	m_Widgets.push_back( pConfig );

	pTabs->addTab( pGeneral, tr( "General" ) );
	pTabs->addTab( pConfig, tr( "Configurations" ) );

	m_pMessageLabel = new QLabel();
	m_pMessageLabel->setStyleSheet( "QLabel { color: red; }" ); //Red color

	QHBoxLayout* pButtonsLayout = new QHBoxLayout();

	QPushButton* pOkButton		= new QPushButton( tr( "Ok" ) );
	QPushButton* pCancelButton	= new QPushButton( tr( "Cancel" ) );
	m_pApplyButton				= new QPushButton( tr( "Apply" ) );

	pButtonsLayout->addWidget( pOkButton );
	pButtonsLayout->addWidget( pCancelButton );
	pButtonsLayout->addWidget( m_pApplyButton );

	connect( pOkButton,			SIGNAL( clicked() ), this, SLOT( Ok() ) );
	connect( pCancelButton,		SIGNAL( clicked() ), this, SLOT( Cancel() ) );
	connect( m_pApplyButton,	SIGNAL( clicked() ), this, SLOT( Apply() ) );

	m_pApplyButton->setEnabled( false );

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pMainLayout->addWidget( pTabs );
	pMainLayout->addWidget( m_pMessageLabel );
	pMainLayout->addLayout( pButtonsLayout );

	setLayout( pMainLayout );

	resize( 400, 600 );

	m_bInitialized = true;
}

COptionsDialog::~COptionsDialog()
{
}

void COptionsDialog::ApplyChanges()
{
	bool bCanSave = true;

	QString szReason;

	for( auto optionPanel : m_Widgets )
	{
		bCanSave = optionPanel->CanSave( szReason );

		if( !bCanSave )
			break;
	}

	if( bCanSave )
	{
		m_pMessageLabel->setText( "" );

		for( auto optionPanel : m_Widgets )
			optionPanel->ApplyChanges();

		for( auto optionPanel : m_Widgets )
			optionPanel->PostApply();

		m_App->SaveSettings();

		for( auto optionPanel : m_Widgets )
			optionPanel->SetChangesMade( false );

		m_pApplyButton->setEnabled( false );

		m_UI->SendMessage( "Options saved\n" );
	}
	else
		m_pMessageLabel->setText( szReason );
}

void COptionsDialog::Ok()
{
	ApplyChanges();
	accept();
}

void COptionsDialog::Cancel()
{
	reject();
}

void COptionsDialog::Apply()
{
	ApplyChanges();
}

void COptionsDialog::ChangesMade( bool )
{
	//Ignore changes made during construction
	if( !m_bInitialized )
		return;

	m_pApplyButton->setEnabled( true );
}
