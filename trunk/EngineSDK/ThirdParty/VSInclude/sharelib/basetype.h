#ifndef BASE_TYPE_H__
#define BASE_TYPE_H__

/*!
	Author : dfhu
	Data: 2002-10
	Des : base type define
*/

namespace BASE_TYPE
{
	typedef char			int8;
#define INT8				char;
	typedef unsigned char	uint8;
#define UINT8				unsigned char;
	typedef short			int16;
	typedef unsigned short  uint16;
	typedef int				int32;
	typedef unsigned int	uint32;

	#ifndef WIN32
	typedef long long int	int64;
	typedef unsigned long long int uint64;
	#else
	typedef __int64			int64;
	typedef unsigned __int64		uint64;
	#endif
}

#ifndef DELARRAY
#define DELARRAY(a) {if(a) {delete []a,a=NULL;} }
#endif

#ifndef DEL
#define DEL(a) {if(a) {delete a,a=NULL;}}
#endif

#define foreachIn(a,b)		for(a=0; a<b; ++a)
#define foreachInB(a,b,c)	for(a=b; a<c; ++a)
#define foreachInDown(a,b)  for(a=b; a>0; --a)
#define foreachInDownB(a,b,c) for(a=b; a>c; --a)


inline
void BITSET(int &k, int n )
{
	if( n > 0 )
		k |= (0x00000001<<n);
	else 
		k |= 0x00000001;
}

inline
BOOL BITCHECK( int &k, int n )
{
	if( n > 0 )
		return k& (0x00000001<<n);
	else
		return k& 0x00000001;
}

inline 
void BITCLEAR( int &k, int n)
{
	if( n > 0 )
		k  &= ~(0x00000001<<n);
	else
		k &= ~(0x00000001);
}

#endif