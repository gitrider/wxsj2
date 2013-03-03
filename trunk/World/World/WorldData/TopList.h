
//-----------------------------------------------------------------------------
// �ļ��� : TopList.h
// ģ��	:	���а�
// ����	 :  �����а��¼
// �޸���ʷ:
//����:		�����
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

	//�õ���ǰ���а�����С����һ����GUID
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
	TOP_LIST_ATTR	m_TopListData;				//���а��������
	IDTable			m_IDTable;					//����������
	GUID_t			m_MinKeyOwner;				//���а���KEY��С�����GUID

};


//�ȼ����а�
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

	// ���µȼ����а�
	VOID				UpDataLevelTopList( USER* pUser );

	// ���̶�ʱ�������а�������������޸�m_UserDataEx�����򣬻�ʹ��������������������
	VOID				HaveOldLevelRank();

private:

	//��չ����
	_LEVEL_TOP_DATA		m_UserDataEx[TOP_USDR_NUM];

};


//��Ϸ�����а�,, ������Ϸ�����а�����ݽṹ��������ݽṹ��һ���ģ������ظ�����
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

	//ˢ�½�Ǯ���а�(һ�������޷���ò�ͬ���͵Ľ�Ǯ�����Ա���ͨ����������)
	VOID				UpDataMoneyTopList(USER* pUser, UINT uMoney);

	VOID				HaveOldMoneyRank();

private:
	//��չ����
	_MONEY_TOP_DATA m_UserDataEx[TOP_USDR_NUM];			
};

//�������а�
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
	//��չ����
	_REPUTE_TOP_DATA m_UserDataEx[TOP_USDR_NUM];

};

//�������а�
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

//�������а�
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


//�������а�
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

//������а�
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


//�������а�
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


//�������а�
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