#ifndef __FI_MSG_STRUCT_H__
#define __FI_MSG_STRUCT_H__

#include <time.h>
#include "fi_autobuffer.h"



#pragma warning( disable : 4244 )

#pragma pack( push, 1 )


#define FI_MACRO_SUCCESS	1
#define FI_MACRO_FAIL		0

struct FI_MSG_HEADER_T
{
	unsigned int nMsgLength;
	unsigned int nSerialNumber;
	
	unsigned short	nFollowMsgNumber;
	unsigned short	nAppendNumber;

	unsigned short nMsgID;
	struct
	{
		unsigned char comLevel : 2;
		unsigned char version : 6;
		unsigned char secType : 4;
		unsigned char bPacket : 1;
		unsigned char bError : 1;
		unsigned char bAdd : 1;	///有附加信息.对于转发或者某些处理可能需要额外的数据进行处理
		unsigned char bKeep : 1;
	} nMsgNorStatus;
	FI_MSG_HEADER_T()
	{
		memset( this, 0, sizeof(this) );
	}

	unsigned int		GetFullMsgLength()
	{
		
	}
};

struct FI_MSG_APPEND_HEADER_T
{
	unsigned int	nAppendBufLength;
	int				nAppendBufferType;
};

struct FI_MSG_T
{
	FI_MSG_HEADER_T m_FI_Header;
	void*		m_FI_Content;
};



struct FI_GID_T
{
	int nRandom;
	int nPlayerID;
	FI_GID_T() { nPlayerID = 0;nRandom=0; }
	FI_GID_T(int r, int u) {nPlayerID=u;nRandom=r;}

	bool	operator == ( const FI_GID_T &r ) const
	{
		return memcmp( this, &r, sizeof(FI_GID_T) ) == 0 ;
	}

	bool	operator != ( const FI_GID_T &r ) const
	{
		return memcmp( this, &r, sizeof(FI_GID_T) ) != 0;
	}

	const	int		&first()	const	{return nRandom;}
	const	int		&second()	const {return nPlayerID;}
};

inline
void GID_2_ARRAY( FI_GID_T &gID, int a[2] )
{
	a[0] = gID.nRandom;
	a[1] = gID.nPlayerID;
}

inline
void ARRAY_2_GID( int a[2], FI_GID_T &gID )
{
	gID.nRandom = a[0];
	gID.nPlayerID = a[1];
}

#define INVALID_FI_ID					FI_GID_T(0,0)
#define FI_TARGET_SMALLREGION_ID		FI_GID_T(66,1000)
#define FI_TARGET_MIDREGION_ID			FI_GID_T(66,1002)
#define FI_TARGET_CUREFFECT_ID			FI_GID_T(66,1003)

struct FI_SecValue_T
{
	char buf[64];
};

#pragma pack( pop )

inline
bool operator < ( const FI_GID_T &left, const FI_GID_T &right )
{
	return memcmp( &left, &right, sizeof(FI_GID_T) ) < 0 ;
	//	return left.nPlayerID < right.nPlayerID;
}

/// get a radn id
inline
FI_GID_T FI_RandID()
{
	static int nIDNum = 1000;
	FI_GID_T tmp;
	tmp.nRandom = time(NULL) + 1000;
	tmp.nPlayerID = ++ nIDNum;
	return tmp;
}

struct FI_CHARID
{
	int n1;
	int n2;

	FI_CHARID & operator = ( const FI_GID_T &gID )
	{
		n1 = gID.nRandom;
		n2 = gID.nPlayerID;
		return *this;
	}

	bool	operator != ( const FI_GID_T &gID )
	{
		if( n1 == gID.first() && n2 == gID.second() )
			return false;
		else
			return true;
	}

	bool	operator ==		( const FI_GID_T &gID )
	{
		if( n1 == gID.first() && n2 == gID.second() )
			return true;
		else
			return false;
	}
};

inline
FI_GID_T CID_2_GID( const FI_CHARID &cID )
{
	FI_GID_T t;
	t.nRandom = cID.n1;
	t.nPlayerID = cID.n2;
	return t;
}


inline
void	CID_2_ARRAY( FI_CHARID &cID, int a[2] )
{
	a[0] = cID.n1;
	a[1] = cID.n2;
}


inline	
void	ARRAY_2_CID( int a[2], FI_CHARID &cID )
{
	cID.n1 = a[0];
	cID.n2 = a[1];
}


inline
FI_SecValue_T FI_RandSecValue()
{
	FI_SecValue_T v;
	for( int i=0; i<63; i++ )
	{
		v.buf[i] = 'a' + rand()%30;
	}
	v.buf[63] = 0;
	return v;
}


#define FI_MSG_COME_FROM_PLAYER		1
#define FI_MSG_COME_FROM_DC			2
#define FI_MSG_COME_FROM_CS			3
#define FI_MSG_COME_FROM_NS			4
#define FI_MSG_COME_FROM_LOCAL		5


template<typename T>
inline
FI_AutoBufferPointer FI_DefaultBuildBuffer( T &val, int nOffset=0, int nDefSize=-1 )
{
	int nSize = FI_GetConvertCharSize(val);
	FI_AutoBufferPointer bufferP = new FI_AutoBuffer;
	if( nDefSize > nOffset+nSize ) 
		bufferP->malloc( nDefSize );
	else
		bufferP->malloc( nOffset+nSize );
	if( nOffset > 0 )
		memset( bufferP->pointer(), 0, nOffset );
//	char *pBegin = (char *)bufferP->pointer();
//	char *pEnd = (char *)pBegin+nSize;
//	bool b = FI_ConvertToChar( pBegin, val, pEnd );
//	if( b )
//	{
//		return bufferP;
//	}
	if( FI_ConvertAnyTypeToBuffer( val, *bufferP, nOffset ) )
		return bufferP;
	else
		return NULL;
}

template<typename T>
inline
FI_AutoBufferPointer FI_DefaultBuildBuffer( T &val, T &val2, int nOffset=0, int nDefSize=-1 )
{
	int nSize = FI_GetConvertCharSize(val);
	int nSize2 = FI_GetConvertCharSize( val2 );
	FI_AutoBufferPointer bufferP = new FI_AutoBuffer;
	if( nDefSize > nOffset+nSize+nSize2 ) 
		bufferP->malloc( nDefSize );
	else
		bufferP->malloc( nOffset+nSize+nSize2 );
	if( nOffset > 0 )
		memset( bufferP->pointer(), 0, nOffset );
//	char *pBegin = (char *)bufferP->pointer();
//	char *pEnd = (char *)pBegin+nSize;
//	bool b = FI_ConvertToChar( pBegin, val, pEnd );
//	if( b )
//	{
//		return bufferP;
//	}
	if( FI_ConvertAnyTypeToBuffer( val, *bufferP, nOffset ) )
	{
		if( FI_ConvertAnyTypeToBuffer( val2, *bufferP, nOffset+nSize ) )
			return bufferP;
		else
			return NULL;
	}
	else
		return NULL;
}

#define __TYPE(a)					static const int typeV() {return a;}

template< typename T >
inline int __getType( const T &t )	{return t.typeV();}


/// Msg Cache
struct FI_MSG_CACHE_T
{
	unsigned int			hSocket;	///当读消息时,返回socket值,客户端应该仅用其作标志
	FI_GID_T				nPlayerID;	///如果失败，这个值就是标志
	short					nError;		///msg id,如果发生错误，由系统写入失败原因
	unsigned short			nMsgFrom;	///消息发送，受到的源
//	int						nMsgLength;
	FI_AutoBufferPointer	pBuffer;	///要发送的缓冲区,是一个打包的FI_MSG_T对象
	FI_MSG_CACHE_T() { hSocket=0; pBuffer=NULL; }
	~FI_MSG_CACHE_T()
	{
		hSocket=0; pBuffer=NULL;nPlayerID.nPlayerID=0;nError=0; nMsgFrom=0; //nMsgLength=0;
	}


	
	int	GetValidBufferLength() const
	{
		return ValidBufferLength()+sizeof(FI_MSG_HEADER_T);
	//	int nValidLength = ValidBufferLength();
	//	nValidLength+=sizeof(FI_MSG_HEADER_T);
	//	return  nValidLength> nMsgLength ? nValidLength:nMsgLength;
	}

	void SetValidBufferLength( int nMsgL )
	{
	//	nMsgLength = nMsgL;
	}

	bool hasBody()
	{
		return GetValidBufferLength()>GetFirstBodyLie();
	}

	int	ValidBufferLength() const
	{
		if( pBuffer == NULL )
			return 0;
		FI_MSG_HEADER_T *pHeader = (FI_MSG_HEADER_T *)pBuffer->pBuffer;
		return pHeader->nMsgLength;
	}

	FI_MSG_APPEND_HEADER_T*		GetFirstAppendHeader( FI_MSG_HEADER_T *pH )
	{
		char *p =  (char *)pH;
	//	p += pH->nMsgLength;
		p+=sizeof(FI_MSG_HEADER_T);
		return (FI_MSG_APPEND_HEADER_T*)p;
	}

	FI_MSG_APPEND_HEADER_T*		GetNextAppendMsgHeader( FI_MSG_APPEND_HEADER_T *pH )
	{
		char *p = (char *)pH;
		p+=pH->nAppendBufLength;
		p+=sizeof( FI_MSG_APPEND_HEADER_T );
		return (FI_MSG_APPEND_HEADER_T*)p;
	}

	FI_MSG_HEADER_T *	GetNextMsgHeader( FI_MSG_HEADER_T *pH )
	{
		if( pH->nFollowMsgNumber < 1 )
			return NULL;
		int nOffset = sizeof(FI_MSG_HEADER_T);//pH->nMsgLength;
	///	nOffset += ;

		if( pH->nAppendNumber > 0 )
		{
			FI_MSG_APPEND_HEADER_T*	pAppend = GetFirstAppendHeader(pH);
			nOffset += pAppend->nAppendBufLength;
			nOffset += sizeof( FI_MSG_APPEND_HEADER_T );
	
			for( int i=1; i<pH->nAppendNumber; ++i )
			{
				pAppend = GetNextAppendMsgHeader( pAppend );
				nOffset += pAppend->nAppendBufLength;
				nOffset += sizeof( FI_MSG_APPEND_HEADER_T );
			}
		}
		else
			nOffset += pH->nMsgLength;

		char *p =  (char *)pH;
		p += nOffset;
		return (FI_MSG_HEADER_T *)pH;
	}

	FI_MSG_HEADER_T *	GetMsgHeader()
	{
		if( pBuffer == NULL )
			return 0;
		FI_MSG_HEADER_T *pHeader = (FI_MSG_HEADER_T *)pBuffer->pBuffer;
		return pHeader;
	}

	int	GetFirstBodyLie()
	{
		FI_MSG_HEADER_T *pHeader = (FI_MSG_HEADER_T *)pBuffer->pBuffer;
		if( pHeader->nMsgNorStatus.bAdd )
		{
			return sizeof(FI_MSG_HEADER_T) + sizeof(FI_MSG_APPEND_HEADER_T);
		}
		else
			return sizeof(FI_MSG_HEADER_T);
	}

	void * GetMsgBody()
	{
		if( pBuffer == NULL )
			return 0;
		FI_MSG_HEADER_T *pHeader = (FI_MSG_HEADER_T *)pBuffer->pBuffer;
		unsigned char *p = NULL;
		if( !pHeader->nMsgNorStatus.bAdd )		
			p = pBuffer->pBuffer + sizeof(FI_MSG_HEADER_T);
		else
			p = pBuffer->pBuffer + sizeof(FI_MSG_HEADER_T) + sizeof(FI_MSG_APPEND_HEADER_T);
		return p;
	}

	FI_MSG_APPEND_HEADER_T*	GetAppendMsgHeader( int nLie )
	{
		FI_MSG_HEADER_T *pHeader = (FI_MSG_HEADER_T *)pBuffer->pBuffer;
		if( (!pHeader->nMsgNorStatus.bAdd) || (nLie >= pHeader->nAppendNumber) || (nLie<0))
			return NULL;
		int nMsgLength = GetValidBufferLength();
		unsigned char *pLie = pBuffer->pBuffer;
		pLie+=sizeof(FI_MSG_HEADER_T);
		pHeader = (FI_MSG_HEADER_T *)pLie;
	
		FI_MSG_APPEND_HEADER_T *pAppendHeader = NULL;
	//	pLie+=pHeader->nMsgLength;
		for( int n=0; n<nLie; n++ )
		{
			if( pLie+sizeof(FI_MSG_APPEND_HEADER_T) >= pBuffer->pBuffer+nMsgLength )
				return NULL;
			pAppendHeader = (FI_MSG_APPEND_HEADER_T *)pLie;
			pLie+=sizeof(FI_MSG_APPEND_HEADER_T);
			pLie+=pAppendHeader->nAppendBufLength;
		}
		pAppendHeader = (FI_MSG_APPEND_HEADER_T *)pLie;
		if( pLie+sizeof(FI_MSG_APPEND_HEADER_T) >= pBuffer->pBuffer+nMsgLength )
			return NULL;
		return pAppendHeader;
	}
};


struct FI_MESSAGE_CACHE_T_WITH_ADDR : public FI_MSG_CACHE_T
{
	char		m_netAddress[16];

	template< typename T >	 T*	GetNetAddr()	{return (T*)m_netAddress;}
	template< typename T >	void SetNetAddr( T *pV)	{memcpy(m_netAddress, pV, 16 );}
};

/// msg Build function
template< typename T >
FI_AutoBufferPointer FI_BuildNorMsg( T &t, int nMsgID , int nSerialNumber, int nVersion )
{
	FI_AutoBufferPointer p =FI_DefaultBuildBuffer(t, sizeof(FI_MSG_HEADER_T) );
	FI_MSG_HEADER_T* pHeader = (FI_MSG_HEADER_T* )p->pointer();
	pHeader->nMsgID = nMsgID;
	pHeader->nSerialNumber = nSerialNumber;
	pHeader->nMsgNorStatus.version=nVersion;
	pHeader->nMsgLength = p->length() - sizeof(FI_MSG_HEADER_T);
	return p;
}


template< typename T >	inline
bool	FI_PickSpecialDataFromMsg( FI_MSG_CACHE_T &msg,	  int nMsgType, T &value)
{
	FI_MSG_HEADER_T *pH = msg.GetMsgHeader();
	if( pH->nMsgNorStatus.bAdd )
	{
		int nMsgLength = msg.GetValidBufferLength();
		unsigned char *pEnd = msg.pBuffer->pointer() + nMsgLength;
		unsigned char *pLie = msg.pBuffer->pointer() ;
		pLie+=sizeof(FI_MSG_HEADER_T);
		FI_MSG_HEADER_T* pHeader = (FI_MSG_HEADER_T *)pLie;
		while( (unsigned char *)pHeader < pEnd )
		{
			if( pHeader->nMsgID == nMsgType )
			{
				char *p = (char *)pHeader;
				p+=sizeof(FI_MSG_HEADER_T);
				char *p2 = p+pHeader->nMsgLength;
				return FI_ConvertFromChar( p, value, p2);
			}
			else
			{
				char *p = (char *)pHeader;
				p+=sizeof(FI_MSG_HEADER_T);
				p+=pHeader->nMsgLength;
				pHeader = (FI_MSG_HEADER_T *)p;
			}			
		}
		return false;
	}
	else
		return false;
}


#pragma warning( default : 4244 )

#endif
