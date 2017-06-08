#include <QRadioButton>

#include "ui_CFindDialog.h"

#include "CFindDialog.h"

const char* CFindDialog::SEARCH_DIRECTION_PROPERTY = "SearchDirection";

CFindDialog::CFindDialog( QWidget* pParent )
	: QDialog( pParent )
	, m_WidgetUI( new Ui::CFindDialog )
{
	m_WidgetUI->setupUi( this );

	connect( m_WidgetUI->m_pFindNextButton, &QPushButton::clicked, this, &CFindDialog::FindNext );
	connect( m_WidgetUI->m_pCloseButton, &QPushButton::clicked, this, &CFindDialog::Close );

	m_WidgetUI->m_pSearchForward->setProperty( SEARCH_DIRECTION_PROPERTY, static_cast<int>( SearchDirection::FORWARD ) );
	m_WidgetUI->m_pSearchBackward->setProperty( SEARCH_DIRECTION_PROPERTY, static_cast<int>( SearchDirection::BACKWARD ) );
}

CFindDialog::~CFindDialog()
{
}

bool CFindDialog::MatchWholeWord() const
{
	return m_WidgetUI->m_pMatchWholeWord->isChecked();
}

bool CFindDialog::MatchCase() const
{
	return m_WidgetUI->m_pMatchCase->isChecked();
}

CFindDialog::SearchDirection CFindDialog::GetSearchDirection() const
{
	for( auto child : m_WidgetUI->m_pSearchDirection->children() )
	{
		auto radioButton = static_cast<QRadioButton*>( child );

		if( radioButton->isChecked() )
		{
			auto prop = radioButton->property( SEARCH_DIRECTION_PROPERTY );

			SearchDirection dir;

			bool fOk = false;

			if( prop.isValid() )
			{
				dir = static_cast<SearchDirection>( prop.toInt( &fOk ) );

				if( fOk )
					return dir;
			}

			break;
		}
	}

	return SearchDirection::FORWARD;
}

void CFindDialog::FindNext()
{
	const QString szString = m_WidgetUI->m_pSearchString->text();

	//Only search if a trimmed version is not empty, but keep the whitespace
	if( !szString.trimmed().isEmpty() )
		Find( szString );
}

void CFindDialog::Close()
{
	close();
}
