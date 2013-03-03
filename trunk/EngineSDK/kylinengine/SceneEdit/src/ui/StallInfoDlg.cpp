
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "StallInfoDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/wxprec.h"
#include ".\stallinfodlg.h"

#include "SceneManipulator.h"
#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include ".\stallinfoaction.h"
#include "WXSJ_DBC.h"				// ��ȡexcel


using namespace Fairy;
BEGIN_EVENT_TABLE(CStallInfoDlg, wxPanel)

	EVT_RADIOBUTTON(ID_RADIOBUTTON_SET_STALL,  CStallInfoDlg::OnSetStallInfo)
 	EVT_RADIOBUTTON(ID_RADIOBUTTON_DEL_STALL,  CStallInfoDlg::OnDelStallInfo)
	EVT_RADIOBUTTON(ID_RADIOBUTTON_VIEW_STALL, CStallInfoDlg::OnViewStallInfo)

	EVT_TEXT(ID_TEXTCTRL__TRADE_TAX,	CStallInfoDlg::OnTradeTaxChange)
	EVT_TEXT(ID_TEXTCTRL__POS_TAX,		CStallInfoDlg::OnPosTaxChange)
	EVT_TEXT(ID_TEXTCTRL__EXTRAINFO,	CStallInfoDlg::OnExtraInfoChange)

END_EVENT_TABLE()



CStallInfoDlg::~CStallInfoDlg(void)
{
}


CStallInfoDlg::CStallInfoDlg(const wxString& resourceGroup)
// �Ƿ��̯
:m_pTextCanStall(NULL)
// ˰��
,m_pTextTax(NULL)	
// ̯λ��
,m_pTextPosTax(NULL)
// ������Ϣ
,m_pExtraInfo(NULL)	
,m_pSceneManipulator(NULL)
,m_pEditAction(NULL)
{
    //Init(resourceGroup);
	//InitData();

	// ���õ�ǰ�鿴��Ϣ.
	m_iCurOperateType = CStallInfoAction::VIEW_STALL_INFO;
}

CStallInfoDlg::CStallInfoDlg(
						wxWindow *parent, 
						const wxString& resourceGroup,
						wxWindowID winid,
						const wxPoint& pos,
						const wxSize& size,
						long style,
						const wxString& name
						)						
										
{
	// �Ƿ��̯
	m_pTextCanStall = NULL;		

	// ˰��
	m_pTextTax = NULL;		

	// ̯λ��
	m_pTextPosTax = NULL;		

	// ������Ϣ
	m_pExtraInfo = NULL;		

	// ��������
	m_pSceneManipulator = NULL;

	// ��Ӧ��action
	m_pEditAction = NULL;

    Create(parent, winid, pos, size, style, name);
	//InitData();

}


bool CStallInfoDlg::Create(
								  wxWindow *parent, 
								  wxWindowID winid,
								  const wxPoint& pos, 
								  const wxSize& size,
								  long style, 
								  const wxString& name
								  )
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

	// ��������������� �Ի���Ŀؼ�������ʾ
	StallInfoDlg(this, true, true);
	
	//// ��ȡ���ؼ�������
	GetAllControl();
	GetAction();
	return true;
}

// ��ʾ����
bool CStallInfoDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

// �õ��ؼ���Ϣ
void CStallInfoDlg::GetAllControl()
{
	
    wxDynamicCast(FindWindow(ID_TEXT__CAN_STALL), wxStaticText)->SetLabel("�Ƿ���԰�̯");

	m_pTextTax    = (wxTextCtrl*)wxDynamicCast(FindWindow(ID_TEXTCTRL__TRADE_TAX), wxTextCtrl);
	m_pTextPosTax = (wxTextCtrl*)wxDynamicCast(FindWindow(ID_TEXTCTRL__POS_TAX),   wxTextCtrl);
	m_pExtraInfo  = (wxTextCtrl*)wxDynamicCast(FindWindow(ID_TEXTCTRL__EXTRAINFO), wxTextCtrl);

	wxDynamicCast(FindWindow(ID_TEXT__TRADE_TAX), wxStaticText)->SetLabel("����˰(0~255)         ");  
	wxDynamicCast(FindWindow(ID_TEXT__POS_TAX),   wxStaticText)->SetLabel("̯λ��(0~0xffffffff)   ");
	wxDynamicCast(FindWindow(ID_TEXT__EXTRAINFO), wxStaticText)->SetLabel("������Ϣ(0~255)      ");

	
	wxDynamicCast(FindWindow(ID_RADIOBUTTON_SET_STALL),	   wxRadioButton)->SetLabel("����̯λ");
	wxDynamicCast(FindWindow(ID_RADIOBUTTON_DEL_STALL),	   wxRadioButton)->SetLabel("ɾ��̯λ");
	wxDynamicCast(FindWindow(ID_RADIOBUTTON_VIEW_STALL),   wxRadioButton)->SetLabel("�鿴̯λ");

	wxRadioButton* pViewStall = (wxRadioButton*)wxDynamicCast(FindWindow(ID_RADIOBUTTON_VIEW_STALL),   wxRadioButton);
	pViewStall->SetValue(1);

	m_pTextTax->SetLabel("0");
	m_pTextPosTax->SetLabel("0");
	m_pExtraInfo->SetLabel("0"); 
    
}




// ��������༭����
void CStallInfoDlg::SetActive(bool bShow)
{
	if (bShow)
    {
		//if(m_pSceneManipulator)
		//{
		//	Fairy::Action* action = m_pSceneManipulator->_getAction("StallInfoEdit");
		//	m_pSceneManipulator->setActiveAction(action);//
		//	m_pEditAction = (CEventAreaEditAction *)action;
		//	
		//}

		GetAction();
       
    }
	else
	{
		if(m_pEditAction)
		{
			//m_pEditAction->SetOldHitPointMaterial();
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// ��ʾ���س���
	if(m_pEditAction)
	{
		m_pEditAction->HideScene(bShow);
		if(bShow)
		{
			// ���ݵ��εĸ߶���ʾ����.
			//m_pEditAction->SetRegionHeight();
		}
	}//
}

//�ؽ�����
void CStallInfoDlg::ReCreateStallInfo()
{		
	m_pEditAction->ReCreateStallInfo();
}

// ���ó����������.
void CStallInfoDlg::SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		GetAction();
	}//
}

// �õ�action
void CStallInfoDlg::GetAction()
{
	if(m_pSceneManipulator)
	{
		Fairy::Action* action = m_pSceneManipulator->_getAction("StallInfoEdit");
		m_pSceneManipulator->setActiveAction(action);//
		m_pEditAction = (CStallInfoAction *)action;
		
	}//
}


	// ��ť������ ����̯λ��Ϣ
void CStallInfoDlg::OnSetStallInfo( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperateType = CStallInfoAction::SET_STALL_INFO;
		m_iCurOperateType = CStallInfoAction::SET_STALL_INFO;
	}
}

	// ��ť������ ѡ��
void CStallInfoDlg::OnDelStallInfo( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperateType = CStallInfoAction::DEL_STALL_INFO;
		m_iCurOperateType = CStallInfoAction::DEL_STALL_INFO;
	}
}

	// ��ť������ ɾ��һ������
void CStallInfoDlg::OnViewStallInfo( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperateType = CStallInfoAction::VIEW_STALL_INFO;
		m_iCurOperateType = CStallInfoAction::VIEW_STALL_INFO;
	}
}


// ˰�ʸı�
void CStallInfoDlg::OnTradeTaxChange(wxCommandEvent& event)
{
	if(m_pEditAction)
	{
		
		wxString strValue = m_pTextTax->GetValue();
		m_pEditAction->m_curStallInfoBrush.m_iTradeTax = atoi(strValue.c_str());

		//if(m_pEditAction->m_curStallInfoBrush.m_iTradeTax <= 0)
		//{
		//	m_pTextTax->SetLabel("0");//
		//}

		//if(m_pEditAction->m_curStallInfoBrush.m_iTradeTax >= 255)
		//{
		//	m_pTextTax->SetLabel("255");//
		//}//

	}
}

// ̯λ�Ѹı�
void CStallInfoDlg::OnPosTaxChange(wxCommandEvent& event)
{
	if(m_pEditAction)
	{
		wxString strValue = m_pTextPosTax->GetValue();
		m_pEditAction->m_curStallInfoBrush.m_dwPosTax = atoi(strValue.c_str());
		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo <= 0)
		//{
		//	m_pTextPosTax->SetLabel("0");//
		//}

		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo >= 0xffffffff)
		//{
		//	char bufInfo[128];
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", 0xffffffff);

		//	m_pTextPosTax->SetLabel(bufInfo);//
		//}//
	}
}

// ������Ϣ�ı�
void CStallInfoDlg::OnExtraInfoChange(wxCommandEvent& event)
{
	if(m_pEditAction)
	{
		wxString strValue = m_pExtraInfo->GetValue();
		m_pEditAction->m_curStallInfoBrush.m_iExtraInfo = atoi(strValue.c_str());
		
		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo <= 0)
		//{
		//	m_pExtraInfo->SetLabel("0");//
		//}

		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo >= 255)
		//{
		//	m_pExtraInfo->SetLabel("255");//
		//}//
	}
}

// �ص������鿴̯λ��Ϣ
void CStallInfoDlg::onViewStallInfo(
								unsigned char	bCanStall,	//�Ƿ���԰�̯
								unsigned char	iTradeTax,	//����˰
								unsigned long	dwPosTax,	//̯λ��
								unsigned char	iExtraInfo	//������Ϣ 
								)
{
	char szbufInfo[256];
	

	wxString strCanStall  = "";
	wxString strTradeTax  = "";
	wxString strPosTax    = "";
	wxString strExtraInfo = "";
	wxString strShowInfo = "";

	if(bCanStall)
	{
		strCanStall = "���԰�̯\r\n";
	}
	else
	{
		strCanStall = "�����԰�̯\r\n";
	}

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "����˰:%d\r\n", iTradeTax);
	strTradeTax = szbufInfo;

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "̯λ��:%d\r\n", dwPosTax);
	strPosTax = szbufInfo;

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "������Ϣ:%d\r\n", iExtraInfo);
	strExtraInfo = szbufInfo;

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "%s,%s,%s,%s", strCanStall.c_str(), strTradeTax.c_str(), strPosTax.c_str(), strExtraInfo.c_str());
	strShowInfo = szbufInfo;

	wxDynamicCast(FindWindow(ID_TEXTCTRL_STALL_INFO), wxTextCtrl)->SetLabel(strShowInfo);


}


// ��ȡ����
void CStallInfoDlg::LoadData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->ReadStallInfoFromFile(strFile);
	}
	
}

	// �洢����
void CStallInfoDlg::SaveData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->SaveStallInfoToFile(strFile);
	}
}

// ����̯λ��Ϣ
void CStallInfoDlg::CreateStallInfo()
{
	if(m_pEditAction)
	{
		m_pEditAction->CreateStallInfo();
	}
}
