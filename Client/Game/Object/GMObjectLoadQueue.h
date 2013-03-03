
#pragma once


#include <map>



class Packet;
class SocketOutputStream;

/** 物体加载队列，防止一桢内同时加载过多物体
 */
class CObjectLoadQueue
{
public:

	CObjectLoadQueue();
	virtual ~CObjectLoadQueue();


	/** 将一个需要Ask的obj放入 
	 */
	BOOL TryAddLoadTask(INT idObj);
	
	/** 工作桢
	 */
	void Tick(void);
	
	/** 强制执行
	 */
	void ExcuteTask(INT idObj);
	
	/** 清空某一个指定Object的队列
	 */
	void ClearTask(INT idObj);
	
	/** 清空工作队列（切换场景时）
	 */
	void ClearAllTask(void);


protected:

	typedef std::multimap< FLOAT, INT > LOAD_TASK_MAP;

	/// 按照距离远近的工作队列
	LOAD_TASK_MAP		m_mapLoadTask;

	///  
	HANDLE				m_hLoadThread;

};	// class