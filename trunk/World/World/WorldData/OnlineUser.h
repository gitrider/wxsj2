/********************************************************************************
 *	�ļ�����	OnlineUser.h
 *	ȫ·����	d:\Prj\World\World\WorldData\OnlineUser.h
 *	�����ˣ�	
 *	����ʱ�䣺	
 *
 *	����˵����	
 *	�޸ļ�¼��	
 *				2005 �� 11 �� 11 ��	17:38 ������Ҷ��ߴ���ӿں�����˳�����ӿ�
 *				2006 �� 1 �� 19 ��	15:40 ���� OnlineUser �Ĺ��� (UserLeaveTeam)
 *				2006 �� 1 �� 21 ��	10:09 ���� OnlineUser �Ĺ���
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
	// add by wuwenbin, �ͷ����������������
	//typedef INT PostCode;
	//typedef std::vector<USER*> PUSERVector;
	//typedef std::map<PostCode, PUSERVector> UserGroupByPostCode;
	//-----------------------------------------------

	typedef Array_Base<USER*> PUSERVector;

public :
	USER*			FindUser( GUID_t guid ) ;
	USER*			FindUser( const CHAR* szName ) ;

	//����
	USER*			LoadUser( GUID_t guid, INT nCountry, INT nMenPai ,CHAR* szAccount) ;
	void			InitUserData( FULLUSERDATA* pData, GUID_t guid  , INT nCountry, INT nMenPai) ;
	//����

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
							PUSERVector& returnUSERVector, /// ���صĽ����
							BOOL& bContinue,			   /// �Ƿ�����һҳ
							INT& iReturnPos,			   /// ���صĽ���Pos
							INT iPostions = 0);			   /// �������ʼPos

public :
	//��ҵ�½��Ϸ
	BOOL			OnUserLogin( USER* pUser ) ;

	//����л�����
	BOOL			OnUserChangeScene( USER* pUser, SceneID_t OldSceneID, SceneID_t NewSceneID ) ;

	//��Ҷ��ߴ���ӿڣ�fancy
	VOID			OnUserOffLine( USER* pUser );

	//����˳�����ӿڣ�fancy
	VOID			OnUserRemove( USER* pUser );

	// ����뿪����ӿڣ�fancy
	BOOL			UserLeaveTeam( USER* pUser );
	
	//�����ʱ�뿪����
	BOOL			UserMomentLeave(USER* pUser);

	// �����¶ӳ���fancy
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
	//���˹������
	//////////////////////////////////////////////////////////////////////////
	BOOL			BeginKickUser(USER* pUser);
	BOOL			IsKickUser(USER* pUser);
	BOOL			EndKickUser(USER* pUser, ID_t serverId, BYTE ret);
	BOOL			ForceEndKickUser(USER* pUser);
	BOOL			SendBillingEnd(USER* pUser, BOOL bForce = FALSE);
	BOOL			SendForceBillingEnd(const CHAR* pAccName, INT acclv);
	BOOL			SendBillingEnd(const CHAR* pAccName, INT acclv);
	//ShareMem������Server���·������˰�
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


