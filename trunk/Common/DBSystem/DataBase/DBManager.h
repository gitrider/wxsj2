/********************************************************************
	
	��������:	2005��12��1��
	����ʱ��:	11:27
	�ļ�����:	DBManager
	�ļ�·��:	d:\Prj\DBSystem\DataBase
	������:		������
	�ļ����ܣ�  ���ݿ������
	�޸ļ�¼��
*********************************************************************/



#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#include "Type.h"
#include "Thread.h"
#include "DBTypes.h"


class ODBCInterface;

/*
 *	 ���ݿ�����࣬Ҳͨ����Ϣ�ķ�ʽ����
 *   �߳�֮��ͨѶ
 */





class DBManager: public Thread
{

public:
	DBManager();
	~DBManager();
	
	//��ʼ��ģ��
	virtual BOOL	Init();
	
	//ģ�鴦��ѭ��
	virtual  VOID	run();

	//ģ���˳�
	virtual VOID	stop( ){ m_Active = FALSE ; } ;
	//ģ���˳�����
	VOID			Quit( ) ;

	//�жϵ�ǰģ���Ƿ��ڻ״̬
	BOOL			IsActive( ){ return m_Active ; };

private :
	

public:
	ODBCInterface*	GetInterface(DB_NAMES name);
	INT				GetAffectRowCount();
protected:
	//Ĭ�����ݿ�����
	ODBCInterface*	mCharDBInterface;
	//�Ƿ��ı�־
	BOOL			m_Active ;

private:
	
};

extern DBManager*	g_pDBManager;


#endif