#include "StdAfx.h"
#include "InputManager.h"
#include "..\Global.h"
#include "..\Procedure\GameProcedure.h"
#include "TDGfxSystem.h"
#include "TDException.h"
#include "TDTimeSystem.h"

#define _IsKeyDown(exp)		(GetAsyncKeyState(exp) & 0xff00)
#define _IsKeyDowned(exp)	(GetAsyncKeyState(exp) & 0x00ff)

TD_IMPLEMENT_DYNAMIC(CInputManager, GETCLASS(tInputSystem));
CInputManager* CInputManager::s_pMe = NULL;
CInputManager::CInputManager()
{
	s_pMe = this;

	m_pDI = NULL;
	m_lpDIDKeyboard = NULL;
	m_hWnd = NULL;
	m_InputCapture = ICS_NONE;
	m_bCatchKeyDown = FALSE;
	m_dwMouseFlag = 0;
	m_dwMouseFlagOld = 0;
	m_dwLBPutdownTime = 0;

	ZeroMemory(m_byCurKeys, sizeof(BYTE)*NUMDIKEYS);
	ZeroMemory(m_byOldKeys, sizeof(BYTE)*NUMDIKEYS);
	ZeroMemory(m_bKeyPresses, sizeof(BOOL)*NUMDIKEYS);
	ZeroMemory(m_bKeyPresseds, sizeof(BOOL)*NUMDIKEYS);

}

CInputManager::~CInputManager()
{
	Release();
}

VOID CInputManager::Initial(VOID*)
{
	HRESULT hr;

	m_hWnd = g_hMainWnd;

    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
	{
		TDThrow(_T("Create a DInput object error:%08X"), hr);
        return;
	}

	// Create the keyboard device
	hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDKeyboard, NULL);
	if(hr == DI_OK)
	{
		m_lpDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_lpDIDKeyboard->SetCooperativeLevel(g_hMainWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		AcquireKeyboard();
	}

}

VOID CInputManager::AcquireKeyboard(VOID)
{
	if (m_lpDIDKeyboard)
	{
		HRESULT rval = m_lpDIDKeyboard->Acquire();
		if (rval == DI_OK || rval == S_FALSE)
		{
			KeyboardFlushData();
			return;
		}
	}	
}

VOID CInputManager::KeyboardFlushData(VOID)
{
	ZeroMemory(m_byCurKeys, sizeof(BYTE)*NUMDIKEYS);
	ZeroMemory(m_byOldKeys, sizeof(BYTE)*NUMDIKEYS);
}

VOID CInputManager::UnacquireKeyboard(VOID)
{
	KeyboardFlushData();
	if (m_lpDIDKeyboard) m_lpDIDKeyboard->Unacquire();
}

VOID CInputManager::MouseSetPos(INT nX, INT nY)
{
	POINT ptClient;
	ptClient.x = nX;
	ptClient.y = nY;

	::ClientToScreen(g_hMainWnd, &ptClient);

	::SetCursorPos(ptClient.x, ptClient.y);

	m_ptMouse.x = m_ptMouseNow.x = nX;
	m_ptMouse.y = m_ptMouseNow.y = nY;
}

VOID CInputManager::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	//������Ϣ
	case WM_MOUSEWHEEL:
		{
			if(GET_WHEEL_DELTA_WPARAM(wParam)>0)
			{
				m_dwMouseFlag |= MOUSE_WHEELUP;
			}
			else
			{
				m_dwMouseFlag |= MOUSE_WHEELDOWN;
			}
		}
		break;
	//���̰���
	case WM_KEYDOWN:
		{
			//���������ظ��ļ��̰���
			m_Buf_WM_KEYDOWN = std::make_pair(wParam, lParam);
			m_bCatchKeyDown = true;
//			AxTrace(0, 2, "Push:%d", wParam);
		}
		break;

	//���
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		{
			m_ptMouseNow.x = GET_X_LPARAM(lParam); 
			m_ptMouseNow.y = GET_Y_LPARAM(lParam); 
		}
		break;

	//��������
	case WM_CHAR:
	case WM_IME_COMPOSITION:
		CGameProcedure::s_pUISystem_->InjectInput_CHAR(hWnd, message, wParam, lParam);
		break;
	}
}

VOID CInputManager::Tick(VOID)
{
	//���������û�м���,����
	HWND hWndActive = ::GetActiveWindow();
	if(hWndActive != m_hWnd) return;

	//----------------------------------------
	//��ȡ�����豸״̬,ͬʱ�������¼����浽�����¼�������
	
	//����¼�����,�������Ӧ�õ�����ʱ�Ѿ��ǿյ�
	if(!m_vEventQueue.empty())
		m_vEventQueue.clear();

	//����
	KeybordTick();
	//���
	MouseTick();

	//----------------------------------------
	//������Ӧ���봦��,
	switch(GetCapture())
	{
	case ICS_NONE:
		{
			CGameProcedure::s_pUISystem_->InjectInput();
			if(GetCapture() != ICS_UI)
			{
				CGameProcedure::ProcessActiveInput();
			}
		}
		break;

	case ICS_UI:
		{
			CGameProcedure::s_pUISystem_->InjectInput();
		}
		break;

	case ICS_GAME:
		{
			CGameProcedure::ProcessActiveInput();
		}
		break;
	}
}

VOID CInputManager::KeybordTick(VOID)
{
	if(!m_lpDIDKeyboard) return;

	//������״̬
	memcpy(m_byOldKeys, m_byCurKeys, NUMDIKEYS);

	HRESULT hRet = m_lpDIDKeyboard->GetDeviceState(NUMDIKEYS, m_byCurKeys);
	if(hRet != DI_OK) 
		AcquireKeyboard();
	else
	{
		for(INT i=0; i<NUMDIKEYS; i++)
		{
			m_bKeyPresses[i]  = (!(m_byOldKeys[i]&0x80) && (m_byCurKeys[i]&0x80)); //�ü��ձ�����
			m_bKeyPresseds[i] = ((m_byOldKeys[i]&0x80) && !(m_byCurKeys[i]&0x80)); //�ü��ձ��ͷ�

			//������������¼�
			if(m_bKeyPresses[i])
			{
				AddEvent(EVENT_KEY_DOWN, i);
			}
			if(m_bKeyPresseds[i])
			{
				AddEvent(EVENT_KEY_UP, i);
			}
		}
	}

	//����Ƿ����Զ����͵�RepeatKeyDown
	if(m_bCatchKeyDown && !IsEventExist(EVENT_KEY_DOWN))
	{
		switch (m_Buf_WM_KEYDOWN.first) 
		{
		case VK_LEFT:	AddEvent(EVENT_KEY_DOWN, KC_LEFT);	break;
		case VK_UP:		AddEvent(EVENT_KEY_DOWN, KC_UP);	break;
		case VK_RIGHT:	AddEvent(EVENT_KEY_DOWN, KC_RIGHT); break;
		case VK_DOWN:	AddEvent(EVENT_KEY_DOWN, KC_DOWN);	break;
		case VK_BACK:	AddEvent(EVENT_KEY_DOWN, KC_BACK);	break;
		}
	}
	m_bCatchKeyDown = FALSE;
}

VOID CInputManager::MouseTick(VOID)
{
	//--------------------------------------------------------
	//�ж��Ƿ���Click�����ݱ�׼
	const INT MAX_TIME_DRAG			= 500;		//ms
	//��ʼ�Զ�����
	const INT MOUSE_REPEAT_DOWN		= 1000;

	//--------------------------------------------------------
	// ����ԭ���λ��
	m_ptMouseOld = m_ptMouse; 
	//ȡ�����λ��
	m_ptMouse = m_ptMouseNow;

	{
		//����ԭ���״̬
		m_dwMouseFlagOld = m_dwMouseFlag;
		m_dwMouseFlag = 0;
		//��ǰʱ��
		UINT dwTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

		//����MouseMove
		BOOL bMouseMove = FALSE;

		if(m_ptMouseOld.x != m_ptMouseNow.x || m_ptMouseOld.y != m_ptMouseNow.y)
		{
			bMouseMove = TRUE;
			//������� ���ƶ��� �¼�
			AddEvent(EVENT_MOUSE_MOVE);
		}

		//�쿴����
		if(_IsKeyDown(VK_LBUTTON)) m_dwMouseFlag |= MOUSE_LBDOWN;
		if(_IsKeyDown(VK_MBUTTON)) m_dwMouseFlag |= MOUSE_MBDOWN;
		if(_IsKeyDown(VK_RBUTTON)) m_dwMouseFlag |= MOUSE_RBDOWN;

		//������µ�ʱ��
		static UINT s_timeMouseLDownTimeBegin = 0;
		static UINT s_timeMouseMDownTimeBegin = 0;
		static UINT s_timeMouseRDownTimeBegin = 0;
		static BOOL s_bLRepeatDownMode = FALSE;

		//----------------------------------------------------------------
		//�쿴�Ƿ���Down
		if((m_dwMouseFlagOld & MOUSE_LBDOWN) == FALSE && (m_dwMouseFlag & MOUSE_LBDOWN)) 
		{
			//�������DOWN�¼�
			AddEvent(EVENT_MOUSE_LBDOWN);
			m_dwMouseFlag |= MOUSE_LBCLICK;

			m_ptMouseLDown = m_ptMouseNow;
			
			//��¼ʱ��
			s_timeMouseLDownTimeBegin = dwTimeNow;
			
			s_bLRepeatDownMode = FALSE;
			//CGameProcedure::s_pGfxSystem->PushDebugString("MouseDown");
		}
		if((m_dwMouseFlagOld & MOUSE_MBDOWN) == FALSE && (m_dwMouseFlag & MOUSE_MBDOWN)) 
		{		
			//�������DOWN�¼�
			AddEvent(EVENT_MOUSE_MBDOWN);
			m_dwMouseFlag |= MOUSE_MBCLICK;

			m_ptMouseMDown = m_ptMouseNow;

			//��¼ʱ��
			s_timeMouseMDownTimeBegin = dwTimeNow;
		}
		if((m_dwMouseFlagOld & MOUSE_RBDOWN) == FALSE && (m_dwMouseFlag & MOUSE_RBDOWN)) 
		{
			//�������DOWN�¼�
			AddEvent(EVENT_MOUSE_RBDOWN);
			m_dwMouseFlag |= MOUSE_RBCLICK;
	
			m_ptMouseRDown = m_ptMouseNow;
			//��¼ʱ��
			s_timeMouseRDownTimeBegin = dwTimeNow;
		}

		//----------------------------------------------------------------
		//�쿴�Ƿ���Clicked
		if((m_dwMouseFlagOld & MOUSE_LBDOWN) && (m_dwMouseFlag & MOUSE_LBDOWN) == FALSE) 
		{
			//����ʱ��
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseLDownTimeBegin, dwTimeNow);
			//������λ��û���ƶ�������ʱ������϶�(500ms)
			if(	dwTime < MAX_TIME_DRAG && 
				abs(m_ptMouseLDown.x-m_ptMouseNow.x) <= 4 && 
				abs(m_ptMouseLDown.y-m_ptMouseNow.y) <= 4)
			{
				//�������UP�¼�
				m_dwMouseFlag |= MOUSE_LBCLICKED;

				AddEvent(EVENT_MOUSE_LBCLICK);
			}

			AddEvent(EVENT_MOUSE_LBUP);

			s_bLRepeatDownMode = FALSE;
		}
		if((m_dwMouseFlagOld & MOUSE_MBDOWN) && (m_dwMouseFlag & MOUSE_MBDOWN) == FALSE) 
		{
			//����ʱ��
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseMDownTimeBegin, dwTimeNow);
			//������λ��û���ƶ�������ʱ������϶�(500ms)
			if( dwTime < MAX_TIME_DRAG)
			{
				//�������UP�¼�
				m_dwMouseFlag |= MOUSE_MBCLICKED;

				AddEvent(EVENT_MOUSE_MBCLICK);
			}

			AddEvent(EVENT_MOUSE_MBUP);
		}
		if((m_dwMouseFlagOld & MOUSE_RBDOWN) && (m_dwMouseFlag & MOUSE_RBDOWN) == FALSE) 
		{
			//����ʱ��
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseRDownTimeBegin, dwTimeNow);
			//���ʱ������϶�(500ms)
			if( dwTime < MAX_TIME_DRAG &&
					abs(m_ptMouseRDown.x-m_ptMouseNow.x) <= 4 && 
					abs(m_ptMouseRDown.y-m_ptMouseNow.y) <= 4)
			{
				//�������UP�¼�
				m_dwMouseFlag |= MOUSE_RBCLICKED;
				AddEvent(EVENT_MOUSE_RBCLICK);
			}

			AddEvent(EVENT_MOUSE_RBUP);
		}

	
		//----------------------------------------------------------------
		//�����϶�

		//������갴�µ�ʱ��
		m_dwLBPutdownTime = 0;
		if((m_dwMouseFlag&MOUSE_LBDOWN)&&(m_dwMouseFlagOld&MOUSE_LBDOWN))
		{
			m_dwLBPutdownTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseLDownTimeBegin, dwTimeNow);
		}

		//����϶��¼�
		if( (m_dwMouseFlag & MOUSE_LBDOWN) && (m_dwMouseFlagOld & MOUSE_LBDOWN) ) //������֡���ϱ��ְ�ס״̬
		{
			if(bMouseMove)  
				AddEvent(EVENT_MOUSE_LDRAG_MOVE);

			//�������
			if(m_dwLBPutdownTime > MOUSE_REPEAT_DOWN)
			{
				AddEvent(EVENT_MOUSE_LBDOWNREPEAT);
				s_bLRepeatDownMode = true;
			}
		}
				
		//�Ҽ��϶��¼�
		if( (m_dwMouseFlag & MOUSE_RBDOWN) && (m_dwMouseFlagOld & MOUSE_RBDOWN) )
		{
			if(bMouseMove)	
				AddEvent(EVENT_MOUSE_RDRAG_MOVE);
		}

		//����Ƿ���Double Click		
		static UINT dwDblClk = ::GetDoubleClickTime();
		if(m_dwMouseFlag & MOUSE_LBCLICKED)
		{
			static UINT dwCLicked = 0;
			if(dwTimeNow < dwCLicked + dwDblClk)
			{
				//������� ��˫���� �¼�
				AddEvent(EVENT_MOUSE_LDBCLICK);

				m_dwMouseFlag |= MOUSE_LBDBLCLK;
			}
			dwCLicked = dwTimeNow;
			m_dwLBPutdownTime = 0;
		}
		if(m_dwMouseFlag & MOUSE_MBCLICKED)
		{
			static UINT dwCLicked = 0;
			if(dwTimeNow < dwCLicked + dwDblClk)
			{
				//������� ��˫���� �¼�
				AddEvent(EVENT_MOUSE_MDBCLICK);

				m_dwMouseFlag |= MOUSE_MBDBLCLK;
			}
			dwCLicked = dwTimeNow;
		}
		if(m_dwMouseFlag & MOUSE_RBCLICKED)
		{
			static UINT dwCLicked = 0;
			if(dwTimeNow < dwCLicked + dwDblClk)
			{
				//������� ��˫���� �¼�
				AddEvent(EVENT_MOUSE_RDBCLICK);

				m_dwMouseFlag |= MOUSE_RBDBLCLK;
			}
			dwCLicked = dwTimeNow;
		}
		
		//������ק��
		if(m_dwMouseFlag & MOUSE_RBCLICK)
		{
			//�Ҽ������� ��ק������
			m_rcRBDrag.left = m_ptMouse.x; 
			m_rcRBDrag.top = m_ptMouse.y;	
		}

		if(m_dwMouseFlagOld & MOUSE_RBCLICKED)
		{	
			//�Ҽ�̧��
			m_rcRBDrag.right = m_ptMouse.x;
			m_rcRBDrag.bottom = m_ptMouse.y;
		}		

		//����
		if(m_dwMouseFlagOld & MOUSE_WHEELUP)
			AddEvent(EVENT_MOUSE_WHEEL_UP);

		if(m_dwMouseFlagOld & MOUSE_WHEELDOWN)
			AddEvent(EVENT_MOUSE_WHEEL_DOWN);
	}
}

BOOL CInputManager::IsEventExist(InputEventID id)
{
	InputEventQueue::iterator it;
	for(it=m_vEventQueue.begin(); it!=m_vEventQueue.end(); it++)
	{
		if(it->nEvent == id) return true;
	}

	return FALSE;
}

VOID CInputManager::Release(VOID)
{
	if(m_lpDIDKeyboard) 
	{
		m_lpDIDKeyboard->Unacquire();
		m_lpDIDKeyboard->Release();
		m_lpDIDKeyboard = NULL;
	}

    if( m_pDI )		m_pDI->Release(); m_pDI = NULL; 
}

VOID CInputManager::SetCapture(InputCapture cap)
{
	m_InputCapture = cap;
}

VOID CInputManager::AddEvent(InputEventID nEvent,INT nIndex)
{
	InputEvent inputEvent;
	inputEvent.nEvent = nEvent;
	inputEvent.kIndex = (KeyCode) nIndex;
	inputEvent.bProcessed = FALSE;
	m_vEventQueue.push_back(inputEvent);
}

