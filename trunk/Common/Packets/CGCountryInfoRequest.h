/********************************************************************************
*	�ļ�����	CGCountryInfoRequest.h
*	ȫ·����	d:\Prj\Server\Common\Packets\CGCountryInfoRequest.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 22 ��	19:34
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __CGCOUNTRYINFOREQUEST_H__
#define __CGCOUNTRYINFOREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryInfoRequest : public Packet 
    {
    public:
        CGCountryInfoRequest( ){} ;
        virtual ~CGCountryInfoRequest( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_INFO_REQUEST; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT); }

        INT                             GetCountryID() { return m_nCountryID; };
        VOID					        SetCountryID( INT nCountry )
        {
            m_nCountryID = nCountry;
        }

    public:
        //����
        INT                             m_nCountryID;
    };


    class CGCountryInfoRequestFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryInfoRequest() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_INFO_REQUEST ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT); }
    };


    class CGCountryInfoRequestHandler 
    {
    public:
        static UINT Execute( CGCountryInfoRequest* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
