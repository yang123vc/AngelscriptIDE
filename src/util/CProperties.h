#ifndef UTIL_CPROPERTIES_H
#define UTIL_CPROPERTIES_H

#include <unordered_map>
#include <string>

/**
*	Provides the ability to load and save properties data
*/
class CProperties
{
public:
	typedef std::unordered_map<std::string, std::string> Keyvalues_t;

	CProperties();
	~CProperties();

	Keyvalues_t& GetKeyvalues() { return m_Keyvalues; }

	const Keyvalues_t& GetKeyvalues() const { return m_Keyvalues; }

	/**
	*	Loads data from the given file
	*	Clears any data that was previously stored
	*	@return Whether the file was successfully loaded
	*/
	bool Load( const std::string& szFilename );

	/**
	*	Saves data to the given file
	*	@return Whether the data was successfully saved
	*/
	bool Save( const std::string& szFilename );

private:
	Keyvalues_t m_Keyvalues;
};

#endif //UTIL_CPROPERTIES_H
