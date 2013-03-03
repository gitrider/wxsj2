#ifndef _CG_USEGEM_H_
#define _CG_USEGEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	struct HOLE_GEM_INFO
	{
		HOLE_GEM_INFO()
		{
			m_nGemHolePos  = INVALID_ITEM_POS;
			m_GemBagIndex  = INVALID_ITEM_POS;
		}
		INT					m_nGemHolePos;
		INT					m_GemBagIndex;
	};
	class CGUseGem:	public Packet
	{
	public:
		CGUseGem()
		{
			m_nValidCount =0;
			m_EquipPosType = INVALID_ITEM_POS_TYPE;
			m_nPos		   = INVALID_ITEM_POS;
			m_ItemId.Reset();			//装备的GUID
		};
		virtual		~CGUseGem(){ };

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_USEGEM; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2+sizeof(INT)+m_nValidCount*sizeof(HOLE_GEM_INFO)+sizeof(_ITEM_GUID); }

		VOID					SetEquipPosType(INT nType){m_EquipPosType = nType;}
		INT						GetEquipPosType(){;return m_EquipPosType;}

		VOID					SetPos(BYTE nPos){m_nPos = nPos;}
		INT						GetPos(){return m_nPos;}
		
		VOID                    SetValidInfoCount(BYTE nCount){  ++m_nValidCount;}
		BYTE					GetValidInfoCount(){ return m_nValidCount;}

		VOID					SetHoleGemInfo(HOLE_GEM_INFO& gemInfo){ if(m_nValidCount>=MAX_ITEM_GEM) m_nValidCount=0; m_HoleGemInfo[m_nValidCount]=gemInfo; ++ m_nValidCount;}
		HOLE_GEM_INFO*			GetHoleGemInfo(BYTE nGemPos)
		{
			if(nGemPos>MAX_ITEM_GEM) 
				return NULL; 
			else if(m_EquipPosType == INVALID_ITEM_POS_TYPE)
				return NULL;
			return &m_HoleGemInfo[nGemPos];
		}

		VOID			setItemID(_ITEM_GUID id){m_ItemId	=	id;}
		_ITEM_GUID		getItemID(){return m_ItemId;}
	private:
		BYTE			m_EquipPosType;
		INT				m_nPos;
		_ITEM_GUID		m_ItemId;			//装备的GUID
		BYTE			m_nValidCount;
		HOLE_GEM_INFO	m_HoleGemInfo[MAX_ITEM_GEM];
	};

	class CGUseGemFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseGem() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_USEGEM; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*2+sizeof(INT)+sizeof(HOLE_GEM_INFO)*MAX_ITEM_GEM+sizeof(_ITEM_GUID); }			
	};

	class CGUseGemHandler
	{
	public:
		static UINT	Execute(CGUseGem* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif