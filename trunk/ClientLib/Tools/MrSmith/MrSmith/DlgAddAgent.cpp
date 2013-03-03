// DlgAddAgent.cpp : implementation file
//

#include "stdafx.h"
#include "MrSmith.h"
#include "DlgAddAgent.h"
#include "SMAgentManager.h"
#include "DlgParamEdit.h"
#include "SMAgent.h"
#include "SMAgentManager.h"


// CDlgAddAgent dialog

IMPLEMENT_DYNAMIC(CDlgAddAgent, CResizableDialog)
CDlgAddAgent::CDlgAddAgent(INT nAgentNum, CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgAddAgent::IDD, pParent)
	, m_nAgentNum(nAgentNum)
{
	m_createParam = new SMITH::AgentCreateParam;
}

CDlgAddAgent::~CDlgAddAgent()
{
	delete m_createParam;
}

void CDlgAddAgent::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_AGENT_NUM, m_editAgentNum);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_COMBO_AGENT_TYPE, m_comTemplateName);
	DDX_Control(pDX, IDC_LIST_PARAM, m_listParam);
}


BEGIN_MESSAGE_MAP(CDlgAddAgent, CResizableDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnBnClickedButtonEdit)
	ON_CBN_SELCHANGE(IDC_COMBO_AGENT_TYPE, OnCbnSelchangeComboAgentType)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PARAM, OnNMDblclkListParam)
END_MESSAGE_MAP()


// CDlgAddAgent message handlers

BOOL CDlgAddAgent::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_EDIT_AGENT_NUM, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_COMBO_AGENT_TYPE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_BASE_GROUP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT_AGENT_FOLD, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT_AGENT_SCRIPT, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_PARAM_GROUP, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_PARAM, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_ADD, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_REMOVE, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_EDIT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	CString strTemp;
	strTemp.Format("%d", m_nAgentNum);
	m_editAgentNum.SetWindowText(strTemp);

	m_editAgentNum.SetSel(0, -1);
	m_editAgentNum.SetFocus();

	//Insert agent template
	SMITH::AgentManager::AgentTemplateIterator templates = 
		SMITH::AgentManager::GetMe()->getAgentTemplateIterator();

	while (!templates.isAtEnd())
	{
		const SMITH::AgentTemplate& theTemplate = templates.getCurrentValue();

		int nIndex = m_comTemplateName.InsertString(m_comTemplateName.GetCount(), templates.getCurrentValue().m_strDesc.c_str());
		std::string strTemplateName = templates.getCurrentValue().m_strName;
		m_setTemplateName.insert(strTemplateName);
		m_comTemplateName.SetItemDataPtr(nIndex, (void*)m_setTemplateName.find(strTemplateName)->c_str());
		templates++;
	}

	//set param head
	m_listParam.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listParam.InsertColumn(0, "Name", LVCFMT_LEFT, 150);
	m_listParam.InsertColumn(1, "Value", LVCFMT_LEFT, 150);

	//fresh first
	m_comTemplateName.SetCurSel(0);
	OnCbnSelchangeComboAgentType();

	return FALSE;
}

void CDlgAddAgent::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString strTemp;
	m_editAgentNum.GetWindowText(strTemp);

	m_nAgentNum = atoi(strTemp);

	const char* szTemplateName = (const char*)m_comTemplateName.GetItemDataPtr(m_comTemplateName.GetCurSel());
	m_createParam->strTemplate = szTemplateName;

	m_createParam->vParams.clear();
	for(int i=0; i<m_listParam.GetItemCount(); i++)
	{
		m_createParam->vParams.push_back(std::make_pair(
			(LPCTSTR)m_listParam.GetItemText(i, 0), 
			(LPCTSTR)m_listParam.GetItemText(i, 1)));
	}

	CDialog::OnOK();
}

void CDlgAddAgent::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgAddAgent::OnBnClickedButtonAdd()
{
	CDlgParamEdit dlgParam("", "");
	if(IDOK != dlgParam.DoModal()) return;

	CString strName = dlgParam.getParamName();

	for(int i=0; i<m_listParam.GetItemCount(); i++)
	{
		if(strName == m_listParam.GetItemText(i, 0))
		{
			CString strMsg;
			strMsg.Format("该变量已经存在!\n%s", strName);
			MessageBox(strMsg, 0, MB_OK|MB_ICONSTOP);
			return;
		}
	}

	int nIndex = m_listParam.InsertItem(m_listParam.GetItemCount(), strName);
	m_listParam.SetItemText(nIndex, 1, dlgParam.getParamValue());
}

void CDlgAddAgent::OnBnClickedButtonRemove()
{
	for(int i=0; i<m_listParam.GetItemCount(); i++)
	{
		if(LVIS_SELECTED==m_listParam.GetItemState(i, LVIS_SELECTED))
		{
			m_listParam.DeleteItem(i);
			return;
		}
	}
}

void CDlgAddAgent::OnBnClickedButtonEdit()
{
	for(int i=0; i<m_listParam.GetItemCount(); i++)
	{
		if(LVIS_SELECTED==m_listParam.GetItemState(i, LVIS_SELECTED))
		{
			CString strName = m_listParam.GetItemText(i, 0);
			CString strValue = m_listParam.GetItemText(i, 1);

			CDlgParamEdit dlgParam(strName, strValue);
			if(IDOK != dlgParam.DoModal()) return;

			//Name changed?
			if(strName != dlgParam.getParamName())
			{
				//check same name
				for(int j=0; j<m_listParam.GetItemCount(); j++)
				{
					if(dlgParam.getParamName() == m_listParam.GetItemText(j, 0))
					{
						CString strMsg;
						strMsg.Format("该变量已经存在!\n%s", dlgParam.getParamName());
						MessageBox(strMsg, 0, MB_OK|MB_ICONSTOP);
						return;
					}
				}
			}

			m_listParam.SetItemText(i, 0, dlgParam.getParamName());
			m_listParam.SetItemText(i, 1, dlgParam.getParamValue());
			return;
		}
	}
}

void CDlgAddAgent::OnCbnSelchangeComboAgentType()
{
	const char* szTemplateName = (const char*)m_comTemplateName.GetItemDataPtr(m_comTemplateName.GetCurSel());

	const SMITH::AgentTemplate *pTemplate = 
		SMITH::AgentManager::GetMe()->getAgentTemplate(szTemplateName);
	if(!pTemplate) return;

	//Set fold
	SetDlgItemText(IDC_EDIT_AGENT_FOLD, pTemplate->m_strWorkFold.c_str());

	//Set script num
	CString strTemp;
	strTemp.Format("%d", pTemplate->m_vScriptFiles.size());
	SetDlgItemText(IDC_EDIT_AGENT_SCRIPT, strTemp);

	// set param
	m_listParam.DeleteAllItems();

	SMITH::AgentTemplate::VariableDefine::const_iterator it;
	int nIndex = 0;
	for(it=pTemplate->m_vVariables.begin(); it!=pTemplate->m_vVariables.end(); it++)
	{
		m_listParam.InsertItem(nIndex, it->first.c_str());
		m_listParam.SetItemText(nIndex, 1, it->second.c_str());

		nIndex++;
	}
}

void CDlgAddAgent::OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	OnBnClickedButtonEdit();
	*pResult = 0;
}

