#ifndef __FI_BIT_CONTROL_H__
#define __FI_BIT_CONTROL_H__

template< typename T>
inline
void FI_BITSET_BY_LIE(T &k, int n )
{
	if( n > 0 )
		k |= (0x00000001<<n);
	else 
		k |= 0x00000001;
}

template< typename T>
inline
int FI_BITCHECK_BY_LIE( T &k, int n )
{
	if( n > 0 )
		return k& (0x00000001<<n);
	else
		return k& 0x00000001;
}

template< typename T> inline 
void FI_BITCLEAR_BY_LIE( T &k, int n)
{
	if( n > 0 )
		k  &= ~(0x00000001<<n);
	else
		k &= ~(0x00000001);
}

template< typename T> inline
void	FI_BITSET_BY_VALUE( T &k, int n )
{
	k |= n;
}

template< typename T> inline
int	FI_BITCHECK_BY_VALUE( T &k, int n )
{
	return k&n;
}

template< typename T> inline
void	FI_BITCLEAR_BY_VALUE( T &k, int n )
{
	k &= ~(n);
}

union UnionIntShort2
{
	int		m_nIntValue;
	struct
	{
		unsigned short	m_nLowValue;
		unsigned short	m_nHighValue;
	};
};

#endif
