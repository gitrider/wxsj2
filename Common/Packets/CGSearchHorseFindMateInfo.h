/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#ifndef __CGSearchHorseFindMateInfo_H__
#define __CGSearchHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGSearchHorseFindMateInfo : public Packet
	{
	public:
		CGSearchHorseFindMateInfo(){}
		virtual					~CGSearchHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_SEARCHHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_szHorseName) + 
				sizeof(m_iHorseLevel) + 
				sizeof(m_iHorseSex) +
				sizeof(m_iHorseGrowRate)+
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

		INT					GetMaxPostion(){return m_iMaxPostion;}
		VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}

	private:
		CHAR m_szHorseName[MAX_CHARACTER_NAME];	// 名称
		INT	m_iHorseLevel;
		INT m_iHorseSex;
		INT m_iHorseGrowRate;
		INT	m_iMaxPostion; //上次查询时在Users中的最大Index
	};

	class CGSearchHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGSearchHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_SEARCHHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const {
			return sizeof(CHAR)*MAX_CHARACTER_NAME + 
				sizeof(INT) + 
				sizeof(INT) +
				sizeof(INT) +
				sizeof(INT);
		}
	};

	class CGSearchHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( CGSearchHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}


#endif // __CG_GUILD_H__
