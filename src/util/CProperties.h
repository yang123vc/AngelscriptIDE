#ifndef CPROPERTIES_H
#define CPROPERTIES_H

#include <unordered_map>
#include <string>

class CProperties
{
public:
	typedef std::unordered_map<std::string, std::string> Keyvalues_t;

	CProperties();
	~CProperties();

	Keyvalues_t& GetKeyvalues() { return m_Keyvalues; }

	const Keyvalues_t& GetKeyvalues() const { return m_Keyvalues; }

	bool Load( const std::string& szFilename );

	bool Save( const std::string& szFilename );

private:
	Keyvalues_t m_Keyvalues;
};

#endif // CPROPERTIES_H
