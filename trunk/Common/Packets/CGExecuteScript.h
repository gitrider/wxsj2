

#ifndef __CGEXECUTESCRIPT_H__
#define __CGEXECUTESCRIPT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Script.h"


namespace Packets
{


class CGExecuteScript : public Packet 
{
public:
	CGExecuteScript( ){} ;
	virtual ~CGExecuteScript( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXECUTESCRIPT ; }
	virtual UINT			GetPacketSize()const { return m_Script.GetSize() ; }
	
public:
	//ʹ�����ݽӿ�
	X_SCRIPT*				GetScript( ){ return &m_Script ; }

private:
	//����
	X_SCRIPT				m_Script ;

};


class CGExecuteScriptFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGExecuteScript() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_EXECUTESCRIPT ; }
	UINT		GetPacketMaxSize()const { return sizeof(X_SCRIPT) ; }
};


class CGExecuteScriptHandler 
{
public:
	static UINT Execute( CGExecuteScript* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
