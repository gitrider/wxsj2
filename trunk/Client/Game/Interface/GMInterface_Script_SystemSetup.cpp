
#include "StdAfx.h"
#include "Network\NetManager.h"
#include "GMGameInterface.h"
#include "GMInterface_Script_SystemSetup.h"
#include "Procedure\GameProcedure.h"
#include "Event/GMEventSystem.h"
#include "Sound/GMSoundSystem.h"
#include "Variable\GMVariable.h"
#include "GIVariable.h"
#include "LuaPlus.h"
#include "Global.h"
#include "CGModifySetting.h"
#include "CGAskPrivateInfo.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "DataPool\GMDP_CharacterData.h"

#include "CGApplyPrivateInfo.h"
#include "GIUtil.h"

//#include "../Network/NetManager.h"



SCRIPT_SANDBOX::SystemSetup*	LuaExport< SCRIPT_SANDBOX::SystemSetup >::s_pMe = 0;
LuaPlus::LuaObject*				LuaExport< SCRIPT_SANDBOX::SystemSetup >::s_pMetaTable = 0;


namespace SCRIPT_SANDBOX
{
	VOID WINAPI SystemSetup::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
	{
		if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
		{
			const STRING& strVariable = pEvent->vArg[0];
			const STRING& strValue = pEvent->vArg[1];

			//----------------------------
			// ��Ļ�ֱ��ʷ����ı�
			if(strVariable == "View_Resoution") 
			{
				fVector2 fResoution = CVariableSystem::GetMe()->GetAs_Vector2(strVariable.c_str());
				s_pMe->_setResolution((INT)fResoution.x, (INT)fResoution.y);
			}
			//----------------------------
			// �Ƿ�ȫ�������ı�
			else if(strVariable == "View_FullScreen")
			{
				s_pMe->_setResolution(strValue == "1");
			}
			//----------------------------
			//...
		}
	}

	VOID SystemSetup::_setResolution(INT nWidth, INT nHeight)
	{
		// �������ȫ��״̬��
		if(CVariableSystem::GetMe()->GetAs_Int("View_FullScreen") == 1) return;

		// �޶��ܹ��趨�ķֱ���
//		if( (nWidth == 800  && nHeight == 600) ||
//			(nWidth == 1024 && nHeight == 768) ||
//			(nWidth == 1280 && nHeight == 1024) )
		{
			// �Ƿ�����еĴ��ڴ�Сһ��
			RECT rectCurrent;
			GetClientRect(g_hMainWnd, &rectCurrent);
			if(nWidth==rectCurrent.right-rectCurrent.left && 
				nHeight == rectCurrent.bottom-rectCurrent.top) return;

			//���ݱ߽������ʵ���ڴ�С
			RECT rectWindow;
			SetRect(&rectWindow, 0, 0, nWidth, nHeight);
			AdjustWindowRect(&rectWindow, GetWindowStyle(g_hMainWnd), FALSE);

			//��Ļ��Ч������(�ų�������)
			RECT rectScreen;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);

			//����޷����ɸô��ڣ�ֱ�����
			if(	(rectScreen.right-rectScreen.left) <= (rectWindow.right-rectWindow.left) ||
				(rectScreen.bottom-rectScreen.top) <= (rectWindow.bottom-rectWindow.top))
			{
				::ShowWindow(g_hMainWnd, SW_MAXIMIZE);
				return;
			}

			//����
			UINT dwX = ((rectScreen.right-rectScreen.left)-(rectWindow.right-rectWindow.left))/2;
			UINT dwY = ((rectScreen.bottom-rectScreen.top)-(rectWindow.bottom-rectWindow.top))/2;

			::ShowWindow(g_hMainWnd, SW_RESTORE);
			::MoveWindow(g_hMainWnd, dwX, dwY, 
				rectWindow.right-rectWindow.left, rectWindow.bottom-rectWindow.top, TRUE);

			CGameProcedure::HandlePossibleSizeChange(WM_EXITSIZEMOVE, 0, 0);
		}
	}

	VOID SystemSetup::_setResolution(BOOL bFullScreen)
	{
		static BOOL s_bFull = FALSE;
		if(s_bFull == bFullScreen) return;

		if(bFullScreen)
		{
			int nScreenWidth  = ::GetSystemMetrics(SM_CXSCREEN);
			int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

			//���ݱ߽������ʵ���ڴ�С
			RECT rectWindow;
			SetRect(&rectWindow, 0, 0, nScreenWidth, nScreenHeight);
			AdjustWindowRect(&rectWindow, GetWindowStyle(g_hMainWnd), FALSE);

			::ShowWindow(g_hMainWnd, SW_RESTORE);
			::MoveWindow(g_hMainWnd, rectWindow.left, rectWindow.top, 
				rectWindow.right-rectWindow.left, rectWindow.bottom-rectWindow.top, TRUE);
			
//			::SetWindowPos(g_hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}
		else
		{
			::ShowWindow(g_hMainWnd, SW_MAXIMIZE);
//			::SetWindowPos(g_hMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}

		s_bFull = bFullScreen;

		CGameProcedure::HandlePossibleSizeChange(WM_EXITSIZEMOVE, 0, 0);
	}

	//������ص��趨��������Ӱ
	INT SystemSetup::Lua_Texture(LuaPlus::LuaState* state)
	{
		
		//�߹�
		static INT nQuality = 0;
		if(nQuality == 0)
			nQuality = 1;
		else if(nQuality == 1)
			nQuality = 2;
		else if(nQuality == 2)
			nQuality = 0;
		CGameProcedure::s_pGfxSystem->Scene_SetLightmapQuality(nQuality);

		return 0;
	}

	//��ʾ������ص��趨
	INT SystemSetup::Lua_Display(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//������ص��趨
	INT SystemSetup::Lua_Sound(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//��ϵͳ������
	INT SystemSetup::Lua_OpenSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_SYSTEMFRAME);

		return 0;
	}
	//����Ƶ���ý���
	INT SystemSetup::Lua_ViewSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_VIEWSETUP);
		return 0;
	}
	//���������ý���
	INT SystemSetup::Lua_SoundSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_SOUNDSETUP);
		return 0;
	}
	//��UI���ý���
	INT SystemSetup::Lua_UISetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_UISETUP);
		return 0;
	}
	//�򿪰������ý���
	INT SystemSetup::Lua_InputSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUTSETUP);
		return 0;
	}
	//����Ϸ�����ý���
	INT SystemSetup::Lua_GameSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_GAMESETUP);
		return 0;
	}
	//��Ϸ����
	INT SystemSetup::Lua_GameHelp(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//������Ϸ
	INT SystemSetup::Lua_BackGame(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//�����Ƶ��������
	INT SystemSetup::Lua_View_GetData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::View_GetData[2] param parameter error");
		}
		std::string str	= args[2].GetString();

		if(str == "dxyy")			//������Ӱ
		{
			INT fTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_LightmapQuality");
			state->PushNumber(fTemp);
		}
		else if(str == "fhj")		//�����
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_Fanhunjiao");
			state->PushNumber(nTemp);
		}
		else if(str == "wtdh")		//���嶯��
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_ObjectMove");
			state->PushNumber(nTemp);
		}
		else if(str == "gamma")		//gamma����
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_Gamma");
			state->PushNumber(nTemp);
		}
		else if(str == "yszl")		//��ɫ����
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_ColorQuality");
			state->PushNumber(nTemp);
		}
		else if(str == "wldx")		//�����С
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_TextureQuality");
			state->PushNumber(nTemp);
		}
		else if(str == "cy")		//����
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_TextureFiltering");
			state->PushNumber(nTemp);
		}
		else if(str == "dbxs")		//�������
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_PolygonQuantity");
			state->PushNumber(nTemp);
		}

		else if(str == "dxgg")		//���θ߹�
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_GroundHighLight");
			state->PushNumber(nTemp);
		}
		else if(str == "rwgg")		//����߹�
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_HumanHighLight");
			state->PushNumber(nTemp);
		}
		else if(str == "rwyy")		//������Ӱ
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_HumanLightmap");
			state->PushNumber(nTemp);
		}
		else if(str == "qpfg")		//ȫ������
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreenLight");
			state->PushNumber(nTemp);
		}
		else if(str == "cztb")		//��ֱͬ��
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_Aplomb");
			state->PushNumber(nTemp);
		}
		else if(str == "sdh")		//ˮ����
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_WaterMove");
			state->PushNumber(nTemp);
		}
		else if(str == "qpms")		//ȫ��ģʽ
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreen");
			state->PushNumber(nTemp);
		}
		else if (str == "rwmx")			//����ģ����
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_HumanNum");
			state->PushNumber( nTemp );
		}

		return 1;
	}
	//������Ƶ����
	INT SystemSetup::Lua_View_SetData(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::View_SetData[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[3] param parameter error");
		}
		std::string str	= args[2].GetString();
		INT fTemp = args[3].GetInteger();

		if(str == "dxyy")			//������Ӱ
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_LightmapQuality",fTemp,FALSE);
		}
		else if(str == "fhj")		//�����
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_Fanhunjiao",fTemp,FALSE);
		}
		else if(str == "wtdh")		//���嶯��
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_ObjectMove",fTemp,FALSE);
		}
		else if(str == "gamma")		//gamma����
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_Gamma",fTemp,FALSE);
		}
		else if(str == "yszl")		//��ɫ����
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_ColorQuality",fTemp,FALSE);
		}
		else if(str == "wldx")		//�����С
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_TextureQuality",fTemp,FALSE);
		}
		else if(str == "cy")		//����
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_TextureFiltering",fTemp,FALSE);
		}
		else if(str == "dbxs")		//�������
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_PolygonQuantity",fTemp,FALSE);
		}

		else if(str == "dxgg")		//���θ߹�
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_GroundHighLight",fTemp,FALSE);
		}
		else if(str == "rwgg")		//����߹�
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_HumanHighLight",fTemp,FALSE);
		}
		else if(str == "rwyy")		//������Ӱ
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_HumanLightmap",fTemp,FALSE);
		}
		else if(str == "qpfg")		//ȫ������
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_FullScreenLight",fTemp,FALSE);
		}
		else if(str == "cztb")		//��ֱͬ��
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_Aplomb",fTemp,FALSE);
		}
		else if(str == "sdh")		//ˮ����
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_WaterMove",fTemp,FALSE);
		}
		else if(str == "qpms")		//ȫ��ģʽ
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_FullScreen",fTemp,FALSE);
		}
		else if(str == "rwmx")		//����ģ������
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_HumanNum",fTemp,FALSE);
		}


		return 0;
	}
	//������Ƶ��������
	INT SystemSetup::Lua_View_SaveToFile(LuaPlus::LuaState* state)
	{
		return 0;
	}

	// ���ݽ���UI���ڵ���ʵ����Ϸ����
	INT SystemSetup::Lua_ChangeViewSize( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() || !args[3].IsNumber() )
		{
			KLThrow(" LUA: ChangeViewSize Parameter Error ");
			return 0;
		}

		CGameProcedure::s_pGfxSystem->Camera_SetOffset( (tGfxSystem::OFFSET_TAG)(args[2].GetInteger()), (FLOAT)(args[3].GetFloat()) );

		return 0;
	}


	//-----------------------------------------------------------------------------
	//��Ϸ�����
	INT SystemSetup::Lua_GameGetData(LuaPlus::LuaState* state)
	{
		//���������������

		if( ( m_GameData & ( 1 << (GSF_CANNOT_ADD_FRIEND)) ) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( ( m_GameData & (1 << (GSF_CANNOT_RECV_MAIL)) ) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_CANNOT_RECV_STRANGER_MAIL))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_REFUSE_TRADE))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_REFUSE_TEAM_INVITE))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}
		
		if( (m_GameData & (1<<(GSF_CLOSE_POPUP))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_REFUSE_LOOK_SPOUSE_INFO))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSE_REFUSE_FACTION_INVITE))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSE_LOCKED_SHORTCUT))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSE_SHOW_OTHERPLAYER_NAME))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSE_SHOW_RESTURN_NAME))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSE_SHOW_NPC_NAME))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}
		state->PushInteger(1);


		return 13;
	}

	INT SystemSetup::Lua_SaveGameSetup(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[3] param parameter error");
		}
		if (!(args[4].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[4] param parameter error");
		}
		if (!(args[5].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[5] param parameter error");
		}
		if (!(args[6].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[6] param parameter error");
		}
		if (!(args[7].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[7] param parameter error");
		}
		if (!(args[8].IsInteger()))
		{
			KLThrow("SystemSetup::SaveGameSetup[8] param parameter error");
		}
		if (!(args[9].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[9] param parameter error");
		}
		if (!(args[10].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[10] param parameter error");
		}
		if (!(args[11].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[11] param parameter error");
		}
		if (!(args[12].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[12] param parameter error");
		}
		if (!(args[13].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[13] param parameter error");
		}
		if (!(args[14].IsInteger()))
		{
			KLThrow("SystemSetup::View_SetData[14] param parameter error");
		}

		INT nRefuseJoinMeToFriend	= args[2].GetInteger();
		INT nRefuseAllMail			= args[3].GetInteger();
		INT nRefuseStrangerMail		= args[4].GetInteger();
		INT nRefuseExchange			= args[5].GetInteger();
		INT nRefuseTeamRequest		= args[6].GetInteger();
		INT nCloseMyPaopao			= args[7].GetInteger();
		INT nRefuseSeeMySpouse		= args[8].GetInteger();

		INT nCloseTalkFrame			= args[9].GetInteger();

		INT nRefuseFactionInvite	= args[10].GetInteger();
		INT nLockedShortCut			= args[11].GetInteger();
		INT nShowOtherplayerName	= args[12].GetInteger();
		INT nShowResturnName		= args[13].GetInteger();
		INT nShowNpcName			= args[14].GetInteger();

		INT Original = m_GameData;


		if(nRefuseJoinMeToFriend == 0)
		{
			INT nGsf =  1 << (GSF_CANNOT_ADD_FRIEND);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf =  1 << (GSF_CANNOT_ADD_FRIEND);
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseAllMail == 0)
		{
			INT nGsf =  1 << (GSF_CANNOT_RECV_MAIL);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf =  1 << (GSF_CANNOT_RECV_MAIL);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseStrangerMail == 0)
		{
			INT nGsf = 1 << (GSF_CANNOT_RECV_STRANGER_MAIL);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_CANNOT_RECV_STRANGER_MAIL);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseExchange == 0)
		{
			INT nGsf = 1 << (GSF_REFUSE_TRADE);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_REFUSE_TRADE);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseTeamRequest == 0)
		{
			INT nGsf = 1 << (GSF_REFUSE_TEAM_INVITE);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_REFUSE_TEAM_INVITE);		
			m_GameData = nGsf | m_GameData;
		}

		if(nCloseMyPaopao == 0)
		{
			INT nGsf = 1 << (GSF_CLOSE_POPUP);
			m_GameData = ~(nGsf) & m_GameData;
			CGameProcedure::s_pUISystem->SetPaoPaoActive(true);
		}
		else
		{
			INT nGsf = 1<< (GSF_CLOSE_POPUP);		
			m_GameData = nGsf | m_GameData;
			CGameProcedure::s_pUISystem->SetPaoPaoActive(false);
		}

		if(nRefuseSeeMySpouse == 0)
		{
			INT nGsf = 1 << (GSF_REFUSE_LOOK_SPOUSE_INFO);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_REFUSE_LOOK_SPOUSE_INFO);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseFactionInvite == 0)
		{
			INT nGsf = 1 << (GSE_REFUSE_FACTION_INVITE);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSE_REFUSE_FACTION_INVITE);		
			m_GameData = nGsf | m_GameData;
		}

		if(nLockedShortCut == 0)
		{
			INT nGsf = 1 << (GSE_LOCKED_SHORTCUT);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSE_LOCKED_SHORTCUT);		
			m_GameData = nGsf | m_GameData;
		}

		if(nShowOtherplayerName == 0)
		{
			INT nGsf = 1 << (GSE_SHOW_OTHERPLAYER_NAME);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSE_SHOW_OTHERPLAYER_NAME);		
			m_GameData = nGsf | m_GameData;
		}

		if(nShowResturnName == 0)
		{
			INT nGsf = 1 << (GSE_SHOW_RESTURN_NAME);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSE_SHOW_RESTURN_NAME);		
			m_GameData = nGsf | m_GameData;
		}

		if(nShowNpcName == 0)
		{
			INT nGsf = 1 << (GSE_SHOW_NPC_NAME);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSE_SHOW_NPC_NAME);		
			m_GameData = nGsf | m_GameData;
		}


		if(Original != m_GameData)
		{
			CGModifySetting msg;
			_OWN_SETTING* pSend_Value = msg.GetValue();

			msg.SetType(SETTING_TYPE_GAME);
			pSend_Value->m_SettingType = -1;
			pSend_Value->m_SettingData = m_GameData;
			//������������CGMODIFYSETTING
	
			CNetManager::GetMe()->SendPacket( &msg );
		}

		return 0;
	}

	INT  SystemSetup::Set_GameData(INT nData)
	{
		m_GameData = nData;
		return 1;
	}
	//���������Ϣ
	INT SystemSetup::Lua_AskPrivateInfo(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//�ύ�����Ϣ
	INT SystemSetup::Lua_ApplyPrivateInfo(LuaPlus::LuaState* state)
	{
		CGApplyPrivateInfo msg;

		msg.SetAge				( (BYTE)CDataPool::GetMe()->PI_GetMyAge() );        //pHuman->GetDB()->GetPrivateInfoDB()->m_Age);
		msg.SetBloodType		( CDataPool::GetMe()->PI_GetMyBloodType());
		msg.SetCharGUID			( CDataPool::GetMe()->PI_GetMyCharGUID());
		msg.SetCityLength		( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyCity() ));
		msg.SetCity				( (CHAR*)CDataPool::GetMe()->PI_GetMyCity());
		msg.SetConsella			( CDataPool::GetMe()->PI_GetMyConsella());
		msg.SetEmailInfoLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyEmailInfo() ));
		msg.SetEmailInfo		( (CHAR*)CDataPool::GetMe()->PI_GetMyEmailInfo());
		msg.SetInfoType			( CDataPool::GetMe()->PI_GetMyInfoType());
		msg.SetJobInfoLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyJobInfo() ));
		msg.SetJobInfo			( (CHAR*)CDataPool::GetMe()->PI_GetMyJobInfo());
		msg.SetLuckWordLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyLuckWord() ));
		msg.SetLuckWord			( (CHAR*)CDataPool::GetMe()->PI_GetMyLuckWord());
		msg.SetSchoolInfoLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMySchoolInfo() ));
		msg.SetSchoolInfo		( (CHAR*)CDataPool::GetMe()->PI_GetMySchoolInfo());
		msg.SetProvince			( CDataPool::GetMe()->PI_GetMyProvince());
		msg.SetSex				( CDataPool::GetMe()->PI_GetMySex());
		msg.SetYearAnimal		( CDataPool::GetMe()->PI_GetMyYearAnimal());

		CNetManager::GetMe()->SendPacket( &msg );
		return 0;
	}
	//��������Ϣ
	INT SystemSetup::Lua_GetPrivateInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::Lua_GetPrivateInfo[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("SystemSetup::Lua_GetPrivateInfo[3] param parameter error");
		}
		std::string strWho = args[2].GetString();
		std::string str = args[3].GetString();

		if( strWho == "self" )
		{
			if( str == "type" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMyInfoType());
			}
			else if( str == "guid" )
			{
				CHAR szTemp[16] = {0};
				_snprintf(szTemp, 16, "%X",CDataPool::GetMe()->PI_GetMyCharGUID());
				state->PushString(szTemp);
			}
			else if( str == "age" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMyAge());
			}
			else if( str == "sex" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMySex());
			}
			else if( str == "job" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetMyJobInfo());
			}
			else if( str == "school" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetMySchoolInfo());
			}
			else if( str == "blood" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMyBloodType());
			}
			else if( str == "animal" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMyYearAnimal());
			}
			else if( str == "Consella" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMyConsella());
			}
			else if( str == "Province" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetMyProvince());
			}
			else if( str == "city" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetMyCity());
			}
			else if( str == "email" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetMyEmailInfo());
			}
			else if( str == "luck" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetMyLuckWord());
			}
			else if( str == "name" )
			{
				state->PushString(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
			}
			else
			{
				state->PushInteger(0);
			}
		}
		else
		{
			if( str == "type" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtInfoType());
			}
			else if( str == "guid" )
			{
				CHAR szTemp[16];
				_snprintf(szTemp, 16, "%X",CDataPool::GetMe()->PI_GetOtCharGUID());
				state->PushString(szTemp);
			}
			else if( str == "age" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtAge());
			}
			else if( str == "sex" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtSex());
			}
			else if( str == "job" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetOtJobInfo());
			}
			else if( str == "school" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetOtSchoolInfo());
			}
			else if( str == "blood" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtBloodType());
			}
			else if( str == "animal" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtYearAnimal());
			}
			else if( str == "Consella" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtConsella());
			}
			else if( str == "Province" )
			{
				state->PushInteger(CDataPool::GetMe()->PI_GetOtProvince());
			}
			else if( str == "city" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetOtCity());
			}
			else if( str == "email" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetOtEmailInfo());
			}
			else if( str == "luck" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetOtLuckWord());
			}
			else if( str == "name" )
			{
				state->PushString(CDataPool::GetMe()->PI_GetOtName());
			}
			else
			{
				state->PushInteger(0);
			}
		}

		return 1;
	}

	//��������Ϣ
	INT SystemSetup::Lua_SetPrivateInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::Lua_SetPrivateInfo[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			KLThrow("SystemSetup::Lua_SetPrivateInfo[3] param parameter error");
		}
		std::string strWho = args[2].GetString();
		std::string str = args[3].GetString();

		std::string strDesc;
		if ((args[4].IsString()))
		{
			strDesc = args[4].GetString();
			//��֤�̵����ƵĺϷ���
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc, FT_NAME) )
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				strDesc = "";

				state->PushInteger(0);
				return 1;
			}
		}
		if( strWho == "self" )
		{
			if( str == "type" )
			{
				CDataPool::GetMe()->PI_SetMyInfoType(args[4].GetInteger() );
			}
			else if( str == "age" )
			{
				CDataPool::GetMe()->PI_SetMyAge((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
			}
			else if( str == "sex" )
			{
				CDataPool::GetMe()->PI_SetMySex((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
			}
			else if( str == "job" )
			{
				CDataPool::GetMe()->PI_SetMyJobInfo(args[4].GetString());
			}
			else if( str == "school" )
			{
				CDataPool::GetMe()->PI_SetMySchoolInfo(args[4].GetString());
			}
			else if( str == "blood" )
			{
				CDataPool::GetMe()->PI_SetMyBloodType((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
			}
			else if( str == "animal" )
			{
				CDataPool::GetMe()->PI_SetMyYearAnimal((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
			}
			else if( str == "Consella" )
			{
				CDataPool::GetMe()->PI_SetMyConsella((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
			}
			else if( str == "Province" )
			{
				CDataPool::GetMe()->PI_SetMyProvince((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
			}
			else if( str == "city" )
			{
				CDataPool::GetMe()->PI_SetMyCity(args[4].GetString());
			}
			else if( str == "email" )
			{
				CDataPool::GetMe()->PI_SetMyEmailInfo(args[4].GetString());
			}
			else if( str == "luck" )
			{
				CDataPool::GetMe()->PI_SetMyLuckWord(args[4].GetString());
			}
		}
		else
		{
			if( str == "type" )
			{
				CDataPool::GetMe()->PI_SetOtInfoType(args[4].GetInteger());
			}
			else if( str == "age" )
			{
				CDataPool::GetMe()->PI_SetOtAge(args[4].GetInteger());
			}
			else if( str == "sex" )
			{
				CDataPool::GetMe()->PI_SetOtSex(args[4].GetInteger());
			}
			else if( str == "job" )
			{
				CDataPool::GetMe()->PI_SetOtJobInfo(args[4].GetString());
			}
			else if( str == "school" )
			{
				CDataPool::GetMe()->PI_SetOtSchoolInfo(args[4].GetString());
			}
			else if( str == "blood" )
			{
				CDataPool::GetMe()->PI_SetOtBloodType(args[4].GetInteger());
			}
			else if( str == "animal" )
			{
				CDataPool::GetMe()->PI_SetOtYearAnimal(args[4].GetInteger());
			}
			else if( str == "Consella" )
			{
				CDataPool::GetMe()->PI_SetOtConsella(args[4].GetInteger());
			}
			else if( str == "Province" )
			{
				CDataPool::GetMe()->PI_SetOtProvince(args[4].GetInteger());
			}
			else if( str == "city" )
			{
				CDataPool::GetMe()->PI_SetOtCity(args[4].GetString());
			}
			else if( str == "email" )
			{
				CDataPool::GetMe()->PI_SetOtEmailInfo(args[4].GetString());
			}
			else if( str == "luck" )
			{
				CDataPool::GetMe()->PI_SetOtLuckWord(args[4].GetString());
			}
		}
		state->PushInteger(1);

		return 1;
	}

	//��ҳ��
	INT SystemSetup::Lua_OpenPrivatePage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::Lua_OpenPrivatePage[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			CGAskPrivateInfo msg;
			ObjID_t SelfId = CObjectManager::GetMe()->GetMySelf()->GetServerID();

			msg.SetPlayerID((ObjID_t)SelfId);

			CNetManager::GetMe()->SendPacket( &msg );
		}
		else
		{
			//ʹ���Ѿ�ˢ�µ����ݳ��е���������ʾ����
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PRIVATE_INFO,"other");
		}

		return 0;
	}
	//�������Ϣ�����л������װ������
	INT SystemSetup::Lua_OpenEquipFrame(LuaPlus::LuaState* state)
	{
		//��װ������
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::Lua_OpenEquipFrame[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			//���Լ���
			CEventSystem::GetMe()->PushEvent(GE_OPEN_EQUIP);
		}
		else
		{
			// �򿪱��˵�
			CEventSystem::GetMe()->PushEvent(GE_OTHERPLAYER_UPDATE_EQUIP);
		}
		return 0;
	}
	//�������Ϣ�����л�����ҳ������
	INT SystemSetup::Lua_OpenPetFrame(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("SystemSetup::Lua_OpenPetFrame[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_PET_PAGE);
		}
		else
		{
			//��ʱû���������
		}
		return 0;
	}
	//���������Ͻ�����ĵ�Obj
	INT SystemSetup::Lua_GetCaredObjId(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->PI_GetObjId());

		return 1;
	}
}
