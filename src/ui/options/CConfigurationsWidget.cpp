#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

#include "Angelscript/CConfiguration.h"
#include "Angelscript/CConfigurationException.h"
#include "Angelscript/IConfigurationManager.h"

#include "ide/CASIDEApp.h"
#include "ide/COptions.h"

#include "ui/CUI.h"

#include "CEditConfigurationsDialog.h"

#include "ui_CConfigurationsWidget.h"
#include "CConfigurationsWidget.h"

CConfigurationsWidget::CConfigurationsWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent )
	: CBaseOptionsWidget( app, ui, pParent )
	, m_WidgetUI( std::make_unique<Ui::CConfigurationsWidget>() )
{
	m_WidgetUI->setupUi( this );

	auto configManager = app->GetOptions()->GetConfigurationManager();

	configManager->AddConfigurationEventListener( this );

	const auto uiCount = configManager->GetConfigurationCount();

	SetFieldsEnabled( uiCount > 0 );

	for( size_t uiIndex = 0; uiIndex < uiCount; ++uiIndex )
	{
		m_WidgetUI->m_pCurrentConfigurationComboBox->addItem( configManager->GetConfiguration( uiIndex )->GetName().c_str() );
	}

	if( uiCount > 0 )
	{
		auto active = configManager->GetActiveConfiguration();

		if( !active || !SetCurrentConfiguration( active->GetName().c_str() ) )
			SetCurrentConfiguration( 0 );
	}

	connect( m_WidgetUI->m_pManageConfigs, SIGNAL( clicked() ), this, SLOT( OpenEditConfigs() ) );

	connect( m_WidgetUI->m_pCurrentConfigurationComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ConfigurationSelected( int ) ) );
	connect( m_WidgetUI->m_pConfigurationNameLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( NameChanged( QString ) ) );

	connect( m_WidgetUI->m_pAngelscriptConfigFileLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( ConfigFileChanged( QString ) ) );
	connect( m_WidgetUI->m_pConfigFileSelectButton, SIGNAL( clicked() ), this, SLOT( OpenSelectConfigFileDialog() ) );

	connect( m_WidgetUI->m_pIncludeFilenameLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( IncludeFilenameChanged( QString ) ) );
	connect( m_WidgetUI->m_pIncludeFilenameButton, SIGNAL( clicked() ), this, SLOT( OpenIncludeFilenameFileDialog() ) );

	connect( m_WidgetUI->m_pFallbackExtLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( FallbackExtensionsChanged( QString ) ) );

	connect( m_WidgetUI->m_pAddWordButton, SIGNAL( clicked() ), this, SLOT( AddWord() ) );
	connect( m_WidgetUI->m_pRemoveWordButton, SIGNAL( clicked() ), this, SLOT( RemoveWord() ) );

	connect( m_WidgetUI->m_pAddIncludePathButton, SIGNAL( clicked() ), this, SLOT( AddIncludePath() ) );
	connect( m_WidgetUI->m_pRemoveIncludePathButton, SIGNAL( clicked() ), this, SLOT( RemoveIncludePath() ) );

	connect( m_WidgetUI->m_pAddExtensionButton, SIGNAL( clicked() ), this, SLOT( AddExtension() ) );
	connect( m_WidgetUI->m_pRemoveExtensionButton, SIGNAL( clicked() ), this, SLOT( RemoveExtension() ) );
}

CConfigurationsWidget::~CConfigurationsWidget()
{
	m_App->GetOptions()->GetConfigurationManager()->RemoveConfigurationEventListener( this );
}

bool CConfigurationsWidget::CanSave( QString& szReason )
{
	const QString szName = m_WidgetUI->m_pConfigurationNameLineEdit->text().trimmed();

	if( szName.isEmpty() )
	{
		szReason = "Configuration name must be valid";
		return false;
	}

	return true;
}

void CConfigurationsWidget::ApplyChanges()
{
	if( HaveChangesBeenMade() )
	{
		SetChangesMade( false );
		SaveCurrentConfiguration();
	}
}

void CConfigurationsWidget::ConfigEventOccurred( const ConfigEvent& event )
{
	switch( event.type )
	{
	case ConfigEventType::ADD:
		{
			if( m_WidgetUI->m_pCurrentConfigurationComboBox->count() == 0 )
				SetFieldsEnabled( true );

			const int iIndex = m_WidgetUI->m_pCurrentConfigurationComboBox->currentIndex();

			m_WidgetUI->m_pCurrentConfigurationComboBox->addItem( event.add.pszName->c_str() );

			m_WidgetUI->m_pCurrentConfigurationComboBox->setCurrentIndex( iIndex != -1 ? iIndex : 0 );
			break;
		}

	case ConfigEventType::REMOVE:
		{
			const int iCurrentIndex = m_WidgetUI->m_pCurrentConfigurationComboBox->currentIndex();

			const int iIndex = m_WidgetUI->m_pCurrentConfigurationComboBox->findText( event.remove.pszName->c_str() );

			if( iIndex != -1 )
				m_WidgetUI->m_pCurrentConfigurationComboBox->removeItem( iIndex );

			if( m_WidgetUI->m_pCurrentConfigurationComboBox->count() > 0 )
				m_WidgetUI->m_pCurrentConfigurationComboBox->setCurrentIndex( iCurrentIndex < iIndex ? iCurrentIndex : iCurrentIndex - 1 );
			else
				SetFieldsEnabled( false );
			break;
		}

	case ConfigEventType::RENAME:
		{
			const int iIndex = m_WidgetUI->m_pCurrentConfigurationComboBox->findText( event.rename.pszOldName->c_str() );

			if( iIndex != -1 )
			{
				m_WidgetUI->m_pCurrentConfigurationComboBox->removeItem( iIndex );
				m_WidgetUI->m_pCurrentConfigurationComboBox->insertItem( iIndex, event.rename.pszNewName->c_str() );
				m_WidgetUI->m_pCurrentConfigurationComboBox->setCurrentIndex( iIndex );
			}

			break;
		}

	default: break;
	}
}

bool CConfigurationsWidget::SetCurrentConfiguration( const QString& szActiveConfig )
{
	return SetCurrentConfiguration( m_WidgetUI->m_pCurrentConfigurationComboBox->findText( szActiveConfig ) );
}

bool CConfigurationsWidget::SetCurrentConfiguration( int iIndex )
{
	bool bSuccess = false;

	if( iIndex != -1 )
	{
		if( HaveChangesBeenMade() )
		{
			QMessageBox question;

			question.setText( "Unsaved changes in configuration" );

			question.setInformativeText( "Do you want to save your changes?" );

			question.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );

			int iRet = question.exec();

			switch( iRet )
			{
			case QMessageBox::Save: SaveCurrentConfiguration(); break;
			case QMessageBox::Discard: break;
			default:
			case QMessageBox::Cancel: return false;
			}

			SetChangesMade( false );
		}

		if( iIndex < m_WidgetUI->m_pCurrentConfigurationComboBox->count() )
		{
			QString szActiveConfig = m_WidgetUI->m_pCurrentConfigurationComboBox->itemText( iIndex );

			m_WidgetUI->m_pCurrentConfigurationComboBox->setCurrentIndex( iIndex );

			m_WidgetUI->m_pWordList->clear();
			m_WidgetUI->m_pIncludePathsList->clear();
			m_WidgetUI->m_pExtensionsList->clear();

			m_CurrentConfiguration = m_App->GetOptions()->GetConfigurationManager()->Find( szActiveConfig.toStdString() );

			if( m_CurrentConfiguration )
			{
				m_WidgetUI->m_pConfigurationNameLineEdit->setText( m_CurrentConfiguration->GetName().c_str() );
				m_WidgetUI->m_pAngelscriptConfigFileLineEdit->setText( m_CurrentConfiguration->GetConfigFilename().c_str() );
				m_WidgetUI->m_pIncludeFilenameLineEdit->setText( m_CurrentConfiguration->GetIncludeFilename().c_str() );
				m_WidgetUI->m_pFallbackExtLineEdit->setText( m_CurrentConfiguration->GetFallbackExtension().c_str() );

				AddStringsToList( m_WidgetUI->m_pWordList, m_CurrentConfiguration->GetWords() );
				AddStringsToList( m_WidgetUI->m_pIncludePathsList, m_CurrentConfiguration->GetIncludePaths() );
				AddStringsToList( m_WidgetUI->m_pExtensionsList, m_CurrentConfiguration->GetExtensions() );

				SetFieldsEnabled( true );

				bSuccess = true;
			}
			else
			{
				m_UI->SendMessage( QString( "Could not load configuration '%1'!\n" ).arg( szActiveConfig ) );

				m_WidgetUI->m_pConfigurationNameLineEdit->setText( "" );
				m_WidgetUI->m_pAngelscriptConfigFileLineEdit->setText( "" );
				m_WidgetUI->m_pIncludeFilenameLineEdit->setText( "" );
				m_WidgetUI->m_pFallbackExtLineEdit->setText( "" );

				SetFieldsEnabled( false );
			}

			//Could have been set by the field updates above
			SetChangesMade( false );
		}
		else
		{
			m_CurrentConfiguration = nullptr;
			std::cout << "Index out of range in CConfigurationsWidget::SetActiveConfiguration!" << std::endl;
		}
	}
	else
	{
		m_CurrentConfiguration = nullptr;
	}

	return bSuccess;
}

void CConfigurationsWidget::SaveCurrentConfiguration()
{
	m_CurrentConfiguration->SetConfigFilename( m_WidgetUI->m_pAngelscriptConfigFileLineEdit->text().trimmed().toStdString() );
	m_CurrentConfiguration->SetIncludeFilename( m_WidgetUI->m_pIncludeFilenameLineEdit->text().trimmed().toStdString() );
	m_CurrentConfiguration->SetFallbackExtension( m_WidgetUI->m_pFallbackExtLineEdit->text().trimmed().toStdString() );

	auto& words = m_CurrentConfiguration->GetWords();

	words.clear();

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pWordList->count(); ++iIndex )
	{
		auto szText = m_WidgetUI->m_pWordList->item( iIndex )->text().trimmed();

		if( !szText.isEmpty() )
			words.push_back( szText.toStdString() );
	}

	auto& includePaths = m_CurrentConfiguration->GetIncludePaths();

	includePaths.clear();

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pIncludePathsList->count(); ++iIndex )
	{
		auto szText = m_WidgetUI->m_pIncludePathsList->item( iIndex )->text().trimmed();

		if( !szText.isEmpty() )
			includePaths.push_back( szText.toStdString() );
	}

	auto& extensions = m_CurrentConfiguration->GetExtensions();

	extensions.clear();

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pExtensionsList->count(); ++iIndex )
	{
		auto szText = m_WidgetUI->m_pExtensionsList->item( iIndex )->text().trimmed();

		if( !szText.isEmpty() )
			extensions.push_back( szText.toStdString() );
	}

	const QString szConfigName( m_CurrentConfiguration->GetName().c_str() );

	QString szNewName = m_WidgetUI->m_pConfigurationNameLineEdit->text().trimmed();

	if( szConfigName != szNewName )
	{
		try
		{
			m_App->GetOptions()->GetConfigurationManager()->RenameConfiguration( m_CurrentConfiguration, szNewName.toStdString() );
			m_UI->SendMessage( QString( "Renaming configuration '%1' to '%2'\n" ).arg( szConfigName ).arg( szNewName ) );
		}
		catch( const CConfigurationException& e )
		{
			m_UI->SendMessage( e.what(), UIMessageType::ERROR );
		}
	}
}

void CConfigurationsWidget::SetFieldsEnabled( bool bState )
{
	m_WidgetUI->m_pConfigOptions->setEnabled( bState );
}

void CConfigurationsWidget::AddStringToList( QListWidget* pList, const std::string& szString )
{
	pList->addItem( szString.c_str() );

	auto item = pList->item( pList->count() - 1 );

	item->setFlags( item->flags() | Qt::ItemIsEditable );
	item->setBackgroundColor( QColor( 240, 240, 240 ) );
}

void CConfigurationsWidget::AddStringsToList( QListWidget* pList, const std::vector<std::string>& strings )
{
	for( const auto& szString : strings )
		AddStringToList( pList, szString );
}

void CConfigurationsWidget::OpenEditConfigs()
{
	CEditConfigurationsDialog edit( m_App, m_UI, this );

	edit.exec();
}

void CConfigurationsWidget::ConfigurationSelected( int iIndex )
{
	SetCurrentConfiguration( iIndex );
}

void CConfigurationsWidget::NameChanged( const QString& )
{
	SetChangesMade( true );
}

void CConfigurationsWidget::ConfigFileChanged( const QString& )
{
	SetChangesMade( true );
}

void CConfigurationsWidget::OpenSelectConfigFileDialog()
{
	QString szResult = QFileDialog::getOpenFileName( this, tr( "Select configuration file" ) );

	if( !szResult.isEmpty() )
		m_WidgetUI->m_pAngelscriptConfigFileLineEdit->setText( szResult );
}

void CConfigurationsWidget::IncludeFilenameChanged( const QString& )
{
	SetChangesMade( true );
}

void CConfigurationsWidget::OpenIncludeFilenameFileDialog()
{
	QString szResult = QFileDialog::getOpenFileName( this, tr( "Select include file" ) );

	if( !szResult.isEmpty() )
		m_WidgetUI->m_pIncludeFilenameLineEdit->setText( szResult );
}

void CConfigurationsWidget::FallbackExtensionsChanged( const QString& )
{
	SetChangesMade( true );
}

void CConfigurationsWidget::AddWord()
{
	SetChangesMade( true );

	AddStringToList( m_WidgetUI->m_pWordList, "" );
}

void CConfigurationsWidget::RemoveWord()
{
	auto index = m_WidgetUI->m_pWordList->currentIndex();

	if( index.isValid() )
	{
		SetChangesMade( true );

		delete m_WidgetUI->m_pWordList->takeItem( index.row() );
	}
}

void CConfigurationsWidget::AddIncludePath()
{
	SetChangesMade( true );

	AddStringToList( m_WidgetUI->m_pIncludePathsList, "" );
}

void CConfigurationsWidget::RemoveIncludePath()
{
	auto index = m_WidgetUI->m_pIncludePathsList->currentIndex();

	if( index.isValid() )
	{
		SetChangesMade( true );

		delete m_WidgetUI->m_pIncludePathsList->takeItem( index.row() );
	}
}

void CConfigurationsWidget::AddExtension()
{
	SetChangesMade( true );

	AddStringToList( m_WidgetUI->m_pExtensionsList, "" );
}

void CConfigurationsWidget::RemoveExtension()
{
	auto index = m_WidgetUI->m_pExtensionsList->currentIndex();

	if( index.isValid() )
	{
		SetChangesMade( true );

		delete m_WidgetUI->m_pExtensionsList->takeItem( index.row() );
	}
}
