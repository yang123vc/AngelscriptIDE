#include <QInputDialog>
#include <QMessageBox>
#include <QDir>

#include "Angelscript/CConfiguration.h"
#include "Angelscript/CConfigurationException.h"
#include "Angelscript/IConfigurationManager.h"

#include "ide/COptions.h"
#include "ide/CASIDEApp.h"

#include "ui/CUI.h"

#include "ui_CEditConfigurationsDialog.h"
#include "CEditConfigurationsDialog.h"

CEditConfigurationsDialog::CEditConfigurationsDialog( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, QWidget* pParent )
	: QDialog( pParent )
	, m_WidgetUI( new Ui::CEditConfigurationsDialog )
	, m_App( app )
	, m_UI( ui )
{
	m_WidgetUI->setupUi( this );

	const auto& configs = m_App->GetOptions()->GetConfigurations();

	for( const auto& config : configs )
		m_WidgetUI->m_pList->addItem( config.c_str() );

	connect( m_WidgetUI->m_pAdd, SIGNAL( clicked() ), this, SLOT( AddConfiguraton() ) );
	connect( m_WidgetUI->m_pRemove, SIGNAL( clicked() ), this, SLOT( RemoveConfiguration() ) );
	connect( m_WidgetUI->m_pOk, SIGNAL( clicked() ), this, SLOT( Ok() ) );
}

CEditConfigurationsDialog::~CEditConfigurationsDialog()
{
}

void CEditConfigurationsDialog::AddConfiguraton()
{
	bool fOk = true;

	do
	{
		QString szName = QInputDialog::getText( this, tr( "Add configuration" ), tr( "Enter a name for your configuration" ), QLineEdit::Normal, "", &fOk );

		szName = szName.trimmed();

		if( szName.isEmpty() )
		{
			QMessageBox message;

			message.setText( "Error" );
			message.setInformativeText( "Please enter a valid name" );

			message.exec();
		}
		else if( szName.contains( COptions::CONFIGS_DELIMITER ) )
		{
			QMessageBox message;

			message.setText( "Error" );
			message.setInformativeText( QString( "Configuration names cannot contain a '%1'" ).arg( COptions::CONFIGS_DELIMITER ) );

			message.exec();
		}
		else
		{
			QDir().mkdir( QString("./%1" ).arg( CConfiguration::CONFIG_DIRECTORY ) );

			try
			{
				if( m_App->GetConfigurationManager()->AddConfiguration( szName.toStdString() ) )
					m_WidgetUI->m_pList->addItem( szName );
			}
			catch( const CConfigurationException& e )
			{
				m_UI->SendMessage( e.what(), UIMessageType::Error );
			}

			fOk = false;
		}
	}
	while( fOk );
}

void CEditConfigurationsDialog::RemoveConfiguration()
{
	auto pWidget = m_WidgetUI->m_pList->currentItem();

	if( pWidget )
	{
		QMessageBox removeConfigMessage;

		removeConfigMessage.setText( "Remove configuration from list" );
		removeConfigMessage.setInformativeText( "Do you want to remove the configuration file as well?" );
		removeConfigMessage.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
		removeConfigMessage.setDefaultButton( QMessageBox::Yes );

		const int iRet = removeConfigMessage.exec();

		try
		{
			m_App->GetConfigurationManager()->RemoveConfiguration( ( pWidget->text() ).toStdString(), iRet == QMessageBox::Yes );
		}
		catch( const CConfigurationException& e )
		{
			m_UI->SendMessage( e.what(), UIMessageType::Error );
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
