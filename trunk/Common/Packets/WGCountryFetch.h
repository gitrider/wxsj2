/********************************************************************************
*	�ļ�����	WGCountryFetch.h
*	ȫ·����	d:\Prj\Server\Common\Packets\WGCountryFetch.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 26 ��	15:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __WGCOUNTRYFETCH_H__
#define __WGCOUNTRYFETCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class WGCountryFetch : public Packet 
    {
    public:
        WGCountryFetch( ){} ;
        virtual ~WGCountryFetch( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_WG_COUNTRY_FETCH ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT)+sizeof(PlayerID_t); }

        VOID					        SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				        GetPlayerID( ) { return m_PlayerID; }

        INT                             GetGold() { return m_nGold; };
        VOID					        SetGold( INT nGold )
        {
            m_nGold = nGold;
        }

    public:
        //����
        PlayerID_t				        m_PlayerID;	//������
        INT                             m_nGold;
    };


    class WGCountryFetchFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new WGCountryFetch() ; }
        PacketID_t	GetPacketID()const { return PACKET_WG_COUNTRY_FETCH ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT)+sizeof(PlayerID_t); }
    };


    class WGCountryFetchHandler 
    {
    public:
        static UINT Execute( WGCountryFetch* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
