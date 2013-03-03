// GCBankSize.h
// 
// 银行当前Size
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKSIZE_H__
#define __GCBANKSIZE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
    class GCBankSize : public Packet
    {
    public:
        GCBankSize( )
        {
            m_cSize = 0;
        }
        virtual ~GCBankSize( ){};

        //公用继承接口
        virtual BOOL			Read( SocketInputStream& iStream ) ;
        virtual BOOL			Write( SocketOutputStream& oStream )const ;
        virtual UINT			Execute( Player* pPlayer ) ;

        virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKSIZE; }
        virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

    public:
        BYTE					GetBankSize(VOID) const {return m_cSize;};
        VOID					SetBankSize(BYTE Size) {m_cSize = Size;};

    private:
        BYTE					m_cSize;
    };

    class GCBankSizeFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCBankSize() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_BANKSIZE; };
        UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
    };

    class GCBankSizeHandler 
    {
    public:
        static UINT Execute( GCBankSize* pPacket, Player* pPlayer ) ;
    };
}

using namespace Packets;

#endif
