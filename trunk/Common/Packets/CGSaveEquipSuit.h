#ifndef _CG_SAVE_EUIPSUIT_H_
#define _CG_SAVE_EUIPSUIT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGSaveEquipSuit:public Packet
	{

	public:
		CGSaveEquipSuit(){};
		virtual				~CGSaveEquipSuit(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SAVEEQUIPSUIT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(_SUIT_SETTING);
		}
		
	public:
		VOID				setSuitNum(BYTE nSuitNum){ m_SuitNum = nSuitNum;}
		BYTE				getSuitNum(){return m_SuitNum;}

		VOID				setSuitSetting(_SUIT_SETTING& nInfo){ memcpy(&m_SuitSetting, &nInfo, sizeof(_SUIT_SETTING));}
		_SUIT_SETTING&		getSuitSetting(){ return m_SuitSetting; }
	private:
		BYTE				m_SuitNum;		//±àºÅ
		_SUIT_SETTING       m_SuitSetting;
	};


	class CGSaveEquipSuitFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGSaveEquipSuit(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_SAVEEQUIPSUIT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)+sizeof(_SUIT_SETTING); }
	};


	class CGSaveEquipSuitHandler
	{
	public:
		static UINT	Execute( CGSaveEquipSuit* pPacket, Player* pPlayer );
	};

}


#endif