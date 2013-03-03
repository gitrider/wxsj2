/********************************************************************************
*	�ļ�����	CGCountryRemove.h
*	ȫ·����	d:\Prj\Server\Common\Packets\CGCountryRemove.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 14 ��	19:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __CGCOUNTRYREMOVE_H__
#define __CGCOUNTRYREMOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryRemove : public Packet 
    {
    public:
        CGCountryRemove( ){} ;
        virtual ~CGCountryRemove( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_REMOVE ; }
        virtual UINT			        GetPacketSize()const { return m_Data.GetPacketSize(); }

        COUNTRY_CGW_COUNTRY_REMOVE*     GetCountryRemoveData() { return &m_Data; };


        VOID					        SetCountryRemoveData( COUNTRY_CGW_COUNTRY_REMOVE* pRemove )
        {
            memcpy((void*)&m_Data, pRemove, sizeof(m_Data));
        }

    public:
        //����
        COUNTRY_CGW_COUNTRY_REMOVE     m_Data;
    };


    class CGCountryRemoveFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryRemove() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_REMOVE ; }
        UINT		GetPacketMaxSize()const { return sizeof(COUNTRY_CGW_COUNTRY_REMOVE); }
    };


    class CGCountryRemoveHandler 
    {
    public:
        static UINT Execute( CGCountryRemove* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
