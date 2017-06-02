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
	, m_WidgetUI( new Ui::CConfigurationsWidget )
	, m_fChangingContents( false )
{
	m_WidgetUI->setupUi( this );

	app->GetConfigurationManager()->AddConfigurationEventListener( this );

	auto options = m_App->GetOptions();
	const auto& configurations = options->GetConfigurations();

	if( !configurations.empty() )
	{
		for( const auto& config : configurations )
			m_WidgetUI->m_pCurrentConfigurationComboBox->addItem( config.c_str() );

		const auto& szActiveConfig = options->GetActiveConfigurationName();

		if( !SetActiveConfiguration( szActiveConfig.c_str() ) )
			SetActiveConfiguration( 0 );
	}
	else
		SetFieldsEnabled( false );

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
	m_App->GetConfigurationManager()->RemoveConfigurationEventListener( this );
}

bool CConfigurationsWidget::CanSave( QString& szReason )
{
	const QString szName = m_WidgetUI->m_pConfigurationNameLineEdit->text().trimmed();

	if( szName.isEmpty() )
	{
		szReason = "Configuration name must be valid";
		return false;
	}
	else if( szName.contains( COptions::CONFIGS_DELIMITER ) )
	{
		szReason = QString( "Configuration names cannot contain a '%1'" ).arg( COptions::CONFIGS_DELIMITER );
		return false;
	}

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pWordList->count(); ++iIndex )
	{
		if( m_WidgetUI->m_pWordList->item( iIndex )->text().contains( CConfiguration::WORD_DELIMITER ) )
		{
			szReason = QString( "Words cannot contain the symbol '%1'" ).arg( CConfiguration::WORD_DELIMITER );
			return false;
		}
	}

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pIncludePathsList->count(); ++iIndex )
	{
		if( m_WidgetUI->m_pIncludePathsList->item( iIndex )->text().contains( CConfiguration::INCLUDEPATH_DELIMITER ) )
		{
			szReason = QString( "Include paths cannot contain the symbol '%1'" ).arg( CConfiguration::INCLUDEPATH_DELIMITER );
			return false;
		}
	}

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pExtensionsList->count(); ++iIndex )
	{
		if( m_WidgetUI->m_pExtensionsList->item( iIndex )->text().contains( CConfiguration::EXTENSION_DELIMITER ) )
		{
			szReason = QString( "Extensions cannot contain the symbol '%1'" ).arg( CConfiguration::EXTENSION_DELIMITER );
			return false;
		}
	}

	return true;
}

void CConfigurationsWidget::ApplyChanges()
{
	if( HaveChangesBeenMade() )
	{
		SetChangesMade( false );
		SaveActiveConfiguration();
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

bool CConfigurationsWidget::SetActiveConfiguration( const QString& szActiveConfig )
{
	return SetActiveConfiguration( m_WidgetUI->m_pCurrentConfigurationComboBox->findText( szActiveConfig ) );
}

bool CConfigurationsWidget::SetActiveConfiguration( int iIndex )
{
	bool fSuccess = false;

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
			case QMessageBox::Save: SaveActiveConfiguration(); break;
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

			auto config = CConfiguration::Load( m_App->GetConfigurationManager(), szActiveConfig.toStdString() );

			m_fChangingContents = true;

			if( config )
			{
				SetFieldsEnabled( true );

				m_WidgetUI->m_pConfigurationNameLineEdit->setText( config->GetName().c_str() );
				m_szConfigName = m_WidgetUI->m_pConfigurationNameLineEdit->text();
				m_WidgetUI->m_pAngelscriptConfigFileLineEdit->setText( config->GetConfigFilename().c_str() );
				m_WidgetUI->m_pIncludeFilenameLineEdit->setText( config->GetIncludeFilename().c_str() );
				m_WidgetUI->m_pFallbackExtLineEdit->setText( config->GetFallbackExtension().c_str() );

				AddStringsToList( m_WidgetUI->m_pWordList, config->GetWords() );
				AddStringsToList( m_WidgetUI->m_pIncludePathsList, config->GetIncludePaths() );
				AddStringsToList( m_WidgetUI->m_pExtensionsList, config->GetExtensions() );

				fSuccess = true;
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

			m_fChangingContents = false;
		}
		else
			std::cout << "Index out of range in CConfigurationsWidget::SetActiveConfiguration!" << std::endl;
	}

	return fSuccess;
}

void CConfigurationsWidget::SaveActiveConfiguration()
{
	auto config = std::make_shared<CConfiguration>( m_App->GetConfigurationManager(), m_szConfigName.toStdString() );

	config->SetConfigFilename( m_WidgetUI->m_pAngelscriptConfigFileLineEdit->text().trimmed().toStdString() );
	config->SetIncludeFilename( m_WidgetUI->m_pIncludeFilenameLineEdit->text().trimmed().toStdString() );
	config->SetFallbackExtension( m_WidgetUI->m_pFallbackExtLineEdit->text().trimmed().toStdString() );

	auto& words = config->GetWords();

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pWordList->count(); ++iIndex )
	{
		auto szText = m_WidgetUI->m_pWordList->item( iIndex )->text().trimmed();

		if( !szText.isEmpty() )
			words.push_back( szText.toStdString() );
	}

	auto& includePaths = config->GetIncludePaths();

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pIncludePathsList->count(); ++iIndex )
	{
		auto szText = m_WidgetUI->m_pIncludePathsList->item( iIndex )->text().trimmed();

		if( !szText.isEmpty() )
			includePaths.push_back( szText.toStdString() );
	}

	auto& extensions = config->GetExtensions();

	for( int iIndex = 0; iIndex < m_WidgetUI->m_pExtensionsList->count(); ++iIndex )
	{
		auto szText = m_WidgetUI->m_pExtensionsList->item( iIndex )->text().trimmed();

		if( !szText.isEmpty() )
			extensions.push_back( szText.toStdString() );
	}

	const QString szConfigName( config->GetName().c_str() );

	if( config->Save() )
		m_UI->SendMessage( QString( "Configuration '%1' saved\n" ).arg( szConfigName ) );
	else
		m_UI->SendMessage( QString( "Configuration '%1' not saved\n" ).arg( szConfigName ) );

	QString szNewName = m_WidgetUI->m_pConfigurationNameLineEdit->text().trimmed();

	if( m_szConfigName != szNewName )
	{
		try
		{
			config->Rename( szNewName.toStdString() );
			m_UI->SendMessage( QString( "Renaming configuration '%1' to '%2'\n" ).arg( szConfigName).arg( szNewName ) );
		}
		catch( const CConfigurationException& e )
		{
			m_UI->SendMessage( e.what(), UIMessageType::ERROR );
		}
	}
}

void CConfigurationsWidget::SetFieldsEnabled( bool fState )
{
	m_WidgetUI->m_pCurrentConfigurationComboBox->setEnabled( fState );

	m_WidgetUI->m_pConfigurationNameLineEdit->setEnabled( fState );

	m_WidgetUI->m_pAngelscriptConfigFileLineEdit->setEnabled( fState );
	m_WidgetUI->m_pConfigFileSelectButton->setEnabled( fState );

	m_WidgetUI->m_pIncludeFilenameLineEdit->setEnabled( fState );
	m_WidgetUI->m_pIncludeFilenameButton->setEnabled( fState );

	m_WidgetUI->m_pWordList->setEnabled( fState );
	m_WidgetUI->m_pAddWordButton->setEnabled( fState );
	m_WidgetUI->m_pRemoveWordButton->setEnabled( fState );

	m_WidgetUI->m_pIncludePathsList->setEnabled( fState );
	m_WidgetUI->m_pAddIncludePathButton->setEnabled( fState );
	m_WidgetUI->m_pRemoveIncludePathButton->setEnabled( fState );

	m_WidgetUI->m_pExtensionsList->setEnabled( fState );
	m_WidgetUI->m_pAddExtensionButton->setEnabled( fState );
	m_WidgetUI->m_pRemoveExtensionButton->setEnabled( fState );
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
	SetActiveConfiguration( iIndex );
}

void CConfigurationsWidget::NameChanged( const QString& )
{
	if( !m_fChangingContents )
		SetChangesMade( true );
}

void CConfigurationsWidget::ConfigFileChanged( const QString& )
{
	if( !m_fChangingContents )
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
	if( !m_fChangingContents )
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
	if( !m_fChangingContents )
		SetChangesMade( true );
}

void CConfigurationsWidget::AddWord()
{
	if( !m_fChangingContents )
		SetChangesMade( true );

	AddStringToList( m_WidgetUI->m_pWordList, "" );
}

void CConfigurationsWidget::RemoveWord()
{
	auto index = m_WidgetUI->m_pWordList->currentIndex();

	if( index.isValid() )
	{
		if( !m_fChangingContents )
			SetChangesMade( true );

		delete m_WidgetUI->m_pWordList->takeItem( index.row() );
	}
}

void CConfigurationsWidget::AddIncludePath()
{
	if( !m_fChangingContents )
		SetChangesMade( true );

	AddStringToList( m_WidgetUI->m_pIncludePathsList, "" );
}

void CConfigurationsWidget::RemoveIncludePath()
{
	auto index = m_WidgetUI->m_pIncludePathsList->currentIndex();

	if( index.isValid() )
	{
		if( !m_fChangingContents )
			SetChangesMade( true );

		delete m_WidgetUI->m_pIncludePathsList->takeItem( index.row() );
	}
}

void CConfigurationsWidget::AddExtension()
{
	if( !m_fChangingContents )
		SetChangesMade( true );

	AddStringToList( m_WidgetUI->m_pExtensionsList, "" );
}

void CConfigurationsWidget::RemoveExtension()
{
	auto index = m_WidgetUI->m_pExtensionsList->currentIndex();

	if( index.isValid() )
	{
		if( !m_fChangingContents )
			SetChangesMade( true );

		delete m_WidgetUI->m_pExtensionsList->takeItem( index.row() );
	}
}
