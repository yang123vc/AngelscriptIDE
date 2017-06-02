#ifndef UTIL_COUTSTREAM_H
#define UTIL_COUTSTREAM_H

#include <ostream>

/**
*	Base class for stream buffers that redirect output to another location
*	Uses the curiously recurring template pattern, provide \code method void WriteString( const char* ) \endcode to receive data.
*	The original buffer is restored when this object is destroyed
*/
template<typename SUBCLASS>
class COutStream : public std::streambuf
{
public:
	/**
	*	@param outStream Stream to redirect
	*/
	COutStream( std::ostream& outStream )
		: m_OutStream( outStream )
		, m_pOriginalBuf( nullptr )
	{
		m_pOriginalBuf = m_OutStream.rdbuf( this );
	}

	/**
	*	Restores the original buffer
	*/
	virtual ~COutStream()
	{
		m_OutStream.rdbuf( m_pOriginalBuf );
	}

	virtual int overflow( int ch )
	{
		int result = EOF;

		if ( ch == EOF )
			result = sync();
		else
		{
			char szBuffer[] = { ( char ) ch, '\0' };
			static_cast<SUBCLASS*>( this )->WriteString( szBuffer );

			result = ch;
		}

		return result;
	}

private:
	std::ostream& m_OutStream;
	std::streambuf* m_pOriginalBuf;

private:
	COutStream( const COutStream& ) = delete;
	COutStream& operator=( const COutStream& ) = delete;
};

#endif //UTIL_COUTSTREAM_H
