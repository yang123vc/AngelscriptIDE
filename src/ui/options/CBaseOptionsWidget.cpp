#include "COptionsDialog.h"

#include "CBaseOptionsWidget.h"

CBaseOptionsWidget::CBaseOptionsWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent )
	: QWidget( pParent )
	, m_App( app )
	, m_UI( ui )
	, m_pParent( pParent )
	, m_fChangesMade( false )
{
	connect( this, SIGNAL( ChangesMade( bool ) ), pParent, SLOT( ChangesMade( bool ) ) );
}

CBaseOptionsWidget::~CBaseOptionsWidget()
{

}

void CBaseOptionsWidget::SetChangesMade( bool fState )
{
	m_fChangesMade = fState;

	ChangesMade( fState );
}
