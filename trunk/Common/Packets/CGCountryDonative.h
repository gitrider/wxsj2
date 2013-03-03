/********************************************************************************
*	�ļ�����	CGCountryDonative.h
*	ȫ·����	d:\Prj\Server\Common\Packets\CGCountryDonative.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 26 ��	15:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __CGCOUNTRYDONATIVE_H__
#define __CGCOUNTRYDONATIVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryDonative : public Packet 
    {
    public:
        CGCountryDonative( ){} ;
        virtual ~CGCountryDonative( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_DONATIVE ; }
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


    class CGCountryDonativeFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryDonative() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_DONATIVE ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class CGCountryDonativeHandler 
    {
    public:
        static UINT Execute( CGCountryDonative* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
