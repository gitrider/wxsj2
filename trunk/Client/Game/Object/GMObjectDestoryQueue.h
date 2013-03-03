
/**	GMObjectDestoryQueue.h
 */

#ifndef _GM_OBJECT_DESTORY_QUEUE_H
#define _GM_OBJECT_DESTORY_QUEUE_H


/**	���ٶ���
 */
class CObjectDestoryQueue
{
public:

	CObjectDestoryQueue();
	virtual ~CObjectDestoryQueue();


	/** ����������Ҫ���ٵĶ���
	 */
	BOOL TryAddDestoryTask(INT idObj);

	void Tick(void);

	/** ��ն���
	 */
	void ClearAllTask(void);

private:

	/// ���������б�
	std::vector<INT>	m_DestoryTask;

};	// class


#endif	// _GM_OBJECT_DESTORY_QUEUE_H