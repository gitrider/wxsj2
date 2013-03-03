/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef _GCReturnHorseFindMateInfo_h_
#define _GCReturnHorseFindMateInfo_h_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Query.h"

namespace Packets
{
	class GCReturnHorseFindMateInfo : public Packet
	{
	public:
		GCReturnHorseFindMateInfo(){}
		virtual					~GCReturnHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_RETURNHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return m_HorseFindMateResult.GetSize() + sizeof(BOOL) + sizeof(BOOL) + sizeof(INT);
		}

		VOID SetHorseFindMateResult(const HorseFindMateInfoPerPage& value){
			m_HorseFindMateResult = value;
		}
		const HorseFindMateInfoPerPage& GetHorseFindMateResult() const{
			return m_HorseFindMateResult;
		}

		VOID SetSearch(BOOL value) { m_bSearch = value; }
		VOID SetContinue(BOOL value) { m_bContinue = value; }
		BOOL GetSearch() const { return m_bSearch; }
		BOOL GetContinue() const { return m_bContinue; }
		VOID SetReturnPostion(INT value) {m_iReturnPostion = value;}
		INT GetReturnPostion() const {return m_iReturnPostion;}

	private:
		BOOL m_bSearch;		// 是否是搜索行为的返回结果
		BOOL m_bContinue;   // 是否还有信息，只对正常查看行为的返回结果而用
		HorseFindMateInfoPerPage m_HorseFindMateResult;
		INT					m_iReturnPostion; //查询返回Users中的最大Index
	};

	class GCReturnHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GCReturnHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_GC_RETURNHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { 
			return sizeof(HorseFindMateInfoPerPage) + sizeof(BOOL) + sizeof(BOOL) + sizeof(INT); 
		}
	};

	class GCReturnHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( GCReturnHorseFindMateInfo* pPacket, Player* pPlayer ) ;
	};

}

#endif // __CG_GUILD_H__
