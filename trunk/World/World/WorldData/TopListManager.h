
//-----------------------------------------------------------------------------
// 文件名 : TopListManager.h
// 模块	:	排行榜管理器
// 功能	 : 管理各排行榜
// 修改历史:
//创建:		王勇鸷
//-----------------------------------------------------------------------------

#ifndef __TOP_LIST_MANAGER__
#define __TOP_LIST_MANAGER__

#include "TopList.h"
#include "WorldTable.h"


class TopListManager
{
public:
	TopListManager();
	~TopListManager();

	BOOL				Init( );
	VOID				CleanUp( );

	BOOL				AddTopList( TOP_LIST_ATTR attr );	//添加一个排行榜
	BOOL				DelTopList( TopListID_t TopListID );//删除一个排行榜

	TopList*			GetTopList( TopListID_t TopListID );// 查找一个排行榜
	
private:
	LevelTopList		m_LevelTopList;					//等级排行榜
	MoneyTopList		m_MoenyTopList;					//财富排行榜
	ReputeTopList		m_ReputeTopList;				//声望排行榜
	PetTopList			m_PetTopList;					//宠物排行榜
	EctypeTopList		m_EctypeTopList;				//副本排行榜
	ErrantryTopList		m_ErrantryTopList;				//侠义排行榜
	CrimeTopList		m_CrimeTopList;					//罪恶排行榜
	MoneyTopList		m_BindMoneyTopList;				//绑定游戏币排行榜
	MoneyTopList		m_RMBTopList;					//人民币排行榜
	MoneyTopList		m_BindRMBTopList;				//绑定人民币排行榜
	FavorTopList		m_FavorTopList;					//人气排行榜
	GuildTopList		m_GuildTopList;					//帮派排行榜

	TopList*			m_pTopList[TOP_LIST_NUM];

};

extern TopListManager* g_pTopListManager;

#endif

