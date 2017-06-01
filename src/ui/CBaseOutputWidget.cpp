#include "CBaseOutputWidget.h"

CBaseOutputWidget::CBaseOutputWidget( const QString& szName, std::shared_ptr<CUI> ui, QWidget* pParent )
	: QWidget( pParent )
	, m_UI( ui )
	, m_szName( szName )
{

}

CBaseOutputWidget::~CBaseOutputWidget()
{

}

