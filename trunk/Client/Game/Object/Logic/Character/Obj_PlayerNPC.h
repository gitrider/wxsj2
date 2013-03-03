
/**	Obj_PlayerNPC.h
 */

#pragma once


#include "Obj_Character.h"
#include "../../../DataPool/GMDP_Struct_Pet.h"



/// NPC ����
enum ENUM_NPC_TYPE
{
	NPC_TYPE_INVALID	= -1,
	NPC_TYPE_MONSTER,			// ����
	NPC_TYPE_PET,				// ����
	NPC_TYPE_MOUNT,				// ����

	NPC_TYPE_NUMBERS
};

/// ����״̬����Ӧ����ң�
enum MISSION_STATE
{
	MS_INVALID = -1,
	
	MS_MISS,				// �пɽ�����
	MS_HAVE_MISS_NO_DONE,	// �ѽ����� δ���
	MS_HAVE_MISS_DONE,		// �ѽ����� �����
};


struct _DBC_CREATURE_ATT;


/** 
	NPC��ɫ�࣬��̳г� PlayerOther
 */
class CObject_PlayerNPC : public CObject_Character
{
public:

	CObject_PlayerNPC();
	virtual ~CObject_PlayerNPC();


	/** 
		���ݳ�ʼ�����壬��ͬ������Ⱦ�� 
	 */
	virtual	VOID Initial(VOID*);


	/** 
		���ؽ�ɫ����
	 */
	virtual CHARACTER_TYPE		GetCharacterType( VOID ) const				{ return CT_MONSTER; }
	ENUM_NPC_TYPE				GetNpcType(VOID) const;
	VOID						SetNpcType(ENUM_NPC_TYPE eType);
	
	const _DBC_CREATURE_ATT*	GetCreatureAtt(void) const					{ return m_pCreatureInfo; }

	/** ������GUID 
	 */
	virtual VOID				SetServerGUID(GUID_t guid)					{ m_GUIDServer = guid; }
	virtual GUID_t				GetServerGUID(VOID) const					{ return m_GUIDServer; }

	/** ���ָ��ķ��� */
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	/** �Ҽ�ָ��ķ��� */
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


	/** װ��ҽ�����Ʒ */
	virtual VOID				EquipItem_BodyLocator( INT nID, ENUM_WEAPON_LOCATOR_TYPE loc );	
	/** ж����Ʒ(��ʾȱʡ����) */
	virtual VOID				UnEquipItem( HUMAN_EQUIP nPart );

protected:

	/** RenderInterface����/ɾ�� */
	virtual VOID				CreateRenderInterface(VOID);

	virtual INT					CalcCharModelID( VOID ) const;

	virtual UINT				GetIdleInterval(VOID) const;

	virtual VOID				UpdateCharRace(VOID);


	/**
	 *	�ӱ��ж��������ϵ���Ч�󵽹�
	 */
	VOID						SetNpcEffect( VOID );
	/**
	 *	�����Ч
	 */
	VOID						ReleaseNpcEffect();

public:
	VOID						SetNpcMountEffect(INT nMountID, SDATA_PET* pPetData);

protected:

	virtual VOID				OnDataChanged_RaceID(VOID);

protected:

	/// �����ݱ��е��߼����ݽṹ
	const _DBC_CREATURE_ATT*	m_pCreatureInfo;

	/// GUID ����������
	GUID_t						m_GUIDServer; 

	ENUM_NPC_TYPE				m_eNpcType;

public:

	virtual INT					CannotBeAttack(VOID) const;


	/** ��ø�npc��Ӧ����ҵ�����״̬ */
	MISSION_STATE				GetNpcMissionState( VOID );

	/**  */
	VOID						SetNpcMissionData( VOID );

	/** ��Ӹ�npc��Ӧ����ҵ����� */
	VOID						AddNpcMission( INT nMissionId, INT nNPCState ); // 20100511 ModifyCode

	/** ɾ����npc��Ӧ����ҵ����� */
	VOID						DelNpcMission( INT nMissionId );

	/** ��ø�npc��Ӧ����ҵ�����״̬ */
	VOID						SetNpcMissionState( INT nMissionId );



	/// ����״̬����
	struct NPC_MISSION_DATA
	{
		INT		nMissionId;
		INT		nMissionNPCState;	// ����NPC��״̬ 1,������NPC; 2,������NPC; 3,�Ƚ��ֽ�NPC  // 20100511 ModifyCode
	};

	/// 
	std::vector< NPC_MISSION_DATA >	m_vMissionData;

	/// ��ǰ״̬
	MISSION_STATE					m_eMissionState;
	/// ״̬�Ƿ�ı���  
	BOOL							m_bMissStateChange;


protected:

/**	�ı���������
 */

	friend class CCharacterData;

	virtual VOID				OnDataChanged_Equip(HUMAN_EQUIP point);

	VOID						UpdateEquip(HUMAN_EQUIP point);


public:

/** ���ƶ�NPC�Ի�ͣ��������Ҫ����NPC��ת������
 */

	VOID						SetNeedFaceToPlayer(BOOL bValue)			{ m_bNeedFaceToPlayer = bValue; }
	BOOL						GetNeedFaceToPlayer(VOID) const				{ return m_bNeedFaceToPlayer; }
	/** �Ƿ��ת����� */
	BOOL						IsCanFaceToPlayer(VOID);
	/** �Ƿ���ʾ��Ϣ�� */
	BOOL						IsDisplayBoard(VOID);
	/** ͷ����Ϣ��߶� */
	FLOAT						GetBoardHeight(VOID);
	/** ѡ�л���С */
	virtual FLOAT				GetProjtexRange(VOID);
	/** ��Ӱ��С */
	virtual FLOAT				GetShadowRange(VOID);

	LPCSTR						GetPortrait(VOID);

protected:

	/** ת����� */
	VOID						FaceToPlayer(VOID);
	VOID						StopFaceToPlayer(VOID);

	/** ����NPCת�� */
	VOID						ProcessFaceToPlayer(VOID); 

private:

	BOOL						m_bNeedFaceToPlayer;
	BOOL						m_bFaceToPlayer;
	UINT						m_uStartFaceToPlayerTime;
	/// ��Obj��ת֮ǰ�ĽǶ�,��ʼ��Ϊ<0.0f, 0.0f, 0.0f>
	fVector3					m_fvPreRotation;

protected:

	virtual VOID				CharacterLogic_OnStopped(BOOL bFinished, LOGIC_TYPE nLogicTag);
	virtual BOOL				Tick_Idle(UINT uElapseTime);
	const fVector3&				GetPreRotation(VOID)const					{ return m_fvPreRotation; }

public:

	KL_DECLARE_DYNAMIC(CObject_PlayerNPC);
	KL_DECLARE_LOGICAL(true);

};
