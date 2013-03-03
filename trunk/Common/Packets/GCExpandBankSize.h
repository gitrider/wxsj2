// GCExpandBankSize.h
// 
// 扩展银行Size
// 
//////////////////////////////////////////////////////

#ifndef __GCEXPANDBANKSIZE_H__
#define __GCEXPANDBANKSIZE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
    class GCExpandBankSize : public Packet
    {
    public:
        GCExpandBankSize( )
        {
            m_cSize = 0;
        }
        virtual ~GCExpandBankSize( ){};

        //公用继承接口
        virtual BOOL			Read( SocketInputStream& iStream ) ;
        virtual BOOL			Write( SocketOutputStream& oStream )const ;
        virtual UINT			Execute( Player* pPlayer ) ;

        virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXPANDBANKSIZE; }
        virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

    public:
        BYTE					GetBankSize(VOID) const {return m_cSize;};
        VOID					SetBankSize(BYTE Size) {m_cSize = Size;};

    private:
        BYTE					m_cSize;
    };

    class GCExpandBankSizeFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCExpandBankSize() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_EXPANDBANKSIZE; };
        UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
    };

    class GCExpandBankSizeHandler 
    {
    public:
        static UINT Execute( GCExpandBankSize* pPacket, Player* pPlayer ) ;
    };
}

using namespace Packets;

#endif
