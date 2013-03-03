
/**	GMObjectDestoryQueue.h
 */

#ifndef _GM_OBJECT_DESTORY_QUEUE_H
#define _GM_OBJECT_DESTORY_QUEUE_H


/**	销毁队列
 */
class CObjectDestoryQueue
{
public:

	CObjectDestoryQueue();
	virtual ~CObjectDestoryQueue();


	/** 向队列中添加要销毁的对象
	 */
	BOOL TryAddDestoryTask(INT idObj);

	void Tick(void);

	/** 清空队列
	 */
	void ClearAllTask(void);

private:

	/// 保存对象的列表
	std::vector<INT>	m_DestoryTask;

};	// class


#endif	// _GM_OBJECT_DESTORY_QUEUE_H