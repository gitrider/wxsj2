/********************************************************************************
*	�ļ�����	CGCountryFetch.h
*	ȫ·����	d:\Prj\Server\Common\Packets\CGCountryFetch.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 26 ��	15:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __CGCOUNTRYFETCH_H__
#define __CGCOUNTRYFETCH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryFetch : public Packet 
    {
    public:
        CGCountryFetch( ){} ;
        virtual ~CGCountryFetch( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_FETCH ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT); }

        INT                             GetDonativeGold() { return m_nGold; };


        VOID					        SetDonativeGold( INT nGold )
        {
            m_nGold = nGold;
        }

    public:
        //����
        INT                             m_nGold;
    };


    class CGCountryFetchFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryFetch() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_FETCH ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class CGCountryFetchHandler 
    {
    public:
        static UINT Execute( CGCountryFetch* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
