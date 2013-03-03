
#include "stdafx.h"
#include "GameStruct_MinorPasswd.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "DB_Struct.h"

/////////////////////////////////////////////////////////////////////////////////

// ����һ������
VOID MINORPASSWD_PWD::CleanUp( )
{
	m_uPasswdSize = 0;
	memset((void*)m_Passwd, 0, sizeof(m_Passwd));
}

UINT MINORPASSWD_PWD::GetSize( ) const
{
	return sizeof(m_uPasswdSize) + sizeof(CHAR) * m_uPasswdSize;
}

VOID MINORPASSWD_PWD::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uPasswdSize, sizeof(m_uPasswdSize) );

	if( m_uPasswdSize > 0 )
	{
		iStream.Read( m_Passwd, sizeof(CHAR) * m_uPasswdSize );
	}
}

VOID MINORPASSWD_PWD::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uPasswdSize, sizeof(m_uPasswdSize) );

	if( m_uPasswdSize > 0 )
	{
		oStream.Write( m_Passwd, sizeof(CHAR) * m_uPasswdSize );
	}
}

// MREQT_MODIFYPASSWD
VOID REQUEST_MINORPASSWD_MODIFY::CleanUp( )
{
	m_uOldPasswdSize = 0;
	memset((void*)m_OldPasswd, 0, sizeof(m_OldPasswd));
	m_uNewPasswdSize = 0;
	memset((void*)m_NewPasswd, 0, sizeof(m_NewPasswd));
}

UINT REQUEST_MINORPASSWD_MODIFY::GetSize( ) const
{
	return sizeof(m_uOldPasswdSize) + sizeof(CHAR) * m_uOldPasswdSize
		 + sizeof(m_uNewPasswdSize) + sizeof(CHAR) * m_uNewPasswdSize;
}

VOID REQUEST_MINORPASSWD_MODIFY::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uOldPasswdSize, sizeof(m_uOldPasswdSize) );

	if( m_uOldPasswdSize > 0 )
	{
		iStream.Read( m_OldPasswd, sizeof(CHAR) * m_uOldPasswdSize );
	}

	iStream.Read( (CHAR*)&m_uNewPasswdSize, sizeof(m_uNewPasswdSize) );

	if( m_uNewPasswdSize > 0 )
	{
		iStream.Read( m_NewPasswd, sizeof(CHAR) * m_uNewPasswdSize );
	}
}

VOID REQUEST_MINORPASSWD_MODIFY::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uOldPasswdSize, sizeof(m_uOldPasswdSize) );

	if( m_uOldPasswdSize > 0 )
	{
		oStream.Write( m_OldPasswd, sizeof(CHAR) * m_uOldPasswdSize );
	}

	oStream.Write( (CHAR*)&m_uNewPasswdSize, sizeof(m_uNewPasswdSize) );

	if( m_uNewPasswdSize > 0 )
	{
		oStream.Write( m_NewPasswd, sizeof(CHAR) * m_uNewPasswdSize );
	}
}

//�ͻ�������������͵���ϵ����ؽṹ����
VOID CG_MINORPASSWD::CleanUp( )
{
	m_Type = MREQT_NONE;
    m_nFlag= 0;
}

UINT CG_MINORPASSWD::GetSize( ) const
{
	UINT uSize = sizeof(m_Type);

	switch( m_Type )
	{
	// �޲�������
	case MREQT_PASSWDSETUP:
	case MREQT_DELETEPASSWDTIME:
	case MREQT_DELETEPASSWD:
		break;
	case MREQT_SETPASSWD:
	case MREQT_UNLOCKPASSWD:
		uSize += m_OnePasswd.GetSize();
		break;
	case MREQT_MODIFYPASSWD:
    case MREQT_CLEANPASSWD:
		uSize += m_ModifyPasswd.GetSize();
		break;
    case MREQT_SETPROTECTTYPE:
        uSize += sizeof(m_nFlag);
        break;
    case MREQT_SETPROTECTTIME:
        uSize += sizeof(m_cTime);
        break;
	default:
		Assert(FALSE);
		return 0;
	}

	return uSize;
}

VOID CG_MINORPASSWD::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	// �޲�������
	case MREQT_PASSWDSETUP:
	case MREQT_DELETEPASSWDTIME:
	case MREQT_DELETEPASSWD:
		break;
	case MREQT_SETPASSWD:
	case MREQT_UNLOCKPASSWD:
		m_OnePasswd.CleanUp();
		m_OnePasswd.Read(iStream);
		break;
	case MREQT_MODIFYPASSWD:
    case MREQT_CLEANPASSWD:
		m_ModifyPasswd.CleanUp();
		m_ModifyPasswd.Read(iStream);
		break;
    case MREQT_SETPROTECTTYPE:
	    iStream.Read( (CHAR*)&m_nFlag, sizeof(m_nFlag) );
        break;
    case MREQT_SETPROTECTTIME:
	    iStream.Read( (CHAR*)&m_cTime, sizeof(m_cTime) );
        break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID CG_MINORPASSWD::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
		// �޲�������
	case MREQT_PASSWDSETUP:
	case MREQT_DELETEPASSWDTIME:
	case MREQT_DELETEPASSWD:
		break;
	case MREQT_SETPASSWD:
	case MREQT_UNLOCKPASSWD:
		m_OnePasswd.Write(oStream);
		break;
	case MREQT_MODIFYPASSWD:
    case MREQT_CLEANPASSWD:
		m_ModifyPasswd.Write(oStream);
		break;
    case MREQT_SETPROTECTTYPE:
        oStream.Write( (CHAR*)&m_nFlag, sizeof(m_nFlag) );
        break;
    case MREQT_SETPROTECTTIME:
        oStream.Write( (CHAR*)&m_cTime, sizeof(m_cTime) );
        break;
	default:
		Assert(FALSE);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////

// MRETT_PASSWDSETUP
VOID RETURN_MINORPASSWD_SETUP::CleanUp( )
{
	m_uFlag = 0;
}

UINT RETURN_MINORPASSWD_SETUP::GetSize( ) const
{
	return sizeof(m_uFlag);
}

VOID RETURN_MINORPASSWD_SETUP::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_uFlag, sizeof(m_uFlag) );
}

VOID RETURN_MINORPASSWD_SETUP::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_uFlag, sizeof(m_uFlag) );
}

// MRETT_DELETEPASSWDTIME
VOID RETURN_DELETE_MINORPASSWD_TIME::CleanUp( )
{
	m_uTime = UINT_MAX;
}

UINT RETURN_DELETE_MINORPASSWD_TIME::GetSize( ) const
{
	return sizeof(m_uTime);
}

VOID RETURN_DELETE_MINORPASSWD_TIME::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_uTime, sizeof(m_uTime) );
}

VOID RETURN_DELETE_MINORPASSWD_TIME::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_uTime, sizeof(m_uTime) );
}

VOID GC_MINORPASSWD::CleanUp( )
{
	m_Type = MRETT_NONE;
}

UINT GC_MINORPASSWD::GetSize( ) const
{
	UINT uSize = sizeof(m_Type);

	switch( m_Type )
	{
	case MRETT_SETPASSWDSUCC:
	case MRETT_MODIFYPASSWDSUCC:
	case MRETT_UNLOCKPASSWDSUCC:
	case MRETT_DELETEPASSWDCANCEL:
	case MRETT_DELETEPASSWDSUCC:
	case MRETT_ERR_SETPASSWDFAIL:
	case MRETT_ERR_MODIFYPASSWDFAIL:
	case MRETT_ERR_UNLOCKPASSWDFAIL:
	case MRETT_ERR_DELETEPASSWDFAIL:
    case MRETT_ERR_CLEANPASSWDFAIL:
    case MRETT_ERR_PASSWD:
		break;
	case MRETT_PASSWDSETUP:
    case MRETT_CLEANPASSWDSUCC:
    case MRETT_SETPASSWDPROTECT:
		uSize += m_ReturnSetup.GetSize();
		break;
	case MRETT_DELETEPASSWDTIME:
    case MRETT_SETPROTECTTIMESUCC:
		uSize += m_ReturnDeleteTime.GetSize();
		break;
	default :
		Assert(FALSE);
		return 0;
	};

	return uSize;
}

VOID GC_MINORPASSWD::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case MRETT_SETPASSWDSUCC:
	case MRETT_MODIFYPASSWDSUCC:
	case MRETT_UNLOCKPASSWDSUCC:
	case MRETT_DELETEPASSWDCANCEL:
	case MRETT_DELETEPASSWDSUCC:
	case MRETT_ERR_SETPASSWDFAIL:
	case MRETT_ERR_MODIFYPASSWDFAIL:
	case MRETT_ERR_UNLOCKPASSWDFAIL:
	case MRETT_ERR_DELETEPASSWDFAIL:
    case MRETT_ERR_CLEANPASSWDFAIL:
    case MRETT_ERR_PASSWD:
		break;
    case MRETT_CLEANPASSWDSUCC:
        m_ReturnSetup.CleanUp();
        m_ReturnSetup.Read(iStream);
        break;
	case MRETT_PASSWDSETUP:
	case MRETT_SETPASSWDPROTECT:
		m_ReturnSetup.CleanUp();
		m_ReturnSetup.Read(iStream);
		break;
	case MRETT_DELETEPASSWDTIME:
    case MRETT_SETPROTECTTIMESUCC:
		m_ReturnDeleteTime.CleanUp();
		m_ReturnDeleteTime.Read(iStream);
		break;
	default :
		Assert(FALSE);
		return;
	};
}

VOID GC_MINORPASSWD::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case MRETT_SETPASSWDSUCC:
	case MRETT_MODIFYPASSWDSUCC:
	case MRETT_UNLOCKPASSWDSUCC:
	case MRETT_DELETEPASSWDCANCEL:
	case MRETT_DELETEPASSWDSUCC:
	case MRETT_ERR_SETPASSWDFAIL:
	case MRETT_ERR_MODIFYPASSWDFAIL:
	case MRETT_ERR_UNLOCKPASSWDFAIL:
	case MRETT_ERR_DELETEPASSWDFAIL:
    case MRETT_ERR_CLEANPASSWDFAIL:
    case MRETT_ERR_PASSWD:
		break;
    case MRETT_CLEANPASSWDSUCC:
        m_ReturnSetup.Write(oStream);
        break;
	case MRETT_PASSWDSETUP:
    case MRETT_SETPASSWDPROTECT:
		m_ReturnSetup.Write(oStream);
		break;
	case MRETT_DELETEPASSWDTIME:
    case MRETT_SETPROTECTTIMESUCC:
		m_ReturnDeleteTime.Write(oStream);
		break;
	default :
		Assert(FALSE);
		return;
	};
}

