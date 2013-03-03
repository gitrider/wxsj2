// CGMissionAccept.h
// 
// ��������
// 
//////////////////////////////////////////////////////

#ifndef __CGMISSIONACCEPT_H__
#define __CGMISSIONACCEPT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMissionAccept : public Packet
	{
	public:
		CGMissionAccept( ){}
		virtual ~CGMissionAccept( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONACCEPT ; }
		virtual UINT			GetPacketSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ); }

	public:
		ObjID_t					getNPCID( VOID )const{ return m_idNPC; }
		VOID					setNPCID( ObjID_t idNPC ) { m_idNPC = idNPC; }

		ScriptID_t				getScriptID( VOID )const{ return m_idScript; }
		VOID					setScriptID( ScriptID_t idScript ) { m_idScript = idScript; }

	private:
		ObjID_t					m_idNPC;
		ScriptID_t				m_idScript;		// ����Ľű�ID
	};


	class CGMissionAcceptFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMissionAccept() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONACCEPT; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ); }
	};

	class CGMissionAcceptHandler 
	{
	public:
		static UINT Execute( CGMissionAccept* pPacket, Player* pPlayer ) ;
	};
}

#endif