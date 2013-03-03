//*******************************************************************************
//	�ļ�����	OnlineUser.h
//	ȫ·����	d:\Prj\World\World\WorldData\AllUser.h
//	����˵����	������������������е�½���������Ϣ�����ڲ�ѯ
//	�޸ļ�¼��	2005��12��15����
//*******************************************************************************

#ifndef __ALLUSER_H__
#define __ALLUSER_H__

#include "Type.h"
#include "IDTable.h"

class AllUser
{
public :
	AllUser( );
	~AllUser( );

	BOOL						Init( );
	VOID						CleanUp( );

	//�����ݿ�������һ��������Ҽ�¼
	BOOL						AddUser( const USER_SIMPLE_DATA* pSimpleUser );

	//���ݽ�ɫguid�������
	UINT						Select( GUID_t guid, RETURN_FINGER_PLAYERLIST* pPlayerList );

	//����������ɫ�����������
	UINT						Select( const CHAR* szName, RETURN_FINGER_PLAYERLIST* pPlayerList, BOOL bPreciseFlag, INT nPosition = 0 );

protected:
	USER_SIMPLE_DATA*			m_pUsers;			//�û����ݳ�
	UINT						m_uMaxUserCount;	//�û����ݳش�С
	UINT						m_uCurUserCount;	//��ǰ�û�����
	UINT						m_uSelectOffset;	//��ǰ������ʼλ��

	IDTable						m_IDTable;	//����������

};


extern AllUser* g_pAllUser;


#endif
