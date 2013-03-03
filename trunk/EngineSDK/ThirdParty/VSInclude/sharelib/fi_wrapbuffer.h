#ifndef __FI_WRAP_BUFFER_H__
#define __FI_WRAP_BUFFER_H__

#include <vector>
#include <list>
#include <set>

//#define FI_TSTR		char *
//#define FI_CTSTR	LPCTSTR
typedef char *			FI_TSTR;
typedef const char *	FI_CTSTR;

#define FI_FALSE_RETURN(x) {if(!x) return false;}
#define FI_FALSE_RETURN_VOID(x) {if(!x) return;}

#pragma warning( disable :  4244 4267 4018 )

template< class T >
void	FI_ConvertMemCpyToBuf( FI_TSTR &p, const T &value )
{
	memcpy( p, &value, sizeof(value) );
	p += sizeof(value );
}

template< class T >
void	FI_ConvertMemCpyFromBuf( FI_CTSTR &p, T &value )
{
	memcpy( &value, p, sizeof(value) );
	p+=sizeof(value);
}

template < class T >
inline
int FI_GetConvertCharSize( const T &value )
{
	return sizeof(value );
}

template < class T >
inline
bool FI_ConvertToChar( FI_TSTR &p, const T &value, FI_CTSTR pEnd )
{
	int nSize = FI_GetConvertCharSize( value );
	if( pEnd - p < nSize)
		return false;
	memcpy( p, &value, nSize );
	p += nSize;
	return true;
}

template < class T >
inline
bool FI_ConvertFromChar( FI_CTSTR &p, T &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	int nNeedSize = FI_GetConvertCharSize( value );
	if( nMaxSize < nNeedSize )
		return false;
	memcpy( &value, p, nNeedSize );
	p += nNeedSize;
	return true;
}

template <>
inline
int FI_GetConvertCharSize( const std::string &value )
{
	return value.size() + sizeof(int);
}

template<>
inline
bool FI_ConvertToChar( FI_TSTR &p, const std::string &value, FI_CTSTR pEnd )
{
	int nSize = FI_GetConvertCharSize( value );
	if( pEnd - p < nSize)
		return false;
	int nStrLength = value.size();
	memcpy( p, &nStrLength, sizeof(int) );
	p+=sizeof(int);
	memcpy( p, value.c_str(), value.size() );
	p += value.size();
	return true;
}

template < >
inline
bool FI_ConvertFromChar( FI_CTSTR &p, std::string &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	if( nMaxSize < 4 )
		return false;
	int nNeedSize = 0;
	memcpy( &nNeedSize, p, sizeof(int));
	p += sizeof(int);
	if( nNeedSize+4 > nMaxSize )
		return false;
	if( nNeedSize > 0 )
	{
		value.append( p, nNeedSize );
		p += nNeedSize;
	}
	return true;
}

template <>
inline
int FI_GetConvertCharSize( const std::wstring &value )
{
	return value.size()*2 + sizeof(int);
}

template<>
inline
bool FI_ConvertToChar( FI_TSTR &p, const std::wstring &value, FI_CTSTR pEnd )
{
	int nSize = FI_GetConvertCharSize( value );
	if( pEnd - p < nSize)
		return false;
	int nStrLength = value.size();
	memcpy( p, &nStrLength, sizeof(int) );
	p+=sizeof(int);
	memcpy( p, value.c_str(), value.size()*2 );
	p += value.size()*2;
	return true;
}

template < >
inline
bool FI_ConvertFromChar( FI_CTSTR &p, std::wstring &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	if( nMaxSize < 4 )
		return false;
	int nNeedSize = 0;
	memcpy( &nNeedSize, p, sizeof(int));
	p += sizeof(int);
	if( nNeedSize+4 > nMaxSize )
		return false;
	if( nNeedSize > 0 )
	{
		value.append( (wchar_t*)p, nNeedSize );
		p += nNeedSize*2;
	}
	return true;
}


template <class T >
inline
int FI_GetConvertCharSize( const std::vector<T> &value )
{
	int nSize = sizeof(int);
	for( unsigned int i=0; i<value.size(); i++ )
	{
		nSize += FI_GetConvertCharSize( value[i] );
	}
	return nSize;
}

template< class T >
inline
bool FI_ConvertToChar( FI_TSTR &p, const std::vector<T> &value, FI_CTSTR pEnd )
{
	int nSize = FI_GetConvertCharSize( value );
	if( pEnd - p < nSize )
		return false;
	unsigned int nArraySize = value.size();
	memcpy( p, &nArraySize, sizeof(int) );
	p += 4;
	for( unsigned int i=0; i<nArraySize; i++ )
	{
		bool b = FI_ConvertToChar( p, value[i], pEnd );
		FI_FALSE_RETURN( b );
	}
	return true;
}

template< class T >
inline
bool FI_ConvertFromChar( FI_CTSTR &p, std::vector<T> &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	if( nMaxSize < sizeof(int) )
		return false;
	int nNum = 0;
	memcpy(&nNum, p,  sizeof(int) );
	p+=4;
	value.reserve( nNum );
	for( int i=0; i<nNum; i++ )
	{
		T tmp;
		bool b = FI_ConvertFromChar( p, tmp, pEnd );
		FI_FALSE_RETURN( b );
		value.push_back( tmp );
	}
	return true;
}


template< class T >
inline
bool FI_QArrayConvertToChar( FI_TSTR &p, const std::vector<T> &value, FI_CTSTR pEnd )
{
	unsigned int nArraySize = value.size();
	int nOneSize = sizeof(T);
	int nSize = nArraySize * nOneSize;
	if( (pEnd - p < nSize) || (nArraySize<1) )
		return false;
	memcpy( p, &nArraySize, sizeof(int) );
	p += 4;
	memcpy( p, &value[0], nSize );
	p+=nSize;	
	return true;
}

template< class T >
inline
bool FI_QArrayConvertFromChar( FI_CTSTR &p, std::vector<T> &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	if( nMaxSize < sizeof(int) )
		return false;
	int nNum = 0;
	memcpy(&nNum, p,  sizeof(int) );
	p+=4;
	if( nNum < 1 )
		return true;
	value.resize( nNum );
	int nSize = nNum * sizeof(T);
	if( pEnd - p < nSize )
		return false;
	memcpy( &value[0], p, nSize );
	p+=nSize;
	return true;
}

template <class T >
inline
int FI_GetConvertCharSize( const std::list<T> &value )
{
	int nSize = sizeof(int);
	std::list<T>::const_iterator itLie = value.begin();
	std::list<T>::const_iterator itEnd = value.end();
	for( ; itLie!=itEnd; ++itLie )
	{
		nSize += FI_GetConvertCharSize( *itLie );
	}
	return nSize;
}


template< class T >
inline
bool FI_ConvertToChar( FI_TSTR &p, const std::list<T> &value, FI_CTSTR pEnd )
{
	int nSize = FI_GetConvertCharSize( value );
	if( pEnd - p < nSize )
		return false;
	unsigned int nArraySize = value.size();
	memcpy( p, &nArraySize, sizeof(int) );
	p += 4;
	std::list<T>::const_iterator itBegin = value.begin();
	std::list<T>::const_iterator itEnd = value.end();
	for( ; itBegin!=itEnd; ++itBegin)
	{
		bool b = FI_ConvertToChar( p, *itBegin, pEnd );
		FI_FALSE_RETURN( b );
	}
	return true;
}

template< class T >
inline
bool FI_ConvertFromChar( FI_CTSTR &p, std::list<T> &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	if( nMaxSize < sizeof(int) )
		return false;
	int nNum = 0;
	memcpy(&nNum, p,  sizeof(int) );
	p+=4;
	for( int i=0; i<nNum; i++ )
	{
		T tmp;
		bool b = FI_ConvertFromChar( p, tmp, pEnd );
		FI_FALSE_RETURN( b );
		value.push_back( tmp );
	}
	return true;
}

template <class T >
inline
int FI_GetConvertCharSize( const std::set<T> &value )
{
	int nSize = sizeof(int);
	std::set<T>::const_iterator itLie = value.begin();
	std::set<T>::const_iterator itEnd = value.end();
	for( ; itLie!=itEnd; ++itLie )
	{
		nSize += FI_GetConvertCharSize( *itLie );
	}
	return nSize;
}


template< class T >
inline
bool FI_ConvertToChar( FI_TSTR &p, const std::set<T> &value, FI_CTSTR pEnd )
{
	int nSize = FI_GetConvertCharSize( value );
	if( pEnd - p < nSize )
		return false;
	unsigned int nArraySize = value.size();
	memcpy( p, &nArraySize, sizeof(int) );
	p += 4;
	std::set<T>::const_iterator itBegin = value.begin();
	std::set<T>::const_iterator itEnd = value.end();
	for( ; itBegin!=itEnd; ++itBegin)
	{
		bool b = FI_ConvertToChar( p, *itBegin, pEnd );
		FI_FALSE_RETURN( b );
	}
	return true;
}

template< class T >
inline
bool FI_ConvertFromChar( FI_CTSTR &p, std::set<T> &value, FI_CTSTR pEnd )
{
	int nMaxSize = pEnd - p;
	if( nMaxSize < sizeof(int) )
		return false;
	int nNum = 0;
	memcpy(&nNum, p,  sizeof(int) );
	p+=4;
	for( int i=0; i<nNum; i++ )
	{
		T tmp;
		bool b = FI_ConvertFromChar( p, tmp, pEnd );
		FI_FALSE_RETURN( b );
		value.insert( tmp );
	}
	return true;
}
#pragma warning( default : 4244 4267 4018)

#endif

