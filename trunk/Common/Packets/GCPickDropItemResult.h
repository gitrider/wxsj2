#ifndef _GC_PICK_DROPITEMRESULT_H_
#define _GC_PICK_DROPITEMRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GCPickDropItemResult:public Packet
	{

	public:
		GCPickDropItemResult(){};
		virtual				~GCPickDropItemResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_PICKDROPITEMRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(m_ObjID)+sizeof(BYTE)*2;
		}

	public:

		VOID			setDropItemId(ObjID_t	id){m_ObjID = id;};
		ObjID_t			getDropItemId(){return m_ObjID;}

		VOID			setResult(BYTE Result){m_Result = Result;}
		BYTE			getResult(){return m_Result;}

		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}

	private:
		ObjID_t			m_ObjID;
		BYTE			m_Result;			//是否成功，不成功包含错误信息
		BYTE			m_BagIndex;			//成功后，存放的位置
	};


	class GCPickDropItemResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCPickDropItemResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_PICKDROPITEMRESULT; }
		UINT		GetPacketMaxSize() const {
			return sizeof(ObjID_t)+sizeof(BYTE)*2; }
	};


	class GCPickDropItemResultHandler
	{
	public:
		static UINT Execute( GCPickDropItemResult* pPacket, Player* pPlayer );
	};

}


#endif