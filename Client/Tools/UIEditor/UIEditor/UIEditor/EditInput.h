#pragma once


// CEditInput

class CEditInput : public CEdit
{
	DECLARE_DYNAMIC(CEditInput)

public:
	CEditInput();
	virtual ~CEditInput();
	BOOL IsPrevWorkFor(ENUM_WORK_FOR_WHO who);
	BOOL IsNowWorkFor(ENUM_WORK_FOR_WHO who);
	void	   SetNowWorkFor(ENUM_WORK_FOR_WHO who);
	void	   SetPrevWorkFor(ENUM_WORK_FOR_WHO who);
	CHAR* GetWindowTextAA();
	INT    GetUsedItem();
	void	  SetUsedItem(INT item);
	void  OnItemChanged(ENUM_WORK_FOR_WHO now);
	void  AddStringFromPEList(PE_List* pList, TCHAR* pSelectText);
	void Cleanup();
private:
	ENUM_WORK_FOR_WHO m_workForWhoNow;
	ENUM_WORK_FOR_WHO m_workForWhoPrev;
	//char m_TextBuff[1024];
	INT m_nItem;

protected:
	DECLARE_MESSAGE_MAP()
};


