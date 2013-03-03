// ObjSingleManager.h
// 
// ���ܣ���һ���͵�Obj����Ĺ���
//////////////////////////////////////////////////////

#ifndef __OBJSINGLEMANAGER_H__
#define __OBJSINGLEMANAGER_H__

#include "Type.h"
#include "GameUtil.h"

class Scene;
class Obj;

struct _OBJSINGLEMANAGER_INIT
{
	UINT	m_uLength;
	Scene	*m_pScene;
	UINT	m_uLogicInterval;		// ����״̬��ÿ�����߼�֮֡���ʱ����
	UINT	m_uCurrentTime;		// ��ǰʱ�䣬��ʼObjSingleManager::m_LogicTimer�Ĳ���
	_OBJSINGLEMANAGER_INIT( VOID )
	{
		m_uLength			= 0;
		m_pScene			= NULL;
		m_uLogicInterval	= 0;
		m_uCurrentTime		= 0;
	}

	VOID Cleanup( VOID )
	{
		m_uLength			= 0;
		m_pScene			= NULL;
		m_uLogicInterval	= 0;
		m_uCurrentTime		= 0;
	}
};

class ObjSingleManager
{
public:
	ObjSingleManager( VOID );
	virtual ~ObjSingleManager( VOID );

	virtual BOOL Init( const _OBJSINGLEMANAGER_INIT *pInit );
	virtual VOID Term( VOID );
	VOID Reset( ) ;

	VOID SetLoadFactor( FLOAT fFactor );
	virtual BOOL HeartBeat( UINT uTime = 0 );

	BOOL Add( Obj *pObj );
	BOOL Remove( Obj *pObj );

	UINT GetCount( VOID )const
	{
		return m_uCount;
	}

	UINT GetLength( VOID )const
	{
		return m_uLength;
	}

	VOID SetScene( Scene* pScene )
	{
		m_pScene = pScene;
	}

	Scene *GetScene( VOID )
	{
		return m_pScene;
	}

	Obj *GetObjByIndex( UINT uIndex )
	{
		if (uIndex < m_uCount )
		{
			return m_papObjs[uIndex];
		}
		else
		{
			return NULL;
		}
	}

private:
	BOOL Resize( UINT uSize );

protected:
	Obj				**m_papObjs;
	UINT			m_uCount;
	UINT			m_uLength;
	UINT			m_uLogicInterval;			// ����״̬��ÿ�����߼�֮֡���ʱ����
	UINT			m_uCurrentLogicInterval;	// ��ǰ��ÿ�����߼�֮֡���ʱ����

	Scene			*m_pScene;
	CMyTimer		m_LogicTimer;
};

#endif	// __OBJSINGLEMANAGER_H__
