
/**	UIModel.h
*/

#ifndef _OBJECT_UI_MODEL_H
#define _OBJECT_UI_MODEL_H


#include "GameDefine_Item.h"



class CObject_PlayerOther;

/**	����UI����ʾ��ģ��
 */
class CModelShowInUI
{
public:

	CModelShowInUI();
	~CModelShowInUI();


	/** �����µ�ģ�� */
	BOOL CreateNewUIModel(const CHAR* szModelName = NULL);

	/** ɾ��uiģ�� */
	BOOL DestroyUIModel();


	/** ����ģ����Ϣ */
	VOID SetUIModelInfo(HUMAN_EQUIP point, INT nID);

	/** ����ģ�� */
	VOID SetFaceMeshId(INT nID);

	/** ͷ��ģ�� */
	VOID SetFaceHairId(INT nID);

	/** ͷ����ɫ */
	VOID SetHairColor(INT nID);


	/** �õ�uiģ������ */
	const TCHAR* GetUIModelName();


public:

	/// ģ�ͼ���
	static UINT				m_TeamNumCount;
	/// ����UI��ʾ���߼�����.
	CObject_PlayerOther*	m_pAvatar;
	/// ģ������.
	TCHAR					m_szBufModel[128];

};	// class


#endif	// _OBJECT_UI_MODEL_H