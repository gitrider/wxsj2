#ifndef _GC_EQUIPSUITEXCHANGE_RESULT_H_
#define _GC_EQUIPSUITEXCHANGE_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

struct  EXCHANGE_EQUIP_INFO
{
	BYTE				m_EquipPoint;			//װ����
	BYTE				m_BagIndex;				//ж������װ����Bag�д�ŵ�λ��
	EXCHANGE_EQUIP_INFO()
	{
		m_EquipPoint = INVALID_ITEM_POS;
		m_BagIndex   = INVALID_ITEM_POS;
	}
};



namespace Packets
{
	class GCEquipSuitExchangeResult:public Packet
	{

	public:
		GCEquipSuitExchangeResult(){};
		virtual				~GCEquipSuitExchangeResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_EQUIPSUITEXCHANGERESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+HEQUIP_NUMBER*sizeof(EXCHANGE_EQUIP_INFO);
		}

	public:

		VOID			setResult(BYTE	Ret){ m_Result = Ret;}
		BYTE			getResult(){return m_Result;}

		VOID			setEquipInfo(EXCHANGE_EQUIP_INFO& nEquipInfo, UINT nPos)
		{ Assert(nPos<HEQUIP_NUMBER);
						memcpy(&m_ExchageEquipInfo[nPos],& nEquipInfo, sizeof(EXCHANGE_EQUIP_INFO));}
		EXCHANGE_EQUIP_INFO&	getEquipInfo(UINT i){Assert(i<HEQUIP_NUMBER); return m_ExchageEquipInfo[i];}
	private:
		BYTE				m_Result;				//�Ƿ�ɹ������ɹ����Ǵ����
		EXCHANGE_EQUIP_INFO m_ExchageEquipInfo[HEQUIP_NUMBER];
	};


	class GCEquipSuitExchangeResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipSuitExchangeResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_EQUIPSUITEXCHANGERESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)+HEQUIP_NUMBER*sizeof(EXCHANGE_EQUIP_INFO); }
	};


	class GCEquipSuitExchangeResultHandler
	{
	public:
		static UINT Execute( GCEquipSuitExchangeResult* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif