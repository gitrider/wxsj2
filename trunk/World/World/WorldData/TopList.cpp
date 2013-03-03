

#include "stdafx.h"
#include "TopList.h"
#include "WGTopList.h"
#include "ServerManager.h"

TopList::TopList( )
{
__ENTER_FUNCTION

	CleanUp( );
	m_IDTable.InitTable(TOP_USDR_NUM);

__LEAVE_FUNCTION
}

TopList::~TopList( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

VOID TopList::CleanUp()
{
__ENTER_FUNCTION

	m_TopListData.m_MemberCount = 0;
	m_TopListData.m_RecordCount = 0;
	m_TopListData.m_UpdateInterval = 0 ; 
	m_TopListData.m_TopListID = INVALID_ID;
	memset(m_TopListData.m_Name,0,MAX_TOP_LIST_NAME);
	m_MinKeyOwner = 0;

__LEAVE_FUNCTION
}


BOOL TopList::Add(GUID_t id,VOID* pPtr)
{
__ENTER_FUNCTION
	
	if ( m_IDTable.Add(id,pPtr) == TRUE )
	{
		m_TopListData.m_RecordCount++;
		return TRUE;
	}
	
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}


//等级排行榜
LevelTopList::LevelTopList()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

LevelTopList::~LevelTopList()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

VOID LevelTopList::CleanUp()
{
__ENTER_FUNCTION

	memset(m_UserDataEx,0,sizeof(_LEVEL_TOP_DATA)*TOP_USDR_NUM);

__LEAVE_FUNCTION
}

VOID LevelTopList::ModifyMinKeyOwner(GUID_t guid,UINT Key)
{
	__ENTER_FUNCTION

	if ( m_TopListData.m_RecordCount == 1)
	{
		m_MinKeyOwner = guid;
	} 
	else if ( guid != m_MinKeyOwner )
	{
		_LEVEL_TOP_DATA* pData = (_LEVEL_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
		if ( (UINT)pData->m_UserData.m_Level>Key )
		{
			m_MinKeyOwner = guid;
		}
	}
	else
	{//自己原来是最小KEY拥有者，现在要找到新的最小KEY
		INT MinKey = Key;
		for (INT i=0;i<m_TopListData.m_RecordCount;i++)
		{
			if ( MinKey > m_UserDataEx[i].m_UserData.m_Level )
			{
				MinKey = m_UserDataEx[i].m_UserData.m_Level;
				m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
			}			
		}

	}
	__LEAVE_FUNCTION
}


VOID LevelTopList::UpDataLevelTopList(USER *pUser)
{
	__ENTER_FUNCTION
	Assert( pUser );

	//判断是否小于进入排行榜的最小等级
	if ( pUser->GetLevel() >= m_TopListData.m_EnterMinKey )
	{
		_LEVEL_TOP_DATA* pMinData = (_LEVEL_TOP_DATA*)Get(m_MinKeyOwner);
		
		//判断玩家是否已经在原榜中
		_LEVEL_TOP_DATA* pMyself = (_LEVEL_TOP_DATA*)Get(pUser->GetGUID());
		if ( pMyself != NULL )
		{//更新原有数据

			FULLUSERDATA* FullData = pUser->GetFullUserData();
			if (FullData)
			{
				pMyself->m_Exp		  = FullData->m_Human.m_Exp;
				pMyself->m_OnlineTime = FullData->m_Human.m_OnlineTime;
			}

			pMyself->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
			strncpy((char*)pMyself->m_GuildName, pUser->GetGuildName(), MAX_GUILD_NAME_SIZE-1);
			pMyself->m_UserData.m_Level = pUser->GetLevel();

			ModifyMinKeyOwner( pUser->GetGUID(), pUser->GetLevel() );
		}
		else
		{
			//如果符合加入排行榜条件
			if ( GetListRecord()<GetMaxListSize() || pUser->GetLevel()>pMinData->m_UserData.m_Level )
			{
				_LEVEL_TOP_DATA* PData = GetNullNode();

				FULLUSERDATA* FullData = pUser->GetFullUserData();
				if (FullData)
				{
					PData->m_OnlineTime = FullData->m_Human.m_OnlineTime;;
					PData->m_Exp		= FullData->m_Human.m_Exp;
				}
	
				PData->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
				strncpy((char*)PData->m_GuildName, pUser->GetGuildName(), MAX_GUILD_NAME_SIZE-1);
				PData->m_UserData.m_Country = pUser->GetCountry();
				PData->m_UserData.m_Guid	= pUser->GetGUID();
				PData->m_UserData.m_Level	= pUser->GetLevel();
				PData->m_UserData.m_MenPai	= pUser->GetMenpai();
				strncpy(PData->m_UserData.m_Name, pUser->GetName(), MAX_CHARACTER_NAME-1);
				PData->m_UserData.m_NameSize= (INT)strlen(PData->m_UserData.m_Name);

				if ( Add(pUser->GetGUID(), PData) )
				{
					ModifyMinKeyOwner(pUser->GetGUID(), pUser->GetLevel());
				}
			}
		}
	}
	__LEAVE_FUNCTION
}
VOID LevelTopList::HaveOldLevelRank()
{
	__ENTER_FUNCTION
	_LEVEL_TOP_DATA LevelTop[1];
	_LEVEL_TOP_DATA TemTopList[TOP_USDR_NUM];
	memcpy(TemTopList,m_UserDataEx,sizeof(_LEVEL_TOP_DATA)*TOP_USDR_NUM);

	INT m_DataCount = 0;		//排行榜实际拥有的玩家数量
	if ( GetListRecord() > GetMaxListSize() )
	{
		m_DataCount = GetMaxListSize();
	}
	else
	{
		m_DataCount = GetListRecord();
	}

	//进行排序
	for(INT i=0; i<m_DataCount; i++)
	{
		for (INT j=i+1; j<m_DataCount; j++)
		{
			if (TemTopList[i].m_UserData.m_Level < TemTopList[j].m_UserData.m_Level )
			{
				memcpy(LevelTop, TemTopList+i, sizeof(_LEVEL_TOP_DATA));
				memcpy(TemTopList+i,TemTopList+j, sizeof(_LEVEL_TOP_DATA));
				memcpy(TemTopList+j, LevelTop, sizeof(_LEVEL_TOP_DATA));
			}
			else if (TemTopList[i].m_UserData.m_Level == TemTopList[j].m_UserData.m_Level )
			{
				if (TemTopList[i].m_Exp < TemTopList[j].m_Exp)
				{
					memcpy(LevelTop, TemTopList+i, sizeof(_LEVEL_TOP_DATA));
					memcpy(TemTopList+i,TemTopList+j, sizeof(_LEVEL_TOP_DATA));
					memcpy(TemTopList+j, LevelTop, sizeof(_LEVEL_TOP_DATA));
				}
				else if (TemTopList[i].m_Exp == TemTopList[j].m_Exp)
				{
					if (TemTopList[i].m_OnlineTime > TemTopList[j].m_OnlineTime)
					{
						memcpy(LevelTop, TemTopList+i, sizeof(_LEVEL_TOP_DATA));
						memcpy(TemTopList+i,TemTopList+j, sizeof(_LEVEL_TOP_DATA));
						memcpy(TemTopList+j, LevelTop, sizeof(_LEVEL_TOP_DATA));
					}
				}
			}
		}
	}
	
	for (INT i=0; i<m_DataCount; i++)
	{
		for (INT j=0; j< m_DataCount; j++)
		{
			if (TemTopList[i].m_UserData.m_Guid == m_UserDataEx[j].m_UserData.m_Guid)
			{
				m_UserDataEx[j].m_OldRank = i+1;
				break;
			}
		}
	}

	__LEAVE_FUNCTION
}
//金币排行榜
MoneyTopList::MoneyTopList()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}


MoneyTopList::~MoneyTopList()
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

VOID MoneyTopList::ModifyMinKeyOwner(GUID_t guid,UINT Key)
{
	__ENTER_FUNCTION
		
		if ( m_TopListData.m_RecordCount == 1)
		{
			m_MinKeyOwner = guid;
		} 
		else if ( guid != m_MinKeyOwner )
		{
			_MONEY_TOP_DATA* pData = (_MONEY_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
			Assert(pData);
			if ( pData->m_Money>Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{//自己原来是最小KEY拥有者，现在要找到新的最小KEY
			UINT MinKey = Key;
			for (INT i=0;i<m_TopListData.m_RecordCount;i++)
			{
				if ( MinKey > m_UserDataEx[i].m_Money )
				{
					MinKey = m_UserDataEx[i].m_Money;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}			
			}
		}

	__LEAVE_FUNCTION
}

VOID MoneyTopList::CleanUp()
{
__ENTER_FUNCTION

	memset(m_UserDataEx,0,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);

__LEAVE_FUNCTION
}

VOID MoneyTopList::UpDataMoneyTopList(USER* pUser,UINT uMoney)
{
	Assert(pUser);

	//判断玩家等级是否已经达到进入排行榜的等级
	if ( pUser->GetLevel() >= m_TopListData.m_EnterMinKey )
	{
		//得到玩家完整的数据
		FULLUSERDATA* FullUserData = pUser->GetFullUserData();

		_MONEY_TOP_DATA* pMinData = (_MONEY_TOP_DATA*)Get(m_MinKeyOwner);

		//判断自己是否在原榜中
		_MONEY_TOP_DATA* pMyself = (_MONEY_TOP_DATA*)Get(pUser->GetGUID());
		if (pMyself!=NULL)
		{//更新原有数据
			pMyself->m_Money = uMoney;
			if (FullUserData)
			{
				pMyself->m_OnLineTime = FullUserData->m_Human.m_OnlineTime;
			}
			pMyself->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
			strncpy((char*)pMyself->m_GuildName,pUser->GetGuildName(),MAX_GUILD_NAME_SIZE-1);
			pMyself->m_UserData.m_Level	= pUser->GetLevel();
			ModifyMinKeyOwner(pUser->GetGUID(), uMoney);
		}
		else
		{//自己不在原榜中
			if ( GetListRecord() < GetMaxListSize() || uMoney > pMinData->m_Money )
			{
				_MONEY_TOP_DATA* pData = GetNullNode();
				pData->m_Money = uMoney;
				if (FullUserData)
				{
					pData->m_OnLineTime = FullUserData->m_Human.m_OnlineTime;
				}
				pData->m_GuildNameSize = (BYTE)(strlen( pUser->GetGuildName() ));
				strncpy((char*)pData->m_GuildName,pUser->GetGuildName(),MAX_GUILD_NAME_SIZE-1);

				pData->m_UserData.m_Country = pUser->GetCountry();
				pData->m_UserData.m_Guid	= pUser->GetGUID();
				pData->m_UserData.m_Level	= pUser->GetLevel();
				pData->m_UserData.m_MenPai = pUser->GetMenpai();
				strncpy((char*)pData->m_UserData.m_Name,pUser->GetName(),MAX_CHARACTER_NAME-1);
				pData->m_UserData.m_NameSize	= (INT)strlen(pData->m_UserData.m_Name);
				if(Add(pUser->GetGUID(), pData))
				{
					ModifyMinKeyOwner( pUser->GetGUID(), uMoney );
				}
			}
		}
	}
}
VOID MoneyTopList::HaveOldMoneyRank()
{
	__ENTER_FUNCTION
	_MONEY_TOP_DATA MoneyTop[1];
	_MONEY_TOP_DATA TemTopList[TOP_USDR_NUM];
	memcpy(TemTopList,m_UserDataEx,sizeof(_MONEY_TOP_DATA)*TOP_USDR_NUM);

	INT m_DataCount = 0;		//排行榜实际拥有的玩家数量
	if ( GetListRecord() > GetMaxListSize() )
	{
		m_DataCount = GetMaxListSize();
	}
	else
	{
		m_DataCount = GetListRecord();
	}

	//进行排序
	for(INT i=0; i<m_DataCount; i++)
	{
		for (INT j=i+1; j<m_DataCount; j++)
		{
			if ( TemTopList[i].m_Money < TemTopList[j].m_Money )
			{
				memcpy(MoneyTop, TemTopList+i, sizeof(_MONEY_TOP_DATA));
				memcpy(TemTopList+i,TemTopList+j, sizeof(_MONEY_TOP_DATA));
				memcpy(TemTopList+j, MoneyTop, sizeof(_MONEY_TOP_DATA));
			}
			else if (TemTopList[i].m_Money == TemTopList[j].m_Money )
			{
				if (TemTopList[i].m_OnLineTime > TemTopList[j].m_OnLineTime)
				{
					memcpy(MoneyTop, TemTopList+i, sizeof(_MONEY_TOP_DATA));
					memcpy(TemTopList+i,TemTopList+j, sizeof(_MONEY_TOP_DATA));
					memcpy(TemTopList+j, MoneyTop, sizeof(_MONEY_TOP_DATA));
				}
			}
		}
	}

	for (INT i=0; i<m_DataCount; i++)
	{
		for (INT j=0; j< m_DataCount; j++)
		{
			if (TemTopList[i].m_UserData.m_Guid == m_UserDataEx[j].m_UserData.m_Guid)
			{
				m_UserDataEx[j].m_OldRank = i+1;
				break;
			}
		}
	}
	__LEAVE_FUNCTION
}
//声望排行榜
ReputeTopList::ReputeTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

ReputeTopList::~ReputeTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID ReputeTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid !=m_MinKeyOwner )
		{
			_REPUTE_TOP_DATA* pData = (_REPUTE_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
			Assert(pData);
			if ( pData->m_Repute > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for ( INT i=0; i<m_TopListData.m_RecordCount;i++ )
			{
				if ( MinKey > m_UserDataEx[i].m_Repute )
				{
					MinKey = m_UserDataEx[i].m_Repute;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}
			}
		}

	__LEAVE_FUNCTION
}

VOID ReputeTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_REPUTE_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}


//宠物排行榜
PetTopList::PetTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

PetTopList::~PetTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID PetTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_PET_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}

VOID PetTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid !=m_MinKeyOwner )
		{
			_PET_TOP_DATA* pData = (_PET_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
			Assert(pData);
			if ( pData->m_PetLevel > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for ( INT i=0; i<m_TopListData.m_RecordCount;i++ )
			{
				if ( MinKey > m_UserDataEx[i].m_PetLevel )
				{
					MinKey = m_UserDataEx[i].m_PetLevel;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}
			}
		}

	__LEAVE_FUNCTION
}

//副本排行榜
EctypeTopList::EctypeTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

EctypeTopList::~EctypeTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID EctypeTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_ECTYPE_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}

VOID EctypeTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid !=m_MinKeyOwner )
		{
			_ECTYPE_TOP_DATA* pData = (_ECTYPE_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
			Assert(pData);
			if ( pData->EctypeNum > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for ( INT i=0; i<m_TopListData.m_RecordCount;i++ )
			{
				if ( MinKey > m_UserDataEx[i].EctypeNum )
				{
					MinKey = m_UserDataEx[i].EctypeNum;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}
			}
		}

		__LEAVE_FUNCTION
}

//侠义排行榜
ErrantryTopList::ErrantryTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

ErrantryTopList::~ErrantryTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID ErrantryTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_ERRANTRY_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}

VOID ErrantryTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid !=m_MinKeyOwner )
		{
			_ERRANTRY_TOP_DATA* pData = (_ERRANTRY_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
			Assert(pData);
			if ( pData->m_Errantry > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for ( INT i=0; i<m_TopListData.m_RecordCount;i++ )
			{
				if ( MinKey > m_UserDataEx[i].m_Errantry )
				{
					MinKey = m_UserDataEx[i].m_Errantry;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}
			}
		}

	__LEAVE_FUNCTION
}

//罪恶排行榜
CrimeTopList::CrimeTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

CrimeTopList::~CrimeTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID CrimeTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_CRIME_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}

VOID CrimeTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid !=m_MinKeyOwner )
		{
			_CRIME_TOP_DATA* pData = (_CRIME_TOP_DATA*)m_IDTable.Get(m_MinKeyOwner);
			Assert(pData);
			if ( pData->m_Crime > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for ( INT i=0; i<m_TopListData.m_RecordCount;i++ )
			{
				if ( MinKey > m_UserDataEx[i].m_Crime )
				{
					MinKey = m_UserDataEx[i].m_Crime;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}
			}
		}
	__LEAVE_FUNCTION
}


//人气排行榜
FavorTopList::FavorTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

FavorTopList::~FavorTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID FavorTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_FAVOR_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}

VOID FavorTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid != m_MinKeyOwner )
		{
			_FAVOR_TOP_DATA* pData = (_FAVOR_TOP_DATA*)m_IDTable.Get( m_MinKeyOwner );
			Assert(pData);
			if ( pData->m_Favor > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for (INT i=0; i<m_TopListData.m_RecordCount; i++)
			{
				if ( MinKey > m_UserDataEx[i].m_Favor )
				{
					MinKey = m_UserDataEx[i].m_Favor;
					m_MinKeyOwner = m_UserDataEx[i].m_UserData.m_Guid;
				}
			}
		}
	__LEAVE_FUNCTION
}

//帮派排行榜
GuildTopList::GuildTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

GuildTopList::~GuildTopList()
{
	__ENTER_FUNCTION
		CleanUp();
	__LEAVE_FUNCTION
}

VOID GuildTopList::CleanUp()
{
	__ENTER_FUNCTION
		memset( m_UserDataEx,0,sizeof(_GUILD_TOP_DATA)*TOP_USDR_NUM );
	__LEAVE_FUNCTION
}

VOID GuildTopList::ModifyMinKeyOwner(GUID_t guid, UINT Key)
{
	__ENTER_FUNCTION
		if ( m_TopListData.m_RecordCount == 1 )
		{
			m_MinKeyOwner = guid;
		}
		else if ( guid != m_MinKeyOwner )
		{
			_GUILD_TOP_DATA* pData = (_GUILD_TOP_DATA*)m_IDTable.Get( m_MinKeyOwner );
			Assert(pData);
			if ( pData->m_GuildLevel > Key )
			{
				m_MinKeyOwner = guid;
			}
		}
		else
		{
			UINT MinKey = Key;
			for (INT i=0; i<m_TopListData.m_RecordCount; i++)
			{
				if ( MinKey > m_UserDataEx[i].m_GuildLevel )
				{
					MinKey = m_UserDataEx[i].m_GuildLevel;
					m_MinKeyOwner = m_UserDataEx[i].m_GuildID;
				}
			}
		}
	__LEAVE_FUNCTION
}