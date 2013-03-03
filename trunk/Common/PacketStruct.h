/////////////////////////////////////////////////////////////////////////////////
//
//	文件名：
//		PacketStruct.h
//	功能描述：
//		消息包结构相关定义
//	修改记录：
//		Aug.16 2007 - Create by Lybin Lee
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __PACKETSTRUCT_H__
#define __PACKETSTRUCT_H__

#include "Type.h"

class		SocketInputStream ;
class		SocketOutputStream ;

namespace	Packets
{
	//////////////////////////////////////////////////
	// About Change Name Packet
	#define	CHANGE_NAME_MAX		32		// 名称长度定义
										// 至少大于角色名 MAX_CHARACTER_NAME、帮会名 MAX_GUILD_NAME_SIZE

	// 更名消息操作类型
	enum	CHANGE_NAME_OPERATE
	{
		CHGNAME_OP_INVALID		= -1 ,	// 无效操作
		CHGNAME_OP_USER ,				// 角色更名
		CHGNAME_OP_GUILD ,				// 帮会更名
	};
	typedef	CHANGE_NAME_OPERATE	CHGNAME_OP ;

	// 更名消息返回类型
	enum	CHANGE_NAME_RESULT
	{
		CHGNAME_RE_OK			= 0 ,	// 更名成功
		CHGNAME_RE_ERROR ,				// 更名失败
		CHGNAME_RE_DBBUSY ,				// DB压力过大，请重新尝试
		CHGNAME_RE_NAMEERROR ,			// 不可接受的新名称
		CHGNAME_RE_REPEATED ,			// 名称重复
	};
	typedef	CHANGE_NAME_RESULT	CHGNAME_RE ;

	// 更名消息结构
	struct	SChgNamePacketUnit
	{
	public :
		SChgNamePacketUnit() ;
		SChgNamePacketUnit&	operator=( SChgNamePacketUnit& sUnit )
		{
			this->SetUserGuid( sUnit.GetUserGuid() ) ;
			this->SetUserOldName( sUnit.GetOperateType(), sUnit.GetUserOldName() ) ;
			this->SetUserNewName( sUnit.GetOperateType(), sUnit.GetUserNewName() ) ;
			this->SetResultType( sUnit.GetResultType() ) ;
			return *this;
		}

		INT					GetOperateType()
		{
			return m_opType ;
		}
		VOID				SetOperateType( INT OpType )
		{
			m_opType		= OpType ;
		}

		GUID_t				GetUserGuid()
		{
			return m_UserGuid ;
		}
		VOID				SetUserGuid( GUID_t UserGuid )
		{
			m_UserGuid		= UserGuid ;
		}

		VOID				SetUserOldName( INT OpType, const CHAR* pName ) ;
		const CHAR*			GetUserOldName()
		{
			return m_UserOldName ;
		}

		VOID				SetUserNewName( INT OpType, const CHAR* pName ) ;
		const CHAR*			GetUserNewName()
		{
			return m_UserNewName ;
		}

		INT					GetResultType()
		{
			return m_reType ;
		}
		VOID				SetResultType( INT reType )
		{
			m_reType		= reType ;
		}

	public :
		static UINT			GetPacketSize() ;
		BOOL				ReadStream( SocketInputStream& iStream ) ;
		BOOL				WriteStream( SocketOutputStream& oStream ) const ;

	private :
		INT					m_opType ;		// 消息操作类型: typedef CHGNAME_OP
		GUID_t				m_UserGuid ;
		CHAR				m_UserOldName[CHANGE_NAME_MAX] ;
		CHAR				m_UserNewName[CHANGE_NAME_MAX] ;
		INT					m_reType ;		// 消息返回类型: typedef CHGNAME_RE
	};

	//////////////////////////////////////////////////
	// About Other Packet

};

using namespace Packets ;

#endif
