
#ifndef __SCENEPLAYERMANAGER_H__
#define __SCENEPLAYERMANAGER_H__


#include "Type.h"
#include "PlayerManager.h"



//�����е����Ⱥ����ģ��
//
class ScenePlayerManager : public PlayerManager
{
public :
	ScenePlayerManager( ) ;
	~ScenePlayerManager( ) ;

	//�������ӿ�
	BOOL				Select( ) ;
	//���ݽ��մ���
	BOOL				ProcessInputs( ) ;
	//���ݷ��ʹ���
	BOOL				ProcessOutputs( ) ;
	//�쳣���Ӵ���
	BOOL				ProcessExceptions( ) ;
	//��Ϣ����
	BOOL				ProcessCommands( ) ;
	//�߼�����
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;


public :
	//ͨ�ýӿ�

	//���õ�ǰ����ID
	VOID				SetSceneID( SceneID_t SceneID ){ m_SceneID = SceneID ; } ;
	//��ȡ����ID
	SceneID_t			GetSceneID( ){ return m_SceneID ; } ;

	//����һ�����
	BOOL				AddPlayer( Player* pPlayer ) ;
	BOOL				AddPlayerSocket( SOCKET fd ) ;
	//ɾ��һ����ң������������п���ת�Ƶ���ĳ�����
	BOOL				DelPlayer( PlayerID_t pid ) ;
	BOOL				DelPlayerSocket( SOCKET fd ) ;

	//�Ͽ�һ����ҡ�����������
	BOOL				RemovePlayer( Player* pPlayer, BOOL bReal=FALSE ) ;
	VOID				RemoveAllPlayer( ) ;


	INT					GetCount( ){ return m_nFDSize ; } ;


	VOID				Lock(){ m_Lock.Lock() ; } ;
	VOID				Unlock(){ m_Lock.Unlock() ; } ;

private :
	//
	//��������
	enum{
		SELECT_BAK = 0,	//��ǰϵͳ��ӵ�е������������
		SELECT_USE = 1,	//����select���õľ������
		SELECT_MAX = 2, //�ṹʹ������
	};
	fd_set		m_ReadFDs[SELECT_MAX];
	fd_set		m_WriteFDs[SELECT_MAX];
	fd_set		m_ExceptFDs[SELECT_MAX];

	timeval		m_Timeout[SELECT_MAX];

	SOCKET		m_MinFD;
	SOCKET		m_MaxFD;

	INT				m_nFDSize ;
	//��������
	//

	SceneID_t	m_SceneID ;

	MyLock		m_Lock ;

//
/////////////////////////////////////////////////////////////////////////////////
public :



};



#endif
