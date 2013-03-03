/////////////////////////////////////////////////////////////////////////////////
//
//	�ļ�����
//		PacketStruct.h
//	����������
//		��Ϣ���ṹ��ض���
//	�޸ļ�¼��
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
	#define	CHANGE_NAME_MAX		32		// ���Ƴ��ȶ���
										// ���ٴ��ڽ�ɫ�� MAX_CHARACTER_NAME������� MAX_GUILD_NAME_SIZE

	// ������Ϣ��������
	enum	CHANGE_NAME_OPERATE
	{
		CHGNAME_OP_INVALID		= -1 ,	// ��Ч����
		CHGNAME_OP_USER ,				// ��ɫ����
		CHGNAME_OP_GUILD ,				// ������
	};
	typedef	CHANGE_NAME_OPERATE	CHGNAME_OP ;

	// ������Ϣ��������
	enum	CHANGE_NAME_RESULT
	{
		CHGNAME_RE_OK			= 0 ,	// �����ɹ�
		CHGNAME_RE_ERROR ,				// ����ʧ��
		CHGNAME_RE_DBBUSY ,				// DBѹ�����������³���
		CHGNAME_RE_NAMEERROR ,			// ���ɽ��ܵ�������
		CHGNAME_RE_REPEATED ,			// �����ظ�
	};
	typedef	CHANGE_NAME_RESULT	CHGNAME_RE ;

	// ������Ϣ�ṹ
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
		INT					m_opType ;		// ��Ϣ��������: typedef CHGNAME_OP
		GUID_t				m_UserGuid ;
		CHAR				m_UserOldName[CHANGE_NAME_MAX] ;
		CHAR				m_UserNewName[CHANGE_NAME_MAX] ;
		INT					m_reType ;		// ��Ϣ��������: typedef CHGNAME_RE
	};

	//////////////////////////////////////////////////
	// About Other Packet

};

using namespace Packets ;

#endif
