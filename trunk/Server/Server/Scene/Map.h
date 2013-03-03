

#ifndef __MAP_H__
#define __MAP_H__

#include "Type.h"
//#include "MapUnit.h"
//
//
//struct _MAP_HEAD
//{
//	UINT		uMagic;		// 'MAPU'   0X5550414D
//	uint		nXSize;			// X�����С
//	uint		nZSize;			// Z�����С
//};
//
//#define BLOCKSIZE 4
//struct _MU
//{
//	_MAP_UNIT	m_MapUnit ;
//	ObjID_t*	m_pObjs ;
//	WORD		m_wSize ;
//	WORD		m_wCount ;
//
//	_MU( )
//	{
//		m_pObjs = NULL ;
//		m_wSize = 0 ;
//		m_wCount = 0 ;
//	};
//
//	~_MU( )
//	{
//		SAFE_DELETE(m_pObjs) ;
//		m_wSize = 0 ;
//		m_wCount = 0 ;
//	};
//
//	BOOL Resize( )
//	{
//		WORD wSize = m_wSize+BLOCKSIZE ;
//		ObjID_t* pNew = new ObjID_t[wSize] ;
//		if( !pNew ) return FALSE ;
//		_MY_TRY
//		{
//			memset( pNew, 0, sizeof(ObjID_t)*wSize ) ;
//			if( m_pObjs )
//			{
//				memcpy( pNew, m_pObjs, sizeof(ObjID_t)*m_wSize ) ;
//			}
//			m_wSize = wSize ;
//			SAFE_DELETE_ARRAY(m_pObjs) ;
//			m_pObjs = pNew ;
//		}
//		_MY_CATCH
//		{
//			return FALSE ;
//		}
//		return TRUE ;
//	};
//
//	BOOL AddObj( ObjID_t id )
//	{
//		if( id==INVALID_ID )
//		{
//			Assert(FALSE) ;
//			return FALSE ;
//		}
//		if( m_wCount==m_wSize ){
//			if( !Resize() ) return FALSE ;
//		}
//		m_pObjs[m_wCount] = id ;
//		m_wCount ++ ;
//		return TRUE ;
//	};
//
//	VOID DelObj( ObjID_t id )
//	{
//		if( id==INVALID_ID )
//			return ;
//		for( uint i=0; i<m_wCount; i++ )
//		{
//			if( m_pObjs[i]==id )
//			{
//				m_pObjs[i] = m_pObjs[m_wCount-1] ;
//				m_wCount -- ;
//				return ;
//			}
//		}
//	};
//
//	WORD GetCount()
//	{ 
//		return m_wCount ; 
//	} ;
//
//	ObjID_t* GetObj() const { return m_pObjs ; } ;
//
//	ObjID_t GetObj(WORD index)
//	{
//		if( index<0 || index>=m_wCount )
//		{
//			Assert(FALSE) ;
//			return INVALID_ID ;
//		}
//		return m_pObjs[index] ; 
//	} ;
//};

class PathFinder;

class Map
{
public :
	Map( ) ;
	~Map( ) ;

	VOID		CleanUp( ) ;

	BOOL		Load( CHAR* filename ) ;

	uint		CX(){ return m_CX ; } ;
	uint		CZ(){ return m_CZ ; } ;


private :
//
//						(0,m_CZ)	(m_CX,m_CZ)
//         y  z			    -------------
//         | /	     	    |			|
//         |/		        |			|
//         +-------> x		|			|
//        					|			|
//       					|			|
//                          -------------
//						 (0,0)	   (m_CX,0)
//

	uint			m_CX ;//��ͼ�᳤����Ϊ0����Ϊ���
	uint			m_CZ ;//��ͼ�ݳ�����Ϊ0����Ϊ���


	//Ѱ·��
	PathFinder*				m_pPathFinder;


public :
	//ͨ�ýӿ�

	VOID			VerifyPos( WORLD_POS* Pos ){
		Pos->m_fX = Pos->m_fX<0?0:Pos->m_fX ;
		Pos->m_fZ = Pos->m_fZ<0?0:Pos->m_fZ ;
		Pos->m_fX = ((Pos->m_fX>(FLOAT)m_CX)?((FLOAT)m_CX-0.1f):Pos->m_fX) ;
		Pos->m_fZ = ((Pos->m_fZ>(FLOAT)m_CZ)?((FLOAT)m_CZ-0.1f):Pos->m_fZ) ;
	};

	PathFinder*		GetPathFinder(){ return m_pPathFinder;}
	BOOL			IsCanGo(const WORLD_POS& pos);
	BOOL			FindEmptyPos( WORLD_POS* pos ) ;

};






#endif
