#pragma once

#include "fi_autobuffer.h"

struct FI_ImageDataStruct
{
	//!图像宽
	int nWidth;	
	//!图像高	
	int nHeight;	
	//!图像的数据, 按像素顺序存放,每行按4字节对齐
	FI_AutoBufferPointer	dBufferPointer; 
	///图像位数
	int	nBits;			
	//!返回图像高度
	int nImageFormat; ///Image Format;

	float	nImageGamma;	///image gammer

	bool	bMipMap;	///Mip Mag

	int xResolution ()  {return nWidth;} 
	//!返回图像宽度
	int yResolution ()  {return nHeight;} 
	//!返回图像数据指针
	unsigned char *pixelArray() const
	{
		if( dBufferPointer != NULL )
			return dBufferPointer->pBuffer;
		else
			return NULL;
	}

	const unsigned char *constPointer() const
	{
		if( dBufferPointer != NULL )
			return dBufferPointer->pBuffer;
		else
			return NULL;
	}

	FI_ImageDataStruct()
	{
		nWidth = nHeight = 0; nBits = 8; nImageFormat=0; bMipMap=false; nImageGamma=0;
	}
	~FI_ImageDataStruct()
	{
		clear();
	}

	//!清空图像数据
	void clear()
	{
		zero();
	}

	//!返回每行图像数据的指针
	unsigned char *linePtr( int nLie) const 
	{
		if( nLie > -1 && nLie < nHeight && (dBufferPointer!=NULL) )
			return dBufferPointer->pBuffer+lineWidth()*nLie;
		else
			return NULL;
	}

	static int	lineWidth( int pN, int nB)
	{
		return ((pN*(nB/8)+3)>>2)<<2;
	}

	/*!
		为图像存放申请空间,
	 *	图像位数在申请之前必须被设置
		\param nX: 图像宽
		\param nY: 图像高
	*/
	void size( int nX, int nY )
	{
		int nSep = nBits/8;
		if( nSep < 1 )
			nSep = 1;
		if( lineWidth(nX,nBits)*nY != bufferSize() )
		{
			clear();
			nBits = nSep * 8;
			dBufferPointer = new FI_AutoBuffer;
			dBufferPointer->malloc( lineWidth(nX,nBits)*nY);
		}
		memset(dBufferPointer->pBuffer, 0, dBufferPointer->nBufLen);
		nWidth = nX;
		nHeight = nY;
	}

	/*!
		从另一缓冲区拷贝图像数据
		\param unsigned char *p: 要拷贝的数据指针,要求按四字节对齐
		\param nX:图像宽
		\param nY:图像高
		\param nBits:图像位数
	*/
	void copy( const unsigned char *p, int nX, int nY, int nBits=-1 )
	{
		if( nBits > 0 && nBits < 33 )
			bits( nBits );
		size( nX, nY );
		dBufferPointer->copy(p, bufferSize() );
	}

	/*!
		从另一Image结构中拷贝图像数据
		\param ImageDataStruct: 要拷贝的数据源
	*/
	void copy( const FI_ImageDataStruct  &other )
	{
		copy( other.dBufferPointer->pBuffer, other.width(), other.height(), other.bits() );
	}

	unsigned char *pixel(int x, int y )
	{
		return linePtr(y)+x;
	}

	/*
		获取每一个图形像素的字节宽度
	*/
	int getSpan() const 
	{
		int nSep = nBits/8;
		if ( nSep < 1 )
			nSep = 1;
		return nSep;
	}

	//!返回图像数据大小(和以前接口兼容)
	int size() const {return bufferSize();}

	//!返回图像数据大小
	int bufferSize() const 
	{
		return lineWidth()*nHeight;
	}

	//!返回图像每行数据的宽度
	int	lineWidth() const 
	{
		return lineWidth(nWidth,nBits);
	}

	//!返回图像的宽度
	int width() const {return nWidth;}

	//!设置图像的宽度
	void width( int n ) {nWidth = n;}

	//!返回图像的高度
	int height() const {return nHeight;}
	
	//!设置图像的高度
	void height( int n ) {nHeight = n;}

	//!返回图像的位数
	int bits() const 	{return nBits;}

	//!设置图像的位数
	void bits( int nB ) {nBits = nB;}

	//!清空图像数据(和以前接口兼容)
	void zero() { nWidth = nHeight =0; nBits = 8;dBufferPointer=NULL; nImageFormat=0; bMipMap=false; nImageGamma=0;}

	FI_AutoBufferPointer	GetInternelBuffer()
	{
		return dBufferPointer;
	}
};

