#ifndef _GC_SHOPSPECIALREPAIRRESULT_H_
#define _GC_SHOPSPECIALREPAIRRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_REPAIRED_NUMBER	15
namespace Packets
{
	class GCShopSpecialRepairResult:	public Packet
	{
	public:
		enum ITEM_POS
		{
			EQUIP = 0,		// 身上
			BAG,			// 包里
			BOX,			// 箱子里
		};
		struct REPAIRED_ITEM
		{
			ITEM_POS	IsIn;
			UINT	 BagIndex;
			INT		 nCurDur;
			INT		 nCurMaxDur;
			REPAIRED_ITEM()
			{
				IsIn = BAG;
				nCurDur = 0;
				nCurMaxDur = 0;
			}
		};
	public:
		GCShopSpecialRepairResult( )
		{
		};
		virtual		~GCShopSpecialRepairResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPSPECIALREPAIRRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(EQUIPSPECIALREPAIR_RESULT)+sizeof(REPAIRED_ITEM); }

		EQUIPSPECIALREPAIR_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(EQUIPSPECIALREPAIR_RESULT	Ret){m_Result	= Ret;}

		REPAIRED_ITEM*			GetReList(VOID) { return &m_aRepairedItem; }
		VOID					SetReList(REPAIRED_ITEM* pSour)
		{
			memcpy(&m_aRepairedItem, pSour, sizeof(REPAIRED_ITEM));
		}
	private:
		REPAIRED_ITEM			m_aRepairedItem;		//修理好的装备列表
		EQUIPSPECIALREPAIR_RESULT		m_Result;

	};

	class GCShopSpecialRepairResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCShopSpecialRepairResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPSPECIALREPAIRRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPSPECIALREPAIR_RESULT)+sizeof(GCShopSpecialRepairResult::REPAIRED_ITEM); }			
	};

	class GCShopSpecialRepairResultHandler
	{
	public:
		static UINT Execute(GCShopSpecialRepairResult* pPacket,Player* pPlayer);
	};

}
using namespace Packets;

#endif