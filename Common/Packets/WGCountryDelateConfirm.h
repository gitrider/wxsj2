/********************************************************************************
*	文件名：	WGCountryDelateConfirm.h
*	全路径：	d:\Prj\Server\Common\Packets\WGCountryDelateConfirm.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __WGCOUNTRYDELATECONFIRM_H__
#define __WGCOUNTRYDELATECONFIRM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class WGCountryDelateConfirm : public Packet 
    {
    public:
        WGCountryDelateConfirm( )
        {
            memset(m_szKindomName, 0, MAX_CHARACTER_NAME);
        } ;
        virtual ~WGCountryDelateConfirm( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_WG_COUNTRY_DELATE_CONFIRM ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t) + sizeof(m_PlayerID) + sizeof(BYTE)*2 + m_cNameSize; }

        VOID					        SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				        GetPlayerID( ) { return m_PlayerID; }

        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };
        GUID_t                          GetGuid() { return m_Guid; };

        VOID                            SetCurNum (BYTE cNum) { m_cCurNum = cNum; };
        BYTE                            GetCurNum () { return m_cCurNum; };

        VOID                            SetNameSize(BYTE cSize) { m_cNameSize = cSize; };
        BYTE                            GetNameSize() { return m_cNameSize; };

        VOID                            SetName(CHAR* szName) { memcpy(m_szKindomName, szName, MAX_CHARACTER_NAME); };
        CHAR*                           GetName() { return m_szKindomName; };

    private:
        //数据
        PlayerID_t				        m_PlayerID;	//连接者
        GUID_t                          m_Guid;     //校验
        BYTE                            m_cCurNum;  //当前票数
        BYTE                            m_cNameSize;
        CHAR                            m_szKindomName[MAX_CHARACTER_NAME];
    };


    class WGCountryDelateConfirmFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new WGCountryDelateConfirm() ; }
        PacketID_t	GetPacketID()const { return PACKET_WG_COUNTRY_DELATE_CONFIRM ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t) + sizeof(PlayerID_t) + sizeof(BYTE)*2 +MAX_CHARACTER_NAME; }
    };


    class WGCountryDelateConfirmHandler 
    {
    public:
        static UINT Execute( WGCountryDelateConfirm* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
