/********************************************************************
	created:	2008/05/30
	created:	30:5:2008   15:15
	filename: 	d:\prj\src\Common\Array_Base.h
	file path:	d:\prj\src\Common
	file base:	Array_Base
	file ext:	h
	author:		wuwenbin
	
	purpose:	数组包装类，请慎用之
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
	 *	在构造函数中分配所有的内存
	 * pAlloc: 是真正指向分配的内存的首地址
	 *		   可以是任意类型的内存空间
	 *			pAlloc=0时会自动new足够的空间，建议不要动态new
	 *			最好使用Stack中预先分配好的内存
	 * iSize: 指定数组的大小
	 *=============================================================
	 * NOTE: pAlloc有效时其所指的memory要 >= sizeof(T)*iSize
	 *=============================================================
	 */
	Array_Base(INT iSize=0, VOID* pAlloc=NULL);
	/*
	*	在析构函数中真正释放所有内存
	*/
	~Array_Base();

	/*
	*	在构造函数中分配所有的内存
	* pAlloc: 是真正指向分配的内存的首地址
	*		   可以是任意类型的内存空间
	*			pAlloc=0时会自动new足够的空间，建议不要动态new
	*			最好使用Stack中预先分配好的内存
	* iSize: 指定数组的大小
	*==============================================================
	* NOTE: pAlloc有效时其所指的memory要 >= sizeof(T)*iSize
	*==============================================================
	*/
	VOID Reserve(INT iSize, VOID* pAlloc=NULL);

	/*
	 *	获取当前有效节点个数
	 */
	INT GetSize() const;
	/*
	 *	获取当前最大节点个数
	 */
	INT GetMaxSize() const;
	/*
	*	移除所有节点，不会真正的释放内存
	*/
	VOID RemoveAll();
	/*
	*	移除单个节点，不会真正的释放内存
	*/
	VOID Remove(INT iIndex);
	/*
	*	增加节点，不会动态分配内存
	*/
	BOOL Add(const T& value);
	/*
	*	获取相关节点内容
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
