#include <QCoreApplication>
#include <QColorDialog>
#include <QSettings>

#include "CSyntaxHighlightWidget.h"
#include "ui_CSyntaxHighlightWidget.h"
#include "ide/CASIDEApp.h"

CSyntaxHighlightWidget::CSyntaxHighlightWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent )
	: CBaseOptionsWidget( app, ui, pParent )
	, m_WidgetUI( std::make_unique<Ui::CSyntaxHighlightWidget>() )
{
	m_WidgetUI->setupUi( this );

	SetColors( Qt::black, Qt::black );

	m_Patterns = m_App->GetOptions()->GetPatterns();

	PopulatePatterns();

	if( !m_Patterns.empty() )
	{
		m_WidgetUI->m_pItems->setCurrentItem( m_WidgetUI->m_pItems->item( 0 ) );
		PopulatePatternData( m_Patterns[ 0 ] );
	}

	SetPatternWidgetsEnabled( !m_Patterns.empty() );

	connect( m_WidgetUI->m_pUseDefaults, &QPushButton::clicked, this, &CSyntaxHighlightWidget::UseDefaults );

	connect( m_WidgetUI->m_pAddPattern, &QPushButton::clicked, this, &CSyntaxHighlightWidget::AddPattern );
	connect( m_WidgetUI->m_pRemovePattern, &QPushButton::clicked, this, &CSyntaxHighlightWidget::RemovePattern );

	connect( m_WidgetUI->m_pItems, &QListWidget::itemChanged, this, &CSyntaxHighlightWidget::PatternChanged );

	connect( m_WidgetUI->m_pItems, &QListWidget::currentItemChanged, this, &CSyntaxHighlightWidget::CurrentPatternChanged );

	connect( m_WidgetUI->m_pPattern, &QLineEdit::textEdited, this, &CSyntaxHighlightWidget::PatternEdited );

	connect( m_WidgetUI->m_pChooseFGColor, &QPushButton::clicked, this, &CSyntaxHighlightWidget::PickFGColor );
	connect( m_WidgetUI->m_pChooseBGColor, &QPushButton::clicked, this, &CSyntaxHighlightWidget::PickBGColor );

	connect( m_WidgetUI->m_pBold, &QCheckBox::stateChanged, this, &CSyntaxHighlightWidget::BoldChanged );
	connect( m_WidgetUI->m_pUnderline, &QCheckBox::stateChanged, this, &CSyntaxHighlightWidget::UnderlineChanged );
}

CSyntaxHighlightWidget::~CSyntaxHighlightWidget()
{
}

bool CSyntaxHighlightWidget::CanSave( QString& )
{
	return true;
}

void CSyntaxHighlightWidget::ApplyChanges()
{
	if( HaveChangesBeenMade() )
	{
		m_App->GetOptions()->SetPatterns( QVector<COptions::Pattern>( m_Patterns ) );

		SetChangesMade( false );
	}
}

void CSyntaxHighlightWidget::PostApply()
{
}

void CSyntaxHighlightWidget::SetPatternWidgetsEnabled( bool bState )
{
	m_WidgetUI->m_pRemovePattern->setEnabled( bState );
	m_WidgetUI->m_pPatternWidgets->setEnabled( bState );
}

void CSyntaxHighlightWidget::PopulatePatterns()
{
	m_WidgetUI->m_pItems->clear();

	for( const auto& pattern : m_Patterns )
	{
		m_WidgetUI->m_pItems->addItem( pattern.m_szPatternName );

		auto item = m_WidgetUI->m_pItems->item( m_WidgetUI->m_pItems->count() - 1 );

		item->setFlags( item->flags() | Qt::ItemIsEditable );
	}
}

COptions::Pattern* CSyntaxHighlightWidget::GetCurrentPattern()
{
	auto row = m_WidgetUI->m_pItems->currentIndex().row();

	if( row != -1 )
		return &m_Patterns[ row ];

	return nullptr;
}

void CSyntaxHighlightWidget::PopulatePatternData( const COptions::Pattern& pattern )
{
	m_WidgetUI->m_pPattern->setText( pattern.m_szPattern );

	SetColors( pattern.m_FGColor.color(), pattern.m_BGColor.color() );

	m_WidgetUI->m_pBold->setChecked( pattern.m_bBold );
	m_WidgetUI->m_pUnderline->setChecked( pattern.m_bUnderline );
}

void CSyntaxHighlightWidget::PopulatePatternDataWithCurrent()
{
	auto pCurrent = m_WidgetUI->m_pItems->currentItem();

	if( pCurrent )
	{
		PopulatePatternData( m_Patterns[ m_WidgetUI->m_pItems->currentRow() ] );
	}
	else
	{
		ClearPatternData();
	}

	SetPatternWidgetsEnabled( pCurrent != nullptr );
}

void CSyntaxHighlightWidget::ClearPatternData()
{
	m_WidgetUI->m_pPattern->clear();

	SetColors( Qt::black, Qt::black );

	m_WidgetUI->m_pBold->setChecked( false );
	m_WidgetUI->m_pUnderline->setChecked( false );
}

void CSyntaxHighlightWidget::SetColor( ColorId colorId, const QColor& color )
{
	QWidget* pColor;

	switch( colorId )
	{
	case ColorId::FG: pColor = m_WidgetUI->m_pFGColor; break;
	case ColorId::BG: pColor = m_WidgetUI->m_pBGColor; break;
	default: return;
	}

	pColor->setStyleSheet( "background-color:" + color.name() + ";" );
}

void CSyntaxHighlightWidget::SetColors( const QColor& fgColor, const QColor& bgColor )
{
	SetColor( ColorId::FG, fgColor );
	SetColor( ColorId::BG, bgColor );
}

void CSyntaxHighlightWidget::UseDefaults()
{
	QSettings settings( "configs/default_syntax_highlights.ini", QSettings::IniFormat );

	QVector<COptions::Pattern> patterns;

	COptions::LoadPatterns( settings, patterns );

	for( const auto& pattern : patterns )
	{
		int iOverwrite;

		for( iOverwrite = 0; iOverwrite < m_Patterns.size(); ++iOverwrite )
		{
			if( m_Patterns[ iOverwrite ].m_szPatternName == pattern.m_szPatternName )
			{
				m_Patterns[ iOverwrite ] = std::move( pattern );
				break;
			}
		}

		if( iOverwrite == m_Patterns.size() )
		{
			m_Patterns.push_back( pattern );
		}
	}

	PopulatePatternDataWithCurrent();

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::AddPattern()
{
	m_Patterns.push_back( COptions::Pattern() );

	m_WidgetUI->m_pItems->addItem( "New pattern" );

	auto item = m_WidgetUI->m_pItems->item( m_WidgetUI->m_pItems->count() - 1 );

	item->setFlags( item->flags() | Qt::ItemIsEditable );

	m_WidgetUI->m_pItems->setCurrentItem( item );
	m_WidgetUI->m_pItems->editItem( item );

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::RemovePattern()
{
	auto pItem = m_WidgetUI->m_pItems->currentItem();

	if( pItem )
	{
		const auto iRow = m_WidgetUI->m_pItems->row( pItem );

		m_WidgetUI->m_pItems->removeItemWidget( pItem );

		m_Patterns.remove( iRow );

		SetChangesMade( true );
	}
}

void CSyntaxHighlightWidget::PatternChanged( QListWidgetItem* pItem )
{
	GetCurrentPattern()->m_szPatternName = pItem->text().trimmed();

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::CurrentPatternChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	PopulatePatternDataWithCurrent();
}

void CSyntaxHighlightWidget::PatternEdited( const QString& szPattern )
{
	GetCurrentPattern()->m_szPattern = szPattern;

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::PickFGColor()
{
	GetCurrentPattern()->m_FGColor.setColor( QColorDialog::getColor( GetCurrentPattern()->m_FGColor.color(), this ) );

	SetColor( ColorId::FG, GetCurrentPattern()->m_FGColor.color() );

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::PickBGColor()
{
	GetCurrentPattern()->m_BGColor.setColor( QColorDialog::getColor( GetCurrentPattern()->m_BGColor.color(), this ) );

	SetColor( ColorId::BG, GetCurrentPattern()->m_BGColor.color() );

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::BoldChanged( int state )
{
	GetCurrentPattern()->m_bBold = m_WidgetUI->m_pBold->isChecked();

	SetChangesMade( true );
}

void CSyntaxHighlightWidget::UnderlineChanged( int state )
{
	GetCurrentPattern()->m_bUnderline = m_WidgetUI->m_pUnderline->isChecked();

	SetChangesMade( true );
}
