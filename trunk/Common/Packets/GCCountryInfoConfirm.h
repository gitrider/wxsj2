/********************************************************************************
*	�ļ�����	GCCountryInfoConfirm.h
*	ȫ·����	d:\Prj\Server\Common\Packets\GCCountryInfoConfirm.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 22 ��	19:34
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __GCCOUNTRYINFOCONFIRM_H__
#define __GCCOUNTRYINFOCONFIRM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GCCountryInfoConfirm : public Packet 
    {
    public:
        GCCountryInfoConfirm( ){} ;
        virtual ~GCCountryInfoConfirm( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GC_COUNTRY_INFO_CONFIRM; }
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

    class GCCountryInfoConfirmFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCCountryInfoConfirm() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_COUNTRY_INFO_CONFIRM ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_INFO) + sizeof(PlayerID_t); }
    };

    class GCCountryInfoConfirmHandler 
    {
    public:
        static UINT Execute( GCCountryInfoConfirm* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
