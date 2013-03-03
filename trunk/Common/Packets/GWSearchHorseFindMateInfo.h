/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __GWSearchHorseFindMateInfo_H__
#define __GWSearchHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GWSearchHorseFindMateInfo : public Packet
	{
	public:
		GWSearchHorseFindMateInfo(){}
		virtual					~GWSearchHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_SEARCHHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_szHorseName) + 
				sizeof(m_iHorseLevel) + 
				sizeof(m_iHorseSex) +
				sizeof(m_iHorseGrowRate)+
				sizeof(PlayerID_t) +
				sizeof(m_iMaxPostion);
		}

		VOID			SetName( const CHAR *pszName ){
			tsnprintf_s(m_szHorseName, MAX_CHARACTER_NAME-1, "%s", pszName);
		}
		const CHAR		*GetName( VOID )const{ return m_szHorseName; }
		
		VOID SetHorseLevel(INT value){
			m_iHorseLevel = value;
		}
		VOID SetHorseSex(INT value){
			m_iHorseSex = value;
		}
		VOID SetHorseGrowRate(INT value){
			m_iHorseGrowRate = value;
		}
		INT GetHorseLevel() const {
			return m_iHorseLevel;
		}
		INT GetHorseSex() const {
			return m_iHorseSex;
		}
		INT GetHorseGrowRate() const {
			return m_iHorseGrowRate;
		}

		VOID				SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t			GetPlayerID( ) { return m_PlayerID; }

		INT					GetMaxPostion(){return m_iMaxPostion;}
		VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}

	private:
		PlayerID_t		m_PlayerID;
		CHAR m_szHorseName[MAX_CHARACTER_NAME];	// 名称
		INT	m_iHorseLevel;
		INT m_iHorseSex;
		INT m_iHorseGrowRate;
		INT	m_iMaxPostion; //上次查询时在Users中的最大Index
	};

	class GWSearchHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWSearchHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_SEARCHHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const {
			return sizeof(CHAR)*MAX_CHARACTER_NAME + 
				sizeof(INT) + 
				sizeof(INT) +
				sizeof(INT) + 
				sizeof(PlayerID_t) +
				sizeof(INT);
		}

	};

	class GWSearchHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( GWSearchHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}


#endif // __CG_GUILD_H__
