// Obj_ItemBox.h
//
///////////////////////////////////////////////////////

#ifndef _OBJ_ITEM_BOX_H_
#define _OBJ_ITEM_BOX_H_

#include "Obj_Static.h"
#include "ItemContainer.h"

struct ITEM_LOG_PARAM;

struct _OBJ_ITEMBOX_INIT : public _OBJ_STATIC_INIT
{

	UINT		m_uCreateTime;
	UINT		m_uRecycleTime;
	BOOL		m_bRecycle;
	INT			m_ItemBoxType;

	_OBJ_ITEMBOX_INIT( VOID )
	{
		CleanUp();
	}

	virtual VOID CleanUp( VOID )
	{
		_OBJ_STATIC_INIT::CleanUp();
		m_uCreateTime	= 0;
		m_uRecycleTime	= 0;
		m_ItemBoxType	= 0;
		m_bRecycle		= TRUE;
	}
};

class Scene;


class Obj_ItemBox : public Obj_Static
{
private:
	Obj_ItemBox();
	~Obj_ItemBox();
public:

	friend class ItemBoxPool;
public:

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_ITEM_BOX; }

	virtual VOID		CleanUp( ) ;
	virtual BOOL		Init( const _OBJ_INIT *pInit );		//��ʼ��Obj_ItemBox����������

	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

public:
	virtual Packet		*CreateNewObjPacket( VOID );
	virtual VOID		DestroyNewObjPacket( Packet *pPacket );

public:
	VOID				Recycle();
	uint				GetPoolPos(){return m_nPoolPos;};	//��ItemBoxPool �е�λ��


	_ITEM*				GetItem(BYTE	itemIndex);			//���itemIndex λ����Ʒ���ݣ�����Ϊ�Ƿ�
	VOID				EraseItem(BYTE itemIndex);			//ɾ��itemIndex λ������
	VOID				SetItemCount(BYTE	itemCount);		//Obj_ItemBox��ʵ����Ʒ����
	BYTE				GetItemCount();						//��ȡʵ����Ʒ����	
	VOID				SetItemData(_ITEM*	pData);			//������Ʒ����
	const	_ITEM*		GetItemData();						//��ȡ��Ʒ����

	INT					AddItem(ItemBoxContaner* pContain,INT nDestIndex);
	BOOL				CreateItem(ITEM_LOG_PARAM* pLogParam,UINT uItemType,UINT uQuality);
	
	inline	VOID		SetItemBoxID( ObjID_t id );
	inline ObjID_t		GetItemBoxID( );
	
	inline GUID_t		GetOwner();										//���������
	inline VOID			SetOwner(GUID_t id);							//����������
	inline VOID			SetDropMonsterID(ObjID_t nID)
	{
		m_MonsterID = nID;
	}

	ObjID_t				GetDropMonsterID()
	{
		return m_MonsterID;
	}

	inline	INT			GetType()	const;
	inline VOID			SetType(INT	Type);
	inline BOOL			GetOpenFlag();
	inline VOID			SetOpenFlag(BOOL bFlag);
	inline	INT			GetRecycleTimes();
	inline VOID			SetRecycleTimes(INT RecycleTimes);
	VOID				SetMaxGrowTime(UINT	uGrowTime);
	inline UINT			GetMaxGrowTime();
	BOOL				CanPickBox(GUID_t	PickerID,ObjID_t HumanID);
	virtual  BOOL		IsCanViewMe( const Obj *pObj );
	VOID				EnablePickOwnerTime();
	VOID				SetPickOwnerTime(UINT	uPickTime);


	inline ItemContainer*		GetContainer();


	virtual ScriptID_t			GetScriptID()	const;

	
private:
	inline VOID			SetPoolPos(uint	iPos);
private:

	uint				m_nPoolPos;							//��Pool ��λ��
	GUID_t				m_OwnerId;							//������Id
	ObjID_t				m_MonsterID;						//�ӵ�������ID
	TeamID_t			m_OwnerTeam;						//����Team
	ObjID_t				m_ItemBoxId;						//Obj_ItemBox ��ID ��ItemBoxManager ͳһ����
	BYTE				m_ItemCount;						//Obj_ItemBox ��������Ʒ����
	_ITEM				m_ItemList[MAX_BOXITEM_NUMBER];		//Obj_ItemBox ����Ʒ����
	IPC					m_IPCList[MAX_BOXITEM_NUMBER];		//Obj_ItemBox ����Ʒʰȡ��������
	INT					m_ItemBoxType;						//Obj_ItemBox ����
	INT					m_RecycleTimes;						//���մ���
private:
	BOOL				m_OpenFlag;							//Obj_ItemBox�Ƿ��Ѿ���
	INT					m_MaxGrowTime;						//����ڳ�����ͣ��ʱ��														
	CMyTimer			m_LifeTimer;						//״̬������
	INT					m_PickOwnerTime;					//����ʰȡ����ʱ�䳤��
	INT					m_FinalPickTime;					//����ʰȡ���ʱ��
private:

	CMyTimer			m_RecycleTimer ;					//���ռ�ʱ��
	UINT				m_CreateTime;						//����ʱ��
	UINT				m_RecycleTime;						//����ʱ��

private:
	ItemContainer		m_Container;
};

#include	"Obj_ItemBox.inl"

#endif
