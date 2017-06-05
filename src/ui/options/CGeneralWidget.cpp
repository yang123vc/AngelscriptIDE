#include "Angelscript/CConfiguration.h"
#include "Angelscript/IConfigurationManager.h"

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
		m_App->GetOptions()->SetTabWidth( m_WidgetUI->m_pTabWidthSpinner->value() );

		m_App->GetOptions()->GetConfigurationManager()->SetActiveConfiguration( m_WidgetUI->m_pActiveConfigComboBox->currentText().toStdString() );

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

	auto configManager = m_App->GetOptions()->GetConfigurationManager();

	//Empty active config option
	m_WidgetUI->m_pActiveConfigComboBox->addItem( "" );

	const auto uiCount = configManager->GetConfigurationCount();

	for( size_t uiIndex = 0; uiIndex < uiCount; ++uiIndex )
	{
		m_WidgetUI->m_pActiveConfigComboBox->addItem( configManager->GetConfiguration( uiIndex )->GetName().c_str() );
	}

	auto active = configManager->GetActiveConfiguration();

	if( active )
	{
		const int iIndex = m_WidgetUI->m_pActiveConfigComboBox->findText( active->GetName().c_str() );

		if( iIndex != -1 )
			m_WidgetUI->m_pActiveConfigComboBox->setCurrentIndex( iIndex );
	}
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
