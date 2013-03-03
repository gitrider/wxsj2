/********************************************************************************
*	�ļ�����	GWCountryInfoRequest.h
*	ȫ·����	d:\Prj\Server\Common\Packets\GWCountryInfoRequest.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 4 �� 22 ��	19:34
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __GWCOUNTRYINFOREQUESTT_H__
#define __GWCOUNTRYINFOREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryInfoRequest : public Packet 
    {
    public:
        GWCountryInfoRequest( ){} ;
        virtual ~GWCountryInfoRequest( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_INFO_REQUEST; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT) + sizeof(GUID_t); }

        INT                             GetCountryID() { return m_nCountryID; };
        VOID					        SetCountryID( INT nCountry ) { m_nCountryID = nCountry; }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

    public:
        //����
        GUID_t                          m_Guid;
        INT                             m_nCountryID;
    };


    class GWCountryInfoRequestFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryInfoRequest() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_INFO_REQUEST ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT) + sizeof(GUID_t); }
    };


    class GWCountryInfoRequestHandler 
    {
    public:
        static UINT Execute( GWCountryInfoRequest* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
