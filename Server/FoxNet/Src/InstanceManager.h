/////////////////////////////////////////////////////////////////////////////////
//文件名称：InstanceManager.h
//开发者：Gaoqi
//功能描述：实例管理器，模板类
//版本说明：系统底层类型基础文件，无授权人员不得修改此文件内容
//			需要使用者自己实现相应的Init函数；
//修改情况：2006-?-? 创建
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

template <typename T, int nSize>
class InstanceManager_T
{
	public:
		InstanceManager_T();
		~InstanceManager_T();
		bool Init(void);
		T const* GetInstanceById(int const nId);
		void RegisterInstance(int nID, T* pLogic);
	private:
		bool m_bReady;
		T* m_apInstances[nSize];
};
template <typename T, int nSize>
InstanceManager_T<T, nSize>::InstanceManager_T()
{
	memset((void*)m_apInstances, 0, sizeof(m_apInstances));
	m_bReady = Init();
};

template <typename T, int nSize>
inline bool InstanceManager_T<T, nSize>::Init(void)
{
	return FALSE;
};

template <typename T, int nSize>
inline InstanceManager_T<T, nSize>::~InstanceManager_T()
{
	m_bReady = FALSE;
	for(int i=0;nSize>i;++i)
	{
		delete m_apInstances[i];
		m_apInstances[i]=NULL;
	};
};

template<typename T, int nSize>
inline T const* InstanceManager_T<T, nSize>::GetInstanceById(int const nId)
{
	if(0<=nId && nSize>nId && true==m_bReady)
	{
		return m_apInstances[nId];
	}
	return NULL;
}

template<typename T, int nSize>
inline void InstanceManager_T<T, nSize>::RegisterInstance(int nID, T* pInstance)
{
	if(NULL==m_apInstances[nID])
	{
		m_apInstances[nID]=pInstance;
	}
	else
	{
		throw("Two instance use a same slot!!!!");
	}
}
#define REGISTER_INSTANCE(INSTANCE) {RegisterInstance(INSTANCE::ID, (new (INSTANCE)));}

#endif //INSTANCEMANAGER_H

