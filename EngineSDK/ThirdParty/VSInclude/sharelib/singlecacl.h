#ifndef SINGLE_CACL_H___
#define SINGLE_CACL_H___
#include <math.h>
#include <algorithm>

namespace math_Basic
{



#ifndef M_PI
#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401
#endif


	//! x-value of k0 
	inline
	double  k0X(double 	k0AbsA, double k0AngA)  { return k0AbsA * cos(k0AngA); }
	//! y-value of k0 
	inline
	double  k0Y(double k0AbsA, double k0AngA)   { return k0AbsA * sin(k0AngA); }

	inline
	void MatrixAXM(double *p1, double *p2, int nW, int nH )
	{
		double *pD = new double[nW*nH];
		memset(pD, 0 ,nW*nH*sizeof(double));
		for (int x=0; x<nH; x++)
		{
			for (int y=0; y<nW; y++)
			{
				for ( int i=0; i<=x; ++i)
					for( int k=0; k<=y; ++k )
						pD[x*nW+y] += p2[(i-x)*nW+k-y]*p1[i*nW+k];
			}
		}
        memcpy(p1, pD, nW*nH*sizeof(double) );
		delete []pD;
	}

	inline
	int dim2Index( int w, int h, int xWidth) {	return  h*xWidth + w;}

	template<typename T>
	inline 
	T pow2( const T &value ) {return value*value;}

#pragma warning ( disable : 4244 )

#ifdef fftw_complex
	inline
	void fftw_div_double( fftw_complex &ffco , const double &d )
	{
		ffco[0] /= d;
		ffco[1] /= d;
	}

	inline
	void fftw_div_int( fftw_complex &ffco , const int &d )
	{
		ffco[0] /= d;
		ffco[1] /= d;
	}
#endif

	const int   PYRAMID_DEFAULT_NUMBER_OF_LEVELS           = 5;
	const int   PYRAMID_DEFAULT_NUMBER_OF_DIRECTIONS       = 8;
	const float PYRAMID_DEFAULT_MAXIMUM                    = static_cast<float>(M_PI/2.0);
	const float PYRAMID_DEFAULT_FACTOR                     = 1.0f/sqrt(2.0f);
	const float PYRAMID_DEFAULT_SIGMA                      = static_cast<float>(2.0*M_PI);
	const float PYRAMID_DEFAULT_ANGLE                      = 0.0f;
	const float PYRAMID_DEFAULT_THRESHOLD                  = 0.001f;
	const int   PYRAMID_DEFAULT_RESOLUTION                 = 4;

#pragma warning ( default : 4244 )
}

#endif
