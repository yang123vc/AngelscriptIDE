#include "Angelscript/CConfiguration.h"

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "ui_CGeneralWidget.h"

#include "CGeneralWidget.h"

CGeneralWidget::CGeneralWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent )
	: CBaseOptionsWidget( app, ui, pParent )
	, m_WidgetUI( new Ui::CGeneralWidget )
{
	m_WidgetUI->setupUi( this );

	LoadConfigurations();

	m_WidgetUI->m_pTabWidthSpinner->setValue( m_App->GetOptions()->GetTabWidth() );

	connect( m_WidgetUI->m_pActiveConfigComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ActiveConfigurationChanged( int ) ) );
	connect( m_WidgetUI->m_pTabWidthSpinner, SIGNAL( valueChanged( int ) ), this, SLOT( TabWidthChanged( int ) ) );
}

CGeneralWidget::~CGeneralWidget()
{
}

bool CGeneralWidget::CanSave( QString& )
{
	return true;
}

void CGeneralWidget::ApplyChanges()
{
	if( HaveChangesBeenMade() )
	{
		const std::string szNewConfig = m_WidgetUI->m_pActiveConfigComboBox->currentText().toStdString();

		m_App->SetActiveConfiguration( szNewConfig );

		auto options = m_App->GetOptions();

		options->SetActiveConfigurationName( szNewConfig );
		options->SetTabWidth( m_WidgetUI->m_pTabWidthSpinner->value() );
		options->Save();

		SetChangesMade( false );
	}
}

void CGeneralWidget::PostApply()
{
	LoadConfigurations();
}

void CGeneralWidget::LoadConfigurations()
{
	m_WidgetUI->m_pActiveConfigComboBox->clear();

	auto options = m_App->GetOptions();

	const auto& configs = options->GetConfigurations();

	//Empty active config option
	m_WidgetUI->m_pActiveConfigComboBox->addItem( "" );

	for( const auto& config : configs )
		m_WidgetUI->m_pActiveConfigComboBox->addItem( config.c_str() );

	const auto& szActiveConfig = options->GetActiveConfigurationName();

	const int iIndex = m_WidgetUI->m_pActiveConfigComboBox->findText( szActiveConfig.c_str() );

	if( iIndex != -1 )
		m_WidgetUI->m_pActiveConfigComboBox->setCurrentIndex( iIndex );
}

void CGeneralWidget::ActiveConfigurationChanged( int iIndex )
{
	if( !HaveChangesBeenMade() )
		SetChangesMade( true );

	m_WidgetUI->m_pActiveConfigComboBox->setCurrentIndex( iIndex );
}

void CGeneralWidget::TabWidthChanged( int )
{
	if( !HaveChangesBeenMade() )
		SetChangesMade( true );
}
