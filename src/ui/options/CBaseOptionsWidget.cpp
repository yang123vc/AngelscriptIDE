#include "COptionsDialog.h"

#include "CBaseOptionsWidget.h"

CBaseOptionsWidget::CBaseOptionsWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent )
	: QWidget( pParent )
	, m_App( app )
	, m_UI( ui )
	, m_pParent( pParent )
{
	ConnectToParent();
}

CBaseOptionsWidget::~CBaseOptionsWidget()
{
}

void CBaseOptionsWidget::SetChangesMade( bool bState )
{
	m_bChangesMade = bState;

	ChangesMade( bState );
}

void CBaseOptionsWidget::ConnectToParent()
{
	connect( this, SIGNAL( ChangesMade( bool ) ), m_pParent, SLOT( ChangesMade( bool ) ) );
}
