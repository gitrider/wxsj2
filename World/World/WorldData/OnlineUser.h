/********************************************************************************
 *	文件名：	OnlineUser.h
 *	全路径：	d:\Prj\World\World\WorldData\OnlineUser.h
 *	创建人：	
 *	创建时间：	
 *
 *	功能说明：	
 *	修改记录：	
 *				2005 年 11 月 11 日	17:38 增加玩家断线处理接口和玩家退出处理接口
 *				2006 年 1 月 19 日	15:40 增加 OnlineUser 的功能 (UserLeaveTeam)
 *				2006 年 1 月 21 日	10:09 增加 OnlineUser 的功能
*********************************************************************************/

#ifndef __ONLINEUSER_H__
#define __ONLINEUSER_H__

#include "Type.h"

#include "Guild.h"
#include "IDTable.h"
#include "StrTable.h"
#include "Array_Base.h"
#include "User.h"

#define MAX_USER_DATA	3000

class OnlineUser
{
public :
	OnlineUser( ) ;
	~OnlineUser( ) ;

	BOOL			Init( ) ;
	BOOL			HeartBeat( DWORD dwTime=0 ) ;


	//-----------------------------------------------
	// add by wuwenbin, 就放在这里做个纪念吧
	//typedef INT PostCode;
	//typedef std::vector<USER*> PUSERVector;
	//typedef std::map<PostCode, PUSERVector> UserGroupByPostCode;
	//-----------------------------------------------

	typedef Array_Base<USER*> PUSERVector;

public :
	USER*			FindUser( GUID_t guid ) ;
	USER*			FindUser( const CHAR* szName ) ;

	//测试
	USER*			LoadUser( GUID_t guid, INT nCountry, INT nMenPai ,CHAR* szAccount) ;
	void			InitUserData( FULLUSERDATA* pData, GUID_t guid  , INT nCountry, INT nMenPai) ;
	//测试

	void			RemoveUser( ID_t ServerID );
	void			RemoveUser(ID_t ServerID,USER_STATUS UserUS );

	void			ChangeUserStatus(ID_t ServerID,USER_STATUS UserUS );


	USER*			NewUser( ) ;
	void			DelUser( USER* pUser ) ;


	FULLUSERDATA*	NewData( ) ;
	void			DelData( FULLUSERDATA* pData ) ;

	BOOL			Register( USER* pUser ) ;
	BOOL			UnRegister( USER* pUser ) ;

	UINT			Select( const CHAR* szName, RETURN_FINGER_PLAYERLIST* pPlayerList, BOOL bPreciseFlag, INT nPosition = 0 );

	UINT			Select( INT nMenPai, GuildID_t GuildID, CHAR Sex, INT m_nBottomLevel, INT m_nTopLevel,
							RETURN_FINGER_PLAYERLIST* pPlayerList, INT nPosition = 0 );

	BOOL			Select(INT iPostCode, 
							PUSERVector& returnUSERVector, /// 返回的结果集
							BOOL& bContinue,			   /// 是否有下一页
							INT& iReturnPos,			   /// 返回的结束Pos
							INT iPostions = 0);			   /// 输入的起始Pos

public :
	//玩家登陆游戏
	BOOL			OnUserLogin( USER* pUser ) ;

	//玩家切换场景
	BOOL			OnUserChangeScene( USER* pUser, SceneID_t OldSceneID, SceneID_t NewSceneID ) ;

	//玩家断线处理接口，fancy
	VOID			OnUserOffLine( USER* pUser );

	//玩家退出处理接口，fancy
	VOID			OnUserRemove( USER* pUser );

	// 玩家离开队伍接口，fancy
	BOOL			UserLeaveTeam( USER* pUser );
	
	//玩家暂时离开队伍
	BOOL			UserMomentLeave(USER* pUser);

	// 提升新队长，fancy
	BOOL			AppointTeamLeader( USER* pOldLeader, USER* pNewLeader );

	INT				GetTotalCount(){ return m_UserCount ; } ;
	USER*			GetUserByIndex(INT nIndex){ 
		if( nIndex<0 || nIndex>=MAX_WORLD_USER )
			return NULL ;
		return &m_aUser[nIndex] ;
	};

	USER*	FindUserByAccount(const CHAR* szAccount);

public:
	//////////////////////////////////////////////////////////////////////////
	//踢人功能相关
	//////////////////////////////////////////////////////////////////////////
	BOOL			BeginKickUser(USER* pUser);
	BOOL			IsKickUser(USER* pUser);
	BOOL			EndKickUser(USER* pUser, ID_t serverId, BYTE ret);
	BOOL			ForceEndKickUser(USER* pUser);
	BOOL			SendBillingEnd(USER* pUser, BOOL bForce = FALSE);
	BOOL			SendForceBillingEnd(const CHAR* pAccName, INT acclv);
	BOOL			SendBillingEnd(const CHAR* pAccName, INT acclv);
	//ShareMem锁定的Server重新发送踢人包
	BOOL			ReSendKickUser_LockByShareMem(USER* pUser);

//protected:
//	BOOL	FindUserFromGroups(USER* pUser);
//	BOOL	AddUserToGroups(USER* pUser);
//	BOOL	RemoveUserFromGroups(USER* pUser);

protected :
	USER				m_aUser[MAX_WORLD_USER] ;
	INT					m_UserCount ;
	INT					m_MaxUserSize;

	FULLUSERDATA*		m_pUserData[MAX_USER_DATA] ;
	INT					m_iLeftUserDataCount ;
	INT					m_iUserDataOffset;

	IDTable				m_GUIDTable ;
	StrTable			m_NameTable ;
	StrTable			m_AccTable;

	CMyTimer			m_HeartBeatTimer ;
};

extern OnlineUser*	g_pOnlineUser ;

#endif


