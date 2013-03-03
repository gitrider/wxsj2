/********************************************************************************
*	�ļ�����	GWCountryAP.h
*	ȫ·����	d:\Prj\Server\Common\Packets\GWCountryAP.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 27 ��	15:24
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __GWCOUNTRYAP_H__
#define __GWCOUNTRYAP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryAP : public Packet 
    {
    public:
        GWCountryAP( ) {} ;
        virtual ~GWCountryAP( ) {} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_AP ; }
        virtual UINT			        GetPacketSize()const { return sizeof(INT)+sizeof(GUID_t)+sizeof(BYTE); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

        INT                             GetAP() { return m_nAP; };
        VOID					        SetAP( INT nAP ) { m_nAP = nAP; }

        BYTE                            GetType() { return m_cType; }
        VOID                            SetType(BYTE cType) { m_cType = cType; }

    public:
        //����
        GUID_t                          m_Guid;
        BYTE                            m_cType;        //0 ����  1����
        INT                             m_nAP;
    };


    class GWCountryAPFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryAP() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_AP ; }
        UINT		GetPacketMaxSize()const { return sizeof(INT)+sizeof(GUID_t)+sizeof(BYTE); }
    };


    class GWCountryAPHandler 
    {
    public:
        static UINT Execute( GWCountryAP* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
