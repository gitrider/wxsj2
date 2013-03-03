// EditInput.cpp : 实现文件
//

#include "stdafx.h"
#include "UIEditor.h"
#include "EditInput.h"
#include "RightEventList.h"
#include "RightPropList.h"

// CEditInput

IMPLEMENT_DYNAMIC(CEditInput, CEdit)

CEditInput::CEditInput()
{
	m_workForWhoNow =EWF_NONE;
	m_workForWhoPrev = EWF_NONE;
	//memset(m_TextBuff, 0, sizeof(m_TextBuff));
	m_nItem = -1;
}

CEditInput::~CEditInput()
{
}


BEGIN_MESSAGE_MAP(CEditInput, CEdit)
END_MESSAGE_MAP()

BOOL CEditInput::IsPrevWorkFor(ENUM_WORK_FOR_WHO who)
{
	return m_workForWhoPrev == who;
}

BOOL CEditInput::IsNowWorkFor(ENUM_WORK_FOR_WHO who)
{
	return m_workForWhoNow == who;
}

void	   CEditInput::SetNowWorkFor(ENUM_WORK_FOR_WHO who)
{
	m_workForWhoPrev = m_workForWhoNow;
	m_workForWhoNow = who;
	m_nItem = -1;
}
void	   CEditInput::SetPrevWorkFor(ENUM_WORK_FOR_WHO who)
{
	m_workForWhoPrev = who;
}

CHAR* CEditInput::GetWindowTextAA()
{
	WCHAR buf[1024];
	GetWindowText(buf, 1024);
	return GetMBCS(buf);
}

INT    CEditInput::GetUsedItem()
{
	return m_nItem;
}
void	  CEditInput::SetUsedItem(INT item)
{
	printf("SetUserItem Edit Item = %d\n", item);
	m_nItem = item;
}
void  CEditInput::OnItemChanged(ENUM_WORK_FOR_WHO now)
{
	TCHAR text[1024];
	m_workForWhoPrev = m_workForWhoNow;
	m_workForWhoNow = now;
	if (m_nItem < 0)
	{
		return;
	}
	GetWindowText(text, 1024);
	switch (m_workForWhoNow)
	{
	case EWF_EVENT_LIST:
		GetRightEventList()->OnUserEditEvent(m_nItem, text);
		break;
	case EWF_PROP_LIST:
		GetRightPropList()->OnUserEditEvent(m_nItem, text);
		break;
	default:
		printf("OnItemChanged State Error\n");
		break;
	}

}

void CEditInput::Cleanup()
{
	m_workForWhoNow;
	m_workForWhoPrev;
	m_nItem = -1;
}

// CEditInput 消息处理程序


