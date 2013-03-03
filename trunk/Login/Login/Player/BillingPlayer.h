


#ifndef __BILLINGPLAYER_H__
#define __BILLINGPLAYER_H__

#include "Type.h"
#include "Player.h"
#include "Config.h"


//�Ʒ����ָ��������ļƷѶ˳����ڵ�ǰ�����ڵ����ݱ���

class BillingPlayer : public Player
{
public :
	BillingPlayer( BOOL bIsServer=TRUE ) ;
	~BillingPlayer( ) ;

	//���ݽ���
	virtual BOOL	ProcessInput( ) ;
	//���ݷ���
	virtual BOOL	ProcessOutput( ) ;
	//��Ϣ����
	virtual BOOL	ProcessCommand( BOOL Option = TRUE ) ;

	//���
	virtual VOID	CleanUp( ) ;

	//�߼��ӿ�
	virtual BOOL	HeartBeat( UINT uTime=0 ) ;


public :
	//ͨ�ýӿ�


	virtual BOOL	IsServerPlayer( ){ return FALSE ; } ;
	virtual BOOL	IsBillingPlayer( ){ return TRUE ; } ;
	virtual	BOOL	IsLoginPlayer( ){return FALSE; };

	//���õ�ǰ�Ʒ���ҵ�״̬
	VOID			SetPlayerStatus( uint status ){ m_Status = status ; } ;
	UINT			GetPlayerStatus( ) { return m_Status ; } ;
	VOID			ClearKeepLiveSendNum(){m_KeepLiveSendNum=0;}

	//�жϵ�ǰ�Ʒ�����Ƿ���Ч
	virtual BOOL	IsValid( ) ;

	//��˼ƷѶ˳�������Ϣ
	//�˽ӿڲ�֧���̼߳�ͬ�������ֻ���е�ǰ�߳�������ִ��
	virtual BOOL	SendPacket( BillPacket* pPacket ) ;

private :
	//�ƷѶ�״̬
	uint			m_Status ;

	UINT			m_LastKeepLiveTime;
	INT				m_KeepLiveSendNum;
};




#endif
