

#ifndef __WGOPERATERESULT_H__
#define __WGOPERATERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class WGOperateResult : public Packet 
{
public:
	WGOperateResult( ){} ;
	virtual ~WGOperateResult( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_WG_OPERATE_RESULT ; }
	virtual UINT			GetPacketSize()const { return 	sizeof(INT)+sizeof(m_PlayerID); }
	
public:
	//使用数据接口
	INT			getResult(VOID) { return m_nResult; }
	VOID		setResult(INT nResult) { m_nResult = nResult; }

	VOID		SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
	PlayerID_t	GetPlayerID( ){ return m_PlayerID ; } ;

private:
	PlayerID_t	m_PlayerID;
	//数据
	INT			m_nResult;		// 返回值 OPERATE_RESULT
};


class WGOperateResultFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGOperateResult() ; }
	PacketID_t	GetPacketID()const { return PACKET_WG_OPERATE_RESULT ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(INT)+sizeof(PlayerID_t); }
};


class WGOperateResultHandler 
{
public:
	static UINT Execute( WGOperateResult* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
