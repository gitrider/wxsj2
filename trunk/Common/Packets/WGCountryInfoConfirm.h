/********************************************************************************
*	�ļ�����	WGCountryInfoConfirm.h
*	ȫ·����	d:\Prj\Server\Common\Packets\WGCountryInfoConfirm.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 22 ��	19:34
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __WGCOUNTRYINFOCONFIRM_H__
#define __WGCOUNTRYINFOCONFIRM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class WGCountryInfoConfirm : public Packet 
    {
    public:
        WGCountryInfoConfirm( ){} ;
        virtual ~WGCountryInfoConfirm( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_WG_COUNTRY_INFO_CONFIRM; }
        virtual UINT			        GetPacketSize()const { return sizeof(COUNTRY_CGW_COUNTRY_INFO) + sizeof(PlayerID_t); }

        VOID					        SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				        GetPlayerID( ) { return m_PlayerID; }

        COUNTRY_CGW_COUNTRY_INFO*       GetCountryInfo() { return &m_Info; };
        VOID					        SetCountryInfo( COUNTRY_CGW_COUNTRY_INFO *pInfo )
        {
            memcpy(&m_Info, pInfo, sizeof(COUNTRY_CGW_COUNTRY_INFO));
        }

    public:
        //����
        PlayerID_t				        m_PlayerID;	//������
        COUNTRY_CGW_COUNTRY_INFO        m_Info;
    };

    class WGCountryInfoConfirmFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new WGCountryInfoConfirm() ; }
        PacketID_t	GetPacketID()const { return PACKET_WG_COUNTRY_INFO_CONFIRM ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_INFO) + sizeof(PlayerID_t); }
    };

    class WGCountryInfoConfirmHandler 
    {
    public:
        static UINT Execute( WGCountryInfoConfirm* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
