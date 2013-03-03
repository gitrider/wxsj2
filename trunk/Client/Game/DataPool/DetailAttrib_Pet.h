// DetailAttrib_Player.h
// 
//////////////////////////////////////////////////////

#pragma once

#include <map>
#include "DB_Struct.h"
#include "GameDefine2.h"
#include "DetailAttrib_Base.h"
#include "TDException.h"


//struct _DBC_LIFEABILITY_DEFINE;
//struct _DBC_LIFEABILITY_ITEMCOMPOSE;
//struct _DBC_SKILL_XINFA;
//class CDetailAttrib_Player;
/*
//-----------------------------------------------
//�ͻ��˼������ݽṹ
struct SClientSkill :public _OWN_SKILL
{
	const CDetailAttrib_Player	*m_pOwner;
	const SSkillData			*m_pSkillData;
	
	//�ڼ������� (UIͬ��)
	INT							m_nPosIndex;
	//�Ƿ��Ѿ�ѧ��
	BOOL						m_bLeaned;

	INT GetCoolDownTime( VOID )const;
	INT GetCoolDownTotalTime( VOID )const;
};

//�ͻ�����������ݽṹ
struct SClientLifeAbility : public _OWN_ABILITY
{
	const _DBC_LIFEABILITY_DEFINE * m_pDefine;
	INT					m_nPosIndex;	//�ڼ������� (UIͬ��)
};

//�ͻ����ķ����ݽṹ
struct SClientXinFa : public _OWN_XINFA
{
	const _DBC_SKILL_XINFA*		m_pDefine;
	//�ڼ����ķ� (UIͬ��)
	INT							m_nPosIndex;
	//�Ƿ��Ѿ�ѧ��
	BOOL						m_bLeaned;
};

//�ͻ����䷽���ݽṹ
struct SClientPrescr
{
	const _DBC_LIFEABILITY_ITEMCOMPOSE* m_pDefine;
};

//---------------------------------------------------------------

typedef std::map<UINT, SClientSkill*>			CSkillMap;
typedef std::map<UINT, SClientXinFa*>			CXinFaMap;
typedef std::map<UINT, SClientLifeAbility* >	CLifeAbilityMap;
typedef std::vector< SClientPrescr* >			CPrescrBuffer;

*/

class CDetailAttrib_Pet :
	public CDetailAttrib_Base
{
	friend class CUIDataPool;
public:
	CDetailAttrib_Player( VOID );
	virtual ~CDetailAttrib_Pet( VOID );

	virtual BOOL Init( VOID );
	virtual VOID Term( VOID );

	virtual VOID Tick( VOID );
/*
	SClientSkill *		GetSkill( SkillID_t idSkill );
	SClientXinFa *		GetXinFa( SkillID_t idXinFa );

	SClientLifeAbility*	GetLifeAbility( INT idAbilityID);
	SClientPrescr*		GetPrescr(INT nIndex);
	INT					GetProscrNum(VOID) { return (INT)m_vPrescr.size(); }

	//virtual VOID ChangeSkillAttrib( const _OWN_SKILL *pSkillInit );

//	virtual VOID ChangeXinFaAttrib( const _OWN_XINFA *pXinFaInit );
	//��������ݷ����ı�
	virtual VOID ChangeLifeAbilityAttrib( INT idAbility, const _OWN_ABILITY* pAbility);
	//��������ݷ����ı�
	virtual VOID ChangeLifeAbilityAttrib( INT idAbility, BYTE type, uint value);
	//�䷽�����ı�
	virtual VOID ChangePrescrAttrib(INT idProscr, BOOL bAdd = TRUE);
*/
	virtual VOID AddMission( const _OWN_MISSION *pMission );

	virtual VOID ModifyMission( const _OWN_MISSION *pMission );

	virtual VOID RemoveMission( MissionID_t idMission );

	virtual VOID UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList );

	virtual VOID AddCanPickMissionItem( UINT dwItemDataID );

	virtual VOID RemoveCanPickMissionItem( UINT dwItemDataID );

	//virtual VOID UpdateCoolDownList( const UINT *pdwCoolDownList, UINT dwUpdateNum );

	// ͨ������IDȡ�������������������ֵ�� �޸�����ʱ����UINT_MAX
	UINT GetMissionIndexByID( MissionID_t idMission )const;

	// �����������
	VOID SetMissionParam( UINT dwIndexMission, UINT dwIndexParam, INT nValue ){
		Assert( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM );
		if ( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM )
			m_listMission.m_aMission[dwIndexMission].m_anParam[dwIndexParam] = nValue;
	}

	// ȡ�������
	INT GetMissionParam( UINT dwIndexMission, UINT dwIndexParam )const{
		Assert( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM );
		if ( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM )
			return m_listMission.m_aMission[dwIndexMission].m_anParam[dwIndexParam];
		else
			return FALSE;
	}

	const _OWN_MISSION *GetMission( UINT dwIndex )const{
		if ( dwIndex < MAX_CHAR_MISSION_NUM )
			return &m_listMission.m_aMission[dwIndex];
		else
			return NULL;
	}
/*
	INT GetCoolDownTime( UINT dwCoolDownID )const{
		if ( dwCoolDownID < COOLDOWN_LIST_SIZE )
			return m_anCoolDownTime[dwCoolDownID];
		else
			return 0;
	}

	INT GetCoolDownTotalTime( UINT dwCoolDownID )const{
		if ( dwCoolDownID < COOLDOWN_LIST_SIZE )
			return m_anCoolDownTotalTime[dwCoolDownID];
		else
			return 0;
	}
*/
	//�������----------------------------------------------
	//player�ĵ�ǰ��������
	INT GetMission_Num()const{return m_listMission.m_Count;}


protected:
//	BOOL FillSkillAttrib( SClientSkill *pSkill );
	

	//�������Է����ı�
//	VOID OnMenPaiChanged(UINT dwMenpaiID);
	//ĳһ���ķ������ı�
//	VOID OnXinFaAttribChanged( SkillID_t idXinFa );

	VOID OnMissionChanged( UINT dwIndex );
//	SClientLifeAbility* FindAbility(INT idAbility, BOOL bCreate = FALSE);

protected:

	// ��Ʒ��
	// װ����
	// ���ܱ�
	//CSkillMap		m_mapSkill;	
	// �ķ���
	//CXinFaMap		m_mapXinFa;
	// �����
	//CLifeAbilityMap m_mapLifeAbility;
	//�ܹ�ʹ�õ��䷽��
	//CPrescrBuffer	m_vPrescr;

	// �����
	_MISSION_LIST		m_listMission;

	// ���Ա����ͻ������ܿ�����������������ƷID�б�
	UINT				m_dwCanPickMissionItemIDNum;
	UINT				m_adwCanPickMissionItemList[MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM];
/*
	// ��ȴʱ��
	INT				m_anCoolDownTime[COOLDOWN_LIST_SIZE];
	INT				m_anCoolDownTotalTime[COOLDOWN_LIST_SIZE];
	*/
};
