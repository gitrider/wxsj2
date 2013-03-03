/********************************************************************************
*	�ļ�����	WGCountryCollect.h
*	ȫ·����	d:\Prj\Server\Common\Packets\WGCountryCollect.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 26 ��	15:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __WGCOUNTRYCOLLECT_H__
#define __WGCOUNTRYCOLLECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class WGCountryCollect : public Packet 
    {
    public:
        WGCountryCollect( ){} ;
        virtual ~WGCountryCollect( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_WG_COUNTRY_COLLECT ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize() + sizeof(m_PlayerID); }

        VOID					        SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				        GetPlayerID( ) { return m_PlayerID; }

        COUNTRY_CGW_COUNTRY_COLLECT*    GetCollectData() { return &m_Data; };


        VOID					        SetCollectData( COUNTRY_CGW_COUNTRY_COLLECT* pCollect )
        {
            memcpy((void*)&m_Data, pCollect, sizeof(m_Data));
        }

    public:
        //����
        PlayerID_t				        m_PlayerID;	//������
        COUNTRY_CGW_COUNTRY_COLLECT     m_Data;
    };


    class WGCountryCollectFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new WGCountryCollect() ; }
        PacketID_t	GetPacketID()const { return PACKET_WG_COUNTRY_COLLECT ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_COLLECT) + sizeof(PlayerID_t); }
    };


    class WGCountryCollectHandler 
    {
    public:
        static UINT Execute( WGCountryCollect* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
