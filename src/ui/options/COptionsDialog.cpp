#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "ui/CUI.h"

#include "CBaseOptionsWidget.h"
#include "CConfigurationsWidget.h"
#include "CGeneralWidget.h"
#include "CSyntaxHighlightWidget.h"

#include "COptionsDialog.h"
#include "ui_COptionsDialog.h"

COptionsDialog::COptionsDialog( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: QDialog( pParent )
	, m_App( app )
	, m_UI( ui )
	, m_WidgetUI( std::make_unique<Ui::COptionsDialog>() )
{
	m_WidgetUI->setupUi( this );

	setModal( true );

	auto pGeneral	= new CGeneralWidget( m_App, ui, this );
	auto pConfig	= new CConfigurationsWidget( m_App, ui, this );
	auto pSyntax	= new CSyntaxHighlightWidget( m_App, ui, this );

	m_Widgets.push_back( pGeneral );
	m_Widgets.push_back( pConfig );
	m_Widgets.push_back( pSyntax );

	m_WidgetUI->m_pTabs->addTab( pGeneral, tr( "General" ) );
	m_WidgetUI->m_pTabs->addTab( pConfig, tr( "Configurations" ) );
	m_WidgetUI->m_pTabs->addTab( pSyntax, tr( "Syntax Highlighting" ) );

	connect( m_WidgetUI->m_pOk,			&QPushButton::clicked, this, &COptionsDialog::Ok );
	connect( m_WidgetUI->m_pCancel,		&QPushButton::clicked, this, &COptionsDialog::Cancel );
	connect( m_WidgetUI->m_pApply,		&QPushButton::clicked, this, &COptionsDialog::Apply );

	m_WidgetUI->m_pApply->setEnabled( false );

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
		m_WidgetUI->m_pMessageLabel->setText( "" );

		for( auto optionPanel : m_Widgets )
			optionPanel->ApplyChanges();

		for( auto optionPanel : m_Widgets )
			optionPanel->PostApply();

		m_App->SaveSettings();

		for( auto optionPanel : m_Widgets )
			optionPanel->SetChangesMade( false );

		m_WidgetUI->m_pApply->setEnabled( false );

		m_UI->SendMessage( "Options saved\n" );
	}
	else
		m_WidgetUI->m_pMessageLabel->setText( szReason );
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

	m_WidgetUI->m_pApply->setEnabled( true );
}
