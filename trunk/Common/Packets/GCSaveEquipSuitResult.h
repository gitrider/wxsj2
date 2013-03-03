#ifndef _GC_SAVEEQUIPSUIT_RESULT_H_
#define _GC_SAVEEQUIPSUIT_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"




namespace Packets
{
	class GCSaveEquipSuitResult:public Packet
	{

	public:
		GCSaveEquipSuitResult(){};
		virtual				~GCSaveEquipSuitResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SAVEEQUIPSUITRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*2+sizeof(CHAR)*MAX_SUIT_NAME_SIZE+sizeof(_ITEM_GUID)*HEQUIP_NUMBER;
		}
	public:

		VOID			setResult(BYTE	Ret){ m_Result = Ret;}
		BYTE			getResult(){return m_Result;}

		VOID			setSuitNum(BYTE nSuitNum){ m_SuitNum = nSuitNum;}
		BYTE			getSuitNum(){return m_SuitNum;}

		VOID			setSuitSetting(const _SUIT_SETTING&	nSetting){ memcpy((void*)&m_nSuitSetting , (void*)&nSetting, sizeof(_SUIT_SETTING));}
		_SUIT_SETTING&	getSuitSetting(){return m_nSuitSetting;}

	private:
		BYTE				m_Result;				//是否成功，不成功则是错误号
		BYTE				m_SuitNum;
		_SUIT_SETTING		m_nSuitSetting;
	};


	class GCSaveEquipSuitResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSaveEquipSuitResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_SAVEEQUIPSUITRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*2+sizeof(CHAR)*MAX_SUIT_NAME_SIZE+
			sizeof(_ITEM_GUID)*HEQUIP_NUMBER; }
	};


	class GCSaveEquipSuitResultHandler
	{
	public:
		static UINT Execute( GCSaveEquipSuitResult* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif