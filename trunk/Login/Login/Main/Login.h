#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Type.h"

#define FILE_LOGIN_FONT		"./Config/ascii.cpf"

class Login
{
public:
	Login();
	~Login();

public:
	//Login ��������ʼ��
	BOOL		Init();

	//Login ��������ѭ��
	BOOL		Loop();

	//Login �������˳�
	BOOL		Exit();


	//ֹͣģ��ִ�У������ô˽ӿں󣬻Ὣ����ӵ���̵߳�ģ������Ϊ���״̬
	//�������̵߳�ѭ��������һ��ѭ��������ͻ��˳�
	VOID		Stop( ) ;

protected:
	//����������ݿռ�
	BOOL		NewLogin();
	//��ʼ��������ݿռ�
	BOOL		InitLogin();
	BOOL		m_bExited;

	CMyTimer		m_FlushLogTimer ;			//��־��������
	CMyTimer		m_ClearConnectThreadTimer;	//�������DB������
};


extern Login	g_Login;

class LoginExceptionHandler
{
public:
	LoginExceptionHandler();
	static VOID INTHandler(INT);
	static VOID TERMHandler(INT);
	static VOID ABORTHandler(INT);
	static VOID ILLHandler(INT);
	static VOID FPEHandler(INT);
	static VOID SEGHandler(INT);
	static VOID XFSZHandler(INT);
};

extern LoginExceptionHandler g_LoginExceptionHandler;


#endif
