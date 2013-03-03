#ifndef _GC_UNEXTRABAG_RESULT_H_
#define _GC_UNEXTRABAG_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GCUnExtraBagResult:public Packet
	{

	public:
		GCUnExtraBagResult(){};
		virtual				~GCUnExtraBagResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_UNEXTRABAGRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(INT)*2;
		}

	public:
		VOID			setBagIndex(INT index){m_BagIndex = index;}
		INT				getBagIndex(){return m_BagIndex;}
		
		VOID			setDestIndex(INT index){m_DestIndex = index;}
		INT				getDestIndex(){return m_DestIndex;}

		VOID			setResult(BYTE	Ret){ m_Result = Ret;}
		BYTE			getResult(){return m_Result;}

	private:
		INT					m_BagIndex;				//卸下来得背包在Bag中存放的位置
		INT					m_DestIndex;
		BYTE				m_Result;				//是否成功，不成功则是错误号		

	};


	class GCUnExtraBagResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUnExtraBagResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_UNEXTRABAGRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)+sizeof(INT)*2; }
	};


	class GCUnExtraBagResultHandler
	{
	public:
		static UINT Execute( GCUnExtraBagResult* pPacket, Player* pPlayer ) ;
	};

}
#endif