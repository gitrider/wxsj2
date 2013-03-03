#ifndef SINGLE_MATRIX_H__
#define SINGLE_MATRIX_H__
#include "singletype.h"
#include "typeconvert.h"
#include "singlecacl.h"
#include "utiltemplatepoint.h"

/*!
	Author : dfhu
	Data: 2002-10
	Des: The matrix template
*/


#define WIDTHBYTESBY32BITS(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

/*!基本数学计算函数*/
namespace math_Basic
{
//	using namespace UTIL_Computer;

/*! 按四字节对齐 */
	inline long CaclAlignBytesNumBits( int n, int nBits )
	{
		return WIDTHBYTESBY32BITS( n*nBits );
	}

/*!
	DES:
		The matrix data storage	, store nx*ny Data Type T
		Support ref num
	METHOD:
		call data to set or get data
*/
	template <typename T>
		struct data_Matrix : public FI_UTIL::ut_BaseTemplateRep
	{
		typedef T TYPE;
#define  TYPESIZE sizeof(T) 
		typedef data_Matrix<T>	TYPESelf;
		TYPE *	m_pData;
		int		m_nX;
		int		m_nY;
		data_Matrix()
		{
			m_nX = m_nY = 0; m_pData = NULL;
		}
		data_Matrix( int nX, int nY )
		{
			m_nX = m_nY = 0; m_pData = NULL;
			init( nX, nY );
		}
	

		template<typename T2>
		data_Matrix (const data_Matrix<T2> &other)
		{
			m_nX = m_nY = 0; m_pData = NULL;
			size( other.width(), other.height() );
			copy(other, CRect(0,0,other.width(), other.height()) );
		}

		data_Matrix( const data_Matrix<T> &other)
		{
			m_nX = m_nY = 0; m_pData = NULL;
			init( other );
		}

		~data_Matrix()
		{
			clear();
		}

	

		/*!
			\des Copy data from other
			\param the value matrix
		*/
		template<typename T2>
		TYPESelf & operator = (const data_Matrix<T2> &other)
		{
			size( other.width(), other.height() );
			copy(other, CRect(0,0,other.width(), other.height()) );
			return *this;
		}

		/*!
			Copy data from other
		*/
		TYPESelf & operator = (const data_Matrix<T> &other)
		{
			init(other);
			return *this;
		}
		/*!
			\des Get matrix max,min value,the default operator(>,<) was used

		*/
		void getMinMaxValue( T &mi, T &ma )
		{
			mi = ma = 0;
			int w,h;
			foreachIn( h, height() )
			{
				foreachIn( w, width() )
				{
					mi = min(mi, const_refdata(w,h) );
					ma = max(ma, const_refdata(w,h) );
				}
			}
		}

	/*!
		\des Init a row,col matrix
	*/
		void init( int nX, int nY )
		{
			size( nX, nY ); m_nX = nX; m_nY = nY;
		}

		void swap (data_Matrix<T> &other)
		{
			TYPE *p = other.pointer();
			other.m_pData = p;
			m_pData = p;
			std::swap( m_nX, other.m_nX );
			std::swap( m_nY, other.m_nY );
		}

		void init( const data_Matrix<T> &other)
		{
			size(other.m_nX,other.m_nY); m_nX = other.m_nX; m_nY = other.m_nY;  _valueDArray( other.m_pData );
		}

		void data( int x, int y, const TYPE &value)
		{
			_valueD(*(m_pData+ y*m_nX + x) , value );
		}

		void dataGet( int x, int y, TYPE &t)
		{
			memcpy( &t, m_pData+y*m_nX +x , TYPESIZE );;
		}


		template< typename T>
			void copy( const data_Matrix<T> &other, const CRect &rcCopy )
		{
			int i,j;
			int	nH = min( other.height(), rcCopy.bottom );
			int nW = min( other.width(), rcCopy.right );
			foreachInB( i, rcCopy.top, nH )
			{
				foreachInB( j, rcCopy.left, nW )
				{
					TYPE_CONVERT::baseType_Convert( ref_data(j,i), other.const_refdata(j,i) );
				//	data(i,j, other.const_refdata(i,j) );
				}
			}
		}
				
		template< typename T, typename Convert>
			void copy( const data_Matrix<T> &other, const CRect &rcCopy ,Convert conv )
		{
			int i,j;
			int	nH = min( other.height(), rcCopy.bottom );
			int nW = min( other.width(), rcCopy.right );
			foreachInB( i, rcCopy.top, nH )
			{
				foreachInB( j, rcCopy.left, nW )
				{
					conv( ref_data(j,i), other.const_refdata(j,i) );
				//	data(i,j, other.const_refdata(i,j) );
				}
			}
		}

/*!
	copy a rect block from other to matrix,
	the matrix change block is the same as other;
*/
		void copy( const TYPESelf &other ,const CRect &rcCopy )
		{
			int i,j;
			int	nH = min( other.height(), rcCopy.bottom );
			int nW = min( other.width(), rcCopy.right );
			int nCopyWidth = nW-rcCopy.left;
			foreachInB( i, rcCopy.top, nH )
			{
				_valueN(rcCopy.left,i, other.pointer(rcCopy.left,i),nCopyWidth);
		/*		foreachInB( j, rcCopy.left, nW )
				{
					data(j,i, other.const_refdata(j,i) );
				}*/
			}
		}


		template< typename T>
			void fill( const data_Matrix<T> &other, const CRect &rcFill )
		{
			int i,j;
			int	nH = min( other.height(), rcFill.Height() );
			int nW = min( other.width(), rcFill.Width() );
			foreachIn( i, nH )
			{
				foreachIn( j, nW )
				{
					TYPE_CONVERT::baseType_Convert( ref_data(j,i), other.const_refdata(j+rcFill.left,i+rcFill.top) );
				//	data(i,j, other.const_refdata(i,j) );
				}
			}
		}

		template< typename T, typename Convert>
			void fill( const data_Matrix<T> &other, const CRect &rcFill ,Convert conv )
		{
			int i,j;
			int	nH = min( other.height(), rcFill.Height() );
			int nW = min( other.width(), rcFill.Width() );
			foreachInB( i, rcCopy.top, nH )
			{
				foreachInB( j, rcCopy.left, nW )
				{
					conv( ref_data(j,i), other.const_refdata(j+rcFill.left,i+rcFill.top) );
				}
			}
		}

/*!
	fill self with other matrix rect
	the fill region Topleft is 0,0
	and bottomright is rect width and height
*/
		void fill( const TYPESelf &other ,const CRect &rcFill )
		{
			int i,j;
			int	nH = min( other.height(), rcFill.bottom );
			int nW = min( other.width(), rcFill.right );
			int nCopyWidth = nW-rcFill.left;
			foreachInB( i, rcFill.top, nH )
			{
				_valueN(0,i, other.pointer(rcFill.left,i),nCopyWidth);
		/*		foreachInB( j, rcCopy.left, nW )
				{
					data(j,i, other.const_refdata(j,i) );
				}*/
			}
		}

		void size(int x, int y)
		{
			if ( x*y != m_nX*m_nY )
			{
				clear();
				_allocD( x, y );
			}
			m_nX = x;
			m_nY = y;
		}

		int size() const 
		{
			return m_nX * m_nY;
		}

		TYPE *pointer() const
		{
			return m_pData;
		}

		TYPE *pointer ( int x, int y ) const
		{
			return m_pData + y*width() + x;
		}

		void valueAll( const T *p )
		{
			_valueDArray( p );
		}

		int	width()		const {return m_nX;}
		int height()	const {return m_nY;}

		/*
			DES: Type must support add
		*/
		TYPESelf & operator += (const TYPE &t )
		{
			int i,j;
			foreachIn( i, m_nY)
				foreachIn( j, m_nX )
					ref_data(j,i) += t;
			return *this;
		}

		TYPESelf & operator -= (const TYPE &t )
		{
			int i,j;
			foreachIn( i, m_nY)
				foreachIn( j, m_nX )
					ref_data(j,i) -= t;
			return *this;
		}

		TYPESelf & operator *= (const TYPE &t )
		{
			int i,j;
			foreachIn( i, m_nY)
				foreachIn( j, m_nX )
					ref_data(j,i) *= t;
			return *this;
		}

		template < typename opt, typename optArg >
		TYPESelf & DoOneOperator( opt &op, const optArg &arg )
		{
			int i,j;
			foreachIn( i, m_nY )
				foreachIn( j, m_nX )
					op(ref_data(j,i), arg );
			return *this;
		}

		TYPESelf & operator /= (const TYPE &t )
		{
			int i,j;
			foreachIn( i, m_nY)
				foreachIn( j, m_nX )
					ref_data(j,i) /= t;
			return *this;
		}

		TYPE & ref_data(int x, int y) const
		{
			return *(m_pData+ y*m_nX + x);
		}

		const TYPE & const_refdata( int x, int y) const
		{
			return *(m_pData+ y*m_nX + x);
		}

		void clear()
		{
			if ( m_pData )
				delete []m_pData, m_pData = NULL;
			m_nX = m_nY = 0;
		}

		void flip( int nlinewap = 1)
		{
			int nNewHeight = height()/nlinewap;
			int nNewWidth = width()*nlinewap;
			char *buf = new char[nNewWidth];
			int i;
			foreachIn(i, nNewHeight/2 )
			{
				memcpy(buf, pointer(0,i*nlinewap), nNewWidth );
				memcpy(pointer(0,i*nlinewap), pointer(0,nNewHeight-(i+1)*nlinewap), nNewWidth);
				memcpy(pointer(0,nNewHeight-(i+1)*nlinewap), buf, nNewWidth);
			}
			delete []buf;
		}

		void lrflip( int nSep = 1)
		{
			int newWidth = width()/nSep;
			char *buf = new char[nSep];
			int w,h;
			foreachIn( h, height() )
			{
				foreachIn(w, newWidth/2 )
				{
					memcpy(buf, pointer(w*nSep,h), nSep);
					memcpy(pointer(w*nSep,h), pointer(width()-(w+1)*nSep, h), nSep );
					memcpy(pointer(width()-(w+1)*nSep, h), buf, nSep);
				}
			}
			delete []buf;
		}

	protected:
		void _allocD(int nX, int nY)
		{
			m_pData = new TYPE[nX * nY];
			memset( m_pData, 0, nX*nY*TYPESIZE );
		}

		void _freeD()
		{
			delete []m_pData; m_pData = NULL;
		}

		void _valueDArray( const TYPE *p )
		{
			memcpy( m_pData, p, m_nX*m_nY*TYPESIZE );
		}

		void _valueD ( TYPE &s, const TYPE &o)
		{
			memcpy( &s, &o, TYPESIZE );
		}

		void _valueN ( int x, int y, const TYPE *p, int nNum )
		{
			memcpy(pointer(x,y), p, nNum );
		}

	};

#pragma warning( disable : 4244 )
/*!
	\des strtch matrix, may be use for image, but without decrease noise
*/
	template < class T >
	void stretchMatrix( data_Matrix<T> &in, data_Matrix<T> &out )
	{
		int	nNewWidth = out.width();
		int nNewHeight = out.height();
		int nOldWidth = in.width();
		int nOldHeight = in.height();
		double dW = (double)nOldWidth/nNewWidth;
		double dH = (double)nOldHeight/nNewHeight;
		int nWEdge = nOldWidth-1;
		int nHEdge = nOldHeight-1;
		while( (nNewWidth--) * dW > nWEdge );
		while( (nNewHeight--)* dH > nHEdge );
		int w,h;
//		int ow, oh;
		foreachIn( h, nNewHeight )
		{
			foreachIn( w, nNewWidth )
			{
		//		ow = min(w*dW, nWEdge);
		//		oh = min(h*dH, nHEdge);
				out.data(w,h,*in.pointer(w*dW,h*dH));
			}
		}
	}
#pragma warning( default : 4244 )
}


#endif

