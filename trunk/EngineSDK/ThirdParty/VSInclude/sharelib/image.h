#pragma once

#include "fi_autobuffer.h"

struct FI_ImageDataStruct
{
	//!ͼ���
	int nWidth;	
	//!ͼ���	
	int nHeight;	
	//!ͼ�������, ������˳����,ÿ�а�4�ֽڶ���
	FI_AutoBufferPointer	dBufferPointer; 
	///ͼ��λ��
	int	nBits;			
	//!����ͼ��߶�
	int nImageFormat; ///Image Format;

	float	nImageGamma;	///image gammer

	bool	bMipMap;	///Mip Mag

	int xResolution ()  {return nWidth;} 
	//!����ͼ����
	int yResolution ()  {return nHeight;} 
	//!����ͼ������ָ��
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

	//!���ͼ������
	void clear()
	{
		zero();
	}

	//!����ÿ��ͼ�����ݵ�ָ��
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
		Ϊͼ��������ռ�,
	 *	ͼ��λ��������֮ǰ���뱻����
		\param nX: ͼ���
		\param nY: ͼ���
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
		����һ����������ͼ������
		\param unsigned char *p: Ҫ����������ָ��,Ҫ�����ֽڶ���
		\param nX:ͼ���
		\param nY:ͼ���
		\param nBits:ͼ��λ��
	*/
	void copy( const unsigned char *p, int nX, int nY, int nBits=-1 )
	{
		if( nBits > 0 && nBits < 33 )
			bits( nBits );
		size( nX, nY );
		dBufferPointer->copy(p, bufferSize() );
	}

	/*!
		����һImage�ṹ�п���ͼ������
		\param ImageDataStruct: Ҫ����������Դ
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
		��ȡÿһ��ͼ�����ص��ֽڿ��
	*/
	int getSpan() const 
	{
		int nSep = nBits/8;
		if ( nSep < 1 )
			nSep = 1;
		return nSep;
	}

	//!����ͼ�����ݴ�С(����ǰ�ӿڼ���)
	int size() const {return bufferSize();}

	//!����ͼ�����ݴ�С
	int bufferSize() const 
	{
		return lineWidth()*nHeight;
	}

	//!����ͼ��ÿ�����ݵĿ��
	int	lineWidth() const 
	{
		return lineWidth(nWidth,nBits);
	}

	//!����ͼ��Ŀ��
	int width() const {return nWidth;}

	//!����ͼ��Ŀ��
	void width( int n ) {nWidth = n;}

	//!����ͼ��ĸ߶�
	int height() const {return nHeight;}
	
	//!����ͼ��ĸ߶�
	void height( int n ) {nHeight = n;}

	//!����ͼ���λ��
	int bits() const 	{return nBits;}

	//!����ͼ���λ��
	void bits( int nB ) {nBits = nB;}

	//!���ͼ������(����ǰ�ӿڼ���)
	void zero() { nWidth = nHeight =0; nBits = 8;dBufferPointer=NULL; nImageFormat=0; bMipMap=false; nImageGamma=0;}

	FI_AutoBufferPointer	GetInternelBuffer()
	{
		return dBufferPointer;
	}
};

