#pragma once

#include "GMTripperObj.h"


/**
	���ڵ��ϵ���Ʒ
*/
class tCreatureBoard;
class CTripperObject_ItemDrop :	public CTripperObject
{
public:
	//Tripper ����
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_ITEMBOX; }
	//�ܷ�������
	virtual BOOL					Tripper_CanOperation(VOID) const;
	//�������
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_PICKUP; }
	//���뼤��
	virtual VOID					Tripper_Active(VOID);

public:
	//-----------------------------------------------------
	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );
			VOID				SetModel(STRING Model)	{ m_sModel= Model ;}

	//���õ�����Ĺ���
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
	//UINT			m_idOwner;		//��Ʒ���˵�ObjID,������ӵ�ID
 	ObjID_t			m_idOwner[MAX_TEAM_MEMBER];

	UINT			m_ProtectTime;
	UINT            m_StartTime;
	UINT			m_PassTime;

	BOOL            m_bCanPick;
	BOOL			m_bCanEveryOnePick;
	           
	/// UI�ӿ�
	tCreatureBoard*			m_pInfoBoard;
	/// λ��
	fVector2				m_fvInfoBoardPos;
	// ��ת�Ƕ�
	fVector3				m_fvRotation;
protected:
	KL_DECLARE_DYNAMIC(CTripperObject_ItemDrop);
};
