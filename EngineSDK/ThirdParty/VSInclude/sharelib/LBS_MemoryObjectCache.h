#pragma once

#include "fi_share/fi_messagestruct.h"

//// cache memory struct

template<typename T >
struct __LBS_CacheData
{
	T		m_value;
	bool	m_bDirty;
	__LBS_CacheData() {m_bDirty=false;}
	void	SetDirty( bool b  )	{m_bDirty=b;}
	bool	IsDirty()			{return m_bDirty;}
	T&		GetData()			{return m_value;}
	void	SetData( T &value )	{m_value=value;}
};

template< typename TIndex,typename T >
class LBS_MemoryObjectCache
{
public:
	LBS_MemoryObjectCache(void) {}
	~LBS_MemoryObjectCache(void) {clear();}

	typedef typename __LBS_CacheData<T>			__SelfCacheData;
	typedef typename LBS_MemoryObjectCache<TIndex,T>	__Self;
	typedef typename std::map<TIndex, __SelfCacheData>::iterator		__SelfCacheDataIterator;

protected:
	std::map< TIndex, __SelfCacheData >			m_cacheIDDataArray;

public:
	void	SetCacheData( const TIndex &id, T &value)
	{
		__SelfCacheDataIterator itLie = m_cacheIDDataArray.find( id );
		if( itLie == m_cacheIDDataArray.end() )
		{
			__SelfCacheData tmp;
			tmp.m_bDirty = true;
			tmp.m_value = value;
			m_cacheIDDataArray[id] = tmp;
		}
		else
		{
			itLie->second.SetData( value );
			itLie->second.SetDirty( true );
		}
	}

	void	SetDirty( const TIndex &id, bool bDirty )
	{
		__SelfCacheDataIterator itLie = m_cacheIDDataArray.find( id );
		if( itLie != m_cacheIDDataArray.end() )
			itLie->second.SetDirty( bDirty );
	}

	bool	GetCacheData( const FI_GID_T &id, T &value )
	{
		__SelfCacheDataIterator itLie = m_cacheIDDataArray.find( id );
		if( itLie != m_cacheIDDataArray.end() )
		{
			value = itLie->second.GetData();
			return true;
		}
		else
			return false;
	}

	__SelfCacheDataIterator	Find ( const TIndex &id )
	{
		return  m_cacheIDDataArray.find( id );
	}

	__SelfCacheDataIterator	Begin()
	{
		return m_cacheIDDataArray.begin();
	}

	__SelfCacheDataIterator		End()
	{
		return m_cacheIDDataArray.end();
	}

	void	clear()
	{
		m_cacheIDDataArray.clear();
	}

	void	Erase(  const TIndex &id )
	{
		m_cacheIDDataArray.erase( id );
	}
};
