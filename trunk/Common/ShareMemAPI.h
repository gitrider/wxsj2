#ifndef _SHAREMEM_API_H_
#define _SHAREMEM_API_H_
#include "Type.h"


#ifdef	__WINDOWS__
#define INVALID_SM_HANDLE	 ((VOID*)0)
#elif	 __LINUX__
#define INVALID_SM_HANDLE	 -1
#endif



namespace	ShareMemAPI
{

	/*����ShareMem �ڴ���
	 *	
	 *	key   ����ShareMem �Ĺؼ�ֵ
	 *
	 *  Size  ������С
	 *
	 *	���� ��ӦShareMem����ֵ
	 */
	SMHandle		CreateShareMem(SM_KEY key,UINT Size);
	/*��ShareMem �ڴ���
	 *	
	 * key   ��ShareMem �Ĺؼ�ֵ
	 * 
	 * Size  �򿪴�С
	 *
	 * ����  ��ӦShareMem ����ֵ
	 */
	SMHandle		OpenShareMem(SM_KEY key,UINT Size);
	
	/*ӳ��ShareMem �ڴ���
	 *	
	 *	handle ӳ��ShareMem �ı���ֵ
	 *
	 *  ���� ShareMem ������ָ��
	 */
	CHAR*			MapShareMem(SMHandle handle);
	
	/*�ر�ӳ�� ShareMem �ڴ���
	 *
	 *	MemoryPtr			ShareMem ������ָ��
	 *	
	 *  
	 */	
	VOID			UnMapShareMem(CHAR* MemoryPtr);
	
	/*	�ر�ShareMem
	 * 	handle  ��Ҫ�رյ�ShareMem ����ֵ
	 */
	VOID			CloseShareMem(SMHandle handle);


}


#endif

