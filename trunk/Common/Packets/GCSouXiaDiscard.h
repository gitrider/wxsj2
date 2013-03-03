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
			m_Result			=	0;		     //�Ƿ�ɹ������ɹ�����������Ϣ
			m_BagIndex			=	0;		    //�ɹ��󣬶�����Ʒ��λ��
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
		BYTE			m_BagIndex;		 //�ɹ��󣬶�����Ʒ��λ��
		BYTE			m_Result;		 //�Ƿ�ɹ������ɹ�����������Ϣ
		BYTE			m_Opt;		     //��������
		SHORT			m_CurPos;		 //�ɹ��󣬶����Ē���¼�������ϵ�λ��
		UINT			m_ItemTableIndex;//��������Ʒ����

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
