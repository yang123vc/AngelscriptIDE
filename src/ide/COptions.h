#ifndef IDE_COPTIONS_H
#define IDE_COPTIONS_H

#include <memory>
#include <vector>
#include <list>

class CConfiguration;

/**
*	Stores the configurations and global settings
*	TODO: this is redundant because it stores data that is stored elsewhere as well. Refactor this class out of existence as much as possible - Solokiller
*/
class COptions
{
public:
	static const char CONFIGS_DELIMITER = ';';
	static const char RECENTFILES_DELIMITER = ';';

	static const int DEFAULT_TAB_WIDTH = 40;

	static const size_t MAX_RECENT_FILES = 5;

public:
	typedef std::vector<std::string> Configurations_t;
	typedef std::list<std::string> RecentFiles_t;

	COptions( const std::string& szFilename );
	~COptions() = default;

	COptions( const COptions& other ) = default;
	COptions& operator=( const COptions& other ) = default;

	Configurations_t& GetConfigurations() { return m_Configurations; }
	const Configurations_t& GetConfigurations() const { return m_Configurations; }

	const std::string& GetActiveConfigurationName() const { return m_szActiveConfiguration; }

	void SetActiveConfigurationName( const std::string& szActiveConfiguration );

	int GetTabWidth() const { return m_iTabWidth; }

	void SetTabWidth( int iTabWidth ) { m_iTabWidth = iTabWidth; }

	bool StartMaximized() const { return m_fMaximized; }

	void SetStartMaximized( bool fMaximized ) { m_fMaximized = fMaximized; }

	const RecentFiles_t& GetRecentFiles() const { return m_RecentFiles; }

	void AddRecentFile( const std::string& szFilename );
	void RemoveRecentFile( const std::string& szFilename );

	void ClearRecentFiles();

	const std::string& GetCurrentDirectory() const { return m_szCurrentDir; }
	void SetCurrentDirectory( const std::string& szCurrentDir ) { m_szCurrentDir = szCurrentDir; }

	static std::shared_ptr<COptions> Load( const std::string& szFilename, bool* pLoaded = nullptr );

	bool Save();

private:
	std::string m_szFilename;
	Configurations_t m_Configurations;
	std::string m_szActiveConfiguration;
	int m_iTabWidth;
	bool m_fMaximized;

	RecentFiles_t m_RecentFiles;

	std::string m_szCurrentDir;				//The directory to start in for open and save operations
};

#endif //IDE_COPTIONS_H
