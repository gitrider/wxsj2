#ifndef __GCCHARALLTITLES_H__
#define __GCCHARALLTITLES_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class	GCCharAllTitles : public Packet 
	{
    public:
        enum
        {
            UPDATE_ALL = 0,                 //更新所有称号信息
            UPDATE_BANGPAI_TITLE_NAME,      //更新帮派称号名字
            UPDATE_CUR_BANGPAI_TITLE,       //更新当前帮派称号ID
            UPDATE_CUR_GUOJIA_TITLE,        //更新当前国家称号ID
            UPDATE_CUR_WANFA_TITLE,         //更新当前玩法称号ID
        };

	public:
		GCCharAllTitles( )
        {
            m_cType = 0;
        } ;
		virtual ~GCCharAllTitles( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARALLTITLES ; }
		virtual UINT			GetPacketSize()const 
		{
            return sizeof(_TITLE) + sizeof(BYTE);
		}

	public:
		//使用数据接口
		_TITLE*     			GetTitleInfo(VOID) { return &m_TitleInfo; }
		VOID					SetTitleInfo(_TITLE* pTitle)
		{
			memcpy(&m_TitleInfo, pTitle, sizeof(_TITLE));
		}

        BYTE                    GetType() { return m_cType; };
        VOID                    SetType(BYTE cType) { m_cType = cType; };
	private:
        
        BYTE        m_cType;
        _TITLE      m_TitleInfo;
	};


	class GCCharAllTitlesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharAllTitles() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARALLTITLES ; }
		UINT		GetPacketMaxSize()const { return sizeof(_TITLE) + sizeof(BYTE);  }
	};


	class GCCharAllTitlesHandler 
	{
	public:
		static UINT Execute( GCCharAllTitles* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets ;

#endif
