/********************************************************************************
*	�ļ�����	CGCountryRemove.h
*	ȫ·����	d:\Prj\Server\Common\Packets\CGCountryAppoint.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 14 ��	19:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __CGCOUNTRYAPPOINT_H__
#define __CGCOUNTRYAPPOINT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryAppoint : public Packet 
    {
    public:
        CGCountryAppoint( ){} ;
        virtual ~CGCountryAppoint( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_APPOINT ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize(); }

        COUNTRY_CGW_COUNTRY_APPOINT*    GetCountryAppointData() { return &m_Data; };


        VOID					        SetCountryAppointData( COUNTRY_CGW_COUNTRY_APPOINT* pRequest )
        {
            memcpy((void*)&m_Data, pRequest, sizeof(m_Data));
        }

    public:
        //����
        COUNTRY_CGW_COUNTRY_APPOINT     m_Data;
    };


    class CGCountryAppointFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryAppoint() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_APPOINT ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_APPOINT); }
    };


    class CGCountryAppointHandler 
    {
    public:
        static UINT Execute( CGCountryAppoint* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
