#ifndef SINGLE_TYPE_TRANSFER_H__
#define SINGLE_TYPE_TRANSFER_H__

/*!	
	Author : dfhu
	Data: 2002-07
	Des: The type convert template, for a simply msvc not support template trait, some function not support by msvc
*/

#include "stdafx.h"
#ifdef SUPPORT_FFTW_TYPE
#include <fftw/fftwinc.h>
#endif
#include "math.h"

namespace TYPE_CONVERT
{
/*	inline
	fftw_complex * STDC2FFCTrans( std::complex<double> * d )
	{
		return (fftw_complex *) d;
	}

	inline
	std::complex<double> * FFC2STDCTrans( fftw_complex * d )
	{
		return (std::complex<double> *) d;
	}*/

#pragma warning( disable : 4244 )

	template < class T1, class T2 >
		inline
		void baseType_Convert ( T1 &t1,const T2 &t2 )
	{
		t1 = t2;
	}

	#ifdef SUPPORT_FFTW_TYPE
	template<  >
		inline
		void baseType_Convert ( fftw_complex &t1,const fftw_complex &t2 )
	{
	//			if ( sizeof(T1) == sizeof(T2) )
		memcpy(&t1, &t2, sizeof(fftw_complex) );
	}

	template < class T1, fftw_complex >
		inline
		void baseType_Convert( T1 &t1, const fftw_complex &t2 )
	{
		t1 = t2[0];
	}

	template < fftw_complex, class T2 >
		inline
		void baseType_Convert( fftw_complex &t1, const T2 &t2 )
	{
		t1[0] = t2;
		t1[1] = 0;
	}

	template<  >
		inline
		void baseType_Convert ( fftw_complex &t1,const double &t2 )
	{
		t1[0] = t2;
		t1[1] = 0;
	}

	template<  >
		inline
		void baseType_Convert ( double &t1,const fftw_complex &t2 )
	{
		t1 = sqrt((t2[0] * t2[0]) + (t2[1] * t2[1]));;
	}

	template< >
		inline
		void baseType_Convert ( fftw_complex &t1,const BYTE &t2 )
	{
		t1[0] = t2;
		t1[1] = 0;
	}

	template<  >
		inline
		void baseType_Convert ( unsigned char &t1,const fftw_complex &t2 )
	{
		t1 = log(1+t2[0] * t2[0] + (t2[1] * t2[1]));
	}
#endif
	#pragma warning( default : 4244 )
}

#endif
