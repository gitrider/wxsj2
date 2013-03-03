/********************************************************************
	��������:	2005��11��1��
	����ʱ��:	13:10
	�ļ�����:	StoreManager.h
	�ļ�·��:	d:\Prj\Server\Server\Obj\StoreManager.h
	������:		������
	
	�ļ����ܣ� �ر��������
			   ÿ������Я��һ���ر��������
	�޸ļ�¼��
	
*********************************************************************/

#ifndef _STORE_MANAGER_H_
#define _STORE_MANAGER_H_
#include "Type.h"



//ÿ������������ر������
#define MAX_MANAGERSTOREPOINT 10


//�ر�������
struct STORE_POINT
{
	WORLD_POS	m_Pos;				//�ر�����������
	INT			m_Type;				//�ر�������----��
};

class	StoreManager
{
	
	INT						m_Count;								//�ر������
	STORE_POINT				m_StorePoint[MAX_MANAGERSTOREPOINT];	//�ر�������


public:
	StoreManager();
	
	/*
	 *	��Ӳر���
	 *
	 *	��ڣ�
	 *		const STORE_POINT* point		�ر���ָ��
	 *	���أ�
	 *		BOOL							0 ����ʧ��		1 ��ʾ�ɹ�
	 */
	BOOL					AddStorePoint(const STORE_POINT* point);
	 /*
	  *	ɾ���ر���
	  *
	  *		����˵����
	  *			INT iIndex	�ر�����
	  *		
	  *		����	 
				BOOL		0��ʾʧ��	1��ʾ�ɹ�
	  *
	  *
	  */
	BOOL					DelStorePoint(INT iIndex);

	/*	���Ҳر���
	 *	
	 *	 ����˵����
	 *
	 *		WORLD_POS& pos			��������
	 *	
	 *		INT& index				�ҵ��󷵻صı��,û�ҵ�����-1
	 *		
	 *	 ����
	 *		STORE_POINT*			
	 *								�ҵ��˷��زر���ָ�룬û�ҵ�����NULL
	 *
	 *
	 */
	STORE_POINT*			FindStorePointByPos(WORLD_POS& pos,INT& index);

	/*	
	 *	�����Ĳ��Ҳر���
	 *	
	 *	 ����˵����
	 *
	 *		WORLD_POS& pos			��������
	 *	
	 *		INT& index				�ҵ��󷵻صı��,û�ҵ�����-1
	 *		
	 *		FLOAT	fRange			��Χ����
	 *		
	 *	 ����
	 *		STORE_POINT*			
	 *								�ҵ��˷��زر���ָ�룬û�ҵ�����NULL
	 *
	 *
	 */

	BOOL					FindStorePointByPosRange(WORLD_POS& pos,INT& index,FLOAT fRange);
	/*
	 *	��òر�������
	 *	
	 *		INT		index			�ر�����
	 *
	 *		���أ�
	 *		INT						����
	 */
	INT						GetStorePointType(INT index);

	/*
	 *  ��òر���λ��
	 *
	 */
	WORLD_POS				GetStorePointPos(INT index);

};







#endif