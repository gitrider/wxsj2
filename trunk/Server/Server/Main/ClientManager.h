

#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include "Type.h"
#include "IncomingPlayerManager.h"


class ClientManager : public Thread
{
public :
	ClientManager( ) ;
	~ClientManager( ) ;

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

extern ClientManager* g_pClientManager ;

#endif
