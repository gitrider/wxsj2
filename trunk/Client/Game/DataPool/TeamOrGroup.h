
/** ����߼�
	�޸����������������Ľ�������
 */

#pragma once


#include "Type.h"
#include "GIBasicType.h"
#include "GameStruct.h"
#include "GameStruct_Team.h"
#include "..\Object\Logic\UIObj\UIModel.h"



/** �ͻ��� ����|�Ŷ� ���ݽṹ
 */
struct TeamMemberInfo
{
	GUID_t					m_GUID;
	ObjID_t					m_OjbID;						//
	SceneID_t				m_SceneID;
	CHAR					m_szNick[MAX_CHARACTER_NAME];		// 1.�ǳ�
	CHAR					m_szUtf8Nick[MAX_CHARACTER_NAME*2];	
	UINT					m_uFamily;							// 2.����
	USHORT					m_uDataID;						// 3.�Ա�
	UINT					m_uLevel;						// 4.�ȼ�
	INT						m_nPortrait;					// 5.ͷ��
	WORLD_POS				m_WorldPos;						// 6.λ�ã����꣩
	INT						m_nHP;							// 7.HP
	UINT					m_dwMaxHP;						// 8.HP����
	INT						m_nMP;							// 9.MP
	UINT					m_dwMaxMP;						// 10.MP����
	INT						m_nAnger;						// 11.ŭ��
	UINT					m_WeaponID;						// ������
	UINT					m_AssiID;						// ������
	UINT					m_CapID;						// 13.ñ��
	UINT					m_ArmourID;						// 14.�·�
	UINT					m_ShoulderID;					// ��
	UINT					m_CuffID;						// 15.����
	UINT					m_FootID;						// 16.ѥ��


	// 17.buff����ʱ������
	SimpleImpactList		m_SimpleImpactList;				// Buff �б�
	BOOL					m_bDeadLink;					// 18.����
	BOOL					m_bDead;						// 19.����
	UINT					m_uFaceMeshID;					// 20.�沿ģ��
	UINT					m_uHairMeshID;					// 21.ͷ��ģ��
	UINT					m_uHairColor;					// 22.ͷ����ɫ

	CModelShowInUI			m_UIModel;						// �ڽ�����ʾ��ģ��

	TeamMemberInfo()
	{
		memset(m_szNick, 0, sizeof(m_szNick));
		memset(m_szUtf8Nick,0,sizeof(m_szUtf8Nick));
		m_GUID		= 0;
		m_OjbID		= 0;						//
		m_SceneID	= 0;
		m_uFamily	= -1;						// 2.����
		m_uDataID   = 0;						// 3.�Ա�
		m_uLevel    = 0;						// 4.�ȼ�
		m_nPortrait	= -1;						// 5.ͷ��
		m_nHP		= 0;						// 7.HP
		m_dwMaxHP	= 0;						// 8.HP����
		m_nMP		= 0;						// 9.MP
		m_dwMaxMP	= 0;						// 10.MP����
		m_nAnger	= 0;						// 11.ŭ��
		m_WeaponID	= 0;						// 12.����
		m_AssiID	= 0;
		m_CapID		= 0;						// 13.ñ��
		m_ArmourID	= 0;						// 14.�·�
		m_ShoulderID= 0;
		m_CuffID	= 0;						// 15.����
		m_FootID	= 0;						// 16.ѥ��
		m_bDeadLink = FALSE;					// 18.����
		m_bDead		= FALSE;					// 19.����
		m_uFaceMeshID = 0;						// 20.�沿ģ��
		m_uHairMeshID = 0;						// 21.ͷ��ģ��
		m_uHairColor = 0;						// 22.ͷ����ɫ
		// �����µ�ģ��
		//m_UIModel.CreateNewUIModel();
	}


};

typedef std::vector<TeamMemberInfo*>		CTeamMembers;

struct TeamInfo
{
	TeamID_t		m_TeamID;			// 1 or 2, 3, ...
	CTeamMembers	m_TeamMembers;		// [MAX_TEAM_MEMBER]
};

typedef std::vector<TeamInfo*>			CTeamInfo;

// ���������Ϊ�������ʾ�����˻�������������Ϣ������
struct TeamCacheInfo 
{
	GUID_t					m_GUID;
	CHAR					m_szNick[MAX_CHARACTER_NAME];	// 1.�ǳ�
	UINT					m_uFamily;						// 2.����
	SceneID_t				m_Scene;						// 3.����
	UINT					m_uLevel;						// 4.�ȼ�
	UINT					m_uDetailFlag;					// ������Ϣ�Ƿ���ʾ
	USHORT					m_uDataID;						// 5.�Ա�
	UINT					m_WeaponID;						// 7.��������
	UINT					m_AssihandID;					// ����
	UINT					m_CapID;						// 8.ñ��
	UINT					m_ArmourID;						// 9.�·�
	UINT					m_ShoulderID;					// ��
	UINT					m_CuffID;						// 10.����
	UINT					m_FootID;						// 11.ѥ��
	UINT					m_uFaceMeshID;					// 12.�沿ģ��
	UINT					m_uHairMeshID;					// 13.ͷ��ģ��
	UINT					m_uHairColor;					// 14.ͷ����ɫ


	CModelShowInUI			m_UIModel;						// �ڽ�����ʾ��ģ��
	
	TeamCacheInfo()
	{
		//memset((VOID*)this, 0, sizeof(TeamMemberInfo));
		memset(m_szNick, 0, sizeof(m_szNick));
		m_GUID			= 0;
		m_uFamily		= 0;
		m_Scene			= 0;						// 3.����
		m_uLevel		= 0;						// 4.�ȼ�
		m_uDetailFlag	= 0;					// ������Ϣ�Ƿ���ʾ
		m_uDataID		= 0;						// 5.�Ա�
		m_WeaponID		= 0;						// 7.����
		m_AssihandID	= 0;
		m_CapID			= 0;						// 8.ñ��
		m_ArmourID		= 0;						// 9.�·�
		m_ShoulderID	= 0;
		m_CuffID		= 0;						// 10.����
		m_FootID		= 0;						// 11.ѥ��
		m_uFaceMeshID	= 0;						// 12.�沿ģ��
		m_uHairMeshID	= 0;						// 13.ͷ��ģ��
		m_uHairColor	= 0;						// 14.ͷ����ɫ

		// �����µ�ģ��
		//m_UIModel.CreateNewUIModel();
	}
};

typedef std::deque< TeamCacheInfo* >		CProposers;

struct InviteTeam
{
	GUID_t					m_InviterGUID;
	CProposers				m_InvitersInfo;
};

typedef std::deque< InviteTeam* >		CInviteTeams;

class CTeamOrGroup
{ 
	// when a team change to a group, the member of m_TeamMembers will be assigned to m_TeamInfo[0].
public:

	enum TEAM_OR_GROUP_TYPE
	{
		TEAM_TYPE				= 1,
		GROUP_TYPE				= 2,
	};

	enum UI_ON_OPEN
	{
		UI_ALL_CLOSE			= 0,
		UI_INVITE_ON_OPEN		= 1,
		UI_APPLY_ON_OPEN		= 2,
	};

	enum
	{
		MAX_INVITE_TEAM			= 7,
		MAX_PROPOSER			= 18,
	};

public:

	CTeamOrGroup()
	{
		CleanUp();
	}
	~CTeamOrGroup();


	VOID CleanUp();

	BOOL HasTeam()
	{
		return (m_Type != (TEAM_OR_GROUP_TYPE)0);
	}

	// �齨���飬���ö���� leader�����Ҽ���Ϊ��һ����Ա
	VOID CreateTeam(TeamMemberInfo* leader, TeamID_t TeamID);

	// ��ɢ����
	VOID DismissTeam();

	// ����һ����Ա��������ŶӵĻ�����Ҫ�����Ŷ����ڵ�С���
	VOID AddMember(TeamMemberInfo* member, TeamID_t TeamID, BYTE TeamIndex = -1);

	// һ����Ա�뿪
	VOID DelMember(GUID_t guid);

	// ������Աλ��
	VOID ExchangeMemberPosition(GUID_t guid1, GUID_t guid2, BYTE TeamIndex1 = -1, BYTE TeamIndex2 = -1);

	// ���¶�Ա��Ϣ����ʱû���õ���
	VOID UpdateMemberInfo(TeamMemberInfo* member, GUID_t guid);

	// �ӳ�
	GUID_t GetLeaderGUID() { return m_LeaderID; }

	// ���öӳ� GUID
	VOID SetLeaderGUID(GUID_t guid);

	// �����¶ӳ�
	VOID Appoint(GUID_t guid);
	// �õ���Ա������
	INT GetTeamMemberCount(BYTE TeamIndex = -1);

	// �õ�ĳ��������ʾ�ڶ��ѽ�������ľ���λ�� 1,2,3...
	INT GetMemberUIIndex(GUID_t guid, BYTE TeamIndex = -1);

	// �����������ת���ɶ��������е�����
	const TeamMemberInfo* GetMemberByUIIndex(INT UIIndex, BYTE TeamIndex = -1);

	// ͨ����������, �õ�server id
	INT GetMemberServerIdByUIIndex(INT UIIndex, BYTE TeamIndex = -1);

	// ͨ�����������õ�ѡ�ж�Ա��guid
	GUID_t GetMemberGUIDByUIIndex(INT UIIndex, BYTE TeamIndex = -1);

	// �õ��Լ�������
	INT GetSelfIndex(BYTE TeamIndex = -1);

	// ����ĳ�� guid �ҵ��������
	TeamMemberInfo* GetMember(GUID_t guid);

	// ����server id �ҵ��������
	TeamMemberInfo* GetMemberByServerId(INT iServerId);

	// �õ������е� idx ����Ա
	const TeamMemberInfo* GetMemberByIndex(UINT idx, BYTE TeamIndex = -1);

	// �ı����롢�������Ĵ�״̬
	VOID SetUIFlag(UI_ON_OPEN flag);

	// ȡ�����롢�������Ĵ�״̬
	UI_ON_OPEN GetUIFlag()			{ return m_UIFlag; }

	// ����һ��������飬TRUE �ɹ�����֮ʧ��
	BOOL AddInviteTeam(InviteTeam* pTeam);

	// �õ���ǰ������������
	INT GetInviteTeamCount()		{ return (INT)m_InviteTeams.size(); }

	// ���������õ�ĳ������
	const InviteTeam* GetInviteTeamByIndex(INT idx);

	// ���ĳ���������
	VOID EraseInviteTeam(INT idx);

	// ����������
	VOID ClearInviteTeam();

	// ����һ��������
	BOOL AddProposer(TeamCacheInfo* pProposer);

	// ���һ��������
	VOID EraseProposer(INT idx);

	// ����������
	VOID ClearProposer();

	// �õ���ǰ������е�����
	INT GetProposerCount()			{ return (INT)m_Proposers.size(); }

	// ���������õ�ĳ��������
	const TeamCacheInfo* GetProposerByIndex(INT idx);

	// �������е�ǰ��ҵ���ϸ��Ϣ
	VOID FillMyInfo();

	// ����ģ����Ϣ
	VOID SetModelLook();

	// �Ƿ���ͬһ��������.
	BOOL IsInScene(INT iIndex);

	// ���� Buff �б�
	VOID UpdateImpactsList( INT ObjID, const SimpleImpactList* pSimpleImpactList );

	// ���� Buff
	VOID AddImpact( INT ObjID, ImpactID_t ImpactID );

	// ���� Buff
	VOID RemoveImpact( INT ObjID, ImpactID_t ImpactID );

	//���ķ���ģʽ
	VOID ChangeMode(BYTE Mode)		{m_Mode = Mode;}

	//��÷���ģʽ
	BYTE GetMode()					{return m_Mode;}


private:

	// �õ��Լ��Ķ�����Ϣ�����Լ������ݳػ�ȡ��
	VOID FillMyInfo(TeamMemberInfo* member);

	// ����С��� N �õ��� N ��С��
	TeamInfo* GetTeam(BYTE TeamIndex) const;

private:

	TEAM_OR_GROUP_TYPE		m_Type;				// team or group
	TeamID_t				m_ID;				// the serial number of the team or group in the game world
	// it can be used to identify the empty team or group
	BYTE					m_MyTeamID;			// the team ID of mine in my group
	GUID_t					m_LeaderID;			// guid of the team leader
	CTeamMembers			m_TeamMembers;		// [MAX_TEAM_MEMBER]
	CTeamInfo				m_TeamInfo;			// [MAX_TEAMS_IN_GROUP]

	BYTE					m_Mode;				//����������Ʒ�ķ���ģʽ
	// ����������Ϣ
	UI_ON_OPEN				m_UIFlag;			// �����ж������˽�����������˽����Ƿ��Ѿ���
	CProposers				m_Proposers;		// �������б��ӳ��ɼ�
	CInviteTeams			m_InviteTeams;		// ���������Ϣ�б��������˿ɼ�
};
