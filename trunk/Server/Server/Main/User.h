//
// User����������������ݵļ��ϣ�������п��ܵ�ǰ��������
//
//
#ifndef __USER_H__
#define __USER_H__


#include "Type.h"



//��Ч���û�ID���
#define INVALID_UID -1
//һ��������Server������������ӵ�е�������ݸ���
#define MAX_USER 2048






struct _USERINFO
{
	ID_t		m_UID ;


	_USERINFO(){
		m_UID = INVALID_UID ;
	};
};





class User
{
public :
	User() ;
	~User() ;



public :
	_USERINFO	m_Info ;

};





#endif
