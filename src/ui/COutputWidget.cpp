#include "angelscript.h"

#include "ide/CASIDEApp.h"

#include "ui_COutputWidget.h"
#include "COutputWidget.h"

COutputWidget::COutputWidget( std::shared_ptr<CASIDEApp> app, const QString szName, std::shared_ptr<CUI> ui, QWidget *pParent )
	: CBaseOutputWidget( szName, ui, pParent )
	, m_WidgetUi( new Ui::COutputWidget )
	, m_App( app )
{
	m_WidgetUi->setupUi( this );

	connect( m_WidgetUi->m_pClearButton, &QPushButton::clicked, this, &COutputWidget::Clear );
}

COutputWidget::~COutputWidget()
{
}

void COutputWidget::Clear()
{
	m_WidgetUi->m_pOutput->clear();
}
