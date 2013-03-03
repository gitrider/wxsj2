#ifndef _CG_ITEMCREATE_H_
#define _CG_ITEMCREATE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


struct  CREATEITEM_MATERIAL_INFO
{
	BYTE m_nMaterialType;
	BYTE m_nMaterialLevel;
};
namespace Packets
{
	class CGItemCreate:	public Packet
	{
	public:
		CGItemCreate()
		{ m_nItemIndex = INVALID_INDEX; m_ValidNum = 0; };
		virtual		~CGItemCreate(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ITEMCREATE; }
		virtual UINT			GetPacketSize()const { return sizeof(UINT)+sizeof(BYTE)+
			m_ValidNum*sizeof(CREATEITEM_MATERIAL_INFO)+sizeof(UINT); }

		VOID					SetItemIndex(UINT nItemIndex){m_nItemIndex = nItemIndex;}
		UINT					GetItemIndex(){return m_nItemIndex;}

		VOID					SetValidNum(BYTE nValidNum){m_ValidNum = nValidNum;}
		BYTE					GetValidNum(){return m_ValidNum;}

		VOID						SetMaterialReq(CREATEITEM_MATERIAL_INFO& nInfo)
		{	Assert(m_ValidNum<MAX_CREATE_MATERIAL_NUM);  m_nMaterial[m_ValidNum] = nInfo; ++m_ValidNum;}
		CREATEITEM_MATERIAL_INFO&	GetMaterialReq(UINT nNum){Assert(nNum<m_ValidNum); return m_nMaterial[nNum];}
		 
		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }
	private:
		UINT					 m_nItemIndex;
		BYTE					 m_ValidNum;
		CREATEITEM_MATERIAL_INFO m_nMaterial[MAX_CREATE_MATERIAL_NUM];
		UINT					 m_UniqueID;	
	};

	class CGItemCreateFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGItemCreate() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_ITEMCREATE; }
		UINT		GetPacketMaxSize()const		{ return sizeof(UINT)+sizeof(BYTE)+
			MAX_CREATE_MATERIAL_NUM*sizeof(CREATEITEM_MATERIAL_INFO)+sizeof(UINT); }			
	};

	class CGItemCreateHandler
	{
	public:
		static UINT	Execute(CGItemCreate* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif