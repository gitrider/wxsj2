#ifndef _WLASKPRIZE_H_
#define _WLASKPRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class WLAskPrize : public Packet 
	{
	public:
		WLAskPrize( )
		{
			m_PrizeUserID = INVALID_ID;
			memset(m_AccName,0,MAX_ACCOUNT+1);
			memset(m_CharName,0,MAX_CHARACTER_NAME+1);
			memset(m_Host,0,IP_SIZE+1);
			m_GUID = INVALID_ID;

			m_askType = ASK_INVALID;
			m_CostPoint = 0;
			m_GoodsNumber = 0;
			m_GoodsType = 0;
			memset(m_NewUserCard,0,MAX_NEWUSER_CARD_SIZE+1);
		} ;
		virtual ~WLAskPrize( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_ASKPRIZE ; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(GUID_t)+sizeof(ID_t)+
						 sizeof(CHAR)*MAX_ACCOUNT+
						 sizeof(CHAR)*MAX_CHARACTER_NAME+
						 sizeof(CHAR)*IP_SIZE+sizeof(BYTE);
			if(ASK_BUY == m_askType)
			{
				uSize += sizeof(UINT)+sizeof(UINT)+sizeof(USHORT);
			}
			else if(ASK_PRIZE == m_askType)
			{
				uSize += sizeof(UINT);
			}
			else if(ASK_NEWUSER_CARD == m_askType)
			{
				uSize += sizeof(CHAR)*MAX_NEWUSER_CARD_SIZE;
			}
			return uSize;
		}

	public:
		//使用数据接口
		ID_t					GetPrizeUserID(){return m_PrizeUserID;}
		VOID					SetPrizeUserID(ID_t id)
		{
			m_PrizeUserID = id;
		}

		CHAR*					GetAccName(){return m_AccName;}
		VOID					SetAccName(const CHAR*	pAccount)
		{
			Assert(pAccount);
			strncpy(m_AccName,pAccount,MAX_ACCOUNT*sizeof(CHAR));
			m_AccName[MAX_ACCOUNT] = 0;
		}

		CHAR*					GetCharName(){return m_CharName;}
		VOID					SetCharName(const CHAR*	pChar)
		{
			Assert(pChar);
			strncpy(m_CharName,pChar,MAX_CHARACTER_NAME*sizeof(CHAR));
			m_CharName[MAX_CHARACTER_NAME] = 0;
		}

		CHAR*					GetHost(){return m_Host;}
		VOID					SetHost(const CHAR*	pHost)
		{
			Assert(pHost);
			strncpy(m_Host,pHost,IP_SIZE*sizeof(CHAR));
			m_Host[IP_SIZE] = 0;
		}

		GUID_t					GetGUID(){return m_GUID;}
		VOID					SetGUID(GUID_t gid)
		{
			m_GUID = gid;
		}

		VOID					SetAskType(BYTE ty)
		{
			Assert(ty >= ASK_INVALID && ty < ASK_NUMBER);
			m_askType = ty;
		}
		BYTE					GetAskType(){return m_askType;}

		VOID					SetCostPoint(UINT pt){m_CostPoint=pt;}
		UINT					GetCostPoint(){return m_CostPoint;}

		VOID					SetGoodsNumber(USHORT pNumber){m_GoodsNumber=pNumber;}
		USHORT					GetGoodsNumber(){return m_GoodsNumber;}

		VOID					SetGoodsType(UINT nt){m_GoodsType = nt;}
		UINT					GetGoodsType(){return m_GoodsType;}

		VOID					SetNewUserCard(CHAR* pCard)
		{
			strncpy(m_NewUserCard,pCard,MAX_NEWUSER_CARD_SIZE);
			m_NewUserCard[MAX_NEWUSER_CARD_SIZE]=0;
		}
		CHAR*					GetNewUserCard()
		{
			return m_NewUserCard;
		}
	private:
		//数据
		ID_t					m_PrizeUserID;							//查询点数(ASK_POINT)时为PlayerID_t，其他是PrizeUser的ID_t
		CHAR					m_AccName[MAX_ACCOUNT+1];
		CHAR					m_CharName[MAX_CHARACTER_NAME+1];
		CHAR					m_Host[IP_SIZE+1];
		GUID_t					m_GUID;

		BYTE					m_askType;								//请求类型
		UINT					m_CostPoint;							//消耗点数
		USHORT					m_GoodsNumber;							//物品数量
		UINT					m_GoodsType;							//物品类型
		CHAR					m_NewUserCard[MAX_NEWUSER_CARD_SIZE+1];	//新手卡卡号
	};

	class WLAskPrizeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLAskPrize() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_ASKPRIZE ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(GUID_t)+sizeof(ID_t)+
				sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(CHAR)*MAX_CHARACTER_NAME+
				sizeof(CHAR)*IP_SIZE+
				sizeof(BYTE)+sizeof(UINT)+
				sizeof(USHORT)+sizeof(UINT)+
				sizeof(CHAR)*MAX_NEWUSER_CARD_SIZE;
		}
	};


	class WLAskPrizeHandler 
	{
	public:
		static UINT Execute( WLAskPrize* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
