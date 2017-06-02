#ifndef UTIL_CLISTENERMANAGER_H
#define UTIL_CLISTENERMANAGER_H

#include <vector>
#include <algorithm>

template<typename LISTENER>
class CListenerManager
{
public:
	typedef LISTENER Listener_t;
	typedef std::vector<Listener_t*> Listeners_t;

public:
	CListenerManager()
	{

	}

	~CListenerManager()
	{

	}

	bool AddListener( Listener_t* pListener )
	{
		if( !pListener )
			return false;

		if( FindListener( pListener ) != m_Listeners.end() )
			return false;

		m_Listeners.push_back( pListener );

		return true;
	}

	void RemoveListener( Listener_t* pListener )
	{
		if( !pListener )
			return;

		auto it = FindListener( pListener );

		if( it != m_Listeners.end() )
			m_Listeners.erase( it );
	}

	typename Listeners_t::iterator FindListener( Listener_t* pListener )
	{
		if( !pListener )
			return m_Listeners.end();

		return std::find( m_Listeners.begin(), m_Listeners.end(), pListener );
	}

	template<typename Func, typename... Args>
	void NotifyListeners( Func func, Args... args )
	{
		for( auto pListener : m_Listeners )
			( pListener->*func )( args... );
	}

private:
	Listeners_t m_Listeners;
};

#endif //UTIL_CLISTENERMANAGER_H
