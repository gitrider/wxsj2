/********************************************************************
	created:	2008/03/13
	created:	13:3:2008   11:43
	filename: 	Common\Packets\GCHorseLevelUp.h
	file path:	Common\Packets
	file base:	GCHorseLevelUp
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef GCHorseLevelUp_h__
#define GCHorseLevelUp_h__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
		class GCHorseLevelUp : public Packet
		{

		public:
			GCHorseLevelUp()
				:m_HorseLevel(0),
				 m_OnwerObjID(INVALID_ID)
			{
				 
			};
			virtual	~GCHorseLevelUp(){};

			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream ) const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID() const { return PACKET_GC_HORSE_LEVEUP; }

			virtual UINT			GetPacketSize() const 
			{
				return sizeof(GCHorseLevelUp)-sizeof(Packet);
			}

		public:
			
			VOID			SetOnwerObjId(ObjID_t	id){m_OnwerObjID = id;};
			ObjID_t			GetOnwerObjId(){return m_OnwerObjID;}
			
			VOID			SetHorseLevel(UINT lvl){m_HorseLevel = lvl;}
			UINT			GetHorseLevel(){return m_HorseLevel;}
			

		private:
			UINT			m_HorseLevel;		//马当前等级
			ObjID_t			m_OnwerObjID;		//主人的ObjID

		};


		class GCHorseLevelUpFactory:public PacketFactory
		{
		public:
			Packet*		CreatePacket() { return new GCHorseLevelUp(); }
			PacketID_t	GetPacketID() const { return PACKET_GC_HORSE_LEVEUP; }
			UINT		GetPacketMaxSize() const {return sizeof(GCHorseLevelUp)-sizeof(Packet); }
		};

		class GCHorseLevelUpHandler
		{
		public:
			static UINT Execute( GCHorseLevelUp* pPacket, Player* pPlayer );
		};
}

#endif // GCHorseLevelUp_h__