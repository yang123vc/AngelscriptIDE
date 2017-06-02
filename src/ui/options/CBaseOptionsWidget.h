#ifndef IDE_UI_OPTIONS_CBASEOPTIONSWIDGET_H
#define IDE_UI_OPTIONS_CBASEOPTIONSWIDGET_H

#include <memory>

#include <QWidget>

class CASIDEApp;
class COptionsDialog;
class CUI;

class CBaseOptionsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CBaseOptionsWidget( std::shared_ptr<CASIDEApp> app, std::shared_ptr<CUI> ui, COptionsDialog* pParent );
	~CBaseOptionsWidget();

	virtual bool CanSave( QString& szReason ) = 0;

	virtual void ApplyChanges() = 0;

	virtual void PostApply() {}

	bool HaveChangesBeenMade() const { return m_fChangesMade; }

	void SetChangesMade( bool fState );

signals:

	void ChangesMade( bool fState );

protected:
	std::shared_ptr<CASIDEApp> m_App;
	std::shared_ptr<CUI> m_UI;
	COptionsDialog* m_pParent;

private:
	bool m_fChangesMade;
};

#endif //IDE_UI_OPTIONS_CBASEOPTIONSWIDGET_H
