#pragma once

#include <map>

#pragma warning( disable : 4267 4018 )

template< typename T1, typename T2 >
	struct WRAP_2_TYPE
{
	T1 v1;
	T2 v2;

	typedef WRAP_2_TYPE<T1,T2>	__self;

	bool operator == ( const __self &e2 )
	{
		if( v1 == e2.v1 && v2 == e2.v2 )
			return true;
		else
			return false;
	}

	bool operator < ( const __self &e2 ) const
	{
		if( v1 < e2.v1 )
			return true;
		else if( v1 == e2.v1 )
		{
			if( v2 < e2.v2 )
				return true;
		}
		return false;
	}

	WRAP_2_TYPE()
	{
	}

	WRAP_2_TYPE( const T1 &r1, const T2 &r2 )
	{
		v1=r1; v2=r2;
	}
};

template< typename T1, typename T2, typename Comp1, typename Comp2 >
	struct WRAP_2_TYPE_WITH_COMP
{
	T1 v1;
	T2 v2;
	Comp1 comp1;
	Comp2 comp2;

	typedef WRAP_2_TYPE_WITH_COMP<T1,T2,Comp1,Comp2>	__self;

	bool operator == ( const __self &e2 )
	{
		if( v1 == e2.v1 && v2 == e2.v2 )
			return true;
		else
			return false;
	}

	bool operator < ( const __self &e2 ) const
	{
		if( comp1(v1,e2.v1) )
			return true;
		else if( v1 == e2.v1 )
		{
			if( comp2(v2,e2.v2) )
				return true;
		}
		return false;
	}

	WRAP_2_TYPE_WITH_COMP()
	{
	}

	WRAP_2_TYPE_WITH_COMP( const T1 &r1, const T2 &r2 , Comp1 c1, Comp2 c2)
	{
		v1=r1; v2=r2; comp1=c1; comp2=c2;
	}
};

template< typename T1, typename T2 >
	struct DOUBLE_INDEX
{
public:
	std::map< T1, T2 >		index1;
	std::map< T2, T1 >		index2;

//	typedef	std::map< T1, T2 >::iterator iterator1;
//	typedef std::map< T2, T1 >::iterator iterator2;


public:
	void	Add( const T1 &i1, const T2 &i2 )
	{
		index1[i1] = i2;
		index2[i2] = i1;
	}

	bool Get( const T1 &t1, T2&	t2 ) const
	{
		std::map< T1, T2 >::const_iterator i = index1.find(t1);
		if( i != index1.end() )
		{
			t2 = i->sceond; return true;
		}
		else
			return false;
	}

	bool Get( const T2 &t2, T1 &t1) const
	{
		std::map< T2, T1 >::const_iterator i = index2.find(t2);
		if(  i != index2.end() )
		{
			t1 = i->second; return true;
		}
		else
			return false;
	}

	void	Delete( const T1 &t )
	{
		std::map< T1, T2 >::iterator i = index1.find(t);
		if( i != index1.end() )
		{
			index2.erase( i->second );
			index1.erase( i );
		}
	}

	void	Delete( const T2 &t2 )
	{
		std::map< T2, T1 >::iterator i = index2.find(t2);
		if( i != index2.end() )
		{
			index1.erase( i->second);
			index2.erase( i );
		}
	}

	void	Delete( const T1 &t1, const T2 &t2 )
	{
		index2.erase(t2);
		index1.erase(t1);
	}
};


template < typename T1, typename T2, typename __Comp >
inline void	SortT1ByT2( std::vector< T1 > &t1INArray,  std::vector< T1 > &t1OutArray, std::vector<T2> &t2Array, __Comp comp )
{
	typedef typename WRAP_2_TYPE_WITH_COMP<typename T2, int, typename __Comp, std::less<int> > __IS;
	int nSize1 = t1INArray.size();
	int nSize2 = t2Array.size();
	if( nSize1 != nSize2 )
		return;
	std::vector< __IS >	array;
	array.reserve( nSize1 );
	for( int i=0; i<nSize1; ++i )
	{
		__IS tmp(t2Array[i],i,comp,std::less<int>() );		
		array.push_back( tmp );
	}

	std::sort( array.begin(), array.end() );
	t1OutArray.resize( nSize1 );
	for ( int i=0; i<nSize1; ++i )
		t1OutArray[i] = t1INArray[array[i].v2];
}

#pragma warning( default : 4267 4018 )
