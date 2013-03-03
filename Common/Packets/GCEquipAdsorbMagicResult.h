#ifndef _GC_EQUIPADSORBMAGICRESULT_H_
#define _GC_EQUIPADSORBMAGICRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCEquipAdsorbMagicResult:	public Packet
	{
	public:
		GCEquipAdsorbMagicResult(){};
		virtual		~GCEquipAdsorbMagicResult(){};

		// 公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;
		
		virtual PacketID_t		GetPacketID()const			{ return PACKET_GC_EQUIPADSORBMAGICRESULT; }
		virtual UINT			GetPacketSize()const		{ return sizeof(EQUIPADSORMAGIC_RESULT); }

		VOID					setResult(EQUIPADSORMAGIC_RESULT Result)		{ m_Result = Result; }
		EQUIPADSORMAGIC_RESULT	getResult()					{ return m_Result; }

	private:
		EQUIPADSORMAGIC_RESULT	m_Result;

	};

	class GCEquipAdsorbMagicResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipAdsorbMagicResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPADSORBMAGICRESULT; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPADSORMAGIC_RESULT); }			
	};

	class GCEquipAdsorbMagicResultHandler
	{
	public:
		static UINT	Execute(GCEquipAdsorbMagicResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif