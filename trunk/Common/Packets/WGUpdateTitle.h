/********************************************************************************
 *	文件名：	WGUpdateTitle.h
 *	全路径：	d:\Prj\Common\Packets\WGUpdateTitle.h
 *	创建人：	Amos
 *	创建时间：	2008 年 3 月 28 日	
 *
 *	功能说明：	处理其他玩家修改自身称号名字
 *	修改记录：
*********************************************************************************/

#ifndef __WG_UPDATETITLE_H__
#define __WG_UPDATETITLE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGUpdateTitle : public Packet
	{
	public:
		WGUpdateTitle()
        {
            memset(m_szTitle, 0, MAX_CHARACTER_TITLE);
        }
		virtual					~WGUpdateTitle(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CHARUPDATECURTITLE; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(PlayerID_t) + sizeof(CHAR) + m_uTitleSize + sizeof(INT) + sizeof(BYTE);
		}

	public:
        VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
        PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

        UCHAR			GetTitleSize() { return m_uTitleSize; }
        VOID			SetTitleSize( UCHAR uSize ) { m_uTitleSize = uSize; };

        const CHAR*		GetTitle( ) { return m_szTitle; }
        VOID			SetTitle( const CHAR* pTitle )
        {
            strncpy( m_szTitle, pTitle, MAX_CHARACTER_TITLE-1 );
            m_uTitleSize = (UCHAR)strlen(m_szTitle);
        }

        BYTE			GetTitleType(VOID) { return m_TitleType; }
        VOID			SetTitleType(BYTE titleType) { m_TitleType	= titleType; }

        INT  			GetTitleID(VOID) { return m_TitleID; }
        VOID			SetTitleID(INT nID) { m_TitleID = nID; }

    private:
		PlayerID_t		m_PlayerID;	//连接者
        UCHAR			m_uTitleSize;						// 称号名称
        BYTE			m_TitleType;	
        INT             m_TitleID;
        CHAR			m_szTitle[MAX_CHARACTER_TITLE];		// 称号

	};

	class WGUpdateTitleFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new WGUpdateTitle(); }
		PacketID_t				GetPacketID() const { return PACKET_WG_CHARUPDATECURTITLE; }
		UINT					GetPacketMaxSize() const
        {
            return sizeof(GUID_t) + sizeof(PlayerID_t) + MAX_CHARACTER_TITLE
                    + sizeof(INT) + sizeof(BYTE);
        }
	};

	class WGUpdateTitleHandler
	{
	public:
		static UINT Execute( WGUpdateTitle* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // 
