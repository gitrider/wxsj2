
#include "stdafx.h"
#include "UIEditor.h"
#include "RightEventList.h"
#include "DataPool.h"


// CRightEventList

IMPLEMENT_DYNAMIC(CRightEventList, CWnd)

CRightEventList::CRightEventList()
{

}

CRightEventList::~CRightEventList()
{
}


BEGIN_MESSAGE_MAP(CRightEventList, CPropertyListBase)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRightEventList ��Ϣ�������
//

void CRightEventList::InsertItemS(const CHAR* szEventName, const CHAR* szParentName)
{
	StringVector v;
	v.push_back("");
	if(m_nCurrentFunctionPrifexName.GetLength() > 0)
	{
		char szTemp[200] = {0};
		sprintf(szTemp, "%s_On%s",m_nCurrentFunctionPrifexName.GetString(), szEventName);
		v.push_back(szTemp);
	}
	InsertComboItem(szEventName,szEventName,v,OTCS_NORMAL,szEventName,szParentName);
}

void CRightEventList::SetCurrentFunctionPrefix(const CString& name)
{
	m_nCurrentFunctionPrifexName = name;
	//���������б�,��������ֵ
	OptionTreeItemMap::iterator it = d_OptionTreeMapBase.begin();
	OptionTreeItemMap::iterator ed = d_OptionTreeMapBase.end();
	EventArray* pArrayBase =  g_DataPool.GetWindowEvent(name);
	
	//����ѡ��
	for (; it != ed; ++it)
	{
		bool bset;
		bset = true;
		((COptionTreeItemComboBox *)it->second)->ResetContent();
		((COptionTreeItemComboBox *)it->second)->AddString("");
		//���֮ǰ�Ѿ��༭��,��ôѡ��ڶ���
		if (pArrayBase)
		{
			size_t size = pArrayBase->size();
			for (size_t i=0; i<size; i++)
			{
				if( (*pArrayBase)[i].eventName == it->first )
				{
					((COptionTreeItemComboBox *)it->second)->AddString((*pArrayBase)[i].funcName);
					((COptionTreeItemComboBox *)it->second)->SetCurSel(1);
					//((COptionTreeItemComboBox *)it->second)->Invalidate();
					bset = false;
					break;
				}
			}
		}
		if (bset)
		{
			char szTemp[200] = {0};
			sprintf(szTemp, "%s_On%s",m_nCurrentFunctionPrifexName.GetString(), it->first);
			((COptionTreeItemComboBox *)it->second)->AddString(szTemp);
		}
	}

	OptionTreeItemMap::iterator itex = d_OptionTreeMap.begin();
	OptionTreeItemMap::iterator edex = d_OptionTreeMap.end();

	//��չѡ��
	for (; itex != edex; ++itex)
	{
		bool bset;
		bset = true;
		((COptionTreeItemComboBox *)itex->second)->ResetContent();
		((COptionTreeItemComboBox *)itex->second)->AddString("");
		//���֮ǰ�Ѿ��༭��,��ôѡ��ڶ���
		if (pArrayBase)
		{
			size_t size = pArrayBase->size();
			for (size_t i=0; i<size; i++)
			{
				if( (*pArrayBase)[i].eventName == itex->first )
				{
					((COptionTreeItemComboBox *)itex->second)->AddString((*pArrayBase)[i].funcName);
					((COptionTreeItemComboBox *)itex->second)->SetCurSel(1);
					//((COptionTreeItemComboBox *)itex->second)->Invalidate();
					bset = false;
					break;
				}
			}
		}
		if (bset)
		{
			char szTemp[200] = {0};
			sprintf(szTemp, "%s_On%s",m_nCurrentFunctionPrifexName.GetString(), itex->first);
			((COptionTreeItemComboBox *)itex->second)->AddString(szTemp);
		}
	}
}


void CRightEventList::OnTabLoseFocus()
{

}

void CRightEventList::OnSize(UINT nType, int cx, int cy)
{
	CPropertyListBase::OnSize(nType, cx, cy);
}

void CRightEventList::GetFunctionString(CString& outString, const CString& windowName)
{
	//GenSystemFunction(outString,windowName);
	GenBaseFunction(outString);
	GenExtentionFunction(outString);
}

void CRightEventList::GenSystemFunction(CString& outFunction, const CString& windowName)
{
	outFunction = "--Ԥ���غ���,�����������ע����Ҫ���¼�\n\nfunction ";
	outFunction += windowName;
	outFunction += "_PreLoad()\n\n\nend\n\n--���غ���,��������ʱ��ִ��һ��\n\nfunction ";
	outFunction +=  windowName;
	outFunction += "_OnLoad()\n\n\nend\n\n--�¼�������,��ע����¼�������ʱ��ִ��\n\nfunction ";
	outFunction +=  windowName;
	outFunction += "_OnEvent(event)\n\n\nend\n\n";
}
void CRightEventList::GenBaseFunction(CString& outFunction)
{
	//���������б�,��������ֵ
	OptionTreeItemMap::iterator it = d_OptionTreeMapBase.begin();
	OptionTreeItemMap::iterator ed = d_OptionTreeMapBase.end();
	for (; it != ed; ++it)
	{
		CString szFunction;
		INT sel = ((COptionTreeItemComboBox *)it->second)->GetCurSel();
		if(sel < 0) continue;
		((COptionTreeItemComboBox *)it->second)->GetLBText(sel, szFunction);
		if (szFunction.GetLength() > 0)
		{
			outFunction += "\nfunction ";
			outFunction += szFunction;
			outFunction += "()\n\nend\n";
		}
	}
}
void CRightEventList::GenExtentionFunction(CString& outFunction)
{
	//���������б�,��������ֵ
	OptionTreeItemMap::iterator it = d_OptionTreeMap.begin();
	OptionTreeItemMap::iterator ed = d_OptionTreeMap.end();
	for (; it != ed; ++it)
	{
		CString szFunction;
		INT sel = ((COptionTreeItemComboBox *)it->second)->GetCurSel();
		if(sel < 0) continue;
		((COptionTreeItemComboBox *)it->second)->GetLBText(sel, szFunction);
		if (szFunction.GetLength() > 0)
		{
			outFunction += "\nfunction ";
			outFunction += szFunction.GetString();
			outFunction += "()\n\nend\n";
		}
	}
}

void CRightEventList::UpdateWindowEventDefine(void)
{
	if(m_nCurrentFunctionPrifexName.GetLength()== 0) return;
	//��дDataPool�������
	OptionTreeItemMap::iterator it = d_OptionTreeMapBase.begin();
	OptionTreeItemMap::iterator ed = d_OptionTreeMapBase.end();
	for (; it != ed; ++it)
	{
		CString szFunction;
		INT sel = ((COptionTreeItemComboBox *)it->second)->GetCurSel();
		if(sel < 0) continue;
		((COptionTreeItemComboBox *)it->second)->GetLBText(sel, szFunction);
		if (szFunction.GetLength() > 0)
		{
			g_DataPool.AddWindowEventItem(m_nCurrentFunctionPrifexName, it->first, szFunction);
		}
	}

	it = d_OptionTreeMap.begin();
	ed = d_OptionTreeMap.end();
	for (; it != ed; ++it)
	{
		CString szFunction;
		INT sel = ((COptionTreeItemComboBox *)it->second)->GetCurSel();
		if(sel < 0) continue;
		((COptionTreeItemComboBox *)it->second)->GetLBText(sel, szFunction);
		if (szFunction.GetLength() > 0)
		{
			g_DataPool.AddWindowEventItem(m_nCurrentFunctionPrifexName, it->first, szFunction);
		}
	}
}