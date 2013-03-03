/********************************************************************************
 *	�ļ�����	AreaManager.h
 *	ȫ·����	d:\Prj\Server\Server\Scene\AreaManager.h
 *	�����ˣ�	
 *	ά���ˣ�	����
 *	����ʱ�䣺	
 *
 *	����˵����	
 *	�޸ļ�¼��	2005 �� 11 �� 5 ��	20:09���� EventZone ���ĳ��� Area������������
*********************************************************************************/


#ifndef __AREAMANGER_H__
#define __AREAMANGER_H__

#include "Type.h"

struct _FRECT
{
	FLOAT		m_fLeft;
	FLOAT		m_fTop;
	FLOAT		m_fRight;
	FLOAT		m_fBottom;

	_FRECT( )
	{
		CleanUp( ) ;
	}

	VOID CleanUp( )
	{
		m_fLeft = 0 ;
		m_fTop =0 ;
		m_fRight =0 ;
		m_fBottom=0 ;
	}

	BOOL IsContain( FLOAT x, FLOAT y ) const
	{
		return ((x >= m_fLeft) && (x < m_fRight) && (y >= m_fTop) && (y < m_fBottom) );
	}
};

struct Area
{
	UINT		m_uAreaID;
	UINT		m_uScriptID;
	_FRECT		m_rcArea;

	Area()
	{
		CleanUp( ) ;
	}

	VOID CleanUp( )
	{
		m_uAreaID = INVALID_ID ;
		m_uScriptID = INVALID_ID ;
		m_rcArea.CleanUp( ) ;
	}

	BOOL IsContain( FLOAT x, FLOAT y ) const
	{
		return m_rcArea.IsContain( x, y );
	}
};

#define MAX_AREA_SCENE 128

class Scene;
class AreaManager
{
public :
	AreaManager( VOID );
	~AreaManager( VOID );

	BOOL Init( const CHAR *pszPathName );
	VOID Term( VOID );

	VOID SetScene( Scene* pScene ) { Assert( pScene ); m_pScene = pScene; }

	UINT GetZoneID( FLOAT x, FLOAT z );

protected:
	UINT			m_uAreaCount;
	Area*			m_paArea;
	Scene*			m_pScene;
};

#endif // __AREAMANGER_H__
