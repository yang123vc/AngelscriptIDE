#ifndef CSCRIPTFILE_H
#define CSCRIPTFILE_H

#include <memory>
#include <string>

class CScriptFile
{
public:
	CScriptFile( const std::string& szFilename );
	~CScriptFile();

	const std::string& GetFilename() const { return m_szFilename; }

	void SetFilename( const std::string& szFilename );

	std::unique_ptr<std::string> GetScriptContents() const;

	bool SetScriptContents( const std::string& szContents );

private:
	std::string m_szFilename;

private:
	CScriptFile( const CScriptFile& ) = delete;
	CScriptFile& operator=( const CScriptFile& ) = delete;
};

#endif // CSCRIPTFILE_H
