/*
 *	080326	IsChildWindowShow
 *
 *
 *
 */

#pragma once


#include "GIBasictype.h"
#include <list>
#include "GIEventSystem.h"
#include "ceguivector.h"


// ���ڹ����࣬����������Ѿ��༭�õ���

namespace CEGUI
{
	class Window;
};
class tScriptEnvironment;
struct _DBC_UI_LAYOUTDEFINE;

namespace LuaPlus
{
	class LuaState;
}
namespace LUA_CONTROL
{
	class Window;
}

// ����һ������
class CUIWindowItem
{
protected:
	DWORD				m_dwID;					// ID
	STRING				m_strWindowName;		// �������֣���һ�޶���
	STRING				m_strLayoutFileName;	// layout�ļ���
	STRING				m_strScriptFileName;	// �ű��ļ���
	int					m_nIsCanDemise;			// �Ƿ�����Զ�������ʾ,��Ļ��������ʾ���������ô���
	STRING				m_strDefaultPositionProperty;	//Ĭ����ʾλ��
	CEGUI::Window*		m_pWindow;				// ����ָ��
	BOOL				m_bShow;				// �Ƿ���ʾ
	tScriptEnvironment*	m_pScriptEnv;			// �ű������ռ�
	BOOL				m_bLayoutLoaded;		// �����Ƿ��Ѿ�������
	INT m_nIDOpenWindow;
	INT m_nIDCloseWindow;

public:
	//Ԥ����
	VOID			PreLoadWindow(VOID);
	// ���ش���
	VOID			LoadWindow(VOID);
	// ��ʾ/���ش���
	VOID			Show(BOOL bShow);
	// �����Ƿ��Ѿ�����
	BOOL			IsWindowShow(VOID) const { return m_bShow; }
	
	// �Ӵ����Ƿ��Ѿ�����
	BOOL			IsChildWindowShow(LPCTSTR szUIName) const;
	//�õ�������
	LPCTSTR			GetWindowName(VOID) const { return m_strWindowName.c_str(); }
	// �������ƶ�ĳһ��λ��
	VOID			MoveTo( CEGUI::Point&  newPos );
	// ����Ӧ����λ��
	void			PositionSelf();
	//���ش���ʱ���Իص�����
	static bool		callbackProperty(CEGUI::Window* window, CEGUI::String& propname, CEGUI::String& propvalue, void* userdata);
	//��������
	void			setOpenCloseSound(INT openSound, INT closeSound) { m_nIDCloseWindow = closeSound; m_nIDOpenWindow = openSound;}
	//���ڴ�С�ı�,�������ڵ����ʵ�λ��
	void		OnSizeChange(void);
	VOID		ReloadScript(VOID);

public:
	typedef std::vector<STRING> TRegisteredEventNameList;
	TRegisteredEventNameList m_EventNameList;
	//------------------------------------------------------------
	//�ű��е��õĺ���
public:
	//ע���ע�¼�
	INT				LUA_RegisterEvent(LuaPlus::LuaState* pState);
	//��ʾ
	INT				LUA_Show(LuaPlus::LuaState* pState);
	//����
	INT				LUA_Hide(LuaPlus::LuaState* pState);
	//��ʾ/����
	INT				LUA_TogleShow(LuaPlus::LuaState* pState);
	//�Ƿ���ʾ
	INT				LUA_IsVisible(LuaPlus::LuaState* pState);
	//ת�����д����ϵ�����
	INT				LUA_TransAllWindowText(LuaPlus::LuaState* pState);
	//������ĳ�߼�Obj��ĳЩ�¼�
	INT				LUA_CareObject(LuaPlus::LuaState* pState);

	//------------------------------------------------------------
	//��Ӧ���ڹر�
protected:
	void onWindowHidden(void);

	//------------------------------------------------------------
	//��Ϸ�¼�֪ͨ
protected:
	static VOID	WINAPI	_OnGameEvent(const EVENT* pEvent, UINT dwOwnerData);
	
	//------------------------------------------------------------
	//�ؼ�<->�ű�����
protected:
	//ע��ؼ�
	VOID			_RegisterControlToScript(CEGUI::Window* pWindow);
	//����ű�����ʵ��
	std::vector< LUA_CONTROL::Window* > m_vAllControl;

	//------------------------------------------------------------
	//�ı�ת��
protected:
	VOID			_TransWindowText(CEGUI::Window* pWindow);

	//------------------------------------------------------------
	//�����û������¼�
public:
	VOID			FireUIEvent(LPCTSTR szEventHandle, CEGUI::Window* pWindow);

public:
	CUIWindowItem(const _DBC_UI_LAYOUTDEFINE* pLayoutDef);
	virtual ~CUIWindowItem();


	BOOL			IsCanDemise(){ return m_nIsCanDemise > 1; };
	int				GetDemiseType() { return m_nIsCanDemise; };
};

//========================================================
class CUIWindowMng
{
public:
	static CEGUI::Window*	GetClientScreen(VOID) { return CUIWindowMng::m_pBackGroundSheet; };
	static CEGUI::Window*   m_pBackGroundSheet; // �ͻ��˴��ڵװ�
	
	//��ʼ��
	VOID		Init(VOID);
	// �ر����д���
	VOID		HideAllWindow(VOID); 
	//ĳ��ui�Ƿ��ڴ���
	BOOL		IsWindowShow(LPCTSTR szUIName, LPCTSTR szChildName = NULL);

	CUIWindowItem* FindWindowItem(const STRING& szChildName);

	void	OpenWindow(const STRING& winName) ;
	void	CloseWindow(const STRING& winName) ;
	void	ToggleWindow(const STRING& winName) ;
	bool	ReloadWindowScript( const LPCTSTR szUIName );
public:
	CUIWindowMng();
	virtual ~CUIWindowMng();

	static CUIWindowMng* GetMe(void) { return s_pMe; }


	VOID		DemiseWindow( CUIWindowItem* pItem, BOOL bShow );
	void		DemiseWindowTest( CUIWindowItem* pItem, BOOL bShow );
	bool		CloseAllWindow();
	void		OnSizeChange(void);

	void		OnEscape(); // ��Ӧesc��
protected:
	static CUIWindowMng* s_pMe;
	

	// ���ڹ�����
	typedef std::list< CUIWindowItem* >  WINDOWLIST;
	typedef std::vector< CUIWindowItem* >  WINDOWVECTOR;
	// ���д��ڵ��б�
	WINDOWLIST   m_aWindowList; 
};
