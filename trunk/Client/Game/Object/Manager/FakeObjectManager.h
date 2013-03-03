
/**	FakeObjectManager.h
*/

#ifndef _FAKE_OBJECT_MANAGER_H
#define _FAKE_OBJECT_MANAGER_H



#include "GIFakeObjectManager.h"




/**	������ui����ʾ�����������
 */
class CFakeObjSystem : public tFakeObjSystem
{
protected:

	/// ui��ʾ������
	struct FakeObject
	{
		STRING	strName;		/// ���֣�UI�ؼ����ݸ����ֿ���
		INT		nObjectID;		/// �߼�Obj����ID
		BOOL	bVisible;		/// ��ǰ�ö����Ƿ���Ҫ��ʾ
		STRING	strWindowName;	/// ��ʾ��ģ�͵Ĵ�����
		STRING	strCameraName;	/// �ô��ڵ����������
	};

public:

	CFakeObjSystem();
	virtual ~CFakeObjSystem();

	static CFakeObjSystem* GetMe( void )			{ return s_pMe; }

	virtual VOID Initial(VOID*);
	virtual VOID Release(VOID);
	virtual VOID Tick(VOID);


	/** ����/������ui����ʾ��obj����
	 */
	virtual tObject* NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName);
	virtual VOID DestroyObject(LPCTSTR szObjName);

	/** UI��Ҫ��ʾĳ���� */
	virtual VOID OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName,  int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName);
	/** UI������Ҫ��ʾĳ���� */
	virtual VOID OnUIUnHook(LPCTSTR szWindowName);

	/** UI��ʾ/����
	 */
	virtual	VOID OnUIShown(LPCTSTR szWindowName, float fAspectRatio);
	virtual VOID OnUIHiden(LPCTSTR szWindowName);

	/** ��תģ��
	 */
	virtual VOID RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle);
	virtual VOID RotateObj_End(LPCTSTR szObjName);
	
	// ���ض������� Nick 2007.12.3
	virtual STRING GetObjName(LPCTSTR szWindowName);


protected:

	static CFakeObjSystem* s_pMe;


	/// FakeObj�������32��
	std::map<STRING, FakeObject>	m_listFakeMap;

	/// �Դ�����Ϊ����
	std::map<STRING, FakeObject*>	m_listFakeUI;

	/// ������ת�е�ģ��
	FakeObject*						m_pRotateObj;
	/// ��ת�ٶ�
	FLOAT							m_fRotateSpeed;


protected:

	KL_DECLARE_DYNAMIC(CFakeObjSystem);

};	// class


#endif	// _FAKE_OBJECT_MANAGER_H