#ifndef _GC_DISCARDITEM_RESULT_
#define _GC_DISCARDITEM_RESULT_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCDiscardItemResult:public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromBank,
			FromBody,
		};
	public:
		GCDiscardItemResult()
		{
			m_Opt				=	FromBag;
			m_Result			=	0;		     //�Ƿ�ɹ������ɹ�����������Ϣ
			m_BagIndex			=	0;		    //�ɹ��󣬶�����Ʒ��λ��
			m_ItemTableIndex	=	0;
		};
		virtual				~GCDiscardItemResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_DISCARDITEMRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*3+sizeof(UINT);
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
		

	private:
		BYTE			m_Opt;		     //�Ƿ�ɹ������ɹ�����������Ϣ
		BYTE			m_Result;		     //�Ƿ�ɹ������ɹ�����������Ϣ
		BYTE			m_BagIndex;		    //�ɹ��󣬶�����Ʒ��λ��
		UINT			m_ItemTableIndex;

	};


	class GCDiscardItemResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCDiscardItemResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_DISCARDITEMRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*3+sizeof(UINT); }
	};


	class GCDiscardItemResultHandler
	{
	public:
		static UINT	Execute( GCDiscardItemResult* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif