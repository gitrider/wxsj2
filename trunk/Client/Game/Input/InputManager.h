/****************************************\
*										*
* 			   ���������				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

#define DIRECTINPUT_VERSION         0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "TDInputSystem.h"
#include <queue>

//���������
class CInputManager : public tInputSystem
{
public:
	//�ڵ��ʼ��
	virtual VOID	Initial(VOID*);
	//�߼���ѭ����
	virtual VOID	Tick(VOID);
	//�ͷ��Լ�����ӵ�е���Դ
	virtual VOID	Release(VOID);

public:
	//����Windows��Ϣ
	virtual VOID			MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	//����״̬
	virtual InputCapture	GetCapture(VOID) const	{ return m_InputCapture; }
	virtual	VOID			SetCapture(InputCapture cap);

public:
	 //ĳ���Ƿ���ڰ���״̬
	virtual BOOL IsKeyDown(KeyCode kIndex)		{ return (m_byCurKeys[kIndex] & 0x80) !=0 ;	}
	//ĳ���Ƿ�ձ�����
	virtual BOOL IsKeyPress(KeyCode kIndex)		{ return m_bKeyPresses[kIndex];				}
	//ĳ���Ƿ�ձ�̧��
	virtual BOOL IsKeyPressed(KeyCode kIndex)	{ return m_bKeyPresseds[kIndex];			}

public:
	virtual VOID	MouseSetPos(INT nX, INT nY);
	virtual POINT	MouseGetPos(VOID)				{ return m_ptMouse; }
	virtual POINT	MouseGetPosOld(VOID)			{ return m_ptMouseOld; }
	
	virtual RECT	MouseGetLBDragRect(VOID)		{ return m_rcLBDrag; }
	virtual RECT	MouseGetMBDragRect(VOID)		{ return m_rcMBDrag; }
	virtual RECT	MouseGetRBDragRect(VOID)		{ return m_rcRBDrag; }	

	virtual UINT	MouseGetFlag(VOID)				{ return m_dwMouseFlag; }
	virtual UINT	MouseGetFlagOld(VOID)			{ return m_dwMouseFlagOld; }

	virtual UINT	GetLPutdownTime(VOID)			{ return m_dwLBPutdownTime; }

public:
	//�õ��������
	virtual InputEventQueue&	GetInputQueue(VOID) { return m_vEventQueue; }	
	//��ѯ�������Ƿ���ĳ�¼�����
	virtual BOOL				IsEventExist(InputEventID id);

protected:
	//��ʼʹ��DINPUT����
	VOID			AcquireKeyboard(VOID);
	//�ͷ�DINPUT����
	VOID			UnacquireKeyboard(VOID);
	//��ռ�������
	VOID			KeyboardFlushData(VOID);

	//�����߼�
	VOID			KeybordTick(VOID);
	//����߼�
	VOID			MouseTick(VOID);

	//�����¼�
	VOID			AddEvent(InputEventID nEvent,INT nIndex = 0x00);

public:
	CInputManager();
	virtual ~CInputManager();
	static CInputManager* GetMe(VOID) { return s_pMe; }

protected:
	static CInputManager*		s_pMe;

	//������Ĵ���
	HWND						m_hWnd;
	//���벶��
	InputCapture				m_InputCapture;

	//-------------------------------------------------------
	//�������
	LPDIRECTINPUT8				m_pDI;
	LPDIRECTINPUTDEVICE8		m_lpDIDKeyboard;

	const static INT NUMDIKEYS	= 256;

	BYTE		m_byCurKeys[NUMDIKEYS];		// ��ǰ����״̬
	BYTE		m_byOldKeys[NUMDIKEYS];		// ��һ�����״̬
	BOOL		m_bKeyPresses[NUMDIKEYS];	// ĳ���Ƿ�ձ�����
	BOOL		m_bKeyPresseds[NUMDIKEYS];	// ı���Ƿ�ձ��ͷ�

	//WM_KEYDOWN��������������������Զ�Repeat
	BOOL						m_bCatchKeyDown;
	std::pair< WPARAM, LPARAM > m_Buf_WM_KEYDOWN;

	//-------------------------------------------------------
	//�������¼��б�
	InputEventQueue		m_vEventQueue;

	//-------------------------------------------------------
	//������
	POINT				m_ptMouse;
	POINT				m_ptMouseOld;

	UINT				m_dwMouseFlag;
	UINT				m_dwMouseFlagOld;

	RECT				m_rcLBDrag; // �϶���
	RECT				m_rcMBDrag; // �϶���
	RECT				m_rcRBDrag; // �϶���

	UINT				m_dwLBPutdownTime; // ���������µ�ʱ��

private:
	POINT				m_ptMouseNow;

	POINT				m_ptMouseLDown;
	POINT				m_ptMouseMDown;
	POINT				m_ptMouseRDown;

protected:
	TD_DECLARE_DYNAMIC(CInputManager);
};