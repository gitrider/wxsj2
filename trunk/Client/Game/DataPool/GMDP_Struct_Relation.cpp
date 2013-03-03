
/** GMDP_Struct_Relation.cpp
 */

#include "StdAfx.h"
#include "GMDataPool.h"
#include "GMDP_Struct_Relation.h"
#include "GMDP_Struct_Mail.h"
#include "GMDP_CharacterData.h"
#include "Object\Manager\ObjectManager.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "Event/GMEventSystem.h"
#include "Procedure/GameProcedure.h"
#include "GIWorldSystem.h"
#include "DBC/GMDataBase.h"



VOID SDATA_RELATION_MEMBER::CleanUp()
{
	m_GUID = INVALID_ID;
	memset((void*)m_szName, 0, sizeof(m_szName));

	m_RelationType = RELATION_TYPE_NONE;
	m_nFriendPoint = 0;

	m_nLevel = 0;
	m_nMenPai = 9;
	m_nPortrait = -1;
	m_GuildID = INVALID_ID;
	memset((void*)m_szGuildName, 0, sizeof(m_szGuildName));
	m_bOnlineFlag = FALSE;
	sprintf( m_szMood, "" );
	memset((void*)m_szTitle, 0, sizeof(m_szTitle));
	m_SceneID = INVALID_ID;
	memset((void*)m_szRelation, 0, sizeof(m_szRelation));
	memset((void*)m_szLocation, 0, sizeof(m_szLocation));
	m_nTeamSize = 0;
	memset((void*)m_szTeamDesc, 0, sizeof(m_szTeamDesc));
	m_nEnterOrder = 0;
}

RelationList::~RelationList()
{
	CleanUp();
}


VOID RelationList::CleanUp()
{
	for(INT i = 0; i < (INT)m_vecRelationMember.size(); ++i)
	{
		SAFE_DELETE(m_vecRelationMember[i]);
	}

	m_vecRelationMember.clear();
}

// ��������
BOOL RelationList::Add( SDATA_RELATION_MEMBER* pMember )
{
	Assert(pMember);

	BOOL bAddedFlag = FALSE;

	if ( m_vecRelationMember.size() >= LIST_MEMBER_COUNT )
	{ // ����̫��
		return FALSE;
	}

	if ( !IsFitType( pMember ) )
	{ // ��Ӧ�÷ŵ�����������
		return FALSE;
	}

	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{ // �����ѶȱȽ�������
		INT nRet;

		nRet = Compare(pMember, m_vecRelationMember[i]);
		if ( nRet < 0 )
		{ // ���ȼ���
			continue;
		}
		else
		{
			m_vecRelationMember.insert(m_vecRelationMember.begin() + i, pMember);
			bAddedFlag = TRUE;
			break;
		}
	}

	if ( !bAddedFlag )
	{
		m_vecRelationMember.push_back(pMember);
	}

	return TRUE;
}

// ���������Ƴ���ֻ�Ƴ�ָ�룬���ͷ�����
BOOL RelationList::Remove( INT idx )
{
	if ( (INT)m_vecRelationMember.size() <= idx || idx < 0 )
	{
		Assert( idx );
		return FALSE;
	}

	m_vecRelationMember.erase( m_vecRelationMember.begin() + idx );

	return TRUE;
}

// ���������Ƴ���ֻ�Ƴ�ָ�룬���ͷ�����
BOOL RelationList::RemoveByGUID( GUID_t guid )
{
	INT idx;
	idx = GetMemberIndex( guid );

	if ( idx == -1 )
	{
		return FALSE;
	}

	return Remove( idx );
}

// ���������Ƴ������ͷ�����
BOOL RelationList::Erase( INT idx )
{
	if ( (INT)m_vecRelationMember.size() <= idx || idx < 0 )
	{
		Assert( idx );
		return FALSE;
	}

	SAFE_DELETE( m_vecRelationMember[idx] );
	m_vecRelationMember.erase( m_vecRelationMember.begin() + idx );

	return TRUE;
}

// ���������Ƴ������ͷ�����
BOOL RelationList::EraseByGUID( GUID_t guid )
{
	INT idx;
	idx = GetMemberIndex( guid );

	if ( idx == -1 )
	{
		return FALSE;
	}

	return Erase( idx );
}

// �õ���ǰ��������
INT RelationList::GetMemberCount()
{
	return (INT)m_vecRelationMember.size();
}

// �õ�һ����ϵ���ݣ������޸����ݣ�
const SDATA_RELATION_MEMBER* RelationList::GetMember( INT idx )
{
	if ( idx < 0 || idx >= (INT)m_vecRelationMember.size() )
	{
		Assert( FALSE );
		return NULL;
	}

	return m_vecRelationMember[idx];
}

// �õ�һ����ϵ���ݣ������޸����ݣ�
SDATA_RELATION_MEMBER* RelationList::GetMemberByGUID( GUID_t guid )
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( m_vecRelationMember[i]->m_GUID == guid )
		{
			return m_vecRelationMember[i];
		}
	}

	return NULL;
}

// ͨ�����ֵõ�һ����ϵ���ݣ������޸����ݣ�
SDATA_RELATION_MEMBER* RelationList::GetMemberByName( const CHAR* szName )
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( strcmp(m_vecRelationMember[i]->m_szName, szName) == 0 )
		{
			return m_vecRelationMember[i];
		}
	}

	return NULL;
}

// ͨ�����ֵõ�һ����ϵ���ݣ������޸����ݣ�
INT RelationList::GetMemberIndexByName( const CHAR* szName )
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( strcmp(m_vecRelationMember[i]->m_szName, szName) == 0 )
		{
			return i;
		}
	}

	return -1;
}

// �жϵ�ǰ�����Ƿ�����
BOOL RelationList::IsFull()
{
	return (m_vecRelationMember.size() >= LIST_MEMBER_COUNT);
}

// �õ�ĳ�� GUID �������е�λ�ã���������ڣ��򷵻� -1
INT RelationList::GetMemberIndex(GUID_t guid)
{
	for( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		if ( m_vecRelationMember[i]->m_GUID == guid )
		{
			return i;
		}
	}

	return -1;
}

// ����ֵ��CSTR_LESS_THAN, CSTR_EQUAL, CSTR_GREATER_THAN,
// ����ʱ���� 0
//INT RelationList::CompareByName(const CHAR* szName1, const CHAR* szName2)
//{
//	Assert(szName1);
//	Assert(szName2);
//
//	return CompareString(LOCALE_SYSTEM_DEFAULT, 0,
//		szName1, (int)strlen(szName1),
//		szName2, (int)strlen(szName2) );
//}

// �Ƚ�������ϵ�˵������ŷ�˳��<0��ʾ���ȼ��ͣ�0��ʾ��ȣ�>0��ʾ���ȼ��ߣ�
INT RelationList::Compare(const SDATA_RELATION_MEMBER* pMember1, const SDATA_RELATION_MEMBER* pMember2)
{
	//INT nRet;

	//nRet = CompareByName( pMember1->m_szName, pMember2->m_szName );

	//switch(nRet)
	//{ // ����Խ�����ȼ�Խ��
	//case CSTR_LESS_THAN:
	//	return 1;
	//case CSTR_EQUAL:
	//	return 0;
	//case CSTR_GREATER_THAN:
	//	return -1;
	//default:
	//	AssertEx( nRet, "Error occurred when compare name." );
	//	return -1;
	//}

	Assert( pMember1 );
	Assert( pMember2 );

	INT nPriority1, nPriority2;

	nPriority1 = CalcPriority( pMember1 );
	nPriority2 = CalcPriority( pMember2 );

	if( nPriority1 > nPriority2 )
	{
		return 1;
	}
	else if( nPriority1 < nPriority2 )
	{
		return -1;
	}
	else
	{
		if ( pMember1->m_nFriendPoint > pMember2->m_nFriendPoint )
		{
			return 1;
		}
		else if ( pMember1->m_nFriendPoint < pMember2->m_nFriendPoint )
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
}

// CGameProcedure::s_pTimeSystem->GetTimeNow()


// ����һ�����ѵ����ȼ�����������Ŀǰ������ Compare ʹ�ã����Է���ֵ������
INT RelationList::CalcPriority( const SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	//if( pMember->m_RelationType == RELATION_TYPE_MARRY )
	//{
	//	return 10;
	//}
	//else if( pMember->m_RelationType == RELATION_TYPE_BROTHER )
	//{
	//	return 9;
	//}
	//else if( pMember->m_nFriendPoint >= FRIEND_POINT_ENOUGH_NOTIFY )
	//{ // ���ܺ���
	//	if( pMember->m_bOnlineFlag )
	//	{ // �������
	//		return 8;
	//	}
	//	else
	//	{
	//		return 7;
	//	}
	//}
	//else
	//{
	//	return 6;
	//}

	// ����������ֻͨ���Ƿ��������Ƚ�
	if( pMember->m_bOnlineFlag )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// ���������б����ں����б��е���ʾ����(ͨ�������߹�ϵ�ͺ��Ѷ�����������)
BOOL RelationList::AfreshArray()
{
	if ( m_vecRelationMember.empty())
	{
		return FALSE;
	}

	INT nRet = 0;
	for ( INT i=0; i<(INT)m_vecRelationMember.size(); ++i )
	{
		for ( INT j=i+1; j<(INT)m_vecRelationMember.size(); ++j )
		{
			nRet = Compare( m_vecRelationMember[i], m_vecRelationMember[j] );
			if ( nRet > 0 )
			{
				continue;
			}
			else
			{
				// ����ij�е�����
				SDATA_RELATION_MEMBER *pTemMember = m_vecRelationMember[i];
				m_vecRelationMember[i] = m_vecRelationMember[j];
				m_vecRelationMember[j] = pTemMember;
			}
		}
	}

	return TRUE;
}




// �ж��Ƿ�������Ҫ������
BOOL FriendList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	return (pMember->m_RelationType == RELATION_TYPE_FRIEND)
		|| (pMember->m_RelationType == RELATION_TYPE_BROTHER)
		|| (pMember->m_RelationType == RELATION_TYPE_MARRY)
		|| (pMember->m_RelationType == RELATION_TYPE_MASTER)
		|| (pMember->m_RelationType == RELATION_TYPE_PRENTICE);
}


// ��������
BOOL TempRelationList::Add( SDATA_RELATION_MEMBER* pMember )
{
	if ( m_vecRelationMember.size() < 1 )
	{ // ��ʼ����� Order
		m_nMaxOrder = 0;
	}

	while ( m_vecRelationMember.size() >= LIST_MEMBER_COUNT )
	{ // ����̫��
		INT nSize;

		nSize = (INT)m_vecRelationMember.size();
		for( INT i=0; i<nSize; ++i )
		{
			if ( m_vecRelationMember[i]->m_nEnterOrder == 1 )
			{
				m_vecRelationMember.erase(m_vecRelationMember.begin()+i);
				--m_nMaxOrder;
			}
			else
			{
				--(m_vecRelationMember[i]->m_nEnterOrder);
			}
		}
	}

	pMember->m_nEnterOrder = ++m_nMaxOrder; // ������С Order Ϊ 1

	return RelationList::Add( pMember );
}

// �ж��Ƿ�������Ҫ������
BOOL TempRelationList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );
	return (pMember->m_RelationType == RELATION_TYPE_TEMPFRIEND);
}

// �ж��Ƿ�������Ҫ������
BOOL BlackList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );
	return (pMember->m_RelationType == RELATION_TYPE_BLACKNAME);
}

// �ж��Ƿ�������Ҫ������
BOOL EnemyList::IsFitType( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );
	return (pMember->m_RelationType == RELATION_TYPE_ENEMY);
}



// �ӷ������˽���һ������
// ���˺��Ѹ��� _FRIEND_INFO �е���Ϣ����Ӧ������һ������
// �����ں��Ѻͺ��������������Ժ�ͳһ��ˢ�½�����¼�����ȥ
BOOL Relation::AddRelation( const _FRIEND_INFO* pFriend )
{
	Assert( pFriend );

	RelationList* pRelationList;

	switch( pFriend->m_uGroup )
	{
	case RELATION_GROUP_F1:
	case RELATION_GROUP_F2:
	case RELATION_GROUP_F3:
	case RELATION_GROUP_F4:
	case RELATION_GROUP_F5:
	case RELATION_GROUP_FRIEND_ALL:	//+
		pRelationList = GetRelationList( (RELATION_GROUP)pFriend->m_uGroup );
		break;

	default:
		Assert( FALSE );
		return FALSE;
	}

	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pNewRelation;
	pNewRelation = pRelationList->GetMemberByGUID( pFriend->m_GUID );

	if( pNewRelation != NULL )
	{
		strncpy( pNewRelation->m_szName, pFriend->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = (RELATION_TYPE)pFriend->m_uRelationType;
		pNewRelation->m_nFriendPoint = pFriend->m_nFriendpoint;
		
		return TRUE;
	}
	else
	{
		pNewRelation = new SDATA_RELATION_MEMBER;

		if( pNewRelation == NULL )
		{
			Assert( pNewRelation );
			return FALSE;
		}

		pNewRelation->m_GUID = pFriend->m_GUID;
		strncpy( pNewRelation->m_szName, pFriend->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = (RELATION_TYPE)pFriend->m_uRelationType;
		pNewRelation->m_nFriendPoint = pFriend->m_nFriendpoint;
		
		return pRelationList->Add(pNewRelation);
	}
}

// �ӷ������˽���һ�����������
// ������Ϣ�ں�����������һ�������Ϣ
// �����ں��Ѻͺ��������������Ժ�ͳһ��ˢ�½�����¼�����ȥ
BOOL Relation::AddRelation( const _BLACKNAME_INFO* pBlackName )
{
	Assert( pBlackName );

	RelationList* pRelationList;

	pRelationList = GetRelationList(RELATION_GROUP_BLACK);
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pNewRelation;
	pNewRelation = pRelationList->GetMemberByGUID( pBlackName->m_GUID );

	if( pNewRelation != NULL )
	{
		strncpy( pNewRelation->m_szName, pBlackName->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = RELATION_TYPE_BLACKNAME;
		return TRUE;
	}
	else
	{
		pNewRelation = new SDATA_RELATION_MEMBER;

		if( pNewRelation == NULL )
		{
			Assert( pNewRelation );
			return FALSE;
		}

		pNewRelation->m_GUID = pBlackName->m_GUID;
		strncpy( pNewRelation->m_szName, pBlackName->m_szTargetName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = RELATION_TYPE_BLACKNAME;
		return pRelationList->Add(pNewRelation);
	}
}

// ����һ������
BOOL Relation::AddRelation( const _ENEMY_INFO* pEnemyName )
{
	Assert( pEnemyName );

	RelationList* pRelationList;

	pRelationList = GetRelationList(RELATION_GROUP_ENEMY);
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pNewRelation;
	pNewRelation = pRelationList->GetMemberByGUID( pEnemyName->m_GUID );

	if( pNewRelation != NULL )
	{
		strncpy( pNewRelation->m_szName, pEnemyName->m_szEnemyName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = RELATION_TYPE_ENEMY;
		return TRUE;
	}
	else
	{
		pNewRelation = new SDATA_RELATION_MEMBER;

		if( pNewRelation == NULL )
		{
			Assert( pNewRelation );
			return FALSE;
		}

		pNewRelation->m_GUID = pEnemyName->m_GUID;
		strncpy( pNewRelation->m_szName, pEnemyName->m_szEnemyName, sizeof(pNewRelation->m_szName) - 1 );
		pNewRelation->m_RelationType = RELATION_TYPE_ENEMY;
		
		return pRelationList->Add(pNewRelation);
	}
}



// ���һ������������
INT Relation::GetListCount( RELATION_GROUP RelationGroup )
{
	RelationList* pRelationList;

	pRelationList = GetRelationList(RelationGroup);
	if ( pRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationList->GetMemberCount();
}

// �ͻ��˻�ù�ϵ����Ϣ��������ʹ��
const SDATA_RELATION_MEMBER* Relation::GetRelationInfo( RELATION_GROUP RelationGroup, INT idx )
{
	RelationList* pRelationList;

	pRelationList = GetRelationList(RelationGroup);
	if ( pRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationList->GetMember( idx );
}

// �ͻ��˼���һ����ϵ��ͨ�����ڼ�����ʱ���ѣ�����ʱ��Ҫָ����Ҫ�������
BOOL Relation::AddRelation( RELATION_GROUP RelationGroup, SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	RelationList* pRelationList;

	pRelationList = GetRelationList(RelationGroup);
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	// �����ظ��Ӻ��ѣ����ܼ��Լ�Ϊ����
	if( pMember->m_GUID != INVALID_ID )
	{
		if( CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == pMember->m_GUID )
		{
			return FALSE;
		}

		// ������ֻ���ӵ��Ƿ����Լ�
		/*RELATION_GROUP rg;
		INT idx;

		if( GetRelationPosition( pMember->m_GUID, rg, idx ) != RELATION_TYPE_STRANGER )
		{
			return FALSE;
		}
		else if( pRelationList->GetMemberByGUID(pMember->m_GUID) != NULL )
		{
			return FALSE;
		}*/
	}
	else if( strlen(pMember->m_szName) > 0 )
	{
		std::string str1 = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name();
		std::string str2 = pMember->m_szName;
		if( str1 == str2 )
		{
			return FALSE;
		}

		// ������ֻ���ӵ��Ƿ����Լ�
		/*INT nrg, idx;

		if( GetRelationByName(pMember->m_szName, nrg, idx ) != RELATION_TYPE_STRANGER )
		{
			return FALSE;
		}
		else if( pRelationList->GetMemberByName(pMember->m_szName) != NULL )
		{
			return FALSE;
		}*/
	}

	SDATA_RELATION_MEMBER* pNewRelation = new SDATA_RELATION_MEMBER;
	if( pNewRelation == NULL )
	{
		Assert( pNewRelation );
		return FALSE;
	}

	pNewRelation->m_GUID = pMember->m_GUID;
	strncpy( pNewRelation->m_szName, pMember->m_szName, sizeof(pNewRelation->m_szName) - 1 );
	pNewRelation->m_RelationType = pMember->m_RelationType;
	pNewRelation->m_nFriendPoint = pMember->m_nFriendPoint;
	pNewRelation->m_nLevel = pMember->m_nLevel;
	pNewRelation->m_nMenPai = pMember->m_nMenPai;
	pNewRelation->m_nPortrait = pMember->m_nPortrait;
	pNewRelation->m_GuildID = pMember->m_GuildID;
	strncpy( pNewRelation->m_szGuildName, pMember->m_szGuildName, sizeof(pNewRelation->m_szGuildName) - 1 );
	pNewRelation->m_bOnlineFlag = pMember->m_bOnlineFlag;
	SetRelationDesc( pNewRelation );

	if( pMember->m_bOnlineFlag )
	{
		strncpy( pNewRelation->m_szMood, pMember->m_szMood, sizeof(pNewRelation->m_szMood) - 1 );
		strncpy( pNewRelation->m_szTitle,  TransTitleStr(pMember->m_szTitle), sizeof(pNewRelation->m_szTitle) - 1 );
		pNewRelation->m_SceneID = pMember->m_SceneID;
		GetLocationName( pNewRelation->m_SceneID, pNewRelation->m_szLocation );
		pNewRelation->m_nTeamSize = pMember->m_nTeamSize;
		SetTeamDesc( pNewRelation );
	}
	else
	{
		strcpy( pNewRelation->m_szLocation, "����" );
	}

	pRelationList->Add(pNewRelation);
	UpdateUIList( RelationGroup );

	return TRUE;
}

// �������˷���Ϣ�����¹�ϵ����Ϣ
BOOL Relation::UpdateRelationInfo( RETURN_RELATION_INFO* pRelationInfo )
{
	Assert( pRelationInfo );

	RelationList* pRelationList;
	RELATION_GROUP grp;

	switch( pRelationInfo->GetRelationType() )
	{
	case RELATION_TYPE_BLACKNAME:
		grp = RELATION_GROUP_BLACK;
		break;
	case RELATION_TYPE_TEMPFRIEND:
		grp = RELATION_GROUP_TEMPFRIEND;
		break;
	case RELATION_TYPE_ENEMY:
		grp = RELATION_GROUP_ENEMY;
		break;
	default:
		grp = (RELATION_GROUP)pRelationInfo->GetGroup();
	}

	_RELATION* pRelationData;
	pRelationData = pRelationInfo->GetRelationData();

	pRelationList = GetRelationList( grp );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	GUID_t guid = pRelationData->GetGUID();
	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByGUID( guid );
	if( pMember == NULL )
	{
		pMember = pRelationList->GetMemberByName( pRelationData->GetName() );
	}
	if( pMember == NULL )
	{ // İ���ˣ��鿴���
		m_ViewPlayer.m_GUID = pRelationData->GetGUID();
		strncpy( m_ViewPlayer.m_szName, pRelationData->GetName(), sizeof(m_ViewPlayer.m_szName) - 1 );
		m_ViewPlayer.m_RelationType = (RELATION_TYPE)pRelationInfo->GetRelationType();
		m_ViewPlayer.m_nFriendPoint = pRelationInfo->GetFriendPoint();
		m_ViewPlayer.m_nLevel = pRelationData->GetLevel();
		m_ViewPlayer.m_nMenPai = pRelationData->GetMenPai();
		m_ViewPlayer.m_nPortrait = pRelationData->GetPortrait();
		m_ViewPlayer.m_GuildID = pRelationData->GetGuildID();
		strncpy( m_ViewPlayer.m_szGuildName, pRelationData->GetGuildName(), sizeof(m_ViewPlayer.m_szGuildName) - 1 );
		m_ViewPlayer.m_bOnlineFlag = (BOOL)pRelationData->GetOnlineFlag();
		//		SetRelationDesc( &m_ViewPlayer );

		if( m_ViewPlayer.m_bOnlineFlag )
		{
			strncpy( m_ViewPlayer.m_szMood, pRelationData->GetMood(), sizeof(m_ViewPlayer.m_szMood) - 1 );
			strncpy( m_ViewPlayer.m_szTitle,  TransTitleStr(pRelationData->GetTitle()), sizeof(m_ViewPlayer.m_szTitle) - 1 );
			m_ViewPlayer.m_SceneID = pRelationData->GetSceneID();
			GetLocationName( m_ViewPlayer.m_SceneID, m_ViewPlayer.m_szLocation );
			m_ViewPlayer.m_nTeamSize = pRelationData->GetTeamSize();
			SetTeamDesc( &m_ViewPlayer );
		}

		// TODO: Push_Event here
		CEventSystem::GetMe()->PushEvent( GE_CHAT_SHOWUSERINFO);
		return TRUE;
	}

	pMember->m_GUID = pRelationData->GetGUID();
	strncpy( pMember->m_szName, pRelationData->GetName(), sizeof(pMember->m_szName) - 1 );
	pMember->m_RelationType = (RELATION_TYPE)pRelationInfo->GetRelationType();
	pMember->m_nFriendPoint = pRelationInfo->GetFriendPoint();
	pMember->m_nLevel = pRelationData->GetLevel();
	pMember->m_nMenPai = pRelationData->GetMenPai();
	pMember->m_nPortrait = pRelationData->GetPortrait();
	pMember->m_GuildID = pRelationData->GetGuildID();
	strncpy( pMember->m_szGuildName, pRelationData->GetGuildName(), sizeof(pMember->m_szGuildName) - 1 );
	pMember->m_bOnlineFlag = (BOOL)pRelationData->GetOnlineFlag();
	SetRelationDesc( pMember );

	if( pMember->m_bOnlineFlag )
	{
		strncpy( pMember->m_szMood, pRelationData->GetMood(), sizeof(pMember->m_szMood) - 1 );
		strncpy( pMember->m_szTitle,  TransTitleStr(pRelationData->GetTitle()), sizeof(pMember->m_szTitle) - 1 );
		pMember->m_SceneID = pRelationData->GetSceneID();
		GetLocationName( pMember->m_SceneID, pMember->m_szLocation );
		pMember->m_nTeamSize = pRelationData->GetTeamSize();
		SetTeamDesc( pMember );
	}
	else
	{
		strcpy( pMember->m_szLocation, "����" );
	}

	UpdateUIInfo( grp, pRelationList->GetMemberIndex(guid) );
	return TRUE;
}

// �ͻ����Ƴ�һ����ϵ��ͨ�������Ƴ���ʱ����
BOOL Relation::RemoveRelation( RELATION_GROUP RelationGroup, INT idx )
{
	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	if( idx < 0 || idx >= pRelationList->GetMemberCount() )
	{
		Assert( FALSE );
		return FALSE;
	}

	pRelationList->Erase( idx );
	UpdateUIList( RelationGroup );

	return TRUE;
}

// �ͻ����Ƴ�һ����ϵ��ͨ�������Ƴ���ʱ����
BOOL Relation::RemoveRelation( RELATION_GROUP RelationGroup, const CHAR* szName )
{
	Assert(szName);

	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	INT nIndex;
	nIndex = pRelationList->GetMemberIndexByName(szName);
	if( nIndex >= 0 )
	{
		RemoveRelation( RelationGroup, nIndex );
		return TRUE;
	}

	return FALSE;
}

// �ӷ��������յ�һ���Ƴ���ϵ�˵���Ϣ
BOOL Relation::RemoveRelation( GUID_t guid )
{
	RELATION_GROUP RelationGroup;
	INT idx;

	if( GetRelationPosition( guid, RelationGroup, idx ) != RELATION_TYPE_STRANGER )
	{
		return RemoveRelation( RelationGroup, idx );
	}

	return FALSE;
}

// �ƶ�һ����ϵ��Srg ��ԭ�����飬guid ��Ҫ�ƶ������ GUID��Drg ���ƶ������
BOOL Relation::MoveRelation( RELATION_GROUP Srg, RELATION_TYPE Dtype, RELATION_GROUP Drg, GUID_t guid )
{
	RelationList* pSRelationList;

	pSRelationList = GetRelationList(Srg);
	if ( pSRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	RelationList* pDRelationList;

	pDRelationList = GetRelationList(Drg);
	if ( pDRelationList == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pSRelationList->GetMemberByGUID( guid );
	if ( pMember == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	if ( pSRelationList->RemoveByGUID( guid ) )
	{
		UpdateUIList( Srg );

		// �޸Ĺ�ϵ��Ϣ
		pMember->m_RelationType = Dtype;

		if ( pDRelationList->Add( pMember ) )
		{
			UpdateUIList( Drg );
			return TRUE;
		}
		else
		{
			SAFE_DELETE( pMember );
			Assert(FALSE);
			return FALSE;
		}
	}

	return FALSE;
}

// ֪ͨ UI ˢ���б�Ĭ�ϸ��������б�
VOID Relation::UpdateUIList( RELATION_GROUP RelationGroup )
{
	CEventSystem::GetMe()->PushEvent( GE_UPDATE_FRIEND );
}

// ֪ͨ UI ˢ�� RelationGroup �е� idx �Ź�ϵ�˵���ϸ��Ϣ
VOID Relation::UpdateUIInfo( RELATION_GROUP RelationGroup, INT idx )
{
	CEventSystem::GetMe()->PushEvent( GE_UPDATE_FRIEND_INFO, int( RelationGroup ), idx );
}

// �õ�һ�� GUID ���ڵ����Լ��������������û���ҵ����� RELATION_TYPE_STRANGER��������ܲ�������ʱ����
RELATION_TYPE Relation::GetRelationPosition( GUID_t guid, RELATION_GROUP& RelationGroup, INT& idx )
{
	RelationList* pRelationList;

	for( INT i = (INT)RELATION_GROUP_FRIEND_ALL; i < (INT)RELATION_GROUP_NUMBER; ++i )	//+
	{
//		if( RELATION_GROUP_F1 == i || 
//			RELATION_GROUP_F2 == i || 
//			RELATION_GROUP_F3 == i || 
//			RELATION_GROUP_F4 == i ||
//			RELATION_GROUP_F5 == i)
//			continue;

		pRelationList = GetRelationList( (RELATION_GROUP)i );
		if( pRelationList == NULL )
		{
			Assert( pRelationList );
			return RELATION_TYPE_STRANGER;
		}

		idx = pRelationList->GetMemberIndex(guid);
		if( idx != -1 )
		{
			RelationGroup = (RELATION_GROUP)i;
			return pRelationList->GetMember(idx)->m_RelationType;
		}
	}

	return RELATION_TYPE_STRANGER;
}

RELATION_TYPE Relation::GetRelationByName( const CHAR* szName, INT& RelationGroup, INT& idx )
{
	RelationList* pRelationList;

	for( INT i = (INT)RELATION_GROUP_FRIEND_ALL; i < (INT)RELATION_GROUP_NUMBER; ++i )	//+
	{
//		if( RELATION_GROUP_F1 == i || 
//			RELATION_GROUP_F2 == i || 
//			RELATION_GROUP_F3 == i || 
//			RELATION_GROUP_F4 == i )
//			continue;

		pRelationList = GetRelationList( (RELATION_GROUP)i );
		if( pRelationList == NULL )
		{
			Assert( pRelationList );
			return RELATION_TYPE_STRANGER;
		}

		idx = pRelationList->GetMemberIndexByName( szName );
		if( idx != -1 )
		{
			RelationGroup = i;
			return pRelationList->GetMember(idx)->m_RelationType;
		}
	}


	//�ٴε�������һ����ʱ�����б�
	pRelationList = GetRelationList( (RELATION_GROUP)(RELATION_GROUP_NUMBER+1) );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return RELATION_TYPE_STRANGER;
	}

	idx = pRelationList->GetMemberIndexByName( szName );
	if( idx != -1 )
	{
		RelationGroup = (RELATION_GROUP_NUMBER+1);
		return pRelationList->GetMember(idx)->m_RelationType;
	}

	return RELATION_TYPE_STRANGER;
}

// �������ߺ���
BOOL Relation::UpdateOnlineFriend( const _RELATION_ONLINE* pOnlineRelation )
{
	Assert( pOnlineRelation );

	RELATION_GROUP RelationGroup;
	INT idx;
	GUID_t guid;

	guid = pOnlineRelation->GetGUID();
	if( GetRelationPosition( guid, RelationGroup, idx ) == RELATION_TYPE_STRANGER )
	{ // ���ݳػ�Ϊ��
		return TRUE;
	}

	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByGUID( guid );
	if( pMember == NULL )
	{
		Assert( pMember );
		return FALSE;
	}

	pMember->m_bOnlineFlag = TRUE;
	strncpy( pMember->m_szMood, pOnlineRelation->GetMood(), sizeof(pMember->m_szMood) - 1 );

	UpdateUIList( RelationGroup ); // ˢ�¹�ϵ������״̬
	
	return TRUE;
}

// ��ϵ������
BOOL Relation::RelationOnline(const CHAR* szName, const CHAR* szMood)
{
	INT nGroup;
	INT idx;

	Assert(szName);
	Assert(szMood);

	if( GetRelationByName( szName, nGroup, idx ) == RELATION_TYPE_STRANGER )
	{ // ���ݳػ�Ϊ��
		return TRUE;
	}

	RELATION_GROUP RelationGroup;
	RelationList* pRelationList;

	RelationGroup = (RELATION_GROUP)nGroup;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByName( szName );
	if( pMember == NULL )
	{
		Assert( pMember );
		return FALSE;
	}
	
	pMember->m_bOnlineFlag = TRUE;
	strncpy( pMember->m_szMood, szMood, sizeof(pMember->m_szMood) - 1 );

	// ������Ѷȳ���100 ������
	if (pMember->m_nFriendPoint >= FRIEND_POINT_ENOUGH_NOTIFY )
	{
		if(szName)
		{
			STRING strTemp = "";
			strTemp = COLORMSGFUNC("GMDP_Struct_Relation_Info_EnterGame", szName);
			ADDTALKMSG(strTemp);
		}
	}

	// ����Ǻ����б���Ժ����б����½�������
	if ( RelationGroup == RELATION_GROUP_FRIEND_ALL )
	{
		pRelationList->AfreshArray();
	}
	
	UpdateUIList( RelationGroup ); // ˢ�¹�ϵ������״̬
	return TRUE;
}

// ��ϵ������
BOOL Relation::RelationOffLine(GUID_t guid)
{
	RELATION_GROUP RelationGroup;
	INT idx;

	GetRelationPosition( guid, RelationGroup, idx );

	RelationList* pRelationList;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return FALSE;
	}

	SDATA_RELATION_MEMBER* pMember;
	pMember = pRelationList->GetMemberByGUID( guid );
	if( pMember == NULL )
	{
		Assert( pMember );
		return FALSE;
	}
	
	pMember->m_bOnlineFlag = FALSE;
	strncpy( pMember->m_szMood, "", sizeof(pMember->m_szMood) - 1 );

	// ������Ѷȳ���100 �������������
	if( pMember->m_nFriendPoint >= FRIEND_POINT_ENOUGH_NOTIFY )
	{
		STRING strTemp = "";
		strTemp = COLORMSGFUNC("GMDP_Struct_Relation_Info_ExitrGame", pMember->m_szName);
		//_snprintf(szText, _MAX_PATH, "%s�뿪��Ϸ�ˡ�", pMember->m_szName);
		ADDTALKMSG(strTemp);
	}

	// ����Ǻ����б���Ժ����б����½�������
	if ( RelationGroup == RELATION_GROUP_FRIEND_ALL )
	{
		pRelationList->AfreshArray();
	}

	UpdateUIList( RelationGroup ); // ˢ�¹�ϵ������״̬
	return TRUE;
}

// �ж��Ƿ����Լ���������
BOOL Relation::IsBlackName( const CHAR* szName )
{
	Assert( szName );

	for( INT i=0; i<m_BlackListGroup.GetMemberCount(); ++i )
	{
		if ( strncmp( m_BlackListGroup.GetMember(i)->m_szName, szName, strlen(szName) ) == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// �ж��Ƿ����Լ��ĵ�����
BOOL Relation::IsEnemyName( const CHAR* szName )
{
	Assert( szName );

	for( INT i=0; i<m_EnemyListGroup.GetMemberCount(); ++i )
	{
		if ( strncmp( m_EnemyListGroup.GetMember(i)->m_szName, szName, strlen(szName) ) == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// �ж��Ƿ����Լ��ĺ�����
BOOL Relation::IsFriendName( const CHAR* szName )
{
	Assert( szName );
	for( INT i=0; i<m_FriendGroup.GetMemberCount(); ++i )
	{
		if ( strncmp( m_FriendGroup.GetMember(i)->m_szName, szName,strlen(szName)) == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// �ж��Ƿ����Լ��������ϵ����
BOOL Relation::IsTempFriendName( const CHAR* szName )
{
	Assert( szName );
	for ( INT i=0; i<m_TeamFriendGroup.GetMemberCount(); ++i)
	{
		if ( strncmp( m_TeamFriendGroup.GetMember(i)->m_szName, szName,strlen(szName)) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}


// ���Ӻ��Ѷ�
VOID Relation::AddFriendPoint( const CHAR* szName, INT nPoint )
{
	Assert( szName );
	if ( nPoint == 0 )
		return;

	SDATA_RELATION_MEMBER* pNewRelation;
	RelationList* pRelationList;

	RELATION_GROUP Srg = RELATION_GROUP_TEMPFRIEND;
	INT idx = 0;

	// ͨ�������ں����б����ҵ�����

	/*for ( INT i=0; i<m_FriendGroup.GetMemberCount(); ++i )
	{
		if ( strncmp( m_FriendGroup.GetMember(i)->m_szName, szName,strlen(szName)) == 0 )
		{
			pNewRelation = m_FriendGroup.GetMember(i);
			break;
		}
	}*/

	pNewRelation = m_FriendGroup.GetMemberByName( szName );

	if( !pNewRelation )
		return;
	pNewRelation->m_nFriendPoint += nPoint;

	// ���⻹Ҫ��������ĵ�Ŀ���ǽ�Ҫͨ����������UI
	GetRelationPosition( pNewRelation->m_GUID, Srg, idx);

	UpdateUIList(Srg);
	UpdateUIInfo(Srg,idx);

	return;
}

// ��ĳ����������һ���ʼ����ʼ����е������ţ�
BOOL Relation::AddMail( const CHAR* szName, UINT uIndex )
{
	INT aInt[] = {	RELATION_GROUP_FRIEND_ALL, 
					RELATION_GROUP_F1,
					RELATION_GROUP_F2, 
					RELATION_GROUP_F3, 
					RELATION_GROUP_F4,
					RELATION_GROUP_F5,
					RELATION_GROUP_TEMPFRIEND };	//+

	RelationList* pRelationList = NULL;
	SDATA_RELATION_MEMBER* pRelationMember = NULL;

	for( INT i = 0; i < sizeof(aInt)/sizeof(INT); ++i )
	{
		pRelationList = GetRelationList( (RELATION_GROUP)aInt[i] );
		if( pRelationList == NULL )
		{
			Assert( pRelationList );
			return FALSE;
		}

		pRelationMember = pRelationList->GetMemberByName(szName);
		if( pRelationMember != NULL )
		{
			break;
		}
	}

	if( pRelationMember == NULL )
	{ // û���ҵ��������
		return FALSE;
	}

	pRelationMember->m_vMailList.push_back(uIndex);
	return TRUE;
}

// �õ�ĳ�����ѵ���ʷ�ʼ�����
UINT Relation::GetMailCount( RELATION_GROUP RelationGroup, INT idx )
{
	const SDATA_RELATION_MEMBER* pRelationMember;

	pRelationMember = GetRelationInfo( RelationGroup, idx );
	if( pRelationMember == NULL )
	{
		Assert(pRelationMember);
		return 0;
	}

	return (UINT)pRelationMember->m_vMailList.size();
}

// �õ�ĳ�����ѵĵ� uIndex ����ʷ�ʼ����ʼ���������
UINT Relation::GetMailIndex( RELATION_GROUP RelationGroup, INT idx, UINT uIndex )
{
	const SDATA_RELATION_MEMBER* pRelationMember;

	pRelationMember = GetRelationInfo( RelationGroup, idx );
	if( pRelationMember == NULL )
	{
		Assert(pRelationMember);
		return 0;
	}

	if( pRelationMember->m_vMailList.size() <= uIndex )
	{
		Assert( pRelationMember->m_vMailList.size() > uIndex );
		return 0;
	}

	return pRelationMember->m_vMailList[uIndex];
}

// ��ĳ������ʱ�������ؽ���ʷ��Ϣ
VOID Relation::RebuildMailHistory( RELATION_GROUP RelationGroup, const CHAR* szName )
{
	RelationList* pRelationList = NULL;
	SDATA_RELATION_MEMBER* pRelationMember = NULL;

	pRelationList = GetRelationList( RelationGroup );
	if( pRelationList == NULL )
	{
		Assert( pRelationList );
		return;
	}

	pRelationMember = pRelationList->GetMemberByName(szName);
	if( pRelationMember == NULL )
	{ // û���ҵ��������
		return;
	}

	CMailPool* pMailPool = CDataPool::GetMe()->GetMailPool();

	for( INT i=0; i<pMailPool->GetMailCount(); ++i )
	{
		const SMail* pMail;

		pMail = pMailPool->GetMail(i);
		if( pMail != NULL )
		{
			if( strcmp(pMail->m_szSourName, pRelationMember->m_szName) == 0
			 || strcmp(pMail->m_szDestName, pRelationMember->m_szName) == 0
			 )
			{
				pRelationMember->m_vMailList.push_back(i);
			}
		}
	}
}

// �õ�����
const CHAR* Relation::GetMood( )
{
	return m_szMood;
}

// ��������
VOID Relation::SetMood( const CHAR* szMood )
{
	if( szMood == NULL )
	{
		Assert( szMood );
		return;
	}

	strncpy( m_szMood, szMood, sizeof(m_szMood)-1 );
	CEventSystem::GetMe()->PushEvent( GE_MOOD_CHANGE );

}

// �鿴��ҽ������Ϣ
const SDATA_RELATION_MEMBER* Relation::GetPlayerCheckInfo( )
{
	return &m_ViewPlayer;
}

// ���һ������
RelationList* Relation::GetRelationList( RELATION_GROUP RelationGroup )
{
	switch( RelationGroup )
	{
	// ������
	case RELATION_GROUP_FRIEND_ALL:	//+
		{
			return &m_FriendGroup;
		}
		break;
	// �û��Զ����5��
	case RELATION_GROUP_F1:
		{
			return &m_FriendGroup1;
		}
		break;
	case RELATION_GROUP_F2:
		{
			return &m_FriendGroup2;
		}
		break;
	case RELATION_GROUP_F3:
		{
			return &m_FriendGroup3;
		}
		break;
	case RELATION_GROUP_F4:
		{
			return &m_FriendGroup4;
		}
		break;
	case RELATION_GROUP_F5:
		{
			return &m_FriendGroup5;
		}
		break;

	// ��ʱ����
	case RELATION_GROUP_TEMPFRIEND:
		{
			return &m_TeamFriendGroup;
		}
		break;

	// ������
	case RELATION_GROUP_BLACK:
		{
			return &m_BlackListGroup;
		}
		break;

	// ������
	case RELATION_GROUP_ENEMY:
		{
			return &m_EnemyListGroup;
		}
		break;

	default:
		Assert(FALSE);
		return NULL;
	}

	return NULL;
}

// ���ݳ��� ID ��ó������֣������� szSceneName
BOOL Relation::GetLocationName( SceneID_t sid, CHAR* szSceneName )
{
	Assert( szSceneName );

	if( sid != INVALID_ID )
	{
		strncpy( szSceneName, CGameProcedure::s_pWorldManager->GetSceneName(sid), LOCATION_DESC_SIZE - 1 );
	}
	else
	{
		strcpy( szSceneName, "δ֪����" );
	}

	return TRUE;
}

// ���ݹ�ϵ���ͻ����Ѻö�ȷ��˫����ϵ�������� m_szRelation
BOOL Relation::SetRelationDesc( SDATA_RELATION_MEMBER* pMember )
{
	Assert( pMember );

	switch( pMember->m_RelationType )
	{
	case RELATION_TYPE_FRIEND:						//����
		{
			if( pMember->m_nFriendPoint < 10 )
			{
				strcpy( pMember->m_szRelation, "һ��֮Ե" );
			}
			else if( pMember->m_nFriendPoint <= 200 )
			{
				strcpy( pMember->m_szRelation, "����֮��" );
			}
			else if( pMember->m_nFriendPoint <= 500 )
			{
				strcpy( pMember->m_szRelation, "����֮��" );
			}
			else if( pMember->m_nFriendPoint <= 1000 )
			{
				strcpy( pMember->m_szRelation, "Ī��֮��" );
			}
			else if( pMember->m_nFriendPoint > 1000 )
			{
				strcpy( pMember->m_szRelation, "�ؾ�֮��" );
			}
			else
			{
				strcpy( pMember->m_szRelation, "��ͨ����" );
			}
		}
		break;
	case RELATION_TYPE_BROTHER:						//���
		strcpy( pMember->m_szRelation, "����֮��" );
		break;
	case RELATION_TYPE_MARRY:						//���
		strcpy( pMember->m_szRelation, "����" );
		break;
	case RELATION_TYPE_BLACKNAME:					//������
		strcpy( pMember->m_szRelation, "����" );
		break;
	case RELATION_TYPE_TEMPFRIEND:					//��ʱ����
		strcpy( pMember->m_szRelation, "��ʱ����" );
		break;
//	case RELATION_TYPE_MASTER:						//ʦ����ϵ
//	case RELATION_TYPE_PRENTICE:					//ͽ�ܹ�ϵ
	default:
		AssertEx( FALSE, "Unexpected Relation Type." );
		return FALSE;
	}

	return TRUE;
}

// ���ݶ����������������ʾ���ݣ������� m_szTeamDesc
BOOL Relation::SetTeamDesc( SDATA_RELATION_MEMBER* pMember )
{
	if( pMember->m_nTeamSize == 0 )
	{
		strcpy( pMember->m_szTeamDesc, "δ���" );
	}
	else
	{
		sprintf( pMember->m_szTeamDesc, "�����%3d��", pMember->m_nTeamSize );
	}

	return TRUE;
}
