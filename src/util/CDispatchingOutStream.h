#ifndef UTIL_CDISPATCHINGOUTSTREAM_H
#define UTIL_CDISPATCHINGOUTSTREAM_H

#include <QObject>

#include "COutStream.h"

/**
*	Dispatches output from a given stream to the given listener
*/
class CDispatchingOutStream : public QObject, public COutStream<CDispatchingOutStream>
{
	Q_OBJECT

public:
	CDispatchingOutStream( std::ostream& outStream )
		: COutStream( outStream )
	{
	}

signals:
	void WriteString( const char* pszString );
};

#endif //UTIL_CDISPATCHINGOUTSTREAM_H
