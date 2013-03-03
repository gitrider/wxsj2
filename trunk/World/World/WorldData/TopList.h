
//-----------------------------------------------------------------------------
// 文件名 : TopList.h
// 模块	:	排行榜
// 功能	 :  各排行榜记录
// 修改历史:
//创建:		王勇鸷
//-----------------------------------------------------------------------------

#ifndef _TOP_LIST_H
#define _TOP_LIST_H

#include "IDTable.h"
#include "WGTopList.h"
#include "GameStruct_TopList.h"
#include "OnlineUser.h"

class TopList
{
public:
	TopList();
	~TopList();
	
	VOID		SetMaxListSize(INT size) { m_TopListData.m_MemberCount = size; }
	INT			GetMaxListSize() {	return m_TopListData.m_MemberCount;	}

	VOID		SetListRecord(INT size) { m_TopListData.m_RecordCount = size; }
	INT			GetListRecord() {	return m_TopListData.m_RecordCount;	}	

	VOID		SetTopListID(TopListID_t id) { m_TopListData.m_TopListID = id;	}
	TopListID_t GetTopListID() {	return m_TopListData.m_TopListID;	}
	
	VOID		SetTopListName(CHAR* pName)	{ strncpy( m_TopListData.m_Name, pName, MAX_TOP_LIST_NAME-1 ); }

	IDTable*	GetIDTable() { return &m_IDTable; }

	//得到当前排行榜中最小的那一个的GUID
	GUID_t			GetMinKeyOwner() { return m_MinKeyOwner; }

	VOID			SetEnterMinKey(INT MinKey) { m_TopListData.m_EnterMinKey = MinKey;  }
	INT				GetEnterMinKey()	{ 	return m_TopListData.m_EnterMinKey; }

	BOOL			Add(GUID_t id,VOID* pPtr);

	VOID*			Get(GUID_t id)
	{	
		return m_IDTable.Get(id) ;
	}

	VOID			CleanUp();

protected:
	TOP_LIST_ATTR	m_TopListData;				//排行榜进入条件
	IDTable			m_IDTable;					//快速索引表
	GUID_t			m_MinKeyOwner;				//排行榜中KEY最小的玩家GUID

};


//等级排行榜
class LevelTopList : public TopList
{
public:
	LevelTopList();
	~LevelTopList();

	VOID				CleanUp();

	_LEVEL_TOP_DATA*	GetData(INT id)
	{	
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	
	_LEVEL_TOP_DATA*	GetNullNode()
	{ 
		return &m_UserDataEx[m_TopListData.m_RecordCount]; 
	}

	VOID				ModifyMinKeyOwner(GUID_t guid,UINT Key);

	// 更新等级排行榜
	VOID				UpDataLevelTopList( USER* pUser );

	// 到固定时间获得排行榜的排名，不能修改m_UserDataEx的排序，会使快速索引表索引出错误
	VOID				HaveOldLevelRank();

private:

	//扩展数据
	_LEVEL_TOP_DATA		m_UserDataEx[TOP_USDR_NUM];

};


//游戏币排行榜,, 其他游戏币排行榜的数据结构和这个数据结构是一样的，可以重复利用
class MoneyTopList : public TopList
{
public:
	MoneyTopList();
	~MoneyTopList();

	VOID				CleanUp();

	_MONEY_TOP_DATA*	GetData(INT id)
	{	
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_MONEY_TOP_DATA*	GetNullNode ()
	{ 	
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID				ModifyMinKeyOwner(GUID_t guid,UINT Key);

	//刷新金钱排行榜(一个函数无法获得不同类型的金钱，所以必须通过参数传递)
	VOID				UpDataMoneyTopList(USER* pUser, UINT uMoney);

	VOID				HaveOldMoneyRank();

private:
	//扩展数据
	_MONEY_TOP_DATA m_UserDataEx[TOP_USDR_NUM];			
};

//声望排行榜
class ReputeTopList : public TopList
{
public:
	ReputeTopList();
	~ReputeTopList();

	VOID					CleanUp();

	_REPUTE_TOP_DATA*		GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_REPUTE_TOP_DATA*	GetNullNode ()
	{ 	
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID				ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:
	//扩展数据
	_REPUTE_TOP_DATA m_UserDataEx[TOP_USDR_NUM];

};

//宠物排行榜
class PetTopList : public TopList
{
public:
	PetTopList();
	~PetTopList();

	VOID					CleanUp();

	_PET_TOP_DATA*			GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_PET_TOP_DATA*			GetNullNode()
	{
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID					ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:

	_PET_TOP_DATA m_UserDataEx[TOP_USDR_NUM];

};

//副本排行榜
class EctypeTopList : public TopList
{
public:
	EctypeTopList();
	~EctypeTopList();

	VOID					CleanUp();

	_ECTYPE_TOP_DATA*		GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_ECTYPE_TOP_DATA*		GetNullNode()
	{
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID					ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:

	_ECTYPE_TOP_DATA m_UserDataEx[TOP_USDR_NUM];
};


//侠义排行榜
class ErrantryTopList : public TopList
{
public:
	ErrantryTopList();
	~ErrantryTopList();

	VOID					CleanUp();

	_ERRANTRY_TOP_DATA*		GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_ERRANTRY_TOP_DATA*		GetNullNode()
	{
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID					ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:

	_ERRANTRY_TOP_DATA m_UserDataEx[TOP_USDR_NUM];
};

//罪恶排行榜
class CrimeTopList : public TopList
{
public:
	CrimeTopList();
	~CrimeTopList();

	VOID					CleanUp();

	_CRIME_TOP_DATA*		GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_CRIME_TOP_DATA*		GetNullNode()
	{
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID					ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:

	_CRIME_TOP_DATA m_UserDataEx[TOP_USDR_NUM];
};


//人气排行榜
class FavorTopList : public TopList
{
public:
	FavorTopList();
	~FavorTopList();

	VOID					CleanUp();

	_FAVOR_TOP_DATA*		GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_FAVOR_TOP_DATA*		GetNullNode()
	{
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID					ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:

	_FAVOR_TOP_DATA m_UserDataEx[TOP_USDR_NUM];
};


//帮派排行榜
class GuildTopList : public TopList
{
public:
	GuildTopList();
	~GuildTopList();

	VOID					CleanUp();

	_GUILD_TOP_DATA*		GetData(INT id)
	{
		AssertEx(id>=0&&id<m_TopListData.m_MemberCount,"GetData");
		return &m_UserDataEx[id];
	}
	_GUILD_TOP_DATA*		GetNullNode()
	{
		AssertEx(m_TopListData.m_RecordCount<TOP_USDR_NUM,"GetData");
		return &m_UserDataEx[m_TopListData.m_RecordCount];
	}

	VOID					ModifyMinKeyOwner(GUID_t guid, UINT Key);

private:

	_GUILD_TOP_DATA m_UserDataEx[TOP_USDR_NUM];
};


#endif