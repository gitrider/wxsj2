/********************************************************************************
*	�ļ�����	GWCountryDelate.h
*	ȫ·����	d:\Prj\Server\Common\Packets\GWCountryDelate.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 28 ��	19:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __GWCOUNTRYDELATE_H__
#define __GWCOUNTRYDELATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCountryDelate : public Packet 
    {
    public:
        GWCountryDelate( ){} ;
        virtual ~GWCountryDelate( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GW_COUNTRY_DELATE ; }
        virtual UINT			        GetPacketSize()const { return sizeof(BOOL)+sizeof(GUID_t); }

        GUID_t                          GetGuid() { return m_Guid; };
        VOID                            SetGuid(GUID_t guid) { m_Guid = guid; };

        INT                             GetResult() { return m_bResult; };
        VOID					        SetResult( BOOL bResult )
        {
            m_bResult = bResult;
        }

    public:
        //����
        GUID_t                          m_Guid;
        BOOL                            m_bResult;
    };


    class GWCountryDelateFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCountryDelate() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_COUNTRY_DELATE ; }
        UINT		GetPacketMaxSize()const { return sizeof(BOOL)+sizeof(GUID_t); }
    };


    class GWCountryDelateHandler 
    {
    public:
        static UINT Execute( GWCountryDelate* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
