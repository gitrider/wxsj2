
//ͼ�������

#pragma once


#include "GIBasicType.h"
#include <map>


namespace LuaPlus
{
	class LuaState;
}

namespace CEGUI
{
	class Imageset;
	class Image;
}

class CUIIconsManager
{
public:
	STRING					GetIconFullName(LPCTSTR szIconName);
	const CEGUI::Image*		GetIcon(LPCTSTR szIconName);
	HCURSOR					CreateAsWinCursor(LPCTSTR szIconName);

public:
	//---------------------
	//����Lua����

	//ȡ��Iconȫ·��
	static INT				LUA_GetIconFullName(LuaPlus::LuaState* pState);

public:
	typedef std::map< STRING, const CEGUI::Imageset* >	MAP_ICONS;

	//��������ʼ��
	VOID				Initial(VOID);

public:
	CUIIconsManager();
	virtual ~CUIIconsManager();
	static CUIIconsManager*	GetMe(VOID) { return s_pMe; }

protected:
	MAP_ICONS				m_mapAllIcons;
	static CUIIconsManager* s_pMe;
};