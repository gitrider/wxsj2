
/** Obj_PlayerMySelf.h
 */

#pragma once


#include "Obj_PlayerOther.h"



/** �����������
 */
class CObject_PlayerMySelf : public CObject_PlayerOther
{
public:

	//// ѹ��һ��ָ��
	//virtual BOOL			PushCommand(const SCommand_Object *pCmd );

	/** ���ָ��ķ��� */
	virtual BOOL			FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** �Ҽ�ָ��ķ��� */
	virtual BOOL			FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

	/** ����ͷ����Ϣ */
	virtual VOID			Tick_UpdateInfoBoard( VOID );
	/** ����ͷ���ƺ���Ϣ��״̬ */
	virtual VOID			SetBoardTitleState( BOOL bShow, INT nType );
	/** ����ͷ��״̬��Ϣ��״̬ */
	virtual VOID			SetBoardHpLevelState( BOOL bShow );

	/** ��������¼�е�ģ�Ͳ��ŵĶ��� */
	VOID					SetSouXiaUseSkill_id( SkillID_t id );

	/** ��������¼�е�����Ԥ�� */
	VOID					SetSouXiaPrewArm( INT ArmID, INT EquipPoint );
	/** �������¼�е�����Ԥ�� */
	VOID					ClearSouXiaPrewArm();
	/** ��������¼�е������ٻ�Ԥ�� */
	VOID					SetSouXiaPrewPet( INT modelId );
	/** ��������¼�е������ٻ�Ԥ�� */
	VOID					SetSouXiaPrewZuoJi( INT modelId );

public:
	
	/** ���ؽ�ɫ���� */
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const					{ return CT_PLAYERMYSELF;	}
	BOOL					IsInVisualFiled( CObject_Character* pObj );

protected:
public:

/** �߼����ݷ����ı�
 */

	friend class CCharacterData;

	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
	/** ͷ��ı��¼����� */
	virtual VOID			OnDataChanged_FaceImage(VOID);			
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_Level(VOID);
	virtual VOID			OnDataChanged_Profession(VOID);
	virtual VOID			OnDataChanged_HP(VOID);				// ��������ֵ
	virtual VOID			OnDataChanged_CurTitles(INT nType);

protected:

	/// ����UI��ʾ���߼�����
	CObject_PlayerOther*	m_pAvatar;

	/// ��������¼����Ԥ����ʾ���߼�����
	CObject_PlayerOther*	m_pAvatar2;

	/// ��������¼����Ԥ����ʾ���߼�����
	CObject_PlayerOther* m_pAvatarArm;

	/// ��������¼Ԥ����ʾ�����ޣ��������
	CObject_PlayerNPC*	m_pAvatarPet;
	CObject_PlayerNPC* m_pAvatarZuoJi;

	/// ����UI��ʾ���߼�����
//	CObject_PlayerOther*	m_pHeadAvatar;

public:

	INT						GetSimulantLogicCount(VOID) const				{ return m_nSimulantLogicCount; }
	VOID					SetSimulantLogicCount(INT nLogicCount)			{ m_nSimulantLogicCount = nLogicCount; }

//	virtual VOID			ResetLogicCount(VOID);
	virtual VOID			ResetBaseLogicCount(VOID);
	virtual VOID			ResetActionLogicCount(VOID);

protected:

	INT		m_nSimulantLogicCount;

public:
	
/** ���ļ̳�
 */

	CObject_PlayerMySelf( VOID );
	virtual ~CObject_PlayerMySelf( VOID );

	/** ���ݳ�ʼ�����壬��ͬ������Ⱦ�� */
	virtual	VOID			Initial(VOID*);
	virtual VOID			Release(VOID);

protected:
	
	KL_DECLARE_DYNAMIC(CObject_PlayerMySelf);
	KL_DECLARE_LOGICAL(true);

};
