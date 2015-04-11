#ifndef CDEFERREDOUTSTREAM_H
#define CDEFERREDOUTSTREAM_H

#include <QTextEdit>

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

#endif // CDEFERREDOUTSTREAM_H

