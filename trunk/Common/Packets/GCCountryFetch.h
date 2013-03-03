/********************************************************************************
*	�ļ�����	GCCountryFetch.h
*	ȫ·����	d:\Prj\Server\Common\Packets\GCCountryFetch.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 26 ��	15:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __GCCOUNTRYFETCH_H__
#define __GCCOUNTRYFETCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GCCountryFetch : public Packet 
    {
    public:
        GCCountryFetch( ){} ;
        virtual ~GCCountryFetch( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GC_COUNTRY_FETCH ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT); }

        INT                             GetGold() { return m_nGold; };


        VOID					        SetGold( INT nGold )
        {
            m_nGold = nGold;
        }

    public:
        //����
        INT                             m_nGold;
    };


    class GCCountryFetchFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCCountryFetch() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_COUNTRY_FETCH ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class GCCountryFetchHandler 
    {
    public:
        static UINT Execute( GCCountryFetch* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
