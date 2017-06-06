#ifndef IDE_COPTIONS_H
#define IDE_COPTIONS_H

#include <memory>
#include <vector>
#include <list>

#include <QBrush>
#include <QObject>
#include <QVector>

class CConfigurationManager;
class QSettings;

/**
*	Stores the configurations and global settings
*/
class COptions : public QObject
{
	Q_OBJECT

public:
	static const int DEFAULT_TAB_WIDTH = 40;

	static const bool DEFAULT_START_MAXIMIZED = true;

	static const size_t MAX_RECENT_FILES = 5;

public:
	struct Pattern
	{
		QString m_szPatternName;
		QString m_szPattern;

		QBrush m_FGColor = Qt::black;
		QBrush m_BGColor = Qt::white;

		bool m_bBold = false;
		bool m_bUnderline = false;
	};

public:
	typedef std::list<std::string> RecentFiles_t;

	COptions();
	~COptions();

	COptions( const COptions& other ) = default;
	COptions& operator=( const COptions& other ) = default;

	int GetTabWidth() const { return m_iTabWidth; }

	void SetTabWidth( int iTabWidth ) { m_iTabWidth = iTabWidth; }

	bool StartMaximized() const { return m_bMaximized; }

	void SetStartMaximized( bool bMaximized ) { m_bMaximized = bMaximized; }

	const std::string& GetCurrentDirectory() const { return m_szCurrentDir; }
	void SetCurrentDirectory( const std::string& szCurrentDir ) { m_szCurrentDir = szCurrentDir; }

	const std::shared_ptr<CConfigurationManager>& GetConfigurationManager() { return m_ConfigurationManager; }

	const RecentFiles_t& GetRecentFiles() const { return m_RecentFiles; }

	void AddRecentFile( const std::string& szFilename );
	void RemoveRecentFile( const std::string& szFilename );

	void ClearRecentFiles();

	const QVector<Pattern>& GetPatterns() const { return m_Patterns; }

	void SetPatterns( QVector<Pattern>&& patterns );

	void LoadOptions( QSettings& settings );
	void SaveOptions( QSettings& settings );

	static void LoadPatterns( QSettings& settings, QVector<Pattern>& patterns );

signals:
	/**
	*	Called when options have been loaded
	*/
	void OptionsLoaded();

	/**
	*	Called when options have been saved
	*/
	void OptionsSaved();

private:
	int m_iTabWidth = DEFAULT_TAB_WIDTH;
	bool m_bMaximized = DEFAULT_START_MAXIMIZED;

	std::string m_szCurrentDir;				//The directory to start in for open and save operations

	std::shared_ptr<CConfigurationManager> m_ConfigurationManager;

	RecentFiles_t m_RecentFiles;

	QVector<Pattern> m_Patterns;
};

#endif //IDE_COPTIONS_H
