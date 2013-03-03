/********************************************************************
	created:	2008/05/30
	created:	30:5:2008   15:15
	filename: 	d:\prj\src\Common\Array_Base.h
	file path:	d:\prj\src\Common
	file base:	Array_Base
	file ext:	h
	author:		wuwenbin
	
	purpose:	�����װ�࣬������֮
*********************************************************************/

#ifndef Array_Base_h__
#define Array_Base_h__

#include "Type.h"
#include <malloc.h>

template <class T>
class Array_Base
{
public:
	/*
	 *	�ڹ��캯���з������е��ڴ�
	 * pAlloc: ������ָ�������ڴ���׵�ַ
	 *		   �������������͵��ڴ�ռ�
	 *			pAlloc=0ʱ���Զ�new�㹻�Ŀռ䣬���鲻Ҫ��̬new
	 *			���ʹ��Stack��Ԥ�ȷ���õ��ڴ�
	 * iSize: ָ������Ĵ�С
	 *=============================================================
	 * NOTE: pAlloc��Чʱ����ָ��memoryҪ >= sizeof(T)*iSize
	 *=============================================================
	 */
	Array_Base(INT iSize=0, VOID* pAlloc=NULL);
	/*
	*	�����������������ͷ������ڴ�
	*/
	~Array_Base();

	/*
	*	�ڹ��캯���з������е��ڴ�
	* pAlloc: ������ָ�������ڴ���׵�ַ
	*		   �������������͵��ڴ�ռ�
	*			pAlloc=0ʱ���Զ�new�㹻�Ŀռ䣬���鲻Ҫ��̬new
	*			���ʹ��Stack��Ԥ�ȷ���õ��ڴ�
	* iSize: ָ������Ĵ�С
	*==============================================================
	* NOTE: pAlloc��Чʱ����ָ��memoryҪ >= sizeof(T)*iSize
	*==============================================================
	*/
	VOID Reserve(INT iSize, VOID* pAlloc=NULL);

	/*
	 *	��ȡ��ǰ��Ч�ڵ����
	 */
	INT GetSize() const;
	/*
	 *	��ȡ��ǰ���ڵ����
	 */
	INT GetMaxSize() const;
	/*
	*	�Ƴ����нڵ㣬�����������ͷ��ڴ�
	*/
	VOID RemoveAll();
	/*
	*	�Ƴ������ڵ㣬�����������ͷ��ڴ�
	*/
	VOID Remove(INT iIndex);
	/*
	*	���ӽڵ㣬���ᶯ̬�����ڴ�
	*/
	BOOL Add(const T& value);
	/*
	*	��ȡ��ؽڵ�����
	*/
	const T& Get(INT iIndex) const;
	T& Get(INT iIndex);
	T& operator[](INT iIndex);
	const T& operator[](INT iIndex) const;

protected:
	/*
	 *	Copy & Assign operation are forbidden!
	 */
	Array_Base<T>& operator=(const Array_Base<T>& value) {return *this;}
	Array_Base(const Array_Base<T>& value) {}

private:
	T* m_pArray;
	VOID* m_pAlloc;
	INT m_iMaxSize;
	INT m_iCurSize;
};

#include "Array_Base.cpp"

#endif // Array_Base_h__
