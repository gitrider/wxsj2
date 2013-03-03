
#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

 #include "Type.h"
#include "GameDefine.h"
#include "GameStruct.h"

#define __PI			3.1415f
#define __HALF_PI		__PI / 2
#define __QUARTER_PI	__PI / 4

///////////////////////////////////////////////////////////////////////
//基本功能性模块定义
///////////////////////////////////////////////////////////////////////

//共享锁
#if defined(__WINDOWS__)
class MyLock
{
	CRITICAL_SECTION m_Lock ;
public :
	MyLock( ){ InitializeCriticalSection(&m_Lock); } ;
	~MyLock( ){ DeleteCriticalSection(&m_Lock); } ;
	VOID	Lock( ){ EnterCriticalSection(&m_Lock); } ;
	VOID	Unlock( ){ LeaveCriticalSection(&m_Lock); } ;
};
#elif defined(__LINUX__)
class MyLock
{
	pthread_mutex_t 	m_Mutex; 
public :
	MyLock( ){ pthread_mutex_init( &m_Mutex , NULL );} ;
	~MyLock( ){ pthread_mutex_destroy( &m_Mutex) ; } ;
	VOID	Lock( ){ pthread_mutex_lock(&m_Mutex); } ;
	VOID	Unlock( ){ pthread_mutex_unlock(&m_Mutex); } ;
};
#endif


class CMyTimer  
{
private:
	UINT m_uTickTerm;
	UINT m_uTickOld;

public:
	BOOL m_bOper;

public:	
	CMyTimer()
	{
		CleanUp() ;
	}

	BOOL IsSetTimer( ) const{ return m_bOper ; }
	
	VOID SetTermTime( UINT uTerm ){ m_uTickTerm =uTerm; }
	UINT GetTermTime( ){ return m_uTickTerm ; }

	UINT GetTickOldTime( ){ return m_uTickOld; }
	
	VOID CleanUp( )
	{ 
		m_uTickTerm = 0 ;
		m_bOper = FALSE ;
		m_uTickOld = 0 ;
	}

	VOID BeginTimer(UINT uTerm, UINT uNow)
	{
		m_bOper = TRUE;
		m_uTickTerm =uTerm;	
		m_uTickOld =uNow;
	}

	BOOL CountingTimer(UINT uNow)
	{
		if(!m_bOper)
			return FALSE;

		UINT uNew =uNow;

		if(uNew<m_uTickOld+m_uTickTerm )
			return FALSE;

		m_uTickOld =uNew;

		return TRUE;
	}
};

///////////////////////////////////////////////////////////////////////
//基本功能性函数定义
///////////////////////////////////////////////////////////////////////

//当前线程挂起一定时间
extern VOID			MySleep( UINT millionseconds=0 ) ;

extern TID			MyGetCurrentThreadID( ) ;

extern FLOAT		MySqrt( const WORLD_POS* pCur, const WORLD_POS* pTar ) ;

extern FLOAT		MyLengthSq( const WORLD_POS* pCur, const WORLD_POS* pTar ) ;

extern FLOAT		MyAngle( const WORLD_POS* pCur, const WORLD_POS* pTar ) ;

extern BOOL			MyPosEqual( const WORLD_POS* pA, const WORLD_POS* pB ) ;

extern VOID			MyRandPos( WORLD_POS* pPos, const WORLD_POS* pDir, FLOAT fRadioMax, FLOAT fRadioMin ) ;

extern CHAR*		MySocketError( ) ;

extern UINT			MyCRC( const CHAR* szString ) ;

//////////////////////////////////////////////////////////////////////////
//数学帮助函数
//////////////////////////////////////////////////////////////////////////
template<class T>

INT Float2Int(T TValue)
{
	INT iValue = (INT)TValue;

	if(TValue-iValue<0.500000f)
	{
		return iValue;
	}
	else
		return iValue+1;

}

struct BigInt 
{
	INT		m_uLowInt ;
	UINT	m_uHighInt ;

	BigInt()
	{
		CleanUp( ) ;
	}

	VOID CleanUp( )
	{
		m_uLowInt	=	0;
		m_uHighInt =   0;
	}
};

struct Flag64 
{
	UINT	m_uLowFlags ;
	UINT	m_uHighFlags ;

	Flag64()
	{
		CleanUp( ) ;
	}

	BOOL isSetBit( INT bit ) const
	{
		if(bit<32)
		{
			if ( m_uLowFlags & (1<<bit) )
				return TRUE;
		}
		else
		{
			if ( m_uHighFlags & (1<<(bit-32)) )
				return TRUE;
		}

		return FALSE;
	}

	VOID UpdateBits( INT bit, BOOL& bUpdate )
	{
		if(bit<32)
		{
			if ( bUpdate )
				m_uLowFlags |= (1<<(INT)bit);
			else
				m_uLowFlags &= (~(1<<(INT)bit));
		}
		else
		{
			if ( bUpdate )
				m_uHighFlags |= (1<<(INT)(bit-32));
			else
				m_uHighFlags &= (~(1<<(INT)(bit-32)));
		}
	}

	VOID CleanUp( )
	{
		m_uLowFlags	=	0;
		m_uHighFlags =  0;
	}
};

struct Flag128 
{
	UINT	m_uLLowFlags ;
	UINT	m_uLHighFlags ;
	UINT	m_uHLowFlags ;
	UINT	m_uHHighFlags ;

	Flag128()
	{
		CleanUp( ) ;
	}

	BOOL isSetBit( INT bit ) const
	{
		if(bit<32)
		{
			if ( m_uLLowFlags & (1<<bit) )
				return TRUE;
		}
		else if(bit<64)
		{
			if ( m_uLHighFlags & (1<<(bit-32)) )
				return TRUE;
		}
		else if(bit<96)
		{
			if ( m_uHHighFlags & (1<<(bit-64)) )
				return TRUE;
		}
		else if(bit<128)
		{
			if ( m_uHHighFlags & (1<<(bit-96)) )
				return TRUE;
		}
		return FALSE;
	}

	VOID UpdateBits( INT bit, BOOL& bUpdate )
	{
		if(bit<32)
		{
			if ( bUpdate )
				m_uLLowFlags |= (1<<(INT)bit);
			else
				m_uLLowFlags &= (~(1<<(INT)bit));
		}
		else if(bit<64)
		{
			if ( bUpdate )
				m_uLHighFlags |= (1<<(INT)(bit-32));
			else
				m_uLHighFlags &= (~(1<<(INT)(bit-32)));
		}
		else if(bit<96)
		{
			if ( bUpdate )
				m_uHHighFlags |= (1<<(INT)(bit-64));
			else
				m_uHHighFlags &= (~(1<<(INT)(bit-64)));
		}
		else if(bit<128)
		{
			if ( bUpdate )
				m_uHHighFlags |= (1<<(INT)(bit-96));
			else
				m_uHHighFlags &= (~(1<<(INT)(bit-96)));
		}
	}

	VOID CleanUp( )
	{
		m_uLLowFlags	=	0;
		m_uLHighFlags =  0;
		m_uHLowFlags	=	0;
		m_uHHighFlags =  0;
	}
};

#define		SM_FREE					0x00	//共享内存空闲
#define		SM_C_READ				0x01	//共享内存自己读取
#define		SM_C_WRITE				0x02	//共享内存自己写
#define		SM_S_READ				0x03	//Server读
#define		SM_S_WRITE				0x04	//Server写
#define		SM_W_READ				0x05	//World 读
#define		SM_W_WRITE				0x06	//World 写


//VOID		sm_lock(INT&	flag,CHAR type);
//BOOL		sm_trylock(INT& flag,CHAR type);
//VOID		sm_unlock(INT& flag,CHAR type);

VOID		sm_lock(CHAR&	flag,CHAR type);
VOID		sm_unlock(CHAR& flag,CHAR type);
BOOL		sm_trylock(CHAR& flag,CHAR type);





#define		SM_LOCK(flag,type)				sm_lock(flag,type);
#define		SM_UNLOCK(flag,type)			sm_unlock(flag,type);
													





struct Flag16
{
	WORD m_wFlags;

	Flag16()
	{
		CleanUp( ) ;
	}

	BOOL isSetBit( INT bit ) const
	{
		if( m_wFlags & (1<<bit) )
			return TRUE;

		return FALSE;
	}

	VOID UpdateBits( INT bit, BOOL& bUpdate )
	{
		if ( bUpdate )
			m_wFlags |= (1<<bit);
		else
			m_wFlags &= (~(1<<bit));
	}

	VOID CleanUp( )
	{
		m_wFlags = 0 ;
	};
};


//可以定义长度的位标记集合
template <UINT nSize>
	class BitFlagSet_T
{
	public:
		enum
		{
			BIT_SIZE = nSize, //位标记的长度，单位是二进制位
			BYTE_SIZE = 1+BIT_SIZE/8, //信息区占用的字节数
		};
		BitFlagSet_T(VOID)
		{
			memset((VOID*)m_aBitFlags, '\0', sizeof(m_aBitFlags));
		};
		//复制构造器
		BitFlagSet_T(BitFlagSet_T const& rhs)
		{
			memcpy((VOID*)m_aBitFlags, (VOID*)(rhs.GetFlags()), sizeof(m_aBitFlags));
		};
		~BitFlagSet_T() {};
		//复制操作符
		BitFlagSet_T& operator=(BitFlagSet_T const& rhs)
		{
			memcpy((VOID*)m_aBitFlags, (VOID*)(rhs.GetFlags()), sizeof(m_aBitFlags));
			return *this;
		};
		//设置所有标记位
		VOID MarkAllFlags(VOID)
		{
			memset((VOID*)m_aBitFlags, 0xFF, sizeof(m_aBitFlags));
		};
		//清除所有标记位
		VOID ClearAllFlags(VOID)
		{
			memset((VOID*)m_aBitFlags, 0x00, sizeof(m_aBitFlags));
		};
		//取指定的标记位
		BOOL GetFlagByIndex(INT const nIdx) const
		{
			if(0>nIdx||BIT_SIZE<=nIdx)
			{
				AssertEx(FALSE,"[BitFlagSet_T::GetFlagByIndex]: Index out of range!");
				return FALSE;
			}
			unsigned int nIndex = nIdx;
			return 0!=(m_aBitFlags[nIdx>>3]&(char)(1<<nIdx%8));
		}
		//清除指定的标记位
		void ClearFlagByIndex(INT const nIdx)
		{
			if(0>nIdx||BIT_SIZE<=nIdx)
			{
				AssertEx(FALSE,"[BitFlagSet_T::ClearFlagByIndex]: Index out of range!");
				return;
			}
			m_aBitFlags[nIdx>>3] &= ~(0x01<<(nIdx%8));
		}
		//设定指定的标记位
		VOID MarkFlagByIndex(INT const nIdx)
		{
			if(0>nIdx||BIT_SIZE<=nIdx)
			{
				AssertEx(FALSE,"[BitFlagSet_T::MarkFlagByIndex]: Index out of range!");
				return;
			}
			m_aBitFlags[nIdx>>3] |=	0x01<<(nIdx%8);
		}
		//所占用的字节数
		UINT GetByteSize(VOID) const {return BYTE_SIZE;}
		//所支持的标记数
		UINT GetBitSize(VOID) const {return BIT_SIZE;}
		//取数据区的指针
		CHAR const* const GetFlags(VOID) const {return m_aBitFlags;}
	protected:
	private:
		CHAR m_aBitFlags[BYTE_SIZE]; //数据存储区
};
//自动控制内存占用的vector
template <typename U, size_t nSizeLimit>
	class ResizableVector_T
{
	public:
		enum
		{
			SIZE_LIMIT = nSizeLimit, //信息区最大单元个数，主要功能是限制数组无限增长
		};
		ResizableVector_T(VOID) : m_nCurrentSize(0), m_pUnits(NULL)
		{
		};
		~ResizableVector_T()
		{
			SAFE_DELETE_ARRAY(m_pUnits);
		};
		//Init Function
		BOOL Init(size_t nSize)
		{
			if(NULL!=m_pUnits)
			{
				AssertEx(FALSE,"[ResizableVector_T::Init]: Init a Used vector!!Check it now.");
				return FALSE;
			}
			if(SIZE_LIMIT<nSize)
			{
				AssertEx(FALSE,"[ResizableVector_T::Init]: Size over flow!! check it now.");
				return FALSE;
			}
			m_pUnits = new U[nSize];
			if(NULL==m_pUnits)
			{
				AssertEx(FALSE,"[ResizableVector_T::Init]: Allocate memory Failed!!Check it now.");
				return FALSE;
			}
			m_nCurrentSize = nSize;
			return TRUE;
		}
		BOOL Resize(size_t nSize)
		{
			if(SIZE_LIMIT<nSize)
			{
				AssertEx(FALSE,"[ResizableVector_T::Resize]: Size over flow!! check it now.");
				return FALSE;
			}
			U* pNewBlock = new U[nSize];
			if(NULL==pNewBlock)
			{
				AssertEx(FALSE,"[ResizableVector_T::Resize]: new memory Failed!!Check it now.");
				return FALSE;
			}
			//calculate the memcpy size
			size_t nCount = 0;
			if(m_nCurrentSize > nSize)
			{
				nCount = nSize;
			}
			else
			{
				nCount = m_nCurrentSize;
			}
			//copy data to new block
			memcpy((VOID*)pNewBlock, (VOID const*)m_pUnits, sizeof(U)*nCount);
			//delete old block
			SAFE_DELETE_ARRAY(m_pUnits);
			//refer to new block
			m_pUnits = pNewBlock;
			//Update new size
			m_nCurrentSize = nSize;
			return TRUE;
		}
		U const* GetUnitByIndex(size_t nIdx) const
		{
			if(0<=nIdx && m_nCurrentSize>nIdx)
			{
				return m_pUnits[nIdx];
			}
			return NULL;
		}
		BOOL SetUnitByIndex(size_t nIdx, U const& rU)
		{
			if(0<=nIdx && m_nCurrentSize>nIdx)
			{
				m_pUnits[nIdx] = rU;
				return TRUE;
			}
			return FALSE;
		}
		//当前的数组长度
		size_t GetCurrentVectorSize(VOID) const {return m_nCurrentSize;}
		//当前占用的内存字节数
		UINT GetByteSize(VOID) const {return sizeof(U)* m_nCurrentSize;}
	protected:
	private:
		size_t m_nCurrentSize; //当前的数组尺寸
		U* m_pUnits; //数据存储区指针
};

	//自动加锁解锁器
	class AutoLock_T
	{
	public:
		AutoLock_T(MyLock& rLock)
		{
			m_pLock = &rLock;
			m_pLock->Lock();
		}
		~AutoLock_T()
		{
			m_pLock->Unlock();
		}
	protected:
	private:
		AutoLock_T();
		MyLock* m_pLock;
	};

#if defined(__WINDOWS__)
#define		tvsnprintf		_vsnprintf
#define		tstricmp		_stricmp
#define		tsnprintf		_snprintf
#elif defined(__LINUX__)
#define		tvsnprintf		vsnprintf
#define		tstricmp		strcasecmp
#define		tsnprintf		snprintf
#endif


CHAR		Value2Ascii(CHAR in);
CHAR		Ascii2Value(CHAR in);

BOOL		Binary2String(const CHAR* pIn,UINT InLength,CHAR* pOut);
BOOL		DBStr2Binary(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength);
BOOL		StrSafeCheck(const CHAR* pIn,UINT InLength);

BOOL		Bin2Str(const CHAR* pIn,UINT InLength,CHAR* pOut);
BOOL		Str2Bin(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength);

BOOL		StrSafeCheck(const CHAR* pIn,UINT InLength);

UINT		GetMailSize( const MAIL& mail ) ;
UINT		GetMailListSize( const MAIL_LIST& maillist ) ;

#if defined (_FOX_CLIENT) && defined (_FOX_LOGIN)  && defined (_FOX_WORLD)
#define DUMP_LOG	"./Log/login_dump.log"
#endif

#if defined (_FOX_CLIENT) && defined (_FOX_SERVER) && defined (_FOX_WORLD)
#define DUMP_LOG	"./Log/server_dump.log"
#endif

#if defined (_FOX_SERVER) && defined (_FOX_WORLD) && defined(_FOX_LOGIN)
#define DUMP_LOG	"./Log/world_dump.log"
#endif

#if (!defined(_FOX_SERVER)) && (!defined (_FOX_WORLD)) && (!defined(_FOX_LOGIN))
#define DUMP_LOG	"./Log/shm_dump.log"
#endif

#if defined(_FOX_BILLING) && defined(_FOX_LOGIN) &&(!defined(_FOX_WORLD))
#define DUMP_LOG	"./Log/billing_dump.log"
#endif

#if defined (_FOX_SERVER) && defined(_FOX_DBFIX)
#define DUMP_LOG	"./Log/mdbfix_dump.log"
#endif

VOID  DumpStack(const CHAR* type);

/*
 *	tsnprintf_s: 更加安全的字符串copy方法
 *  buffer: 目标buffer地址
 *  count:  要copy的个数，范围：0~size-1，size为buffer的大小
 *	format: 格式化字符串
 */
VOID		tsnprintf_s(char *buffer, size_t count, const char *format, ...);


extern		INT				g_CmdArgv;

extern		WORLD_TIME		g_WorldTime;

struct FULLUSERDATA;
UINT	FullDataCrc(FULLUSERDATA* pData, BOOL bLog = FALSE);

BOOL	AccNameCheck(const CHAR* pIn,UINT InLength,INT nLanguage);


class ExceptionHandler
{
public:
	ExceptionHandler();
	static VOID Handler(INT);
};
extern ExceptionHandler		g_ExceptionHandler;

extern		INT				g_DBConnectThread;

#endif
