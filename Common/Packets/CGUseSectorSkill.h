///********************************************************************
//	created:	2007/10/31
//	created:	31:10:2007   17:23
//	filename: 	d:\project\MapServer\Server\CGUseSectorSkill.h
//	file path:	d:\project\MapServer\Server
//	file base:	CGUseSectorSkill
//	file ext:	h
//	author:		Richard
//	
//	purpose:	
//*********************************************************************/
//
//#ifndef CGUseSectorSkill_h__31_10_2007_17_23
//#define CGUseSectorSkill_h__31_10_2007_17_23
//
//
//#include "Type.h"
//#include "Packet.h"
//#include "PacketFactory.h"
//
//namespace Packets
//{
//	class CGUseSectorSkill : public Packet
//	{
//	public:
//		CGUseSectorSkill( ) : m_nTargetID(0) {};
//		virtual ~CGUseSectorSkill( ){};
//
//		//公用继承接口
//		virtual BOOL			Read( SocketInputStream& iStream ) ;
//		virtual BOOL			Write( SocketOutputStream& oStream )const ;
//		virtual UINT			Execute( Player* pPlayer ) ;
//
//		virtual PacketID_t		GetPacketID()const { return PACKET_CG_USE_SECTORSKILL; }
//		virtual UINT			GetPacketSize()const { return	sizeof(m_nTargetID);}
//
//	public:
//		ObjID_t			getTargetID(VOID) const {return m_nTargetID;};
//		VOID			setTargetID(ObjID_t const nID) {m_nTargetID = nID;};
//	private:
//		ObjID_t m_nTargetID;
//	};
//
//
//	class CGUseSectorSkillFactory : public PacketFactory 
//	{
//	public:
//		Packet*		CreatePacket() { return new CGUseSectorSkill() ; }
//		PacketID_t	GetPacketID()const { return PACKET_CG_USE_SECTORSKILL; };
//		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);};
//	};
//
//	class CGUseSectorSkillHandler 
//	{
//	public:
//		static UINT Execute( CGUseSectorSkill* pPacket, Player* pPlayer ) ;
//	};
//}
//
//using namespace Packets;
//
//
//#endif // CGUseSectorSkill_h__