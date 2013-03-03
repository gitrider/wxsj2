

#ifndef __CGCOMMAND_H__
#define __CGCOMMAND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_COMMAND_SIZE 128

namespace Packets
{


class CGCommand : public Packet 
{
public:
	CGCommand( ){ m_CommandSize=0; } ;
	virtual ~CGCommand( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_CG_COMMAND ; }
	virtual UINT			GetPacketSize() const { return	sizeof(BYTE)+
															m_CommandSize ; }
	
public:
	//ʹ�����ݽӿ�
	VOID					SetCommandSize( BYTE size ){ m_CommandSize = size ; } ;
	BYTE					GetCommandSize( ){ return m_CommandSize ; } ;

	VOID					SetCommand( CHAR* command ){ strncpy( m_Command, command, MAX_COMMAND_SIZE-1) ; } ;
	CHAR*					GetCommand( ){ return m_Command ; } ;


private:
	//����
	BYTE					m_CommandSize ;					//��ǰ����ĳ���
	CHAR					m_Command[MAX_COMMAND_SIZE] ;	//��������

};


class CGCommandFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGCommand() ; }
	PacketID_t	GetPacketID() const { return PACKET_CG_COMMAND ; }
	UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+
													MAX_COMMAND_SIZE*sizeof(CHAR) ; }
};


class CGCommandHandler 
{
public:
	static UINT Execute( CGCommand* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
