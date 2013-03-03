// ������Ϣ�ṹ
// fancy

#ifndef __GAMESTRUCT_MINORPASSWD_H__
#define __GAMESTRUCT_MINORPASSWD_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

/////////////////////////////////////////////////////////////////////////////////

enum MINORPASSWD_REQUEST_TYPE
{
	MREQT_NONE							= 0,
	MREQT_PASSWDSETUP,									// ѯ�ʶ��������Ƿ��Ѿ�����
	MREQT_DELETEPASSWDTIME,								// ѯ���Ƿ���ǿ�ƽ���׶�
	MREQT_SETPASSWD,									// ���ö�������
	MREQT_MODIFYPASSWD,									// �޸Ķ�������
	MREQT_UNLOCKPASSWD,									// �����������
	MREQT_DELETEPASSWD,									// ǿ�ƽ����������
	MREQT_CLEANPASSWD,									// ��ն�������
	MREQT_SETPROTECTTYPE,								// ���ö������뱣������
	MREQT_SETPROTECTTIME,								// �������߱���ʱ��
};

enum MINORPASSWD_RETURN_TYPE
{
	MRETT_NONE							= 0,
	MRETT_PASSWDSETUP,									// ���������Ƿ��Ѿ�����
	MRETT_DELETEPASSWDTIME,								// ����������ʣ��ʱ�䣨���߲��ڽ���׶Σ�
	MRETT_SETPASSWDSUCC,								// �����������óɹ�
	MRETT_MODIFYPASSWDSUCC,								// ���������޸ĳɹ�
	MRETT_UNLOCKPASSWDSUCC,								// ������������ɹ�
	MRETT_DELETEPASSWDCANCEL,							// ǿ�ƽ����������ʧЧ
	MRETT_DELETEPASSWDSUCC,								// ��������ǿ�ƽ���ɹ�
	MRETT_CLEANPASSWDSUCC,								// ��ն�������ɹ�
	MRETT_SETPASSWDPROTECT, 							// ���ñ������ͳɹ�
	MRETT_SETPROTECTTIMESUCC,    						// �������߱���ʱ��ɹ�

	MRETT_ERR_START,
	MRETT_ERR_SETPASSWDFAIL,							// ������������ʧ��
	MRETT_ERR_MODIFYPASSWDFAIL,							// ���������޸�ʧ��
	MRETT_ERR_UNLOCKPASSWDFAIL,							// �����������ʧ��
	MRETT_ERR_DELETEPASSWDFAIL,							// ��������ǿ�ƽ��ʧ��
	MRETT_ERR_CLEANPASSWDFAIL,					    	// ��ն�������ʧ��
    MRETT_ERR_SETPROTECTTIMEFAIL,                       // �������߱���ʱ��ʧ��
    MRETT_ERR_PASSWD,								    // У������ʧ�� 
};

// �޲���
// MREQT_PASSWDSETUP
// MREQT_DELETEPASSWDTIME
// MREQT_DELETEPASSWD

// ����һ������
// MREQT_SETPASSWD
// MREQT_UNLOCKPASSWD
struct MINORPASSWD_PWD
{
	//����
	UCHAR								m_uPasswdSize;
	CHAR								m_Passwd[MAX_PWD];		//��������

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetPasswdSize( ) { return m_uPasswdSize; }

	const CHAR*							GetPasswd( ) { return m_Passwd; }
	VOID								SetPasswd( const CHAR* szPasswd )
	{
		Assert( szPasswd );
		strncpy(m_Passwd, szPasswd, sizeof(m_Passwd) - 1);
		m_uPasswdSize = (UCHAR)strlen(m_Passwd);
	}
};

// MREQT_MODIFYPASSWD
struct REQUEST_MINORPASSWD_MODIFY
{
	//����
	UCHAR								m_uOldPasswdSize;
	CHAR								m_OldPasswd[MAX_PWD];	// �ɶ�������
	UCHAR								m_uNewPasswdSize;
	CHAR								m_NewPasswd[MAX_PWD];	// �¶�������

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UCHAR								GetOldPasswdSize( ) { return m_uOldPasswdSize; }

	const CHAR*							GetOldPasswd( ) { return m_OldPasswd; }
	VOID								SetOldPasswd( const CHAR* szPasswd )
	{
		Assert( m_OldPasswd );
		strncpy(m_OldPasswd, szPasswd, sizeof(m_OldPasswd) - 1);
		m_uOldPasswdSize = (UCHAR)strlen(m_OldPasswd);
	}

	UCHAR								GetPasswdSize( ) { return m_uNewPasswdSize; }

	const CHAR*							GetNewPasswd( ) { return m_NewPasswd; }
	VOID								SetNewPasswd( const CHAR* szPasswd )
	{
		Assert( m_NewPasswd );
		strncpy(m_NewPasswd, szPasswd, sizeof(m_NewPasswd) - 1);
		m_uNewPasswdSize = (UCHAR)strlen(m_NewPasswd);
	}
};

// e.g.
// set minor password
// CGMinorPasswd Msg;
// CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
// pMinorPasswd->CleanUp();
// pMinorPasswd->m_Type = MREQT_SETPASSWD;
// pMinorPasswd->m_OnePasswd.CleanUp();
// pMinorPasswd->m_OnePasswd.SetPasswd( szPasswd );

struct CG_MINORPASSWD
{
	UCHAR								m_Type;				// enum MINORPASSWD_REQUEST_TYPE

	union
	{
		MINORPASSWD_PWD					m_OnePasswd;
		REQUEST_MINORPASSWD_MODIFY		m_ModifyPasswd;
        INT                             m_nFlag;            // ��������
        CHAR                            m_cTime;            // ���߱���ʱ��
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

/////////////////////////////////////////////////////////////////////////////////

// �޲���

// MRETT_SETPASSWDSUCC
// MRETT_MODIFYPASSWDSUCC
// MRETT_UNLOCKPASSWDSUCC
// MRETT_DELETEPASSWDCANCEL
// MRETT_DELETEPASSWDSUCC

// MRETT_ERR_SETPASSWDFAIL
// MRETT_ERR_MODIFYPASSWDFAIL
// MRETT_ERR_UNLOCKPASSWDFAIL
// MRETT_ERR_DELETEPASSWDFAIL

// MRETT_PASSWDSETUP
struct RETURN_MINORPASSWD_SETUP
{
	INT 								m_uFlag;			// 0 Ϊδ���ã�1 Ϊ������

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	INT 								GetSetupFlag( ) { return m_uFlag; }
	VOID								SetSetupFlag( INT uFlag ) { m_uFlag = uFlag; }
};

// MRETT_DELETEPASSWDTIME
struct RETURN_DELETE_MINORPASSWD_TIME
{
	UINT								m_uTime;			// 0 Ϊδ����ǿ�ƽ���׶Σ�����Ϊǿ�ƽ����ʱ��

	//�����ӿ�
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//����Ӧ�ýӿ�
	UINT								GetDeleteTime( ) { return m_uTime; }
	VOID								SetDeleteTime( UINT uTime ) { m_uTime = uTime; }
};

struct GC_MINORPASSWD
{
	UCHAR								m_Type;				// enum MINORPASSWD_RETURN_TYPE

	union
	{
		RETURN_MINORPASSWD_SETUP		m_ReturnSetup;
		RETURN_DELETE_MINORPASSWD_TIME	m_ReturnDeleteTime;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

struct MINORPASSWD_INFO                                 //���뱣�����ݽṹ
{
    enum PROTECT_TYPE
    {
        PROTECT_EQUIP = 1,                              //�Ѿ��󶨵�װ��
        PROTECT_BANK  = 2,                              //�ֿ�
        PROTECT_MONEY = 4,                              //���ʹ��
        PROTECT_STALL = 8,                              //��̯�͹���
        PROTECT_GUILD = 16,                             //���
    };

public:
    MINORPASSWD_INFO ()
    {
        m_nFlag             = 0;
        m_cProtectTime      = 3;                        //Ĭ�ϱ���ʱ��3min
        m_bUnProtect        = FALSE;
        //memset(m_szPasswd, 0, MAX_PWD);
    }

public:
    VOID            SetFlag( PROTECT_TYPE type ) { m_nFlag |= type; };
    VOID            SetFlag( INT nType ) { m_nFlag = nType; };
    BOOL            CompareFlag ( PROTECT_TYPE type ) { return type & m_nFlag; };

    VOID            SetProtectTime ( CHAR cTime ) { m_cProtectTime = cTime; };
    CHAR            GetProtectTime () { return m_cProtectTime; };

    VOID            SetProtectFlag (BOOL bFlag) { m_bUnProtect = bFlag; };
    BOOL            GetProtectFlag () { return m_bUnProtect; };
private:
    //CHAR			m_szPasswd[MAX_PWD];				//��������
    BOOL            m_bUnProtect;                       //�Ƿ���ʱȡ�����뱣��
    INT             m_nFlag;                            //��������
    CHAR            m_cProtectTime;                     //��¼����ʱ��
};

//////////////////////////////////////////////////////////////////////////

#endif // __GAMESTRUCT_MINORPASSWD_H__
