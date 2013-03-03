#pragma once

#include "utiltemplatepoint.h"
#include "fi_wrapbuffer.h"

#pragma warning (disable : 4267 )

typedef void (*freeBufferFunc)(void *p);
typedef void* (*mallocBufferFunc)(int n);

struct FI_AutoBuffer
{
public:
	unsigned char *	pBuffer;
	int				nBufLen;
	freeBufferFunc	pFreeFunc;
	mallocBufferFunc pMallocFunc;

protected:
	long		m_nRef;

public:
	void	AddRef()		{++m_nRef;}
	int		ReleaseRef()	{  --m_nRef; return m_nRef;}

	FI_AutoBuffer()
	{
		pBuffer = NULL; nBufLen = 0; pFreeFunc = NULL;m_nRef=0;pMallocFunc=NULL;
	}
	~FI_AutoBuffer()
	{
		Release();
	}

	void Release()
	{
		if( pBuffer )
		{
			if( pFreeFunc )
			{
				(*pFreeFunc)(pBuffer);
			}
			else
			{
				delete []pBuffer;
			}
		}
		pBuffer = NULL;
		nBufLen = 0;
	}

	unsigned char *pointer() const
	{
		return pBuffer;
	}

	long length() const
	{
		return nBufLen;
	}

	freeBufferFunc freeFunc() const
	{
		return pFreeFunc;
	}

	void malloc( int nSize )
	{
		if ( nSize != nBufLen )
		{
			Release();
			if( pMallocFunc )
			{
				pBuffer = (unsigned char *) (*pMallocFunc)(nSize);
			}
			else
			{
				pBuffer = new unsigned char[ nSize ];
			}
			nBufLen = nSize;
		}
	}

	void reserve( int nSize )
	{
		if( nBufLen < nSize )
			malloc( nSize );
	}

	void copy( const FI_AutoBuffer &other )
	{
	//	Release();
	//	pMallocFunc = other.pMallocFunc;
		malloc( other.length());
		memcpy( pBuffer, other.pointer(), nBufLen );
	//	pFreeFunc = other.freeFunc();
	}

	void copy( const void *p, int nLength )
	{
		malloc( nLength );
		memcpy( pBuffer, p, nLength );
	}

	void swap( FI_AutoBuffer &other )
	{
		std::swap( pBuffer, other.pBuffer );
		std::swap( nBufLen, other.nBufLen );
		std::swap( pFreeFunc, other.pFreeFunc );
		std::swap( pMallocFunc, other.pMallocFunc );
	}

	template< typename T >
	void write( const T &value, int nOffset )
	{
		memcpy( pBuffer+nOffset, &value, sizeof(value) );
	}

	void writeBuffer( const void *p, int nLength, int nOffset )
	{
		memcpy( pBuffer+nOffset, p, nLength );
	}

	void	writeInt( int n, int nOffset )	{write(n,nOffset);}
	void	writeString( const std::string &s, int nOffset )
	{
		writeInt( s.length(), nOffset );		
		writeBuffer( s.c_str(), s.length(), nOffset+4 );
	}

	template< typename T >
		void read( T &value, int nOffset )
	{
		memcpy( &value, pBuffer+nOffset, sizeof(T) );
	}

	void readInt( int &n, int nOffset )	{read(n, nOffset);}
	void readBuffer( void *p, int nL, int nOff )	{memcpy( p,pBuffer+nOff, nL );}
	void readString( std::string &s, int nOffset )
	{
		int nStrLength;
		readInt(nStrLength, nOffset );
		s.resize( nStrLength );
		readBuffer( &(*s.begin()), nStrLength, nOffset+4 );
	}
};


typedef FI_UTIL::ut_TemplatePointer< FI_AutoBuffer >	FI_AutoBufferPointer;

template<>
inline int FI_GetConvertCharSize( const FI_AutoBuffer &value )
{
	return sizeof(int) + value.length();
}

template<>
inline bool FI_ConvertToChar( FI_TSTR &pBegin, const FI_AutoBuffer &value, FI_CTSTR pEnd )
{
	if( pEnd-pBegin < value.length() + 4 )
		return false;
	memcpy( pBegin, &value.nBufLen, sizeof(int) );
	pBegin += sizeof(int);
	if( value.length() > 0 )
	{
		memcpy( pBegin, value.pBuffer, value.length() );
		pBegin += value.length();
	}
	return true;
}

template<>
inline bool
FI_ConvertFromChar( FI_CTSTR &pBegin, FI_AutoBuffer &value, FI_CTSTR pEnd )
{
	if( pEnd - pBegin < sizeof(int) )
		return false;
	int nBufLength = 0;
	memcpy( &nBufLength, pBegin, sizeof(int) );
	pBegin += sizeof(int);
	if( nBufLength > pEnd-pBegin )
		return false;
	if( nBufLength > 0 )
	{
		value.malloc( nBufLength );
		memcpy( value.pBuffer, pBegin, value.nBufLen );
		pBegin += value.nBufLen;
	}
	else
		value.Release();
	return true;
}

template<>
inline int FI_GetConvertCharSize( const FI_AutoBufferPointer &value )
{
	if( value != NULL )
		return FI_GetConvertCharSize(*value);
	else
		return FI_GetConvertCharSize(FI_AutoBuffer());
}

template<>
inline bool FI_ConvertToChar( FI_TSTR &pBegin, const FI_AutoBufferPointer &value, FI_CTSTR pEnd )
{
	if( value != NULL )
		return FI_ConvertToChar( pBegin, *value, pEnd );
	else
		return FI_ConvertToChar( pBegin, FI_AutoBuffer(), pEnd );
}

template<>
inline bool
FI_ConvertFromChar( FI_CTSTR &pBegin, FI_AutoBufferPointer &value, FI_CTSTR pEnd )
{
	if( value == NULL )
		value = new FI_AutoBuffer;
	return FI_ConvertFromChar( pBegin, *value, pEnd );
}

template < class T >
bool FI_ConvertAnyTypeToBuffer( const T &value, FI_AutoBuffer &autoBuffer, int nBegin )
{
/*	if( autoBuffer.length()-nBegin < sizeof(T) )
		return false;
	memcpy( autoBuffer.pointer()+nBegin, &value, sizeof(T) );
	return true;*/
	char *pBegin = (char *)autoBuffer.pBuffer + nBegin;
	char *pEnd = (char *)autoBuffer.pBuffer + autoBuffer.length();
	return FI_ConvertToChar( pBegin, value, pEnd );
}

template < class T >
int FI_CaclStructSize( T &value )
{
	return sizeof(value);
}

template < class T >
bool FI_ConvertBuildFromBuffer( T &value, const FI_AutoBuffer &autoBuffer, int nBegin )
{
/*	if( autoBuffer.length() - nBegin < sizeof(T) )
		return false;
	memcpy( &value, autoBuffer.pointer()+nBegin, sizeof(T) );
	return true;*/
	char *pBegin = (char *)autoBuffer.pBuffer + nBegin;
	char *pEnd = (char *)autoBuffer.pBuffer + autoBuffer.length();
	return FI_ConvertFromChar( pBegin, value, pEnd );
}

#pragma warning (default : 4267 )


