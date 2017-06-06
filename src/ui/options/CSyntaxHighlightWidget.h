#ifndef IDE_UI_OPTIONS_CSYNTAXHIGHLIGHTWIDGET_H
#define IDE_UI_OPTIONS_CSYNTAXHIGHLIGHTWIDGET_H

#include <QVector>
#include <QWidget>

#include "CBaseOptionsWidget.h"
#include "ide/COptions.h"

namespace Ui
{
class CSyntaxHighlightWidget;
}

class CASIDEApp;
class QListWidgetItem;

/*
*	Provides a way to define syntax highlighting for patterns
*/
class CSyntaxHighlightWidget : public CBaseOptionsWidget
{
	Q_OBJECT

private:
	enum class ColorId
	{
		FG,
		BG
	};

public:
	explicit CSyntaxHighlightWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent );
	~CSyntaxHighlightWidget();

	bool CanSave( QString& szReason ) override;

	void ApplyChanges() override;

	void PostApply() override;

private:
	void SetPatternWidgetsEnabled( bool bState );

	void PopulatePatterns();

	COptions::Pattern* GetCurrentPattern();

	void PopulatePatternData( const COptions::Pattern& pattern );

	void PopulatePatternDataWithCurrent();

	void ClearPatternData();

	void SetColor( ColorId colorId, const QColor& color );

	void SetColors( const QColor& fgColor, const QColor& bgColor );
	
private slots:
	void UseDefaults();

	void AddPattern();
	void RemovePattern();

	void PatternChanged( QListWidgetItem* pItem );

	void CurrentPatternChanged( QListWidgetItem* current, QListWidgetItem* previous );

	void PatternEdited( const QString& szPattern );

	void PickFGColor();

	void PickBGColor();

	void BoldChanged( int state );

	void UnderlineChanged( int state );

private:
	std::unique_ptr<Ui::CSyntaxHighlightWidget> m_WidgetUI;

	QVector<COptions::Pattern> m_Patterns;

	bool m_bPopulatingList = false;
};

#endif //IDE_UI_OPTIONS_CSYNTAXHIGHLIGHTWIDGET_H
