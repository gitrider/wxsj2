/********************************************************************************
*	文件名：	CGCountryInfoRequest.h
*	全路径：	d:\Prj\Server\Common\Packets\CGCountryInfoRequest.h
*	创建人：	李一立
*	创建时间：	2008 年 4 月 22 日	19:34
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __CGCOUNTRYINFOREQUEST_H__
#define __CGCOUNTRYINFOREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryInfoRequest : public Packet 
    {
    public:
        CGCountryInfoRequest( ){} ;
        virtual ~CGCountryInfoRequest( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_INFO_REQUEST; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT); }

        INT                             GetCountryID() { return m_nCountryID; };
        VOID					        SetCountryID( INT nCountry )
        {
            m_nCountryID = nCountry;
        }

    public:
        //数据
        INT                             m_nCountryID;
    };


    class CGCountryInfoRequestFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryInfoRequest() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_INFO_REQUEST ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class CGCountryInfoRequestHandler 
    {
    public:
        static UINT Execute( CGCountryInfoRequest* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
