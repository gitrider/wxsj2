
//-----------------------------------------------------------------------------
// �ļ��� : TopListManager.h
// ģ��	:	���а������
// ����	 : ��������а�
// �޸���ʷ:
//����:		�����
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

	BOOL				AddTopList( TOP_LIST_ATTR attr );	//���һ�����а�
	BOOL				DelTopList( TopListID_t TopListID );//ɾ��һ�����а�

	TopList*			GetTopList( TopListID_t TopListID );// ����һ�����а�
	
private:
	LevelTopList		m_LevelTopList;					//�ȼ����а�
	MoneyTopList		m_MoenyTopList;					//�Ƹ����а�
	ReputeTopList		m_ReputeTopList;				//�������а�
	PetTopList			m_PetTopList;					//�������а�
	EctypeTopList		m_EctypeTopList;				//�������а�
	ErrantryTopList		m_ErrantryTopList;				//�������а�
	CrimeTopList		m_CrimeTopList;					//������а�
	MoneyTopList		m_BindMoneyTopList;				//����Ϸ�����а�
	MoneyTopList		m_RMBTopList;					//��������а�
	MoneyTopList		m_BindRMBTopList;				//����������а�
	FavorTopList		m_FavorTopList;					//�������а�
	GuildTopList		m_GuildTopList;					//�������а�

	TopList*			m_pTopList[TOP_LIST_NUM];

};

extern TopListManager* g_pTopListManager;

#endif

