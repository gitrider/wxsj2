#ifndef _GC_SOUXIA_DISCARD_H_
#define _GC_SOUXIA_DISCARD_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCSouXiaDiscard:public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromBank,
			FromBody,
		};
	public:
		GCSouXiaDiscard()
		{
			m_Opt				=	FromBag;
			m_Result			=	0;		     //是否成功，不成功包含错误信息
			m_BagIndex			=	0;		    //成功后，丢弃物品的位置
			m_ItemTableIndex	=	0;
			m_CurPos			=	-1;
		};
		virtual				~GCSouXiaDiscard(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIADISCARD; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*3+sizeof(SHORT)+sizeof(UINT);
		}

	public:
		
		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}

		VOID			setResult(BYTE Result){m_Result = Result;}
		BYTE			getResult(){return m_Result;}

		VOID			setItemTableIndex(UINT	index){m_ItemTableIndex = index;}
		UINT			getItemTableIndex(){return m_ItemTableIndex;}

		VOID			setOpt(BYTE Opt){m_Opt = Opt;}
		BYTE			getOpt(){return m_Opt;}
		
		VOID			setCurPos(SHORT pos){m_CurPos = pos;}
		SHORT			getCurPos(){return m_CurPos;}


	private:
		BYTE			m_BagIndex;		 //成功后，丢弃物品的位置
		BYTE			m_Result;		 //是否成功，不成功包含错误信息
		BYTE			m_Opt;		     //操作类型
		SHORT			m_CurPos;		 //成功后，丢弃的捜侠录在人身上的位置
		UINT			m_ItemTableIndex;//丢弃的物品索引

	};


	class GCSouXiaDiscardFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSouXiaDiscard(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_SOUXIADISCARD; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*3+sizeof(SHORT)+sizeof(UINT); }
	};


	class GCSouXiaDiscardHandler
	{
	public:
		static UINT	Execute( GCSouXiaDiscard* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif //_GC_SOUXIA_DISCARD_H_
