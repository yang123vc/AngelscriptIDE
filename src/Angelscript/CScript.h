#ifndef ANGELSCRIPT_CSCRIPT_H
#define ANGELSCRIPT_CSCRIPT_H

#include <vector>
#include <string>
#include <memory>

class CConfiguration;

class CScript
{
public:
	CScript( const std::string& szSectionName, const std::string& szContents,
			 std::shared_ptr<const CConfiguration> config = nullptr );
	~CScript();

	CScript( const CScript& other );
	CScript& operator=( const CScript& other );

	const std::string& GetSectionName() const { return m_szSectionName; }

	const std::string& GetContents() const { return m_szContents; }

	std::shared_ptr<const CConfiguration> GetConfiguration() const { return m_Configuration; }

protected:
	void Copy( const CScript& other );

private:
	std::string m_szSectionName;
	std::string m_szContents;

	std::shared_ptr<const CConfiguration> m_Configuration;
};

#endif //ANGELSCRIPT_CSCRIPT_H
