#include <QInputDialog>
#include <QMessageBox>
#include <QDir>

#include "Angelscript/CConfiguration.h"
#include "Angelscript/CConfigurationException.h"
#include "Angelscript/IConfigurationManager.h"

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "ui/CUI.h"

#include "ui_CEditConfigurationsDialog.h"
#include "CEditConfigurationsDialog.h"

CEditConfigurationsDialog::CEditConfigurationsDialog( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: QDialog( pParent )
	, m_WidgetUI( std::make_unique<Ui::CEditConfigurationsDialog>() )
	, m_App( app )
	, m_UI( ui )
{
	m_WidgetUI->setupUi( this );

	auto configManager = m_App->GetOptions()->GetConfigurationManager();

	const auto uiCount = configManager->GetConfigurationCount();

	for( size_t uiIndex = 0; uiIndex < uiCount; ++uiIndex )
	{
		m_WidgetUI->m_pList->addItem( configManager->GetConfiguration( uiIndex )->GetName().c_str() );
	}

	connect( m_WidgetUI->m_pAdd, SIGNAL( clicked() ), this, SLOT( AddConfiguraton() ) );
	connect( m_WidgetUI->m_pRemove, SIGNAL( clicked() ), this, SLOT( RemoveConfiguration() ) );
	connect( m_WidgetUI->m_pOk, SIGNAL( clicked() ), this, SLOT( Ok() ) );
}

CEditConfigurationsDialog::~CEditConfigurationsDialog()
{
}

void CEditConfigurationsDialog::AddConfiguraton()
{
	bool bOk = true;

	do
	{
		QString szName = QInputDialog::getText( this, tr( "Add configuration" ), tr( "Enter a name for your configuration" ), QLineEdit::Normal, "", &bOk );

		szName = szName.trimmed();

		if( szName.isEmpty() )
		{
			QMessageBox message;

			message.setText( "Error" );
			message.setInformativeText( "Please enter a valid name" );

			message.exec();
		}
		else
		{
			try
			{
				auto config = std::make_shared<CConfiguration>( szName.toStdString() );

				if( m_App->GetOptions()->GetConfigurationManager()->AddConfiguration( config ) )
					m_WidgetUI->m_pList->addItem( szName );
			}
			catch( const CConfigurationException& e )
			{
				m_UI->SendMessage( e.what(), UIMessageType::ERROR );
			}

			bOk = false;
		}
	}
	while( bOk );
}

void CEditConfigurationsDialog::RemoveConfiguration()
{
	auto pWidget = m_WidgetUI->m_pList->currentItem();

	if( pWidget )
	{
		try
		{
			m_App->GetOptions()->GetConfigurationManager()->RemoveConfiguration( m_App->GetOptions()->GetConfigurationManager()->Find( pWidget->text().toStdString() ) );
		}
		catch( const CConfigurationException& e )
		{
			m_UI->SendMessage( e.what(), UIMessageType::ERROR );
		}

		//The item returned by takeItem is the same as currentItem
		m_WidgetUI->m_pList->takeItem( m_WidgetUI->m_pList->currentIndex().row() );

		delete pWidget;
	}
}

void CEditConfigurationsDialog::Ok()
{
	accept();
}
