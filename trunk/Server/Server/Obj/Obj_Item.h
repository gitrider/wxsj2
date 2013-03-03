/********************************************************************
��������:	2008��1��22��
�ļ�����:	Obj_ItemManager.h
������:		�

�ļ����ܣ� ʵ�ֶ�Obj_Item�Ĺ���
ÿ������һ��������
�޸ļ�¼��

*********************************************************************/

#ifndef _OBJ_ITEM_H_
#define _OBJ_ITEM_H_

#include "Obj_Static.h"
#include "Item.h"
struct ITEM_LOG_PARAM;


struct _OBJ_ITEM_INIT : public _OBJ_STATIC_INIT
{

	UINT		m_uCreateTime;
	UINT		m_uRecycleTime;
	BOOL		m_bRecycle;
	INT			m_ItemType;

	_OBJ_ITEM_INIT( VOID )
	{
		CleanUp();
	}

	virtual VOID CleanUp( VOID )
	{
		_OBJ_STATIC_INIT::CleanUp();
		m_uCreateTime	= 0;
		m_uRecycleTime	= 0;
		m_ItemType	= 0;
		m_bRecycle		= TRUE;
	}
};

class Scene;

class Obj_Item : public Obj_Static
{
private:
	Obj_Item();
	~Obj_Item();
public:

	friend class Obj_ItemPool;
public:

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_DROP_ITEM; }

	virtual VOID		CleanUp( ) ;
	virtual BOOL		Init( const _OBJ_INIT *pInit );		//��ʼ��Obj_Item����������

	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

public:
	virtual Packet		*CreateNewObjPacket( VOID );
	virtual VOID		DestroyNewObjPacket( Packet *pPacket );

public:
	VOID				Recycle();
	uint				GetPoolPos(){return m_nPoolPos;};	//��Obj_ItemPool �е�λ��

	VOID				SaveObj_Item(_ITEM* pItem);			//������Ʒ����
	_ITEM*				GetObj_Item();						//��ȡ��Ʒ����

	inline	VOID		SetObj_ItemID( ObjID_t id );
	inline ObjID_t		GetObj_ItemID( );

	//inline GUID_t		GetOwner();										//���������
	inline GUID_t		GetOwner(INT nPos);
	inline INT			GetOwnerCount(){ return m_OwnerCount;}
	inline VOID			SetOwner(GUID_t id);

	inline VOID			SetDropObjID(ObjID_t nID)
	{
		m_DropObjID = nID;
	}

	ObjID_t				GetDropObjID()
	{
		return m_DropObjID;
	}

	inline	INT			GetType()	const;
	inline VOID			SetType(INT	Type);
	inline	INT			GetRecycleTimes();
	inline VOID			SetRecycleTimes(INT RecycleTimes);

	BOOL				CanPick(GUID_t	PickerID,ObjID_t HumanID);
	virtual  BOOL		IsCanViewMe( const Obj *pObj );
	VOID				EnablePickOwnerTime();
	inline	UINT		GetPickOwnerTime();
	VOID				SetPickOwnerTime(UINT	uPickTime);

	virtual ScriptID_t			GetScriptID()	const;


private:
	inline VOID			SetPoolPos(uint	iPos);
private:

	uint				m_nPoolPos;							//��Pool ��λ��
	GUID_t				m_OwnerId[MAX_TEAM_MEMBER];			//���ӵ��
	UINT				m_OwnerCount;
	ObjID_t				m_DropObjID;							//�ӵ������(����)��ID
	ObjID_t				m_Obj_ItemId;						//Obj_Item ��ID ��Obj_ItemManager ͳһ����
	_ITEM				m_Obj_Item;							//Obj_Item ����Ʒ����
	INT					m_Obj_ItemType;						//Obj_Item ����
	INT					m_RecycleTimes;						//���մ���
private:
	INT					m_PickOwnerTime;					//����ʰȡ����ʱ�䳤��
	INT					m_FinalPickTime;					//����ʰȡ���ʱ��
private:

	CMyTimer			m_RecycleTimer ;					//���ռ�ʱ��
	UINT				m_CreateTime;						//����ʱ��
	UINT				m_RecycleTime;						//����ʱ��
};

#include	"Obj_Item.inl"

#endif