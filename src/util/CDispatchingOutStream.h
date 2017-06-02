#ifndef UTIL_CDEFERREDOUTSTREAM_H
#define UTIL_CDEFERREDOUTSTREAM_H

#include "COutStream.h"

class IOutStreamListener
{
public:

	virtual ~IOutStreamListener() = 0;

	virtual void WriteString( const char* pszString ) = 0;
};

inline IOutStreamListener::~IOutStreamListener()
{
}

/**
*	Dispatches output from a given stream to the given listener
*/
class CDispatchingOutStream : public COutStream<CDispatchingOutStream>
{
public:
	CDispatchingOutStream( std::ostream& outStream, IOutStreamListener* pListener )
		: COutStream( outStream )
		, m_pListener( pListener )
	{
	}

	void WriteString( const char* pszString )
	{
		if( m_pListener )
			m_pListener->WriteString( pszString );
	}

private:
	IOutStreamListener* m_pListener;
};

#endif //UTIL_CDEFERREDOUTSTREAM_H
