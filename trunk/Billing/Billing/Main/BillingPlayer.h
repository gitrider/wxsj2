


#ifndef __BILLINGPLAYER_H__
#define __BILLINGPLAYER_H__

#include "Type.h"
#include "Player.h"
#include "Config.h"


//计费玩家指的是另外的计费端程序在当前程序内的数据表现

class BillingPlayer : public Player
{
public :
	BillingPlayer( BOOL bIsServer=TRUE ) ;
	~BillingPlayer( ) ;

	//数据接收
	virtual BOOL	ProcessInput( ) ;
	//数据发送
	virtual BOOL	ProcessOutput( ) ;
	//消息处理
	virtual BOOL	ProcessCommand( BOOL Option = TRUE ) ;

	//清除
	virtual VOID	CleanUp( ) ;

	//逻辑接口
	virtual BOOL	HeartBeat( UINT uTime=0 ) ;


public :
	//通用接口


	virtual BOOL	IsServerPlayer( ){ return FALSE ; } ;
	virtual BOOL	IsBillingPlayer( ){ return TRUE ; } ;
	virtual	BOOL	IsLoginPlayer( ){return FALSE; };

	//设置当前计费玩家的状态
	VOID			SetPlayerStatus( uint status ){ m_Status = status ; } ;
	UINT			GetPlayerStatus( ) { return m_Status ; } ;
	VOID			ClearKeepLiveSendNum(){m_KeepLiveSendNum=0;}

	//判断当前计费玩家是否有效
	virtual BOOL	IsValid( ) ;

	//向此计费端程序发送消息
	//此接口不支持线程间同步，因此只能有当前线程来调用执行
	virtual BOOL	SendPacket( BillPacket* pPacket ) ;

private :
	//计费端状态
	uint			m_Status ;

	UINT			m_LastKeepLiveTime;
	INT				m_KeepLiveSendNum;
};




#endif
