
#pragma once


#include <map>



class Packet;
class SocketOutputStream;

/** ������ض��У���ֹһ����ͬʱ���ع�������
 */
class CObjectLoadQueue
{
public:

	CObjectLoadQueue();
	virtual ~CObjectLoadQueue();


	/** ��һ����ҪAsk��obj���� 
	 */
	BOOL TryAddLoadTask(INT idObj);
	
	/** ������
	 */
	void Tick(void);
	
	/** ǿ��ִ��
	 */
	void ExcuteTask(INT idObj);
	
	/** ���ĳһ��ָ��Object�Ķ���
	 */
	void ClearTask(INT idObj);
	
	/** ��չ������У��л�����ʱ��
	 */
	void ClearAllTask(void);


protected:

	typedef std::multimap< FLOAT, INT > LOAD_TASK_MAP;

	/// ���վ���Զ���Ĺ�������
	LOAD_TASK_MAP		m_mapLoadTask;

	///  
	HANDLE				m_hLoadThread;

};	// class