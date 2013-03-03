
#include "stdafx.h"
#include "HumanRelation.h"
#include "Obj_Human.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWRelation.h"
#include "GWBatchMail.h"
#include "TimeManager.h"

HumanRelation::HumanRelation( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

HumanRelation::~HumanRelation( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL HumanRelation::Init( Obj_Human* pHuman )
{
__ENTER_FUNCTION

	Assert( pHuman );
	m_pHuman = pHuman;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID HumanRelation::CleanUp( )
{
__ENTER_FUNCTION

	m_pHuman = NULL;

__LEAVE_FUNCTION
}

// �ж��Ƿ��Լ��ĺ���
BOOL HumanRelation::IsFriend( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<RELATION_BLACKNAME_OFFSET; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

// �ж��Ƿ��ں�����
BOOL HumanRelation::IsBlackName( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid != INVALID_ID );

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_BLACKNAME_OFFSET; i<RELATION_ENEMY_OFFSET; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

// �õ��������֮��Ĺ�ϵ����
RELATION_TYPE HumanRelation::GetRelationType( GUID_t guid )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return (RELATION_TYPE)(pRelationList[i].m_Type);
		}
	}

	return RELATION_TYPE_STRANGER;

__LEAVE_FUNCTION

	return RELATION_TYPE_STRANGER;
}

// �ж��������֮���Ƿ����ĳ�ֹ�ϵ
BOOL HumanRelation::HaveRelation( RELATION_TYPE RelationType, GUID_t guid )
{
__ENTER_FUNCTION

	if( RelationType <= RELATION_TYPE_NONE || RelationType >= RELATION_TYPE_SIZE )
	{
		Assert(FALSE);
		return FALSE;
	}

	INT nStart, nEnd;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
		{
			nStart = RELATION_FRIEND_OFFSET;
			nEnd = RELATION_BLACKNAME_OFFSET;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			nStart = RELATION_BLACKNAME_OFFSET;
			nEnd = RELATION_ENEMY_OFFSET;
		}
		break;
	case RELATION_TYPE_ENEMY:
		{
			nStart = RELATION_ENEMY_OFFSET;
			nEnd = MAX_RELATION_SIZE;
		}
		break;
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
		{
			Assert(FALSE);
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=nStart; i<nEnd; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			if( pRelationList[i].m_Type == RelationType )
			{
				return TRUE;
			}

			break;
		}
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// ���һ����ϵ��
BOOL HumanRelation::AddRelation( RELATION_TYPE RelationType, _RELATION* pRelationData )
{
__ENTER_FUNCTION

	Assert( pRelationData );

	RELATION_MEMBER Member;
	Member.m_MemberGUID = pRelationData->GetGUID();
	strncpy(Member.m_szMemberName, pRelationData->GetName(), sizeof(Member.m_szMemberName) - 1);
	Member.m_nLevel = pRelationData->GetLevel();
	Member.m_nMenPai = pRelationData->GetMenPai();
	Member.m_nPortrait = pRelationData->GetPortrait();
	Member.m_GuildID = pRelationData->GetGuildID();

	return AddRelation( RelationType, &Member );

__LEAVE_FUNCTION

	return FALSE;
}

// ���һ����ϵ��
BOOL HumanRelation::AddRelation( RELATION_TYPE RelationType, const RELATION_MEMBER* pMember )
{
__ENTER_FUNCTION

	Assert( pMember );

	if( HaveRelation( RelationType, pMember->m_MemberGUID ) )
	{ // ��ͬ����Ա����
		return FALSE;
	}

	UCHAR uGroup = 0;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
		{
			uGroup = RELATION_GROUP_FRIEND_ALL;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			uGroup = RELATION_GROUP_BLACK;
		}
		break;
	case RELATION_TYPE_ENEMY:
		{
			uGroup = RELATION_GROUP_ENEMY;
		}
		break;
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
		{
			Assert(FALSE);
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	if( IsGroupFull((RELATION_GROUP)uGroup) )
	{ // ������
		return FALSE;
	}

	INT index;

	if( GetRelation(RelationType, INVALID_ID, index) != NULL )
	{ // �ҳ�һ����λ��
		_OWN_RELATION Relation;
		Relation.m_Type = RelationType;
		Relation.m_Group = uGroup;
		memcpy((void*)&(Relation.m_Member), pMember, sizeof(RELATION_MEMBER));
		const CHAR *pName = m_pHuman->GetDB()->GetRelationDB()->m_aRelation[index].m_GroupName;
		memcpy(Relation.m_GroupName,pName,sizeof(Relation.m_GroupName));

		m_pHuman->GetDB()->SetRelation(index, &Relation);
		m_pHuman->GetDB()->IncRelationCount(uGroup);
		return TRUE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// ɾ��һ����ϵ��
BOOL HumanRelation::DelRelation( RELATION_TYPE RelationType, GUID_t guid )
{
__ENTER_FUNCTION

	BOOL bFriendFlag = FALSE;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
		{
			bFriendFlag = TRUE;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
		}
		break;
	case RELATION_TYPE_ENEMY:
		{
				//����ǳ��ˣ���ʱֱ��ɾ��
		}
		break;
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
		{
			Assert(FALSE);
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	INT index;

	const _OWN_RELATION* pRelation;
	pRelation = GetRelation(RelationType, guid, index);
	if( pRelation != NULL )
	{
		if( bFriendFlag )
		{ // ���ٺ��ѵ�����
			//m_pHuman->GetDB()->DecRelationCount(RELATION_GROUP_FRIEND_ALL);
		}

		m_pHuman->GetDB()->DecRelationCount(pRelation->m_Group);
		m_pHuman->GetDB()->DeleteRelation(index);
		return TRUE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// �����ϵ
BOOL HumanRelation::RelationTransition( RELATION_TYPE newRelationType, GUID_t guid )
{
__ENTER_FUNCTION

	// 1���õ�ת��ǰ��ϵ
	RELATION_TYPE oldRelationType = GetRelationType(guid);

	// 2����֤���ֹ�ϵ��ת���Ϸ���
	if( CanTransition(oldRelationType, newRelationType) != TRUE )
	{
		return FALSE;
	}

	// 3����֤ת����Ĺ�ϵ��ռ�ݵĴ洢�ռ��㹻
	if( newRelationType == RELATION_TYPE_FRIEND )
	{
		if( IsGroupFull(RELATION_GROUP_FRIEND_ALL) )
		{
			return FALSE;
		}
	}
	else if( newRelationType == RELATION_TYPE_BLACKNAME )
	{
		if( IsGroupFull(RELATION_GROUP_BLACK) )
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	// 4��ʵ��ת��������������
	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( oldRelationType, guid, index );
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	AddRelation( newRelationType, &(pRelation->m_Member) );
	DelRelation( oldRelationType, guid );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// �Ƿ�ĳ���Ѿ���Ա
BOOL HumanRelation::IsGroupFull( RELATION_GROUP RelationGroup )
{
__ENTER_FUNCTION

	if( RelationGroup < RELATION_GROUP_FRIEND_ALL
	 || RelationGroup >= RELATION_GROUP_NUMBER
	 )
	{
		Assert(FALSE);
		return TRUE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return TRUE;
	}

	INT nCount;
	nCount = pRelationDB->m_aRelationCount[RelationGroup];

	switch(RelationGroup)
	{
	case RELATION_GROUP_FRIEND_ALL:
	case RELATION_GROUP_F1:
	case RELATION_GROUP_F2:
	case RELATION_GROUP_F3:
	case RELATION_GROUP_F4:
	case RELATION_GROUP_F5:
		{
			if( nCount >= RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET )
			{
				return TRUE;
			}
		}
		break;
	
	case RELATION_GROUP_BLACK:
		{
			if( nCount >= RELATION_ENEMY_OFFSET - RELATION_BLACKNAME_OFFSET )
			{
				return TRUE;
			}
		}
		break;
	case RELATION_GROUP_ENEMY:
		{
			if ( nCount >= MAX_RELATION_SIZE - RELATION_ENEMY_OFFSET )
			{
				return TRUE;			
			}
		}
		break;
	default:
		Assert(FALSE);
		return TRUE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return TRUE;
}

// �ж�һ�ֹ�ϵ�Ƿ��ܹ�ֱ��ת������һ�ֹ�ϵ
BOOL HumanRelation::CanTransition( RELATION_TYPE oldRelationType, RELATION_TYPE newRelationType )
{
__ENTER_FUNCTION

	if( oldRelationType <= RELATION_TYPE_NONE || oldRelationType >= RELATION_TYPE_SIZE
	 || newRelationType <= RELATION_TYPE_NONE || newRelationType >= RELATION_TYPE_SIZE
	 )
	{
		Assert(FALSE);
		return FALSE;
	}

	switch(oldRelationType)
	{
	case RELATION_TYPE_FRIEND:
		{
			if( newRelationType == RELATION_TYPE_BLACKNAME )
			{
				return TRUE;
			}
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			if ( newRelationType == RELATION_TYPE_FRIEND )
			{
				return TRUE;
			}
		}
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
	case RELATION_TYPE_MASTER:
	case RELATION_TYPE_PRENTICE:
		{
			return FALSE;
		}
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}

	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

// �õ�����
const CHAR* HumanRelation::GetMood( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_szMood);

__LEAVE_FUNCTION

	return NULL;
}

// ��������
VOID HumanRelation::SetMood( const CHAR* szMood )
{
__ENTER_FUNCTION

	if( szMood == NULL )
	{
		Assert( szMood );
		return;
	}

	m_pHuman->GetDB()->SetMood(szMood);

__LEAVE_FUNCTION
}

// ��ѯ��ϵֵ
INT HumanRelation::GetFriendPoint( GUID_t guid )
{
__ENTER_FUNCTION

	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, index);
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return 0;
	}

	return pRelation->m_FriendPoint;

__LEAVE_FUNCTION

	return 0;
}

// ���ӹ�ϵֵ
VOID HumanRelation::IncFriendPoint( GUID_t guid, INT nPoint )
{
__ENTER_FUNCTION

	Assert( nPoint > 0 );

	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, index );
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return;
	}

	if( pRelation->m_FriendPoint + nPoint > MAX_FRIEND_POINT )
	{
		nPoint = MAX_FRIEND_POINT - pRelation->m_FriendPoint;
	}

	if( nPoint <= 0 )
	{ // �Ѿ���������
		return;
	}

	m_pHuman->GetDB()->ModifyFriendPoint(index, nPoint);

	// ������Ѷȴﵽ��ʾ���µ�����
	if( pRelation->m_FriendPoint > g_Config.m_ConfigInfo.m_nPromptPoint )
	{
		GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
		GW_RELATION* pRelation = pMsg->GetRelation();
		pRelation->CleanUp();
		pRelation->m_Type = REQ_NEWGOODFRIEND;
		pRelation->SetGUID( m_pHuman->GetGUID() );
		pRelation->m_RelationGUID.CleanUp();
		pRelation->m_RelationGUID.SetTargetGUID(guid);

		g_pServerManager->SendPacket( pMsg, INVALID_ID );
	}

__LEAVE_FUNCTION
}

// ���ٹ�ϵֵ
VOID HumanRelation::DecFriendPoint( GUID_t guid, INT nPoint )
{
__ENTER_FUNCTION

	Assert( nPoint > 0 );

	INT index;
	if( GetRelation( RELATION_TYPE_FRIEND, guid, index ) == NULL )
	{
		Assert(FALSE);
		return;
	}

	m_pHuman->GetDB()->ModifyFriendPoint(index, -nPoint);

	__LEAVE_FUNCTION
}

// ���÷��飨�ýӿڴ�����Ϊ�˿ͻ�����ʾ�����ã�
BOOL HumanRelation::SetFriendGroup( GUID_t guid, RELATION_GROUP RelationGroup )
{
__ENTER_FUNCTION

	INT index;
	const _OWN_RELATION* pRelation;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, index );
	if( pRelation == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	RELATION_GROUP oldRelationGroup;
	oldRelationGroup = (RELATION_GROUP)pRelation->m_Group;

	if( oldRelationGroup == RelationGroup )
	{
		return TRUE;
	}

	if( IsGroupFull(RelationGroup) )
	{
		return FALSE;
	}

	if( oldRelationGroup != RELATION_GROUP_FRIEND_ALL )
	{
		m_pHuman->GetDB()->DecRelationCount(oldRelationGroup);
	}

	m_pHuman->GetDB()->AdjustRelationGroup(index, RelationGroup);

	if( RelationGroup != RELATION_GROUP_FRIEND_ALL )
	{
		m_pHuman->GetDB()->IncRelationCount(RelationGroup);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// ����ϵ�б���Ϣ�ṹ
VOID HumanRelation::FillRelationList( GC_RELATIONLIST& RelationList )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=0; i<MAX_RELATION_SIZE; ++i )
	{
		switch( pRelationList[i].m_Type )
		{
		case RELATION_TYPE_NONE:
			{
			}
			break;
		case RELATION_TYPE_FRIEND:
		case RELATION_TYPE_BROTHER:
		case RELATION_TYPE_MARRY:
		case RELATION_TYPE_MASTER:
		case RELATION_TYPE_PRENTICE:
			{
				RelationList.AddFriend( &pRelationList[i] );
			}
			break;
		case RELATION_TYPE_BLACKNAME:
			{
				RelationList.AddBlackName( &pRelationList[i] );
			}
			break;
		case RELATION_TYPE_ENEMY:
			{
				RelationList.AddEnemyName( &pRelationList[i] );
			}	
			break;
		default:
			Assert(FALSE);
			return;
		}

	}

__LEAVE_FUNCTION
}

// ���¹�ϵ������
const _OWN_RELATION* HumanRelation::UpdateRelationInfo( _RELATION* pRelationData )
{
__ENTER_FUNCTION
	// ���ҵ�����ˣ�����Ҳ����Ͳ�������
	Assert( pRelationData );

	GUID_t guid = pRelationData->GetGUID();

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			RELATION_MEMBER Member;
			Member.m_MemberGUID = pRelationData->GetGUID();
			strncpy(Member.m_szMemberName, pRelationData->GetName(), sizeof(Member.m_szMemberName) - 1);
			Member.m_nLevel = pRelationData->GetLevel();
			Member.m_nMenPai = pRelationData->GetMenPai();
			Member.m_nPortrait = pRelationData->GetPortrait();
			Member.m_GuildID = pRelationData->GetGuildID();

			m_pHuman->GetDB()->UpdateRelationInfo(i, &Member);
			return &(pRelationList[i]);
		}
	}

	// ���½�ȥ
__LEAVE_FUNCTION

	return NULL;
}

// ��ù�ϵ������
const _OWN_RELATION* HumanRelation::GetRelationInfo( GUID_t guid )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			return &(pRelationList[i]);
		}
	}

__LEAVE_FUNCTION

	return NULL;
}

// ͨ�����ֻ�ù�ϵ������
const _OWN_RELATION* HumanRelation::GetRelationInfo(const CHAR *szName)
{
__ENTER_FUNCTION
	if( szName == NULL )
	{
		Assert( szName );
		return NULL;
	}

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();
	if ( pRelationList == NULL)
	{
		Assert( pRelationList );
		return NULL;
	}
	for ( INT i=RELATION_FRIEND_OFFSET; i<MAX_RELATION_SIZE; i++)
	{
		if ( strcmp(pRelationList[i].m_Member.m_szMemberName, szName) == 0  )
		{
			return & (pRelationList[i]);
		}
	}

__LEAVE_FUNCTION
	return NULL;
}

// ����ϵ�������е�ȱ�������
//BOOL HumanRelation::CompleteRelationInfo( _RELATION& RelationData )
//{
//__ENTER_FUNCTION
//
//	if( RelationData.GetGUID() == NULL )
//	{
//		Assert( FALSE );
//		return FALSE;
//	}
//
//__LEAVE_FUNCTION
//
//	return FALSE;
//}

// �����ǽ����ص�һЩ����
BOOL HumanRelation::IsMarried()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_MarriageInfo.m_SpouseGUID != INVALID_ID);

__LEAVE_FUNCTION

	return NULL;
}

// �õ���ż�� GUID
GUID_t HumanRelation::GetSpouseGUID()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return INVALID_ID;
	}

	return pRelationDB->m_MarriageInfo.m_SpouseGUID;

__LEAVE_FUNCTION

	return INVALID_ID;
}

// ��ĳ�� GUID ��Ӧ����ҽ�飨�����ﲻ���Ա�
BOOL HumanRelation::Marry( GUID_t guid )
{
__ENTER_FUNCTION

	if( IsMarried() )
	{
		Assert( IsMarried() );
		return FALSE;
	}

	// �ҵ��� GUID �ĺ���
	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_FRIEND) )
	{ // û��������ѻ��ϵ������ͨ���ѹ�ϵ
		AssertEx(FALSE, "�Բ���Ŀǰ����ֻ����ͨ���ѹ�ϵ���Խ�顣");
		return FALSE;
	}

	// �����ѵ�������Ϊ����
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_MARRY );

	// ����ż�� GUID ��Ϊ guid
	MarriageInfo info;
	info.m_SpouseGUID = guid;

	m_pHuman->GetDB()->SetSpouse(&info);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// ���
BOOL HumanRelation::Divorce( )
{
__ENTER_FUNCTION

	GUID_t guid;
	guid = GetSpouseGUID();

	if( guid == INVALID_ID )
	{
		Assert( INVALID_ID != GetSpouseGUID() );
		return FALSE;
	}

	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_MARRY, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_MARRY) )
	{ // û��������ѻ��ϵ���Ƿ��޹�ϵ
		AssertEx(FALSE, "�Բ���ֻ�з��޹�ϵ������顣");
		return FALSE;
	}

	// �����ѵ�������Ϊ��ͨ����
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_FRIEND );

	// ����ż�� GUID �ÿ�
	MarriageInfo info;
	info.m_SpouseGUID = INVALID_ID;

	m_pHuman->GetDB()->SetSpouse(&info);

__LEAVE_FUNCTION

	return FALSE;
}

// �ж� guid ��Ӧ������Ƿ����Լ���ʦ��
BOOL HumanRelation::IsMaster( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert( guid != INVALID_ID );
		return FALSE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_MasterInfo.m_MasterGUID == guid);

__LEAVE_FUNCTION

	return FALSE;
}

// �õ�ʦ���� GUID
GUID_t HumanRelation::GetMasterGUID()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return INVALID_ID;
	}

	return pRelationDB->m_MasterInfo.m_MasterGUID;

__LEAVE_FUNCTION

	return INVALID_ID;
}

// �ж� guid ��Ӧ������Ƿ����Լ���ͽ��
BOOL HumanRelation::IsPrentice( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert( guid != INVALID_ID );
		return FALSE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
	{
		if( pRelationDB->m_PrenticeInfo.m_PrenticeGUID[i] == guid )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

// �ж��Ƿ���ʦ��
BOOL HumanRelation::HaveMaster( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return (pRelationDB->m_MasterInfo.m_MasterGUID != INVALID_ID);

__LEAVE_FUNCTION

	return FALSE;
}

// ȡ���м���ͽ��
INT HumanRelation::GetPrenticeCount()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return 0;
	}

	return pRelationDB->m_PrenticeInfo.m_uPrenticeCount;

__LEAVE_FUNCTION

	return 0;
}

// ȡ�õ� nIndex ��ͽ�ܵ� GUID
GUID_t HumanRelation::GetNthPrentice( INT nIndex )
{
__ENTER_FUNCTION

	if( nIndex >= MAX_PRENTICE_COUNT )
	{
		Assert( nIndex < MAX_PRENTICE_COUNT );
		return INVALID_ID;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationDB->m_PrenticeInfo.m_PrenticeGUID[nIndex];

__LEAVE_FUNCTION

	return INVALID_ID;

}

// ȡ��ʦ�µ�
UINT HumanRelation::GetMoralPoint( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationDB->m_PrenticeInfo.m_uMoralPoint;

__LEAVE_FUNCTION

	return 0;

}

// ����ʦ�µ�
BOOL HumanRelation::SetMoralPoint( UINT uPoint )
{
__ENTER_FUNCTION

	if( uPoint > MAX_MORAL_POINT )
	{
		Assert( uPoint <= MAX_MORAL_POINT );
		return FALSE;
	}

	m_pHuman->GetDB()->SetMoralPoint(uPoint);

__LEAVE_FUNCTION

	return FALSE;

}

// ȡ�����һ��ͽ����ʦʱ��ݵ�ǰʱ��ĳ��ȣ�����������
UINT HumanRelation::GetPrenticeBetrayTime()
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return 0;
	}

	UINT sec; // ���һ����ʦ������
	sec = (UINT)(g_pTimeManager->GetANSITime() - pRelationDB->m_PrenticeInfo.m_BetrayingTime);

	return sec;

__LEAVE_FUNCTION

	return 0;
}

// �� guid ��Ӧ�����Ϊʦ
BOOL HumanRelation::Aprentice( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert(guid != INVALID_ID);
		return FALSE;
	}

	if( HaveMaster() )
	{
		Assert( FALSE );
		return FALSE;
	}

	// �ҵ��� GUID �ĺ���
	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_FRIEND) )
	{ // û��������ѻ��ϵ������ͨ���ѹ�ϵ
		AssertEx(FALSE, "�Բ���Ŀǰ����ֻ����ͨ���ѹ�ϵ���԰�ʦ��");
		return FALSE;
	}

	// �����ѵ�������Ϊʦ��
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_MASTER );

	// ��ʦ���� GUID ��Ϊ guid
	MasterInfo info;
	info.m_MasterGUID = guid;

	m_pHuman->GetDB()->SetMaster( &info );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// �� guid ��Ӧ�����Ϊͽ
BOOL HumanRelation::Recruit( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert(guid != INVALID_ID);
		return FALSE;
	}

	if( GetPrenticeCount() >= MAX_PRENTICE_COUNT )
	{
		Assert( GetPrenticeCount() < MAX_PRENTICE_COUNT );
		return FALSE;
	}

	if( IsPrentice( guid ) )
	{
		Assert( FALSE );
		return FALSE;
	}

	// �ҵ��� GUID �ĺ���
	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_FRIEND, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_FRIEND) )
	{ // û��������ѻ��ϵ������ͨ���ѹ�ϵ
		AssertEx(FALSE, "�Բ���Ŀǰֻ�ܽ���ͨ������Ϊͽ�ܡ�");
		return FALSE;
	}

	// �����ѵ�������Ϊʦ��
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_PRENTICE );

	m_pHuman->GetDB()->AddPrentice( guid );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// ����ʦ��
BOOL HumanRelation::LeaveMaster( )
{
__ENTER_FUNCTION

	GUID_t guid;
	guid = GetMasterGUID();

	if( guid == INVALID_ID )
	{
		Assert( INVALID_ID != GetMasterGUID() );
		return FALSE;
	}

	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_MASTER, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_MASTER) )
	{ // û��������ѻ�������ʦ��
		AssertEx(FALSE, "�Բ���ֻ�ж�ʦ������ִ�д˲�����");
		return FALSE;
	}

	// �����ѵ�������Ϊ��ͨ����
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_FRIEND );

	// ��ʦ���� GUID �ÿ�
	MasterInfo info;
	info.m_MasterGUID = INVALID_ID;

	m_pHuman->GetDB()->SetMaster(&info);

__LEAVE_FUNCTION

	return FALSE;
}

// ���� GUID Ϊ guid ��ͽ��
BOOL HumanRelation::ExpelPrentice( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert(guid != INVALID_ID);
		return FALSE;
	}

	if( IsPrentice( guid ) == FALSE )
	{
		Assert( IsPrentice( guid ) );
		return FALSE;
	}

	const _OWN_RELATION* pRelation;
	INT nIndex;
	pRelation = GetRelation( RELATION_TYPE_PRENTICE, guid, nIndex );
	if ( (pRelation == NULL) || (pRelation->m_Type != RELATION_TYPE_PRENTICE) )
	{ // û��������ѻ�������ͽ��
		AssertEx(FALSE, "�Բ���ֻ�ж�ͽ�ܿ���ִ�д˲�����");
		return FALSE;
	}

	// �����ѵ�������Ϊ��ͨ����
	m_pHuman->GetDB()->ModifyFriendType( nIndex, RELATION_TYPE_FRIEND );
	m_pHuman->GetDB()->DelPrentice( guid );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

// �ж��Ƿ��ǽ���ֵ�
BOOL HumanRelation::IsBrother( GUID_t guid )
{
__ENTER_FUNCTION

	if( guid == INVALID_ID )
	{
		Assert( guid != INVALID_ID );
		return FALSE;
	}

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	// Not Accomplished.
	// return TRUE; // (pRelationDB->m_MasterInfo.m_MasterGUID == guid);

__LEAVE_FUNCTION

	return FALSE;
}

// �����к��ѷ����ʼ�
VOID HumanRelation::SendMailToAllFriend( const CHAR* szMail )
{
__ENTER_FUNCTION

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	GWBatchMail* pMsg = (GWBatchMail*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_BATCHMAIL);
	BATCH_MAIL* pBatchMail = pMsg->GetBatchMail();

	pBatchMail->SetGUID( m_pHuman->GetGUID() );
	pBatchMail->SetSourName( m_pHuman->GetName() );

	for( INT i=RELATION_FRIEND_OFFSET; i<RELATION_BLACKNAME_OFFSET; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID != INVALID_ID )
		{
			pBatchMail->AddDestName( pRelationList[i].m_Member.m_szMemberName );
		}
	}

	pBatchMail->SetMailContent( szMail );
	pBatchMail->SetMailFlag( MAIL_TYPE_NORMAL2 );

	pBatchMail->SetCreateTime( g_pTimeManager->GetANSITime() );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

__LEAVE_FUNCTION
}

const _RELATION_DB_LOAD* HumanRelation::GetRelationDB( )
{
__ENTER_FUNCTION

	if( m_pHuman == NULL )
	{
		Assert(FALSE);
		return NULL;
	}

	return m_pHuman->GetDB()->GetRelationDB();

__LEAVE_FUNCTION

	return NULL;
}

const _OWN_RELATION* HumanRelation::GetRelationList( )
{
__ENTER_FUNCTION

	const _RELATION_DB_LOAD* pRelationDB;
	pRelationDB = GetRelationDB();
	if( pRelationDB == NULL )
	{
		Assert(FALSE);
		return FALSE;
	}

	return pRelationDB->m_aRelation;

__LEAVE_FUNCTION

	return NULL;
}

// ȡ����ϵ�˵����ݣ�����ָ���Լ� Index ֵ
const _OWN_RELATION* HumanRelation::GetRelation( RELATION_TYPE RelationType, GUID_t guid, INT& index )
{
__ENTER_FUNCTION

	INT nStart, nEnd;

	switch( RelationType )
	{
	case RELATION_TYPE_FRIEND:
	case RELATION_TYPE_BROTHER:
	case RELATION_TYPE_MARRY:
	case RELATION_TYPE_MASTER:
	case RELATION_TYPE_PRENTICE:
		{
			nStart = RELATION_FRIEND_OFFSET;
			nEnd = RELATION_BLACKNAME_OFFSET;
		}
		break;
	case RELATION_TYPE_BLACKNAME:
		{
			nStart = RELATION_BLACKNAME_OFFSET;
			nEnd = RELATION_ENEMY_OFFSET;
		}
		break;
	case RELATION_TYPE_ENEMY:
		{
			nStart = RELATION_ENEMY_OFFSET;
			nEnd = MAX_RELATION_SIZE;
		}
		break;
	case RELATION_TYPE_TEMPFRIEND:
	case RELATION_TYPE_STRANGER:
		{
			Assert(FALSE);
			return NULL;
		}
		break;
	default:
		Assert(FALSE);
		return NULL;
	}

	const _OWN_RELATION* pRelationList;
	pRelationList = GetRelationList();

	for( INT i=nStart; i<nEnd; ++i )
	{
		if( pRelationList[i].m_Member.m_MemberGUID == guid )
		{
			index = i;
			return &pRelationList[i];
		}
	}

__LEAVE_FUNCTION

	return NULL;
}

