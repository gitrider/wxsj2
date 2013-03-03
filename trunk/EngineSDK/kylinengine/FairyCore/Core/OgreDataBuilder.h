#pragma once

#include <set>
#include "OgreHardwareVertexBuffer.h"
#include "OgreVector3.h"
namespace Ogre
{
	class VertexData;
	class IndexData;
}

#define MAKECOLORRGBA(r,g,b,a)		((((int)r)&0xFF)|((((int)g)&0xFF)<<8)|((((int)b)&0xFF)<<16)|((( (int)a)&0xFF)<<24))

class IDataTypeGet
{
public:
	virtual Ogre::VertexElementType	getSettingElementType( Ogre::VertexElementSemantic seg, int nIndex)	=	0;
};

enum E_VD_MODIFY_TYPE
{
	E_VT_POS = 1,
	E_VT_NORMAL = (1<<1),
	E_VT_BLEND_WEIGHT = (1<<2),
	E_VT_BLEND_INDICES = (1<<3),
	E_VT_DIFFUSE=(1<<4),
	E_VT_SPECULAR=(1<<5),
	E_VT_BINORMAL=(1<<6),
	E_VT_TANGENT=(1<<7),
	E_VT_TEXCOORD_0	=	(1<<16),
	E_VT_TEXCOORD_1	=	(1<<17),
	E_VT_TEXCOORD_2	=	(1<<18),
	E_VT_TEXCOORD_3	=	(1<<19),
	E_VT_TEXCOORD_4	=	(1<<20),
	E_VT_TEXCOORD_5	=	(1<<21),
	E_VT_TEXCOORD_6	=	(1<<22),
	E_VT_TEXCOORD_7	=	(1<<23),
};


struct __VertexBufferID
{
	unsigned short nStreamNumber;
	unsigned short nOffset;

	int				nBufferID;

	bool operator < (const __VertexBufferID &other ) const
	{
		if( nStreamNumber < other.nStreamNumber )
			return true;
		if( nStreamNumber > other.nStreamNumber )
			return false;
		if( nOffset < other.nOffset )
			return true;
		return false;
	}
};

/// not check buffer size
struct __VertexBufferPointer
{
	void*			mp_CurrentPointLie;
	int				m_nStride;

	/// int			m_nSizeLimit;

	unsigned short*	GetShortPointer()	{return (unsigned short *)mp_CurrentPointLie;}
	unsigned int *	GetIntPointer()		{return (unsigned int *)mp_CurrentPointLie;}
	unsigned char * GetCharPointer()		{return (unsigned char * )mp_CurrentPointLie;}
	float*			GetFloatPointer()	{return (float *)mp_CurrentPointLie;}
	void	Put( int a, int b, int c )	{ *GetIntPointer() = a; * (GetIntPointer()+1) = b;* (GetIntPointer()+2) = c;}
	void	PutShort( unsigned short a, unsigned short b, unsigned short c )	{ *GetShortPointer() = a; * (GetShortPointer()+1) = b;* (GetShortPointer()+2) = c;}
	void	Put( unsigned int nNewValue ) {	*GetIntPointer() = nNewValue;}
	void	Put( const float &a, const float &b )	{ *GetFloatPointer() = a; * (GetFloatPointer()+1) = b;}
	void	Put( const float &a, const float &b, const float &c )	
	{ *GetFloatPointer() = a; * (GetFloatPointer()+1) = b;* (GetFloatPointer()+2) = c;}
	void	Put( const float &a, const float &b, const float &c, const float &d )	
	{ *GetFloatPointer() = a; * (GetFloatPointer()+1) = b;* (GetFloatPointer()+2) = c;* (GetFloatPointer()+3) = d;}
	void	Put( const float *p, int nNumber)	{memcpy(mp_CurrentPointLie,p, sizeof(float)*nNumber );}
	void	Advance( int n )	{	mp_CurrentPointLie = ((unsigned char *)mp_CurrentPointLie) + n*m_nStride;}
	void	Advance()			{   mp_CurrentPointLie = ((unsigned char *)mp_CurrentPointLie) + m_nStride; }
	bool	IsValid()			{return mp_CurrentPointLie!=NULL;}
};

class COgreDataBuilder
{
public:
	enum E_VD_TYPE
	{
		E_TYPE_POS = 0,
		E_TYPE_NORMAL,
		E_TYPE_BLEND_WEIGHT,
		E_TYPE_BLEND_INDICES,
		E_TYPE_DIFFUSE,
		E_TYPE_SPECULAR,
		E_TYPE_BINORMAL,
		E_TYPE_TANGENT,
		E_TYPE_TEXCOORD_0	=	(16),
		E_TYPE_TEXCOORD_1	=	(17),
		E_TYPE_TEXCOORD_2	=	(18),
		E_TYPE_TEXCOORD_3	=	(19),
		E_TYPE_TEXCOORD_4	=	(20),
		E_TYPE_TEXCOORD_5	=	(21),
		E_TYPE_TEXCOORD_6	=	(22),
		E_TYPE_TEXCOORD_7	=	(23),

		E_FORCE_32BITS = 0xFFFFFFFF,
	};

public:
	COgreDataBuilder(void);
	~COgreDataBuilder(void);

	bool	StartVertexBuilder( Ogre::VertexData *pV, int nNeedBuildType, int nCount, int nOffset  );
	bool	EndVertexBuilder();

	bool	StartIndexBuilder( Ogre::IndexData *pI, int nCount, int nOffset );
	bool	EndIndexBuilder();

	__VertexBufferPointer*	GetFromID( int nID );
	__VertexBufferPointer*	GetFromType( int nType )	{return &m_bindBufferArray[nType];}

	void	AdvanceVertexLie();
	void	AdvanceIndexLie( int n );

	void Pos3f( const float &x, const float &y, const float &z )	{ GetFromType(E_TYPE_POS)->Put(x,y,z);}
	void Pos3f( const Ogre::Vector3 &x )							{Pos3f(x.x,x.y,x.z);}
	void Normal3f( const float &x, const float &y, const float &z )	{ GetFromType(E_TYPE_NORMAL)->Put(x,y,z);}
	void Normal3f( const Ogre::Vector3 &x )							{ Normal3f(x.x,x.y,x.z); }
	void Color4f( const float &x, const float &y, const float &z, const float &w  )	{ GetFromType(E_TYPE_DIFFUSE)->Put( MAKECOLORRGBA(x*255,y*255,z*255,w*255) );}
	void Color4I( int x, int y, int z, int w  )	{ GetFromType(E_TYPE_DIFFUSE)->Put( MAKECOLORRGBA(x,y,z,w) );}
	void Tex12F( const float &x, const float &y)	{ GetFromType(E_TYPE_TEXCOORD_0)->Put( x,y );}
	void Tex22F( const float &x, const float &y)	{ GetFromType(E_TYPE_TEXCOORD_1)->Put( x,y );}
	void Tex32F( const float &x, const float &y)	{ GetFromType(E_TYPE_TEXCOORD_2)->Put( x,y );}

	void Index3I( int x, int y, int z )				{m_indexBuffer.Put(x,y,z);}
	void Index3S( unsigned short x, unsigned short y, unsigned short z )				{m_indexBuffer.PutShort(x,y,z);}
protected:
	std::set< __VertexBufferID >		m_lockBufferArray;
	typedef std::set< __VertexBufferID >::iterator	VBIIterator;

	Ogre::VertexData*		mp_VD;
	Ogre::IndexData*		mp_ID;

	__VertexBufferPointer 		m_bindBufferArray[32];
	__VertexBufferPointer		m_indexBuffer;



	static E_VD_TYPE	VDTypeFromMType( int nMType );
public:	
///	For Buffer added
	static void AddVertexDataSeg(Ogre::VertexData *pV,  E_VD_TYPE &nType, int nDataType = -1 );
	static void AddVertexDataSeg(Ogre::VertexData *pV,  int nSeg, int nIndex, int nDataType  );
	static void BuildVertexData( Ogre::VertexData *pV, int nVertexCount, int nContainDataType, IDataTypeGet *pDTG=NULL );
};



