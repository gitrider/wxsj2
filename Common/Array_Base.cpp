/********************************************************************
	created:	2008/05/30
	created:	30:5:2008   15:26
	filename: 	d:\prj\src\Common\Array_Base.cpp
	file path:	d:\prj\src\Common
	file base:	Array_Base
	file ext:	cpp
	author:		wuwenbin
	
	purpose:	数组包装类，请慎用之
*********************************************************************/

#ifndef Array_Base_cpp__
#define Array_Base_cpp__

#include "Array_Base.h"

template<class T>
Array_Base<T>::Array_Base(INT iSize/*=0*/, VOID* pAlloc/*=NULL*/)
{
__ENTER_FUNCTION
	m_pArray = 0;
	if(iSize > 0){
		if(pAlloc){
			m_pArray = (T*)pAlloc;
			for(INT i=0; i<iSize; ++i){
				new (&m_pArray[i]) T; //没有真正的new，只是调用构造函数而已
			}
			m_pAlloc = pAlloc;
		}
		else{
			m_pArray = new T[iSize];
			m_pAlloc = NULL;
		}
	}

	m_iCurSize = 0;
	m_iMaxSize = iSize;
__LEAVE_FUNCTION
}

template<class T>
Array_Base<T>::~Array_Base()
{
__ENTER_FUNCTION
	if(m_pAlloc){
		for(INT i=0; i<m_iMaxSize; ++i){
			if(&m_pArray[i]){
				(&m_pArray[i])->~T();
			}
		}
		m_pAlloc = NULL;
	}
	else{
		SAFE_DELETE_ARRAY(m_pArray);
	}
__LEAVE_FUNCTION
}

template<class T> VOID
Array_Base<T>::Reserve(INT iSize, VOID* pAlloc/*=NULL*/)
{
__ENTER_FUNCTION
	m_pArray = 0;
	if(iSize > 0){
		if(pAlloc){
			m_pArray = (T*)pAlloc;
			for(INT i=0; i<iSize; ++i){
				new (&m_pArray[i]) T; //没有真正的new，只是调用构造函数而已
			}
			m_pAlloc = pAlloc;
		}
		else{
			m_pArray = new T[iSize];
			m_pAlloc = NULL;
		}
	}

	m_iCurSize = 0;
	m_iMaxSize = iSize;
__LEAVE_FUNCTION
}

template<class T> INT 
Array_Base<T>::GetSize() const
{
	return m_iCurSize;
}

template<class T> INT 
Array_Base<T>::GetMaxSize() const
{
	return m_iMaxSize;
}

template<class T> VOID 
Array_Base<T>::RemoveAll()
{
	m_iCurSize = 0;
}

template<class T> VOID 
Array_Base<T>::Remove(INT iIndex)
{
	Assert(iIndex>=0 && iIndex<m_iCurSize);
	m_pArray[iIndex] = m_pArray[m_iCurSize-1];
	--m_iCurSize;
}

template<class T> BOOL 
Array_Base<T>::Add(const T& value)
{
	if(m_iCurSize >= m_iMaxSize)
		return FALSE;
	m_pArray[m_iCurSize++] = value;
	return TRUE;
}

template<class T> const T& 
Array_Base<T>::Get(INT iIndex) const
{
	Assert(iIndex>=0 && iIndex<m_iCurSize);
	return m_pArray[iIndex];
}

template<class T> T& 
Array_Base<T>::Get(INT iIndex)
{
	Assert(iIndex>=0 && iIndex<m_iCurSize);
	return m_pArray[iIndex];
}

template<class T> T& 
Array_Base<T>::operator[](INT iIndex)
{
	Assert(iIndex>=0 && iIndex<m_iCurSize);
	return m_pArray[iIndex];
}

template<class T> const T& 
Array_Base<T>::operator[](INT iIndex) const
{
	Assert(iIndex>=0 && iIndex<m_iCurSize);
	return m_pArray[iIndex];
}


#endif // Array_Base_h__
