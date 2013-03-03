// ObjManage.h
//
// ���ã�ObjManager��Ϊ����ObjID_t��Objָ��Ķ�Ӧ��ϵģ��
//		�����Ѿ����ڴ��е�Obj����ʵ����ͨ������ObjManager
//		��AddObj�ӿں󣬽��ḳ��Obj����ʵ��һ��ObjID_t��ֵ
//		�˺�Ϳ��Դ�ObjManager��ͨ��ObjID_t��ѯ��Obj��ʵ��
//		��ָ�롣
//
//////////////////////////////////////////////////////////

#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

#include "Type.h"

enum ENUM_OBJ_ID_TYPE
{
	OBJ_ID_TYPE_INVALID	= -1,
	OBJ_ID_TYPE_NORMAL,
	OBJ_ID_TYPE_HUMAN,
	OBJ_ID_TYPE_NUMBERS
};

struct _OBJMAGAGER_INIT
{
	UINT		m_aInitCount[OBJ_ID_TYPE_NUMBERS];
	_OBJMAGAGER_INIT( VOID )
	{
		memset( m_aInitCount, 0, sizeof( m_aInitCount ) );
	}

	VOID Cleanup( VOID )
	{
		memset( m_aInitCount, 0, sizeof( m_aInitCount ) );
	}
};

class Obj;
class ObjManager
{
private:
	static ObjID_t		m_aidBegin[OBJ_ID_TYPE_NUMBERS];

public:
	ENUM_OBJ_ID_TYPE GetObjIDType( ObjID_t idObj )const
	{
		INT i;
		for ( i = OBJ_ID_TYPE_NUMBERS - 1; i >= 0; i-- )
		{
			if ( idObj >= m_aidBegin[i] )
			{
				return (ENUM_OBJ_ID_TYPE)(i);
			}
		}
		return OBJ_ID_TYPE_INVALID;
	}

public:
	ObjManager( VOID );
	~ObjManager( VOID );

	BOOL Init( const _OBJMAGAGER_INIT *pInit );
	VOID Term( VOID );

	BOOL AddObj( Obj *pObj, ENUM_OBJ_ID_TYPE eIDType = OBJ_ID_TYPE_NORMAL );
	BOOL RemoveObj( ObjID_t idObj );

	Obj *GetObj( ObjID_t idObj )
	{
		if( idObj == INVALID_ID )
			return NULL;

		ENUM_OBJ_ID_TYPE eIDType = GetObjIDType( idObj );
		if ( idObj > m_aidBegin[eIDType] + (ObjID_t)(m_aLength[eIDType]) )
		{
			Assert( idObj > m_aidBegin[eIDType] + (ObjID_t)(m_aLength[eIDType]) && "ObjManager::GetObj not find object" );
			return NULL;
		}
		return m_appObjs[eIDType][idObj - m_aidBegin[eIDType]];
	}

private:
	BOOL Resize( ENUM_OBJ_ID_TYPE eIDType, UINT uNewSize );

private:
	Obj				**m_appObjs[OBJ_ID_TYPE_NUMBERS];
	UINT			m_aCount[OBJ_ID_TYPE_NUMBERS];
	UINT			m_aPosition[OBJ_ID_TYPE_NUMBERS];
	UINT			m_aLength[OBJ_ID_TYPE_NUMBERS];
};

#endif	// __OBJMANAGER_H__
