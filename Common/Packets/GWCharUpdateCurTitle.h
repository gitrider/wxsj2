
/**	
*	
*	功能：	
*
*	修改记录：
*			080327	转发更新人物称号的名称
*			
*			
*/

#ifndef __GW_CHARUPDATECURTITLE_H__
#define __GW_CHARUPDATECURTITLE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
    class GWCharUpdateCurTitle : public Packet 
    {
    public:
        GWCharUpdateCurTitle()
        {
            m_TitleID = INVALID_ID;
        } ;
        virtual ~GWCharUpdateCurTitle(){} ;

        //公用继承接口
        virtual BOOL			Read( SocketInputStream& iStream ) ;
        virtual BOOL			Write( SocketOutputStream& oStream )const ;
        virtual UINT			Execute( Player* pPlayer ) ;

        virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHARUPDATECURTITLE ; }
        virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2 + sizeof(INT) +
                                                              sizeof(GUID_t) +
                                                              m_cTitleSize; }

    public:
        //使用数据接口
        BYTE					getTitleType(VOID) { return m_TitleType; }
        VOID					setTitleType(BYTE titleType) { m_TitleType	= titleType; }

        INT  					getTitleID(VOID) { return m_TitleID; }
        VOID					setTitleID(INT nID) { m_TitleID = nID; }

        VOID                    setGuid(GUID_t guid) { m_Guid = guid; };
        GUID_t                  getGuid() { return m_Guid; };

        VOID                    setTitleSize(BYTE titleSize) { m_cTitleSize = titleSize; };
        BYTE                    getTitleSize() { return m_cTitleSize; };

        VOID                    setTitleName(const CHAR* pSzTitle)
        {
            memcpy(m_szTitleName, pSzTitle, MAX_CHARACTER_TITLE);
        }
        const CHAR*             getTitleName() {return m_szTitleName; };

    private:
        //数据
        GUID_t                  m_Guid;                             //要修改称号的玩家
        BYTE                    m_cTitleSize;                       
        BYTE					m_TitleType;	
        INT                     m_TitleID;
        CHAR                    m_szTitleName[MAX_CHARACTER_TITLE];
    };


    class GWCharUpdateCurTitleFactory : public PacketFactory 
    {
    public:
        Packet*		CreatePacket() { return new GWCharUpdateCurTitle() ; }
        PacketID_t	GetPacketID()const { return PACKET_GW_CHARUPDATECURTITLE ; }
        UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2 +
                                                     sizeof(INT) +
                                                     sizeof(GUID_t) +
                                                     MAX_CHARACTER_TITLE; }
    };


    class GWCharUpdateCurTitleHandler 
    {
    public:
        static UINT Execute( GWCharUpdateCurTitle* pPacket, Player* pPlayer ) ;
    };
};

using namespace Packets ;

#endif
