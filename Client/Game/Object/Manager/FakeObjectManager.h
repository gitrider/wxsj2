
/**	FakeObjectManager.h
*/

#ifndef _FAKE_OBJECT_MANAGER_H
#define _FAKE_OBJECT_MANAGER_H



#include "GIFakeObjectManager.h"




/**	用于在ui上显示的物体管理器
 */
class CFakeObjSystem : public tFakeObjSystem
{
protected:

	/// ui显示的物体
	struct FakeObject
	{
		STRING	strName;		/// 名字，UI控件根据该名字控制
		INT		nObjectID;		/// 逻辑Obj对象ID
		BOOL	bVisible;		/// 当前该对象是否需要显示
		STRING	strWindowName;	/// 显示该模型的窗口名
		STRING	strCameraName;	/// 该窗口的摄象机名字
	};

public:

	CFakeObjSystem();
	virtual ~CFakeObjSystem();

	static CFakeObjSystem* GetMe( void )			{ return s_pMe; }

	virtual VOID Initial(VOID*);
	virtual VOID Release(VOID);
	virtual VOID Tick(VOID);


	/** 创建/销毁在ui上显示的obj对象
	 */
	virtual tObject* NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName);
	virtual VOID DestroyObject(LPCTSTR szObjName);

	/** UI需要显示某物体 */
	virtual VOID OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName,  int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName);
	/** UI不再需要显示某物体 */
	virtual VOID OnUIUnHook(LPCTSTR szWindowName);

	/** UI显示/隐藏
	 */
	virtual	VOID OnUIShown(LPCTSTR szWindowName, float fAspectRatio);
	virtual VOID OnUIHiden(LPCTSTR szWindowName);

	/** 旋转模型
	 */
	virtual VOID RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle);
	virtual VOID RotateObj_End(LPCTSTR szObjName);
	
	// 返回对象名字 Nick 2007.12.3
	virtual STRING GetObjName(LPCTSTR szWindowName);


protected:

	static CFakeObjSystem* s_pMe;


	/// FakeObj链表，最多32个
	std::map<STRING, FakeObject>	m_listFakeMap;

	/// 以窗口名为索引
	std::map<STRING, FakeObject*>	m_listFakeUI;

	/// 正在旋转中的模型
	FakeObject*						m_pRotateObj;
	/// 旋转速度
	FLOAT							m_fRotateSpeed;


protected:

	KL_DECLARE_DYNAMIC(CFakeObjSystem);

};	// class


#endif	// _FAKE_OBJECT_MANAGER_H