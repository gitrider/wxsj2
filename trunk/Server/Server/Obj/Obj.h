// Obj.h
// 
/////////////////////////////////////////////////////////////////////////

#ifndef __OBJ_H__
#define __OBJ_H__

#include "Type.h"
#include "Rand.h"
#include "GameUtil.h"

struct _CAMP_DATA;
struct _OBJ_INIT
{
	WORLD_POS		m_Pos;
	FLOAT			m_Dir;
	_OBJ_INIT( VOID )
	{
		m_Pos.CleanUp();
		m_Dir		= 0.f;
	}

	virtual VOID CleanUp( VOID )
	{
		m_Pos.CleanUp();
		m_Dir		= 0.f;
	}
};

// OBJLIST
//#define MAX_OBJ_LIST_SIZE 255

class Obj;
/*
struct OBJLIST
{
	Obj*		m_aObj[MAX_OBJ_LIST_SIZE];
	BYTE		m_Count;

	OBJLIST( )
	{
		m_Count = 0;
	}
};
*/
typedef struct _OBJLIST
{
	enum
	{
		MAX_OBJ_LIST_SIZE = 255,
	};
	Obj*		m_aObj[MAX_OBJ_LIST_SIZE];
	BYTE		m_Count;
	VOID CleanUp(VOID)
	{
		memset((VOID*)m_aObj, NULL, sizeof(m_aObj));
		m_Count = 0;
	}
	_OBJLIST( )
	{
		CleanUp();
	};
} OBJLIST;
typedef BitFlagSet_T<OBJLIST::MAX_OBJ_LIST_SIZE> HitFlagsForOBJLIST_T;

struct _OBJ_LIST_NODE;
class Scene;
class Packet;
class Obj
{
public:
	enum ObjType
	{
		OBJ_TYPE_INVALID,			// ��Ч
		OBJ_TYPE_HUMAN,				// ���
		OBJ_TYPE_MONSTER,			// ս����NPC�����
		OBJ_TYPE_PET,				// ����
		OBJ_TYPE_ITEM_BOX,			// �����
		OBJ_TYPE_DROP_ITEM,			// ������Ʒ
		OBJ_TYPE_PLATFORM,			// ����̨
		OBJ_TYPE_SPECIAL,			// ���⣨�磬���壩
		OBJ_TYPE_HORSE,				// ��
		OBJ_TYPE_BUS,					//bus
		OBJ_TYPE_NUMBERS
	};

public:
	Obj( );
	virtual ~Obj( );

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_INVALID; }

	virtual VOID		OnEnterScene(VOID);
	virtual VOID		OnLeaveScene(VOID);

	virtual VOID		OnRegisterToZone(VOID){}
	virtual VOID		OnUnregisterFromZone(VOID){}

	virtual VOID		CleanUp( );
	virtual BOOL		Init( const _OBJ_INIT *pInit );

	// �����˵�Obj��ִ�е��߼�
	virtual BOOL		HeartBeat( UINT uTime=0 );

	// δ�������Obj��ִ�е��߼�
	virtual BOOL		HeartBeat_OutZone( UINT uTime=0 );

public:
	ObjID_t				GetID( )const{ return m_ObjID; }	//��ȡ��ɫID
	VOID				SetID( ObjID_t id ){ m_ObjID = id; }	//���ý�ɫID
	virtual UINT		GetUniqueID(VOID) const;	//��ȡΨһID
	
	// ��ObjPool�е�λ�ã�����ObjPool���Ż�
	VOID				SetPoolID( UINT uID ){ m_uPoolID =uID; }
	UINT				GetPoolID( VOID )const{ return m_uPoolID; }

	// ��ObjSingleManager�е�λ�ã�����ObjSingleManager���Ż�
	VOID				SetSingleMgrIndex( UINT uIndex ){ m_uSingleMgrIndex =uIndex; }
	UINT				GetSingleMgrIndex( VOID )const{ return m_uSingleMgrIndex; }

	//���ε���HeartBeat���ʱ���(��λ�����룬����)
	UINT				GetLogicTime( )const{ return (m_uNowTime-m_uLastTime); }
	UINT				LastTime( )const{ return m_uLastTime; }
	UINT				NowTime( )const{ return m_uNowTime; }
	UINT				GetCreateTime( )const{ return m_uCreateTime; }

private:
	VOID				UpdateTime(UINT uTime);

public:
	virtual VOID		OnKillObject( ObjID_t idObj ){}
	// ���ߺ���
	//�ڳ����л��ָ��ID�ĳ�������
	Obj* 				GetSpecificObjInSameSceneByID(ObjID_t nID);
	Obj* 				GetSpecificHumanInSameSceneByGUID(GUID_t nID);
public:
	_OBJ_LIST_NODE*		ObjNode(){ return m_pObjNode; }

	// pObj�Ƿ���Կ��ü���
	virtual BOOL		IsCanViewMe( const Obj *pObj ){ return TRUE; }
	virtual BOOL		IsPrevCanViewMe( const Obj *pObj ){ return IsCanViewMe(pObj); }

	BOOL				IsActiveObj( )const{ return (BOOL)m_bActive; }
	VOID				SetActiveFlag( BOOL bFlag );

	const WORLD_POS*	getWorldPos()const{ return &m_Pos; }				//��ȡ��ɫ��������
	VOID				setWorldPos( const WORLD_POS* pos ){ m_Pos = *pos; }//���ý�ɫ��������
	virtual VOID		Teleport( const WORLD_POS* pos );			//˲��

	VOID				setDir( FLOAT dir ){ m_Dir = dir; }
	FLOAT				getDir( )const{ return m_Dir; }

	VOID				setScene( Scene* pScene ){ m_pScene = pScene; }
	Scene*				getScene( ){ return m_pScene; }

	VOID				setZoneID( ZoneID_t zid ){ m_ZoneID = zid; }
	ZoneID_t			getZoneID( )const{ return m_ZoneID; }

	virtual VOID		SetScriptID( ScriptID_t idScript ){ }
	virtual ScriptID_t	GetScriptID( VOID )const{ return INVALID_ID; }
	//�ж��Ƿ�����Ч�뾶
	BOOL				IsInValidRadius(FLOAT fX1, FLOAT fZ1, FLOAT fX2, FLOAT fZ2, FLOAT fRadius);
	BOOL				IsInValidRadius(const Obj *pOb, FLOAT fRadius);
	BOOL				IsInValidRadius(const WORLD_POS *pPos, FLOAT fRadius);

	virtual const _CAMP_DATA	*GetCampData(VOID){ return NULL; }

public:
	INT					GetLogicCount( VOID )const
	{ 
		return m_nLogicCount; 
	}
	VOID				AddLogicCount( VOID )
	{
		m_nLogicCount++; 
	}
protected:
	VOID				SetLogicCount( INT nLogicCount )
	{ 
		m_nLogicCount = nLogicCount;
	}

public:
	virtual Packet		*CreateNewObjPacket( VOID ) = 0;
	virtual VOID		DestroyNewObjPacket( Packet *pPacket ) = 0;

	virtual Packet		*CreateDeleteObjPacket( VOID );
	virtual VOID		DestroyDeleteObjPacket( Packet *pPacket );


	BOOL				UpdateZone( VOID );		//������������

private:
	ObjID_t					m_ObjID;			//����ID
	UINT					m_uPoolID;			//�����ID
	UINT					m_uSingleMgrIndex;	//
	_OBJ_LIST_NODE*			m_pObjNode;
	BYTE					m_bActive;

	WORLD_POS				m_Pos;				// �ڳ����еľ�ȷλ��
	FLOAT					m_Dir;				// �����еķ���
	Scene*					m_pScene;			// ���ڳ�����ָ��
	ZoneID_t				m_ZoneID;			// ��ǰ���ڳ�����Zoneλ��
	UINT					m_uLastTime;		// �ϴ��߼�������ɺ��ʱ��
	UINT					m_uNowTime;			// ��ǰʱ��
	UINT					m_uCreateTime;		// obj�Ĵ���ʱ��

	INT						m_nLogicCount;		// �߼��ļ���

/////////////////////////////////////////////////////////////////////////////////
//obj �������Խӿ�
public :
	virtual const SceneID_t		__GetSceneID( ) const;//������ obj
	virtual Scene*				__GetScene( );//����ָ��
	virtual VOID				__SetScene( Scene* scene );
	virtual const ZoneID_t		__GetZoneID( ) const;//����� obj
	virtual VOID				__SetZoneID( const ZoneID_t zoneid );
	virtual const ScriptID_t	__GetScriptID( ) const;//�ű��� obj
	virtual VOID				__SetScriptID( const ScriptID_t scriptid );
	virtual const WORLD_POS*	__GetWorldPos( ) const;//λ�� obj
	virtual VOID				__SetWorldPos( const WORLD_POS* worldpos );
	virtual const FLOAT			__GetDir( ) const;//���� obj
	virtual VOID				__SetDir( const FLOAT dir );
	virtual const ObjID_t		__GetID( ) const;//obj�� obj
	virtual VOID				__SetID( const ObjID_t id );
};

// �Ƿ�Ϊ���˶���OBJ
inline BOOL IsDynamicObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_HORSE:
	case Obj::OBJ_TYPE_SPECIAL:
	case Obj::OBJ_TYPE_BUS:
		return TRUE;
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_DROP_ITEM:
	case Obj::OBJ_TYPE_PLATFORM:
	default:
		return FALSE;
	}
}

// �Ƿ�ֹ��OBJ
inline BOOL IsStaticObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_SPECIAL:
	case Obj::OBJ_TYPE_HORSE:
	case Obj::OBJ_TYPE_BUS:
		return FALSE;
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_DROP_ITEM:
	case Obj::OBJ_TYPE_PLATFORM:
	default:
		return TRUE;
	}
}

// �Ƿ�Ϊ��ɫ
inline BOOL IsCharacterObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_HORSE:
	case Obj::OBJ_TYPE_BUS:
		return TRUE;
	case Obj::OBJ_TYPE_SPECIAL:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_DROP_ITEM:
	case Obj::OBJ_TYPE_PLATFORM:
	default:
		return FALSE;
	}
}

// �Ƿ�Ϊ�������
inline BOOL IsSpecialObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_SPECIAL:
		return TRUE;
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_HORSE:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_DROP_ITEM:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_BUS:
	default:
		return FALSE;
	}
}

// �Ƿ�Ϊ���
inline BOOL IsHumanObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
		return TRUE;
	case Obj::OBJ_TYPE_SPECIAL:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_BUS:
	default:
		return FALSE;
	}
}
// �Ƿ�Ϊ����
inline BOOL IsMonsterObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_MONSTER:
		return TRUE;
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_SPECIAL:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_BUS:
	default:
		return FALSE;
	}
}
// �Ƿ�Ϊ����
inline BOOL IsPetObj( Obj::ObjType eType )
{
	switch ( eType )
	{
	case Obj::OBJ_TYPE_PET:
		return TRUE;
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_SPECIAL:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_BUS:
	default:
		return FALSE;
	}
}

#endif	// __OBJ_H__
