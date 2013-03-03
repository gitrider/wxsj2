

#ifndef __IDTABLE_H__
#define __IDTABLE_H__

#include "Type.h"


struct _TABLEITEM
{
	uint	m_ID ;
	VOID*	m_pPtr ;
	uint	m_Status ;
};

class IDTable
{
public :
	IDTable( ) ;
	~IDTable( ) ;

	//��ʼ����
	VOID		InitTable( uint MaxItem ) ;
	//����һ������
	BOOL		Add( uint id, VOID* pPtr ) ;
	//��ȡ��Ϣ
	VOID*		Get( uint id ) ;
	//ɾ������
	VOID		Remove( uint id ) ;
	//�����������
	VOID		CleanUp( ) ;

protected :
	enum {
		IDTS_EMPTY = 0 ,
		IDTS_SET = 1 ,
		IDTS_USE = 2 ,
	};

	UINT		toIndex( uint id )
	{
		return (UINT)((id&0xffff)+(id>>6))%m_Count ;
	};

private :

	_TABLEITEM*		m_pTable ;
	uint			m_Count ;


};

#endif
