

#ifndef __CONNECTMANAGER_H__
#define __CONNECTMANAGER_H__

#include "Type.h"
#include "LoginPlayerManager.h"
#include "ReLogin.h"


//����������ӽ���Ŀͻ���
class ConnectManager : public Thread
{
public :
	ConnectManager( ) ;
	~ConnectManager( ) ;

	//��ʼ��ģ��
	BOOL			Init( ) ;
	//ģ�鴦��ѭ��
	virtual VOID	run () ;
	//ģ���˳�
	virtual VOID	stop( ){ m_Active = FALSE ; } ;
	//ģ���˳�����
	VOID			Quit( ) ;

	//�жϵ�ǰģ���Ƿ��ڻ״̬
	BOOL			IsActive( ){ return m_Active ; } ;
private :
	BOOL			m_Active ;//�Ƿ��ı�־


};

extern ConnectManager* g_pConnectManager ;

#endif
