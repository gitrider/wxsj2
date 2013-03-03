
/** ������ui����ʾ������������ӿ�
 */

#pragma once


#include "GIFakeObjectManager.h"



class CFakeObjSystem : public tFakeObjSystem
{
protected:

/** ���ܽӿ�
 */

	struct FakeObject
	{
		STRING			strName;		/// ���֣�UI�ؼ����ݸ����ֿ���
		INT				nObjectID;		/// �߼�Obj����ID
		BOOL			bVisible;		/// ��ǰ�ö����Ƿ���Ҫ��ʾ
		STRING			strWindowName;	/// ��ʾ��ģ�͵Ĵ�����
		STRING			strCameraName;	/// �ô��ڵ����������
	};

public:

	/** ����/������ui����ʾ��obj���� */
	virtual tObject*		NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName);
	virtual VOID			DestroyObject(LPCTSTR szObjName);

	/** UI��Ҫ��ʾĳ���� */
	virtual VOID			OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName);
	/** UI������Ҫ��ʾĳ���� */
	virtual VOID			OnUIUnHook(LPCTSTR szWindowName);

	/** UI��ʾ/���� */
	virtual	VOID			OnUIShown(LPCTSTR szWindowName, float fAspectRatio);
	virtual VOID			OnUIHiden(LPCTSTR szWindowName);

	/** ��תģ�� */
	virtual VOID			RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle);
	virtual VOID			RotateObj_End(LPCTSTR szObjName);

	
protected:

	/// FakeObj�������32��
	std::map< STRING, FakeObject > m_listFakeMap;
	
	/// �Դ�����Ϊ����
	std::map< STRING, FakeObject* > m_listFakeUI;

	/// ������ת�е�ģ��
	FakeObject*						m_pRotateObj;
	FLOAT							m_fRotateSpeed;

public:

/** ���ļ̳�
 */

	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

public:

	CFakeObjSystem();
	virtual ~CFakeObjSystem();

	static CFakeObjSystem*	GetMe(void)										{ return s_pMe; }

protected:

	static CFakeObjSystem* s_pMe;

protected:

	KL_DECLARE_DYNAMIC(CFakeObjSystem);

};