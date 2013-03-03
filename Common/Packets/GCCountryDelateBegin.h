/********************************************************************************
*	文件名：	GCCountryDelateBegin.h
*	全路径：	d:\Prj\Server\Common\Packets\GCCountryDelateBegin.h
*	创建人：	李一立
*	创建时间：	2008 年 5 月 26 日	15:09
*
*	功能说明：	
*	修改记录：
*********************************************************************************/


#ifndef __GCCOUNTRYDELATEBEGIN_H__
#define __GCCOUNTRYDELATEBEGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GCCountryDelateBegin : public Packet 
    {
    public:
        GCCountryDelateBegin( )
        {
            memset( m_szKindomName, 0, MAX_CHARACTER_NAME);
        };
        virtual ~GCCountryDelateBegin( ){} ;

        //公用继承接口
        virtual BOOL			        Read( SocketInputStream& iStream ) ;
        virtual BOOL			        Write( SocketOutputStream& oStream )const ;
        virtual UINT			        Execute( Player* pPlayer ) ;

        virtual PacketID_t		        GetPacketID()const { return PACKET_GC_COUNTRY_DELATE_BEGIN ; }
        virtual UINT			        GetPacketSize()const { return sizeof(BYTE)*2 + m_cNameSize; }

        VOID                            SetCurNum (BYTE cNum) { m_cCurNum = cNum; };
        BYTE                            GetCurNum () { return m_cCurNum; };

        VOID                            SetName(CHAR* szName) { memcpy(m_szKindomName, szName, MAX_CHARACTER_NAME); };
        CHAR*                           GetName() { return m_szKindomName; };

        VOID                            SetNameSize(BYTE cSize) { m_cNameSize = cSize; }
        BYTE                            GetNameSize() { return m_cNameSize; }

    private:
        BYTE                            m_cCurNum;  //当前票数
        BYTE                            m_cNameSize;
        CHAR                            m_szKindomName[MAX_CHARACTER_NAME];
    };


    class GCCountryDelateBeginFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GCCountryDelateBegin() ; }
        PacketID_t	GetPacketID()const { return PACKET_GC_COUNTRY_DELATE_BEGIN ; }
        UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2 +MAX_CHARACTER_NAME; }
    };


    class GCCountryDelateBeginHandler 
    {
    public:
        static UINT Execute( GCCountryDelateBegin* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
