/********************************************************************************
*	�ļ�����	CGCountryDelate.h
*	ȫ·����	d:\Prj\Server\Common\Packets\CGCountryDelate.h
*	�����ˣ�	��һ��
*	����ʱ�䣺	2008 �� 5 �� 28 ��	19:09
*
*	����˵����	
*	�޸ļ�¼��
*********************************************************************************/


#ifndef __CGCOUNTRYDELATE_H__
#define __CGCOUNTRYDELATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class CGCountryDelate : public Packet 
    {
    public:
        CGCountryDelate( ){} ;
        virtual ~CGCountryDelate( ){} ;

        //���ü̳нӿ�
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_CG_COUNTRY_DELATE ; }
        virtual UINT			        GetPacketSize()const { return sizeof(GUID_t) + sizeof(BOOL); }

        VOID                            SetResult (BOOL bResult) { m_bResult = bResult; }
        BOOL                            GetResult () { return m_bResult; }

        VOID                            SetGuid (GUID_t guid) { m_Guid = guid; }
        GUID_t                          GetGuid () { return m_Guid; }

    public:

        BOOL                            m_bResult;
        GUID_t                          m_Guid;
    };


    class CGCountryDelateFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new CGCountryDelate() ; }
        PacketID_t	GetPacketID()const { return PACKET_CG_COUNTRY_DELATE ; }
        UINT		GetPacketMaxSize()const { return sizeof(GUID_t) + sizeof(BOOL); }
    };


    class CGCountryDelateHandler 
    {
    public:
        static UINT Execute( CGCountryDelate* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
