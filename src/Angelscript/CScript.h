#ifndef ANGELSCRIPT_CSCRIPT_H
#define ANGELSCRIPT_CSCRIPT_H

#include <string>

/**
*	Represents a single script file
*/
class CScript
{
public:
	explicit CScript( std::string&& szSectionName );

	CScript( std::string&& szSectionName, std::string&& szContents );

	~CScript() = default;

	CScript( const CScript& other ) = default;
	CScript( CScript&& other ) = default;
	CScript& operator=( const CScript& other ) = default;
	CScript& operator=( CScript&& other ) = default;

	/**
	*	Creates a new script initialized with the contents from the given file
	*	@param szFileName Name of the file to load
	*	@param szSectionName If not empty, the name of the script section. If empty, the name of the file is used
	*/
	static CScript FromFile( const std::string& szFileName, const std::string& szSectionName = "" );

	const std::string& GetSectionName() const { return m_szSectionName; }

	/**
	*	Sets the section name
	*/
	void SetSectionName( std::string&& szSectionName );

	const std::string& GetContents() const { return m_szContents; }

	/**
	*	Replaces the contents with the given contents
	*/
	void SetContents( std::string&& szContents );

	/**
	*	Loads the contents of the given file
	*	Newlines are normalized to only contain \n
	*	@param[ out ] bSuccess Whether the file was successfully loaded
	*/
	static std::string LoadContentsFromFile( const std::string& szFileName, bool& bSuccess );

	static std::string LoadContentsFromFile( const std::string& szFileName );

	/**
	*	Loads contents from the given file. Any existing contents are discarded
	*	@return Whether the contents were successfully loaded from file
	*/
	bool LoadFromFile( const std::string& szFileName );

	/**
	*	Saves the contents to the given file
	*	@return Whether the contents were successfully saved to file
	*/
	bool SaveToFile( const std::string& szFileName );

private:
	std::string m_szSectionName;
	std::string m_szContents;
};

#endif //ANGELSCRIPT_CSCRIPT_H
