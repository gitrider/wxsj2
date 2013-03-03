/********************************************************************************
 *	�ļ�����	Team.h
 *	ȫ·����	d:\Prj\Server\Server\Team\Team.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 4 ��	9:25
 *
 *	����˵����	����
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __TEAM_H__
#define __TEAM_H__

#include "Type.h"

//��ӳ�Ա

struct TEAMMEMBER
{
	GUID_t							m_GUID;
	ObjID_t							m_ObjID;
	SceneID_t						m_SceneID;

	TEAMMEMBER( )
	{
		CleanUp( );
	}

	VOID							CleanUp( )
	{
		m_GUID = INVALID_ID;
		m_ObjID = INVALID_ID;
		m_SceneID= INVALID_ID; 
	}
};

class Obj_Human;

struct _FOLLOWEDMEMBER
{
	GUID_t							m_GUID;
	Obj_Human*						m_pHuman;

	_FOLLOWEDMEMBER( )
	{
		CleanUp( );
	}

	VOID							CleanUp( )
	{
		m_GUID = INVALID_ID;
		m_pHuman= NULL; 
	}
};

#define LEADER_ID 0

class TeamInfo
{
public:
	TeamInfo( ) { CleanUp(); }
	~TeamInfo( ) { }

	inline VOID						CleanUp( );

	// �����Լ��� GUID
	inline VOID						SetMyGUID( GUID_t guid );

	// �����Լ����ڵĳ�������ҵ�һ����ӵ�ʱ��Ҫ��һ�Σ��� TeamID ��ʱ��
	inline VOID						SetMySceneID( SceneID_t sid );

	inline VOID						DisMiss( );

	// �ж��Ƿ��ڶ�����
	inline BOOL						HasTeam( ) const;

	// �ж϶����Ƿ��Ѿ���Ա
	inline BOOL						IsFull( );

	// �ж��Ƿ�ӳ�
	inline BOOL						IsLeader( ) const;

	// �õ����� ID
	inline TeamID_t					GetTeamID( ) const;

	// ���ö��� ID
	inline VOID						SetTeamID( const TeamID_t teamid );

	// �õ���������
	inline INT						GetTeamMemberCount( ) const;

	// �õ�ͬ�������ѵ��������������Լ���
	inline INT						GetSceneMemberCount() const;

	// �õ�ͬ�������ѵ� ObjID �б��������Լ���
	inline const ObjID_t			GetSceneMemberObjID( const INT memberindex ) const;

	// ����һ������
	VOID							AddMember( const TEAMMEMBER* pMember );

	// ����һ������
	VOID							DelMember( GUID_t guid );

	// �����¶ӳ�
	VOID							Appoint( GUID_t guid );

	// ��Ա��ʼ�л�����
	VOID							StartChangeScene( GUID_t guid );
	
	// ��Ա�����³���
	VOID							EnterScene( GUID_t guid, SceneID_t SceneID, ObjID_t oid );

	// ���Ѷ���
	VOID							MemberOffLine( GUID_t guid );

	// �õ��ӳ�����Ϣ
	inline const TEAMMEMBER*		Leader( ) const;

	// �õ�ĳ����Ա����Ϣ
	inline const TEAMMEMBER*		GetTeamMember( const INT memberindex ) const;

	// ͨ�� GUID �õ�ĳ����Ա
	inline const TEAMMEMBER*		GetTeamMemberByGUID( const GUID_t guid ) const;

	// �����Լ��Ķ������״̬
	inline VOID						SetTeamFollowFlag(BOOL flag);

	// ����Լ��Ķ������״̬
	inline const BOOL				GetTeamFollowFlag() const;

	// ����һ�������Ա
	VOID							AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember );

	// �Ƴ�ĳ�������Ա
	VOID							DelFollowedMember( GUID_t guid );
	// �õ������Ա������
	inline const INT				GetFollowedMembersCount() const;

	// �õ���i������Ķ���
	inline const _FOLLOWEDMEMBER*	GetFollowedMember( INT i ) const;

	// �õ��Լ�����Ķ���
	inline const Obj_Human*			GetMyGuide() const;

	// ������еĸ�������б�
	inline VOID						ClearFollowedMembers();

	// ���÷���ģʽ
	inline VOID						SetDistribMode(BYTE nMode){ m_DistribMode = nMode;}

	// ��÷���ģʽ
	inline const BYTE				GetDistribMode() const { return m_DistribMode;}

	// ���÷���ģʽ
	inline VOID						SetLastOwnerPos(BYTE nPos){ m_LastOwnerPos = nPos;}

	// ��÷���ģʽ
	inline const BYTE				GetLastOwnerPos() const { return m_LastOwnerPos;}
protected:

	// ���ӵ�ͬ���������б�	
	inline VOID						AddSceneMember( ObjID_t oid );

	// ����һ��ͬ��������
	inline VOID						DelSceneMember( ObjID_t oid );

	// ���������ָ��
	inline VOID						EraseFollowedMemberPointer( GUID_t guid );
private:
	TeamID_t						m_TeamID;						// �����
	INT								m_MemberCount;					// ��Ա����
	TEAMMEMBER						m_aMember[MAX_TEAM_MEMBER];		// ��Ա������Ϣ

	GUID_t							m_MyGUID;						// �Լ��� GUID
	SceneID_t						m_MySceneID;					// �Լ��ĳ��� ID
	INT								m_SceneMemberCount;				// ͬ������Ա����
	ObjID_t							m_aScenePlayer[MAX_TEAM_MEMBER];// ͬ������Ա�� ObjID �б�

	BOOL							m_bTeamFollowFlag;				// �����Ƿ������Ӹ���״̬�ı��
	INT								m_nFollowedMembersCount;
	_FOLLOWEDMEMBER					m_FollowedMembers[MAX_TEAM_MEMBER];	// �������б�

	BYTE							m_DistribMode;					//����ģʽ
	BYTE							m_LastOwnerPos;					//��һ�η���λ��
};

#include "Team.inl"
#endif // __TEAM_H__
