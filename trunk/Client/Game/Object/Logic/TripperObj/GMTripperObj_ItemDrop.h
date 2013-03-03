#pragma once

#include "GMTripperObj.h"


/**
	放在地上的物品
*/
class tCreatureBoard;
class CTripperObject_ItemDrop :	public CTripperObject
{
public:
	//Tripper 类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_ITEMBOX; }
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const;
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_PICKUP; }
	//进入激活
	virtual VOID					Tripper_Active(VOID);

public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );
			VOID				SetModel(STRING Model)	{ m_sModel= Model ;}

	//设置掉落箱的归属
	//VOID						SetOwnerGUID(UINT nID)  { m_idOwner = nID; }
	VOID						SetOwnerGUID(ObjID_t* nID);
	VOID						Tripper_AddEffect(); 
	VOID						Tripper_RealseEffect();

	STRING						m_sModel;
	VOID                        SetProtectTime( UINT time){ m_ProtectTime = time;}
	UINT						GetProtectTime(VOID){ return m_ProtectTime ;}	
	VOID                        SetStartTime( UINT time);
	UINT						GetStartTime(VOID){ return m_StartTime ;}	
	VOID                        SetPassTime( UINT time){ m_PassTime = time;}
	UINT						GetPassTime(VOID){ return m_PassTime ;}	

	BOOL						GetDropCanPick(){ return m_bCanPick;}
	VOID						SetDropCanPick(BOOL bCanPick){ m_bCanPick = bCanPick;}
	VOID						SetDropEveryOneCanPick(BOOL bCanPick){ m_bCanEveryOnePick = bCanPick;}
	BOOL						GetDropEveryOneCanPick( ){ return m_bCanEveryOnePick ;}
	void						Tick_UpdateInfoBoard();
	virtual VOID				Tick(VOID);
	BOOL						m_bUpdataBoard;


public:
	CTripperObject_ItemDrop();
	virtual ~CTripperObject_ItemDrop();

protected:
	//UINT			m_idOwner;		//物品主人的ObjID,或者组队的ID
 	ObjID_t			m_idOwner[MAX_TEAM_MEMBER];

	UINT			m_ProtectTime;
	UINT            m_StartTime;
	UINT			m_PassTime;

	BOOL            m_bCanPick;
	BOOL			m_bCanEveryOnePick;
	           
	/// UI接口
	tCreatureBoard*			m_pInfoBoard;
	/// 位置
	fVector2				m_fvInfoBoardPos;
	// 旋转角度
	fVector3				m_fvRotation;
protected:
	KL_DECLARE_DYNAMIC(CTripperObject_ItemDrop);
};
