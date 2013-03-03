/********************************************************************
创建日期:	2008年1月22日
文件名称:	Obj_ItemManager.h
创建人:		李珣

文件功能： 实现对Obj_Item的管理
每个场景一个管理器
修改纪录：

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
	virtual BOOL		Init( const _OBJ_INIT *pInit );		//初始化Obj_Item的生存周期

	virtual BOOL		HeartBeat( UINT uTime=0 ) ;

public:
	virtual Packet		*CreateNewObjPacket( VOID );
	virtual VOID		DestroyNewObjPacket( Packet *pPacket );

public:
	VOID				Recycle();
	uint				GetPoolPos(){return m_nPoolPos;};	//在Obj_ItemPool 中的位置

	VOID				SaveObj_Item(_ITEM* pItem);			//设置物品数据
	_ITEM*				GetObj_Item();						//获取物品数据

	inline	VOID		SetObj_ItemID( ObjID_t id );
	inline ObjID_t		GetObj_ItemID( );

	//inline GUID_t		GetOwner();										//获得所有者
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

	uint				m_nPoolPos;							//在Pool 中位置
	GUID_t				m_OwnerId[MAX_TEAM_MEMBER];			//最大拥有
	UINT				m_OwnerCount;
	ObjID_t				m_DropObjID;							//从掉落怪物(人物)的ID
	ObjID_t				m_Obj_ItemId;						//Obj_Item 的ID 由Obj_ItemManager 统一管理
	_ITEM				m_Obj_Item;							//Obj_Item 中物品数据
	INT					m_Obj_ItemType;						//Obj_Item 类型
	INT					m_RecycleTimes;						//回收次数
private:
	INT					m_PickOwnerTime;					//主人拾取保护时间长度
	INT					m_FinalPickTime;					//主人拾取最后时间
private:

	CMyTimer			m_RecycleTimer ;					//回收计时器
	UINT				m_CreateTime;						//创建时间
	UINT				m_RecycleTime;						//回收时间
};

#include	"Obj_Item.inl"

#endif