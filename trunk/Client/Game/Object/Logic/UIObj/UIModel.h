
/**	UIModel.h
*/

#ifndef _OBJECT_UI_MODEL_H
#define _OBJECT_UI_MODEL_H


#include "GameDefine_Item.h"



class CObject_PlayerOther;

/**	用于UI上显示的模型
 */
class CModelShowInUI
{
public:

	CModelShowInUI();
	~CModelShowInUI();


	/** 创建新的模型 */
	BOOL CreateNewUIModel(const CHAR* szModelName = NULL);

	/** 删除ui模型 */
	BOOL DestroyUIModel();


	/** 设置模型信息 */
	VOID SetUIModelInfo(HUMAN_EQUIP point, INT nID);

	/** 脸部模型 */
	VOID SetFaceMeshId(INT nID);

	/** 头发模型 */
	VOID SetFaceHairId(INT nID);

	/** 头发颜色 */
	VOID SetHairColor(INT nID);


	/** 得到ui模型名字 */
	const TCHAR* GetUIModelName();


public:

	/// 模型计数
	static UINT				m_TeamNumCount;
	/// 用于UI显示的逻辑对象.
	CObject_PlayerOther*	m_pAvatar;
	/// 模型名字.
	TCHAR					m_szBufModel[128];

};	// class


#endif	// _OBJECT_UI_MODEL_H