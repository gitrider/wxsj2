
#pragma once


#include "Obj_PlayerNPC.h"



class CObject_Item_Equip;

/** ������ҽ�ɫ��
 */
class CObject_PlayerOther : public CObject_PlayerNPC
{
public:

	CObject_PlayerOther();
	virtual ~CObject_PlayerOther();

	/** ���ݳ�ʼ�����壬��ͬ������Ⱦ�� */
	virtual	VOID			Initial(VOID*);


	/** ���ؽ�ɫ���� */
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const					{ return CT_PLAYEROTHER; }
	/** �����������ϸ��Ϣ������ */
	virtual VOID			SendEuipRequest(VOID);
	/** ָ�����, �����ƽ�ɫ��Ψһ�ӿ� */
	virtual RC_RESULT		OnCommand(const SCommand_Object *pCmd );

	/** ���ָ��ķ��� */
	virtual BOOL			FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** �Ҽ�ָ��ķ��� */
	virtual BOOL			FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


	//================
	// װ����Ʒ
	//================

	// �滻���岿��
	VOID					EquipItem_BodyPart(HUMAN_EQUIP nPart, INT nID );
	// �������װ������Ч
	VOID					DelEquipEffect( HUMAN_EQUIP nPart );
	// ����װ����Ч
	VOID					SetBodyEquipEffect( INT nEquipId, HUMAN_EQUIP nPart );
	VOID					SetBodyEquipEffect( HUMAN_EQUIP nPart, LPCTSTR szBindPoint, LPCTSTR szEffect );

	//�ͷ�ĳ������
	VOID					SetUseSkillID( SkillID_t id );

	/** �ı䶯��
	*/
	BOOL					ChangeAction( INT nSetID, BOOL bLoop, ENUM_ACTION_TYPE ePart=ACTION_BODY, FLOAT fFuseParam=-0.03 );

	// ���������Ĭ�ϵ���Ч
	VOID					AddEquipEffect( INT nEquipId, HUMAN_EQUIP eEquipType );

	//	������
	virtual VOID			EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc );

	//  ж����Ʒ(��ʾȱʡ����) 
	virtual VOID			UnEquipItem( HUMAN_EQUIP nPart );

	virtual INT				GetBusObjID(VOID)const;

	


protected:

	//================
	// �߼����ݷ����ı�
	//================

	friend class CCharacterData;

	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_EquipVer(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
//	virtual VOID			OnDataChanged_Titles(INT index);
	/** ͷ��ı��¼����� */
	virtual VOID			OnDataChanged_FaceImage(VOID);
	virtual VOID			OnDataChanged_Profession(VOID);

protected:

	VOID					UpdateFaceMesh(VOID);
	VOID					UpdateHairMesh(VOID);
	VOID					UpdateHairColor(VOID);
	VOID					UpdateEquip(HUMAN_EQUIP point);

protected:

	virtual VOID			OnHideWeapon( INT nAppointedWeaponID, ENUM_WEAPON_LOCATOR_TYPE loc );
	virtual VOID			OnShowWeapon(VOID);

protected:

	/** RenderInterface����/ɾ�� */
	virtual VOID			CreateRenderInterface(VOID);

	virtual INT				CalcCharModelID( VOID ) const;

	virtual UINT			GetIdleInterval(VOID) const;

	virtual VOID			UpdateCharRace(VOID);


protected:

	/// CharRace���еĶ���
	const _DBC_CHAR_RACE*	m_pCharRace;

	// װ���ϵ���Ч�б�
	// map< HUMAN_EQUIP����Ч�б� > 
	typedef std::map<INT, std::vector<UINT> > EquitEffectMap;

	EquitEffectMap m_EquitEffectMap;

protected:

	KL_DECLARE_DYNAMIC(CObject_PlayerOther);
	KL_DECLARE_LOGICAL(true);

};	// class
