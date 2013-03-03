
/** UI���ݳ�
 */

#pragma once


#include "GIUIDataPool.h"
#include "..\GameCommand.h"
#include "DetailAttrib_Player.h"
#include "..\Interface\GMGameInterface_Script.h"
#include "TeamOrGroup.h"							// ������. 
#include "GCStudyXinfa.h"
#include "GameStruct_Skill.h"



#define MAX_INVITE_COUNT	5


struct MEMBERINVITEINFO
{
	GUID_t	SourceId;
	GUID_t	DestinId;
	BOOL	bHave;

	MEMBERINVITEINFO()
	{
		bHave = FALSE;
	}
};

struct ScriptParam_EventList;
struct ScriptParam_MissionInfo;
struct ScriptParam_MissionDemandInfo;
struct ScriptParam_MissionContinueInfo;
struct ScriptParam_MissionTips;
struct SMissionBonus;
struct SMissionBonusItem;

class CAI_Base;

/** ��UI��ʾ��ص�����
 */
class CUIDataPool : public tUIDataPool
{
public:

	CUIDataPool( VOID );
	virtual ~CUIDataPool( VOID );

	virtual VOID Initial( VOID* );
	virtual VOID Release(VOID);
	virtual VOID Tick( VOID );

	virtual RC_RESULT OnCommand_( const SCommand_DPC *pCmd );

	const CDetailAttrib_Player *GetMySelfDetailAttrib( VOID )const
	{
		return m_pPlayerData;
	}

protected:

	//virtual VOID OnMySelfIDChanged( VOID );
	virtual VOID OnEventListResponse( VOID );
	virtual VOID OnMissionInfoResponse( VOID );
	virtual VOID OnMissionRegie( VOID );
	virtual VOID OnMissionDemandInfoResponse( VOID );
	virtual VOID OnMissionContinueInfoResponse( VOID );
	virtual VOID OnMissionTips( VOID );

public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ������ص���Ϣ
	//
	// ����ֻ���ṩһ������ĵ���ʱ�书�ܣ���û�����Ӧ�Ŀؼ�֧�֣�����ֻ����ʱ�����ַ���ʵ��
	//

	// ��ʼ������ʱ��ļ�ʱ
	VOID BeginOutGhostTimer( INT nOutGhostTimer );

	// ֹͣ������ʱ��ļ�ʱ
	VOID EndOutGhostTimer( VOID );

protected:

	/// �����ϵ�ʣ��ʱ������Ƿ�Ϊ����״̬
	BOOL		m_bOutGhostTimerWorking;	
	/// �����ϵ�ʣ��ʱ��
	INT			m_nOutGhostTimer;			

	/// ��ǰ�Ի�NpcId
	INT			m_nCurDialogNpcId;
	/// ��ǰ���˵�Id
	INT			m_nCurShopNpcId;

public:
	
	/** ��ǰ�Ի�NpcId */
	INT GetCurDialogNpcId()							{ return m_nCurDialogNpcId; }
	VOID SetCurDialogNpcId(INT nCurDialogNpcId)		{ m_nCurDialogNpcId = nCurDialogNpcId; }

	/** ��ǰ���˵�Id */
	INT GetCurShopNpcId()							{ return m_nCurShopNpcId; }
	VOID SetCurShopNpcId(INT nCurShopNpcId)			{ m_nCurShopNpcId = nCurShopNpcId; }

	//----------------------------------------------------------------------------------------------------------------------------
	// �����صĲ���
	//
	// ���ڶ�����Ϣֻ�뵱ǰ����й�ϵ, ��������Ϸ�ͻ���ֻ����һ��, Ӧ��д�����ݳ���.

	/// ��ǰ��ʾ�Ķ��������.
	INT			m_iCurTeamType;

	/** ���õ�ǰ���������. */
	void SetCurTeamType(INT iTeamType);
	/** �õ���ǰ�Ķ��������. */
	INT GetCurTeamType();


	//----------------------------------------------------------------------------------------------------------------------------
	// �򿪶ӳ�����

	/// �򿪶ӳ�����, ѡ�������.
	INT			m_iSelectedTeamMember_Leader;

	/// ѡ�еĶ�������.
	INT			m_iSelectedTeamMember;

	/// ѡ�е������ߵ�����
	INT			m_iSelectApply_Apply;


	//---------------------------------------------
	//  ����, ����, ����ʹ��.

	/// ����������id
	GUID_t		m_IdSource;

	/// ��������id
	GUID_t		m_IdDestination;


	/** ��ԱҪ��������Ա�����ݽṹ */
	MEMBERINVITEINFO	m_MemberInviteInfo[MAX_INVITE_COUNT];

	/** ���ö�Ա�����ߺͱ�������guid */
	INT SetMemberInviteGUID(GUID_t SId, GUID_t DId);

	/** �õ���Ա�����ߺͱ�������guid�� ��ɾ�� */
	BOOL GetMemberInviteGUID(int iPos, GUID_t& SId, GUID_t& DId, BOOL bDel = TRUE);
	
	/// ����|�Ŷ���Ϣ
	CTeamOrGroup	m_TeamOrGroup;

	/** ���ö��������ߵ�Id */
	VOID SetTeamInvitorGUID(GUID_t Id) ;
 
	/** �õ����������ߵ�id */
	GUID_t GetTeamInvitorGUID();

	/** �õ����������ߵ�id */
	GUID_t GetTeamInvitorGUID(int iTeamIndex);

	/** ����SOURCE_Id */
	VOID SetSourceGUID(GUID_t Id) ;

	/** �õ�SOURCE_Id */
	GUID_t GetSourceGUID();

	/** �õ�������SOURCE_Id */
	GUID_t GetApplySourceGUID(int iApplyIndex);


	/** ����DESTINATION_Id */
	VOID SetDestinationGUID(GUID_t Id) ;

	/** �õ�DESTINATION_Id */
	GUID_t GetDestinationGUID();



	// �õ�������Ϣ
	CTeamOrGroup* GetTeamOrGroup()		{ return &m_TeamOrGroup; }

	// ���������Ϣ
	VOID ClearTeamInfo(VOID);

	// �뿪����
	VOID LeaveTeam();

	// ��ɢ����
	VOID DismissTeam();	

	// ������Ϣ
	VOID KickTeamMember();

	// ������Ϣ
	VOID KickTeamMember(int iIndex);

	// �����ӳ�
	VOID AppointLeader();

	// �����ӳ�
	VOID AppointLeader(int iIndex);

	// ĳ����Ա�Ƿ��Ƕӳ�.
	BOOL IsTeamLeader(INT iIndex);		

	// �Լ��Ƿ��Ƕӳ�.
	BOOL IsTeamLeader_Me();

	// ��ǰ�Ƿ���һ��������
	BOOL IsInTeam();

	// ��Ա���Լ��Ƿ���ͬһ��������
	BOOL IsInScene(INT iIndex);

	// �ж�һ��������Լ��Ƿ���ͬһ�������С�
	BOOL IsTeamMember(GUID_t Id);


	// ͨ��guid�õ���������.
	LPCTSTR GetTeamNameByGUID(GUID_t Id);

	// ͨ��guid�õ�����HP.
	int GetTeamHPByGUID(GUID_t Id);

	// ͨ��guid�õ�����MP.
	int GetTeamMPByGUID(GUID_t Id);

	// ͨ��guid�õ�����Range.
	int GetTeamRangeByGUID(GUID_t Id);

	// ͨ��guid�õ�����Level.
	int GetTeamLevelByGUID(GUID_t Id);

	// ͨ��guid�õ�����ͷ��.
	LPCTSTR GetTeamIconByGUID(GUID_t Id);

	// ͨ��guid�õ�����HP.
	float GetTeamHPPercentByGUID(GUID_t Id);

	// ͨ��guid�õ�����MP.
	float GetTeamMPPercentByGUID(GUID_t Id);



	//-----------------------------------------------------------------------------------
	// ����������

	/// ��ǰ�ܾ����������.
	INT			m_iCurRejectTeamIndex;

	/** ͬ�����һ�������, ������е��������. */
	VOID ClearAllInviteTeam();	

	/** �ܾ���ǰ�ܾ��Ķ���. */
	VOID EraseCurRejectTeam();

	//
	// �����صĲ���
	//---------------------------------------------------------------------------------------------------------------------------------


public:

	VOID SendSelectEvent(INT nIndex,INT nExIndex1,INT nExIndex2);
	VOID SendAcceptEvent();
	VOID SendRefuseEvent();
	VOID SendContinueEvent();
	VOID SendCompleteEvent(INT nIndex);
	VOID SendAbandonEvent(INT nIndex);
	VOID SendMissionItemInfo(INT nPetIndex);


	//------------------------------------------------------
	//��������Ʒ
	struct QuestRewardItem
	{
		SMissionBonus*			pItemData;
		tObject_Item*			pItemImpl;
		BOOL					bSelected;
	};

	//------------------------------------------------------
	//������Ҫ��Ʒ
	struct QuestDemandItem
	{
		SMissionBonusItem*		pDemandItem;
		tObject_Item*			pItemImpl;
	};

	VOID QuestReward_Clear(VOID);
	VOID QuestDemand_Clear(VOID);
	VOID QuestReward_AddItem(const QuestRewardItem& itemReward);
	VOID QuestDemand_AddItem(const QuestDemandItem& itemDemand);

public:

	static CUIDataPool*	GetMe(VOID)		{ return s_pMe; }

	// 20100510 AddCodeBegin
	// ��ǰNpc�����Ƿ��пɽ�����(����������ǰ�������Ѿ����)
	BOOL	IsNpcMissionCanAccept( LPCTSTR szNpcName );
	// 20100510 AddCodeEnd

	// �µ�һ��ͬ������ÿ�ջ���� 20100728 BLL 
	VOID	TickMySelfDetailAttrib();

protected:

	static CUIDataPool*	s_pMe;

protected:

	std::vector<QuestRewardItem>	m_vecQuestRewardItem;

	/// ����������Ʒ
	std::vector< QuestDemandItem >  m_vecQuestDemandItem;

	KL_DECLARE_DYNAMIC( CUIDataPool );

	friend class SCRIPT_SANDBOX::DataPool;



protected:

	/// Player attrib
	CDetailAttrib_Player*				m_pPlayerData;

	/// ��һ�κ�npc�Ի����õ�npc���ܼ���Ĳ���
	ScriptParam_EventList*				m_pEventList;
	/// �ڽ�����ʱ��������������Ϣ
	ScriptParam_MissionInfo*			m_pMissionInfo;
	/// ����������ٴκ�npc�Ի������õ�������������Ϣ
	ScriptParam_MissionDemandInfo*		m_pMissionDemandInfo;
	/// �������󣬵�����������ֵ�������Ϣ
	ScriptParam_MissionContinueInfo*	m_pMissionContinueInfo;
	/// �����������У��׶�����ʾ��Ϣ
	ScriptParam_MissionTips*			m_pMissionTips;


};