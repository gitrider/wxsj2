#ifndef _GC_USEEXTRABAG_RESULT_H_
#define _GC_USEEXTRABAG_RESULT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCUseExtraBagResult:public Packet
	{

	public:
		GCUseExtraBagResult(){};
		virtual				~GCUseExtraBagResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_USEEXTRABAGRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(INT)*2+sizeof(BYTE);
		}

	public:
		VOID			setDestBagIndex(INT index){m_BagIndex = index;}
		INT				getDestBagIndex(){return m_BagIndex;}

		VOID			setSourIndex(INT index){m_SourIndex = index;}
		INT				getSourIndex(){return m_SourIndex;}

		VOID			setResult(BYTE Result){m_Result = Result;}
		BYTE			getResult(){return m_Result;}

	private:
		BYTE			m_Result;		//结果信息
		INT				m_SourIndex;
		INT				m_BagIndex;		//存放的位置
	};


	class GCUseExtraBagResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseExtraBagResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_USEEXTRABAGRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(INT)*2+sizeof(BYTE);}
	};


	class GCUseExtraBagResultHandler
	{
	public:
		static UINT Execute( GCUseExtraBagResult* pPacket, Player* pPlayer ) ;
	};
}



#endif