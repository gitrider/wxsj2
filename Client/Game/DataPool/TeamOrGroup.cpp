
// TeamOrGroup.cpp

#include "StdAfx.h"
#include "Network/NetManager.h"
#include "TeamOrGroup.h"
#include "GIException.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "GMDP_CharacterData.h"
#include "CGAskTeamMemberInfo.h"
#include "Procedure\GameProcedure.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "World\Scene.h"
#include "World\WorldManager.h"
#include "Engine\EngineInterface.h"
#include "DataPool\GMDataPool.h"




VOID CTeamOrGroup::CleanUp()
{
	m_Type = (TEAM_OR_GROUP_TYPE)0;
	m_ID = INVALID_ID;
	m_MyTeamID = 0xFF;
	m_LeaderID = INVALID_ID;
	m_Mode = 0;
}

CTeamOrGroup::~CTeamOrGroup()
{
	if ( HasTeam() )
	{
		DismissTeam();
	}

	//ClearInviteTeam();
	//ClearProposer();
}


VOID CTeamOrGroup::CreateTeam(TeamMemberInfo* leader, TeamID_t TeamID)
{
	if( m_Type )
	{
		Assert(FALSE);
		return;
	}

	m_Type = TEAM_TYPE;
	m_ID = TeamID;

	// 20100511 ModifyCode 先设置是否组队标记，再SetLeaderGUID设置队长标记
	// 设置角色层数据， 是否有组队的标记
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HaveTeamFlag(TRUE);

	SetLeaderGUID( leader->m_GUID );

	m_TeamMembers.push_back(leader);

	// 通知Talk Interface，队伍建立
	SCRIPT_SANDBOX::Talk::s_Talk.TeamCreate(m_ID);
}

VOID CTeamOrGroup::DismissTeam()
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ 
			// 队伍的情况
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				delete (*it_member); // 释放队员信息
			}

			m_TeamMembers.clear();
		}
		break;
	case GROUP_TYPE:
		{ 
			// 团队的情况
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					delete (*it_member); // 释放队员信息
				}

				(*it_team)->m_TeamMembers.clear();

				delete (*it_team);
			}

			m_TeamInfo.clear();
		}
		break;

	default:
		Assert(FALSE);
		return;
	}

	CleanUp();

	//通知Talk Interface，队伍解散
	//SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();

	// 设置角色层数据
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HaveTeamFlag(FALSE);		// 是否组队标记
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamLeaderFlag(FALSE);	// 是否组队标记
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFullFlag(FALSE);		// 满队标记
	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFollowFlag(FALSE);	// 组队跟随
}

TeamInfo* CTeamOrGroup::GetTeam(BYTE TeamIndex) const
{
	for(std::vector< TeamInfo* >::const_iterator it = m_TeamInfo.begin(); it != m_TeamInfo.end(); ++it)
	{
		if( (*it)->m_TeamID == TeamIndex )
		{
			return *it;
		}
	}

	return NULL;
}

INT CTeamOrGroup::GetMemberUIIndex(GUID_t guid, BYTE TeamIndex)
{	
	// 这里的 TeamIndex 不需要自增
	CTeamMembers* pTeamMembers;
	GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	if ( guid == myGUID )
	{ 
		// 自己没有序号
		return INVALID_ID;
	}

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ 
			// 队伍的情况
			pTeamMembers = &m_TeamMembers;
		}
		break;

	case GROUP_TYPE:
		{ 
			// 团队的情况
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return INVALID_ID;
			}

			if( m_MyTeamID != TeamIndex )
			{ 
				// 暂时不考虑显示团队成员信息
				return INVALID_ID;
			}

			pTeamMembers = &(GetTeam(TeamIndex)->m_TeamMembers);
		}
		break;

	default:
		Assert(FALSE);
		return INVALID_ID;
	}

	std::vector< TeamMemberInfo* >::iterator it_member;
	INT i;

	for( it_member = pTeamMembers->begin(), i=1; it_member != pTeamMembers->end(); ++it_member, ++i )
	{
		if ( (*it_member)->m_GUID == myGUID )
		{ 
			// 自己不占用序号
			--i;
			continue;
		}

		if( i > (INT)pTeamMembers->size() )
		{
			MessageBox(NULL, "Team UI Member Index Error occurred.", "Err", MB_OK);
		}

		if( (*it_member)->m_GUID == guid )
		{
			return i;
		}
	}

	return INVALID_ID;
}

const TeamMemberInfo* CTeamOrGroup::GetMemberByUIIndex(INT UIIndex, BYTE TeamIndex)
{
	CTeamMembers* pTeamMembers;
	GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ 
			// 队伍的情况
			pTeamMembers = &m_TeamMembers;
		}
		break;

	case GROUP_TYPE:
		{ 
			// 团队的情况
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return NULL;
			}

			if( m_MyTeamID != TeamIndex )
			{ 
				// 暂时不考虑显示团队成员信息
				return NULL;
			}

			pTeamMembers = &(GetTeam(TeamIndex)->m_TeamMembers);
		}
		break;

	default:
		Assert(FALSE);
		return NULL;
	}

	std::vector< TeamMemberInfo* >::iterator it_member;
	INT i;

	for( it_member = pTeamMembers->begin(), i=1; it_member != pTeamMembers->end(); ++it_member, ++i )
	{
		if( (*it_member)->m_GUID == myGUID )
		{ 
			// 自己不占用序号
			--i;
		}

		if( i == UIIndex )
		{
			return (*it_member);
		}
	}

	return NULL;
}

INT CTeamOrGroup::GetTeamMemberCount(BYTE TeamIndex)
{
	switch(m_Type)
	{
	case 0:
		return 0;

	case TEAM_TYPE:
		{ 
			// 队伍的情况
			return (INT)m_TeamMembers.size();
		}
		break;

	case GROUP_TYPE:
		{
			// 团队的情况
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return 0;
			}

			return (INT)GetTeam(TeamIndex)->m_TeamMembers.size();
		}
		break;

	default:
		Assert(FALSE);
		return 0;
	}
}

VOID CTeamOrGroup::FillMyInfo()
{
	if( !HasTeam() )
	{ 
		// 没有队伍不进行操作
		return;
	}

	TeamMemberInfo* pMyTMInfo;
	GUID_t guid;

	guid = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
	pMyTMInfo = GetMember(guid);

	if( pMyTMInfo == NULL )
	{
		Assert( FALSE && "Error: I'm not in my team...'" );
		return;
	}

	FillMyInfo( pMyTMInfo );
}

// 设置模型信息
VOID CTeamOrGroup::SetModelLook()
{
	if( !HasTeam() )
	{ 
		// 没有队伍不进行操作
		return;
	}

	TeamMemberInfo* pMyTMInfo;
	GUID_t guid;

	guid = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
	pMyTMInfo = GetMember(guid);
	if ( pMyTMInfo == NULL )
	{
		return;
	}

	// 设置ui模型
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_MAINHAND, pMyTMInfo->m_WeaponID);
	pMyTMInfo->m_UIModel.SetUIModelInfo(HEQUIP_ASSIHAND, pMyTMInfo->m_AssiID);
	
	// 设置ui模型
	pMyTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_HEAD, pMyTMInfo->m_CapID);
	pMyTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_BACK, pMyTMInfo->m_ArmourID);
	pMyTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_SHOULDER, pMyTMInfo->m_ShoulderID );
	pMyTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_WRIST,pMyTMInfo->m_CuffID );
	pMyTMInfo->m_UIModel.SetUIModelInfo( HEQUIP_FEET,pMyTMInfo->m_FootID );

	if(pMyTMInfo->m_uFaceMeshID < 255)
	{
		// 设置脸形
		pMyTMInfo->m_UIModel.SetFaceMeshId(pMyTMInfo->m_uFaceMeshID);
	}
		
	if(pMyTMInfo->m_uHairMeshID < 255)
	{
		// 设置发型
		pMyTMInfo->m_UIModel.SetFaceHairId(pMyTMInfo->m_uHairMeshID);
	}
	
	//if(pMyTMInfo->m_uHairColor < 255)
	//{
	//	// 设置颜色
	//	pMyTMInfo->m_UIModel.SetHairColor(pMyTMInfo->m_uHairColor);
	//}
	//else
	//{
	//	// 设置颜色
	//	pMyTMInfo->m_UIModel.SetHairColor(0);
	//}//

	// 设置颜色
	pMyTMInfo->m_UIModel.SetHairColor(pMyTMInfo->m_uHairColor);
	
}


VOID CTeamOrGroup::FillMyInfo(TeamMemberInfo* member)
{
	Assert( member );

	CObject_PlayerMySelf* pMe = CObjectManager::GetMe()->GetMySelf();

	Assert( member->m_GUID == pMe->GetServerGUID() );

	CCharacterData* pMyData = pMe->GetCharacterData();

	if(NULL == pMyData)
	{
		return ;
	}
	strncpy(member->m_szNick, pMyData->Get_Name(), MAX_CHARACTER_NAME-1);
	std::string utfstr;
	StringConvert::mbcs_to_utf8(member->m_szNick,utfstr);
	strncpy(member->m_szUtf8Nick,utfstr.c_str(),MAX_CHARACTER_NAME*2-1);
	member->m_szNick[MAX_CHARACTER_NAME-1] = '\0';
	member->m_szUtf8Nick[MAX_CHARACTER_NAME*2-1];

	member->m_uFamily		= pMyData->Get_MenPai();
	member->m_uDataID		= pMyData->Get_RaceID();
	member->m_uLevel		= pMyData->Get_Level();
	member->m_nPortrait		= pMyData->Get_PortraitID();	// 头像
	member->m_WorldPos.m_fX = pMe->GetPosition().x;
	member->m_WorldPos.m_fZ = pMe->GetPosition().z;
	member->m_nHP			= pMyData->Get_HP();
	member->m_dwMaxHP		= pMyData->Get_MaxHP();
	member->m_nMP			= pMyData->Get_MP();
	member->m_dwMaxMP		= pMyData->Get_MaxMP();
	member->m_nAnger		= pMyData->Get_Rage();		// 怒气

	member->m_WeaponID		= pMyData->Get_Equip(HEQUIP_MAINHAND);
	member->m_AssiID		= pMyData->Get_Equip(HEQUIP_ASSIHAND);
	member->m_CapID			= pMyData->Get_Equip(HEQUIP_HEAD);
	member->m_ArmourID		= pMyData->Get_Equip(HEQUIP_BACK);
	member->m_ShoulderID	= pMyData->Get_Equip(HEQUIP_SHOULDER);
	member->m_CuffID		= pMyData->Get_Equip(HEQUIP_WRIST);
	member->m_FootID		= pMyData->Get_Equip(HEQUIP_FEET);

	member->m_uFaceMeshID = pMyData->Get_FaceMesh();
	member->m_uHairMeshID = pMyData->Get_HairMesh();
	member->m_uHairColor  = pMyData->Get_HairColor();

	member->m_bDead			= ( pMe->CharacterLogic_Get( CObject_Character::LOGIC_BASE ) == CObject_Character::CHARACTER_LOGIC_DEAD );
}

VOID CTeamOrGroup::AddMember(TeamMemberInfo* member, TeamID_t TeamID, BYTE TeamIndex)
{
	TeamMemberInfo* pTMInfo = new TeamMemberInfo;
	//memcpy((VOID*)pTMInfo, member, sizeof(TeamMemberInfo));

	pTMInfo->m_GUID			= member->m_GUID;
	pTMInfo->m_OjbID		= member->m_OjbID;						//
	pTMInfo->m_SceneID		= member->m_SceneID;
	pTMInfo->m_uFamily		= member->m_uFamily;					// 2.职业
	pTMInfo->m_uDataID		= member->m_uDataID;					// 3.性别
	pTMInfo->m_nPortrait	= member->m_nPortrait;					// 5.头像
	memcpy(pTMInfo->m_szNick, member->m_szNick, sizeof(pTMInfo->m_szNick));
	
	std::string utfstr;
	StringConvert::mbcs_to_utf8(pTMInfo->m_szNick,utfstr);
	strncpy(pTMInfo->m_szUtf8Nick,utfstr.c_str(),MAX_CHARACTER_NAME*2-1);
	CObject_PlayerMySelf* pObj = CObjectManager::GetMe()->GetMySelf();

	if( pTMInfo->m_GUID != pObj->GetServerGUID() )
	{ // 需要加载队友的头像窗口
		// 请求该队友的数据
		CGAskTeamMemberInfo Msg;
		Msg.SetSceneID( pTMInfo->m_SceneID );
		Msg.setObjID( pTMInfo->m_OjbID );
		Msg.SetGUID( pTMInfo->m_GUID );

		CNetManager::GetMe()->SendPacket( &Msg );
	}
	else
	{
		FillMyInfo( pTMInfo );
	}

	switch(m_Type)
	{
	// 创建队伍
	case 0:
		{
			CreateTeam(pTMInfo, TeamID);
		}
		break;

	// 队伍加人的情况
	case TEAM_TYPE:
		{ 
			if( m_TeamMembers.size() >= MAX_TEAM_MEMBER )	// 最大的队伍人数
			{
				Assert(FALSE);
				return;
			}

			m_TeamMembers.push_back(pTMInfo);
		}
		break;

	// 团队加人的情况
	case GROUP_TYPE:
		{ 
			if( TeamIndex < 0 || TeamIndex >= MAX_TEAMS_IN_GROUP )	// 团队中最大队伍数量
			{
				Assert(FALSE);
				return;
			}

			++TeamIndex; // 默认传入的数字为 0 ~ MAX_TEAMS_IN_GROUP-1

			TeamInfo* pTeamInfo = GetTeam(TeamIndex);

			if(pTeamInfo != NULL)
			{
				if(pTeamInfo->m_TeamMembers.size() >= MAX_TEAM_MEMBER)
				{
					Assert(FALSE);
					return;
				}
			}
			// 如果该小组不存在，则创建，并添加入团队
			else
			{
				pTeamInfo = new TeamInfo;
				pTeamInfo->m_TeamID = TeamIndex;
				m_TeamInfo.push_back(pTeamInfo);

				//队伍满
				if(pTeamInfo->m_TeamMembers.size() >= MAX_TEAM_MEMBER)
				{
					CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFullFlag(TRUE);
				}
			}

			pTeamInfo->m_TeamMembers.push_back(pTMInfo);

			if( pTMInfo->m_GUID == pObj->GetServerGUID() )
			{
				m_MyTeamID = TeamIndex;
			}
		}
		break;

	default:
		Assert(FALSE);
		return;
	}



	// 创建一个界面的模型.
	pTMInfo->m_UIModel.CreateNewUIModel();
	
	// 设置ui模型

	// 设置职业
	pTMInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_MenPai( pTMInfo->m_uFamily );
	// 设置性别
	pTMInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID( pTMInfo->m_uDataID );
	
}

VOID CTeamOrGroup::DelMember(GUID_t guid)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍情况
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if( (*it_member)->m_GUID == guid )
				{
					delete (*it_member); // 释放队员信息
					m_TeamMembers.erase(it_member);
					break;
				}
			}

			//队伍不再满
			if(m_TeamMembers.size() < MAX_TEAM_MEMBER)
			{
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamFullFlag(FALSE);
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队情况
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			BOOL bFind = FALSE;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					if( (*it_member)->m_GUID == guid )
					{
						delete (*it_member); // 释放队员信息
						(*it_team)->m_TeamMembers.erase(it_member);
						bFind = TRUE;

						if( (*it_team)->m_TeamMembers.size() < 1 )
						{ // 该小组空了，则释放该小组的数据
							delete (*it_team);
							m_TeamInfo.erase(it_team);
						}

						break;
					}
				}

				if( bFind )
				{
					break;
				}
			}
		}
		break;
	default:
		Assert(FALSE);
		return;
	}

	if(guid == m_LeaderID)
	{ // choose a new leader
		switch(m_Type)
		{
		case TEAM_TYPE:
			{ // 队伍情况
				if( m_TeamMembers.size() < 1 )
				{ // 不在此处处理队伍解散的情况
					Assert(FALSE);
					return;
				}

				SetLeaderGUID( m_TeamMembers[0]->m_GUID );
			}
			break;
		case GROUP_TYPE:
			{ // 团队情况，暂时不考虑团队选择新团长的规则
				Assert(FALSE);
			}
			break;
		default:
			Assert(FALSE);
			return;
		}
	}
}

VOID CTeamOrGroup::ExchangeMemberPosition(GUID_t guid1, GUID_t guid2, BYTE TeamIndex1, BYTE TeamIndex2)
{
	TeamMemberInfo *pTMInfo1, *pTMInfo2;

	pTMInfo1 = GetMember(guid1);
	if ( pTMInfo1 == NULL )
	{
		Assert(FALSE);
		return;
	}

	pTMInfo2 = GetMember(guid2);
	if ( pTMInfo2 == NULL )
	{
		Assert(FALSE);
		return;
	}

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍情况
			for( UINT i = 0; i < m_TeamMembers.size(); ++i )
			{
				if ( m_TeamMembers[i]->m_GUID == pTMInfo1->m_GUID )
				{
					m_TeamMembers[i] = pTMInfo2;
					continue;
				}

				if ( m_TeamMembers[i]->m_GUID == pTMInfo2->m_GUID )
				{
					m_TeamMembers[i] = pTMInfo1;
					continue;
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队情况（暂时不考虑）
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID CTeamOrGroup::UpdateMemberInfo(TeamMemberInfo* member, GUID_t guid)
{
	TeamMemberInfo* pTMInfo;

	pTMInfo = GetMember(guid);
	if( pTMInfo == NULL )
	{
		Assert(FALSE);
		return;
	}

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍情况
		}
		break;
	case GROUP_TYPE:
		{ // 团队情况
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

// 任命新队长
VOID CTeamOrGroup::Appoint(GUID_t guid)
{ // 这个地方要 hardcode 了，否则很难整
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍情况
			std::vector< TeamMemberInfo* >::iterator it_member;
			TeamMemberInfo *pTMInfo;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if( (*it_member)->m_GUID == guid )
				{
					pTMInfo = *it_member; // 释放队员信息
					m_TeamMembers.erase(it_member);
					m_TeamMembers.insert(m_TeamMembers.begin(), pTMInfo);
					SetLeaderGUID( guid );
					break;
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队情况
		}
		break;
	default:
		Assert(FALSE);
		return;
	}
}

TeamMemberInfo* CTeamOrGroup::GetMember(GUID_t guid)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍情况
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if( (*it_member)->m_GUID == guid )
				{
					return (*it_member); // 返回队员信息
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队情况
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			BOOL bFind = FALSE;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					if( (*it_member)->m_GUID == guid )
					{
						return (*it_member); // 释放队员信息
					}
				}
			}
		}
		break;
	default:
		//assert(FALSE);
		return NULL;
	}

	return NULL;
}

// 根据server id 找到具体队友
TeamMemberInfo*	CTeamOrGroup::GetMemberByServerId(INT iServerId)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍情况
			std::vector< TeamMemberInfo* >::iterator it_member;

			for( it_member = m_TeamMembers.begin(); it_member != m_TeamMembers.end(); ++it_member )
			{
				if ( (*it_member)->m_OjbID == iServerId )
				{ 
					return (*it_member);
				}
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队情况
			std::vector< TeamInfo* >::iterator it_team;
			std::vector< TeamMemberInfo* >::iterator it_member;

			BOOL bFind = FALSE;

			for( it_team = m_TeamInfo.begin(); it_team != m_TeamInfo.end(); ++it_team )
			{
				for( it_member = (*it_team)->m_TeamMembers.begin(); it_member != (*it_team)->m_TeamMembers.end(); ++it_member )
				{
					if ( (*it_member)->m_OjbID == iServerId )
					{ 
						return (*it_member);
					}
				}
			}
		}
		break;
	default:
		//assert(FALSE);
		return NULL;
	}

	return NULL;
}

const TeamMemberInfo* CTeamOrGroup::GetMemberByIndex(UINT idx, BYTE TeamIndex)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍的情况
			if( m_TeamMembers.size() > idx )
			{
				return m_TeamMembers[idx];
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队的情况
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return NULL;
			}

			if( GetTeam(TeamIndex)->m_TeamMembers.size() > idx )
			{
				return GetTeam(TeamIndex)->m_TeamMembers[idx];
			}
		}
		break;
	default:
		Assert(FALSE);
		return NULL;
	}

	return NULL;
}

VOID CTeamOrGroup::SetUIFlag(UI_ON_OPEN flag)
{
	if ( flag == 0 )
	{
		// 刷新数据结构，关闭申请（邀请）界面的时候将一些过期数据清除
		switch(m_UIFlag)
		{
		case UI_INVITE_ON_OPEN:
			{
				while( m_InviteTeams.size() > MAX_INVITE_TEAM )
				{
					EraseInviteTeam(0);
				}
			}
			break;
		case UI_APPLY_ON_OPEN:
			{
				while( m_Proposers.size() > MAX_PROPOSER )
				{
					EraseProposer(0);
				}
			}
			break;
		default:
			Assert( FALSE );
			return;
		}
	}

	m_UIFlag = flag;
}

// 增加一个邀请队伍
BOOL CTeamOrGroup::AddInviteTeam(InviteTeam* pTeam)
{
	for( INT i=0; i<(INT)m_InviteTeams.size(); ++i )
	{
		if( m_InviteTeams[i]->m_InviterGUID == pTeam->m_InviterGUID )
		{ // 同样的申请人就不接受了
			return FALSE;
		}
	}

	m_InviteTeams.push_back(pTeam);

	if ( m_InviteTeams.size() > MAX_INVITE_TEAM )
	{ // 只有在邀请界面关闭时才做即时操作
		if ( m_UIFlag == UI_ALL_CLOSE )
		{
			EraseInviteTeam(0);
		}
	}

	return TRUE;
}

// 根据索引得到某个队伍
const InviteTeam* CTeamOrGroup::GetInviteTeamByIndex(INT idx)
{
	if ( idx<0 || idx>=(INT)m_InviteTeams.size() )
	{
		Assert( FALSE && "无法取出邀请对象。" );
		return NULL;
	}

	return m_InviteTeams[idx];
}

// 清除某个邀请队伍
VOID CTeamOrGroup::EraseInviteTeam(INT idx)
{
	InviteTeam* pInviteTeam;

	if ( idx<0 || idx>=(INT)m_InviteTeams.size() )
	{
		Assert( FALSE );
		return;
	}

	pInviteTeam = m_InviteTeams[idx];

	for( INT i=0; i<(INT)(pInviteTeam->m_InvitersInfo.size()); ++i )
	{ // 清除队员信息
		SAFE_DELETE( pInviteTeam->m_InvitersInfo[i] );
	}

	CInviteTeams::iterator it;

	pInviteTeam->m_InvitersInfo.clear();
	SAFE_DELETE( m_InviteTeams[idx] ); // 删除队伍信息
	it = m_InviteTeams.begin() + idx;
	m_InviteTeams.erase(it);
}

// 清除邀请队列
VOID CTeamOrGroup::ClearInviteTeam()
{
	for( INT i=(INT)(m_InviteTeams.size())-1; i>-1; --i )
	{
		EraseInviteTeam(i);
	}

	m_InviteTeams.clear();

	
	// 根据索引删除提示
	INT index = CDataPool::GetMe()->SystemTip_GetIndexByType( STT_TEAMINVITE, 2 );
	if( index != -1 )
		CDataPool::GetMe()->SystemTip_DelEventInfo( index - 1 );

	
}

// 增加一个申请人
BOOL CTeamOrGroup::AddProposer(TeamCacheInfo* pProposer)
{
	for( INT i=0; i<(INT)m_Proposers.size(); ++i )
	{
		if( m_Proposers[i]->m_GUID == pProposer->m_GUID )
		{ // 同样的申请人就不接受了
			return FALSE;
		}
	}

	m_Proposers.push_back(pProposer);

	if ( m_Proposers.size() > MAX_PROPOSER )
	{ // 只有在申请界面关闭时才做即时操作
		if ( m_UIFlag == UI_ALL_CLOSE )
		{
			EraseProposer(0);
		}
	}

	return TRUE;
}

// 清除一个申请者
VOID CTeamOrGroup::EraseProposer(INT idx)
{
	if ( idx<0 || idx>=(INT)m_Proposers.size() )
	{
		Assert( FALSE );
		return;
	}

	CProposers::iterator it;

	SAFE_DELETE( m_Proposers[idx] );
	it = m_Proposers.begin() + idx;
	m_Proposers.erase(it);
}

// 清除申请队列
VOID CTeamOrGroup::ClearProposer()
{
	for( INT i=(INT)(m_Proposers.size())-1; i>-1; --i )
	{
		EraseProposer(i);
	}

	m_Proposers.clear();

	// 根据索引删除提示
	INT index = CDataPool::GetMe()->SystemTip_GetIndexByType( STT_TEAMINVITE, 1 );
	if( index != -1 )
		CDataPool::GetMe()->SystemTip_DelEventInfo( index - 1 );
}

// 根据索引得到某个申请人
const TeamCacheInfo* CTeamOrGroup::GetProposerByIndex(INT idx)
{
	if ( idx<0 || idx>=(INT)m_Proposers.size() )
	{
		Assert( FALSE );
		return NULL;
	}

	return m_Proposers[idx];
}

VOID CTeamOrGroup::SetLeaderGUID(GUID_t guid) 
{ 
	if(guid == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
	{
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamLeaderFlag(TRUE);
	}
	else
	{
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_TeamLeaderFlag(FALSE);
	}

	m_LeaderID = guid; 
}

GUID_t	CTeamOrGroup::GetMemberGUIDByUIIndex(INT UIIndex, BYTE TeamIndex)
{
	const TeamMemberInfo*	pInfo = GetMemberByUIIndex(UIIndex, TeamIndex);
	if(pInfo)
	{
		return pInfo->m_GUID;
	}
	return -1;
}
// 通过界面索引, 得到server id
INT	CTeamOrGroup::GetMemberServerIdByUIIndex(INT UIIndex, BYTE TeamIndex)
{
	INT iServerId = -1;

	CTeamMembers* pTeamMembers;
	GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍的情况
			pTeamMembers = &m_TeamMembers;
		}
		break;
	case GROUP_TYPE:
		{ // 团队的情况
			if(TeamIndex<1 || TeamIndex>MAX_TEAMS_IN_GROUP)
			{
				Assert(FALSE);
				return -1;
			}

			if( m_MyTeamID != TeamIndex )
			{ // 暂时不考虑显示团队成员信息
				return -1;
			}

			pTeamMembers = &(GetTeam(TeamIndex)->m_TeamMembers);
		}
		break;
	default:
		Assert(FALSE);
		return -1;
	}

	std::vector< TeamMemberInfo* >::iterator it_member;
	INT i;

	for( it_member = pTeamMembers->begin(), i=1; it_member != pTeamMembers->end(); ++it_member, ++i )
	{
		if ( (*it_member)->m_GUID == myGUID )
		{ // 自己不占用序号
			--i;
		}

		if ( i == UIIndex )
		{
			return (*it_member)->m_OjbID;
		}
	}

	return -1;
}


// 是否在同一个场景中.
BOOL CTeamOrGroup::IsInScene(INT iIndex)
{
	switch(m_Type)
	{
	case TEAM_TYPE:
		{ // 队伍的情况
			if( (int)m_TeamMembers.size() > iIndex )
			{
				TeamMemberInfo* pInfo = m_TeamMembers[iIndex];
				if(pInfo)
				{
					if(pInfo->m_SceneID == CWorldManager::GetMe()->GetActiveSceneID())
					{
						return TRUE;
					}
				}
				else
				{
					return FALSE;
				}
					
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case GROUP_TYPE:
		{ // 团队的情况
			
			return FALSE;
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	return FALSE;

}

// 更新 Buff 列表
VOID CTeamOrGroup::UpdateImpactsList( INT ObjID, const SimpleImpactList* pSimpleImpactList )
{
	TeamMemberInfo* pTMInfo = GetMemberByServerId( ObjID );
	if( pTMInfo == NULL )
	{
		return;
	}

	pTMInfo->m_SimpleImpactList.SetImpactList( pSimpleImpactList );

	INT idx = GetMemberUIIndex( pTMInfo->m_GUID );
	if( idx != INVALID_ID )
	{
//		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
	}

}

// 增加 Buff
VOID CTeamOrGroup::AddImpact( INT ObjID, ImpactID_t ImpactID )
{
	TeamMemberInfo* pTMInfo = GetMemberByServerId( ObjID );
	if( pTMInfo == NULL )
	{
		return;
	}

	pTMInfo->m_SimpleImpactList.AddImpact( ImpactID );

	INT idx = GetMemberUIIndex( pTMInfo->m_GUID );
	if( idx != INVALID_ID )
	{
//		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
	}
}

// 减少 Buff
VOID CTeamOrGroup::RemoveImpact( INT ObjID, ImpactID_t ImpactID )
{
	TeamMemberInfo* pTMInfo = GetMemberByServerId( ObjID );
	if( pTMInfo == NULL )
	{
		return;
	}

	pTMInfo->m_SimpleImpactList.RemoveImpact( ImpactID );

	INT idx = GetMemberUIIndex( pTMInfo->m_GUID );
	if( idx != INVALID_ID )
	{
//		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_UPTEDATA_MEMBER_INFO, idx);
	}
}
