

#ifndef __GWRefreshDataToWorld_H__
#define __GWRefreshDataToWorld_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "WGRetUserData.h" //为了使用FULLUSERDATA结构
#include "GameDefine_Item.h"

namespace Packets
{

enum REFRESHDATATOWORLD_FLAG {
	REFRESH_COUNTRY,
	REFRESH_MENPAI,
	REFRESH_LEVEL,
	REFRESH_GUILD,
	REFRESH_POSTCODE,
	REFRESH_MONEY,
	REFRESH_BANK_MONEY,

	REFRESH_NUMBER
};

class GWRefreshDataToWorld : public Packet 
{
public:
	GWRefreshDataToWorld( ){} ;
	virtual ~GWRefreshDataToWorld( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_REFRESHDATATOWORLD ; }
	virtual UINT			GetPacketSize() const 
	{ 
		UINT uSize = sizeof(PlayerID_t) + sizeof(GUID_t) + sizeof(Flag16);
		if(m_Flags.isSetBit(REFRESH_COUNTRY))
			uSize += sizeof(INT);
		if(m_Flags.isSetBit(REFRESH_MENPAI))
			uSize += sizeof(INT);
		if(m_Flags.isSetBit(REFRESH_LEVEL))
			uSize += sizeof(INT);
		if(m_Flags.isSetBit(REFRESH_GUILD))
			uSize += sizeof(GuildID_t);
		if(m_Flags.isSetBit(REFRESH_POSTCODE))
			uSize += sizeof(INT);
		if(m_Flags.isSetBit(REFRESH_MONEY))
			uSize += sizeof(UINT);
		if(m_Flags.isSetBit(REFRESH_BANK_MONEY))
			uSize += sizeof(UINT);
		return uSize;
	}

	Flag16			GetFlags( VOID )const{ return m_Flags; }

	BOOL			IsUpdateAttrib( INT eAttrib )const{ return m_Flags.isSetBit((INT)eAttrib)?(TRUE):(FALSE); }
	VOID			SetUpdateAttrib( INT eAttrib, BOOL bUpdate ){ m_Flags.UpdateBits((INT)eAttrib,bUpdate); }

	VOID			SetGUID(GUID_t guid){m_GUID=guid;}
	GUID_t			GetGUID(){return m_GUID;}

	VOID			SetPlayerID(PlayerID_t id) {m_PlayerID = id;}
	PlayerID_t		GetPlayerID(){return m_PlayerID;}

	VOID			SetCountry(INT value){ m_iCountry = value;SetUpdateAttrib(REFRESH_COUNTRY, TRUE);}	
	VOID			SetMenPai(INT value){ m_iMenPai = value;SetUpdateAttrib(REFRESH_MENPAI, TRUE);}	
	VOID			SetLevel(INT value){ m_iLevel = value;SetUpdateAttrib(REFRESH_LEVEL, TRUE);}	
	VOID		SetGuildID(GuildID_t id) {m_GuildID = id;SetUpdateAttrib(REFRESH_GUILD, TRUE);}
	VOID		SetPostCode(INT value) {m_iPostCode = value;SetUpdateAttrib(REFRESH_POSTCODE, TRUE);}
	VOID		SetMoney(UINT value)  { m_uMoney = value; SetUpdateAttrib(REFRESH_MONEY, TRUE);}
	VOID		SetBankMoney(UINT value)  { m_uBankMoney = value; SetUpdateAttrib(REFRESH_BANK_MONEY, TRUE);}

	INT			GetCountry(){ return m_iCountry;}	
	INT			GetMenPai(){ return m_iMenPai;}	
	INT			GetLevel(){ return m_iLevel;}	

	GuildID_t	GetGuildID() {return m_GuildID;}
	INT			GetPostCode() const {return m_iPostCode;}
	UINT		GetMoney() const {return m_uMoney;}
	UINT		GetBankMoney() const {return m_uBankMoney;}

private:
	Flag16						m_Flags;	// 每个位表示一个属性是否要刷新
	PlayerID_t					m_PlayerID;	//当前连接的玩家号
	GUID_t						m_GUID;		//玩家的GUID

	INT							m_iCountry;		//国家
	INT							m_iMenPai;		//门派
	INT							m_iLevel;		//等级
	GuildID_t					m_GuildID;		//帮派
	INT							m_iPostCode;	//邮编
	UINT						m_uMoney;		//角色身上货币
	UINT						m_uBankMoney;	//角色银行货币
};


class GWRefreshDataToWorldFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWRefreshDataToWorld() ; }
	PacketID_t	GetPacketID() const { return PACKET_GW_REFRESHDATATOWORLD ; }
	UINT		GetPacketMaxSize() const { return sizeof(GWRefreshDataToWorld) - sizeof(Packet); }
};


class GWRefreshDataToWorldHandler 
{
public:
	static UINT Execute( GWRefreshDataToWorld* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif





















