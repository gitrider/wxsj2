#pragma once

#include "OptionTreeItem.h"
// COptionTreeDialogPanel

class COptionTreeDialogPanel : public CWnd,  public COptionTreeItem
{
	DECLARE_DYNAMIC(COptionTreeDialogPanel)

public:
	COptionTreeDialogPanel(INT dlgResId, CWnd* pParent, DLGPROC dlgProc);
	virtual ~COptionTreeDialogPanel();
protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL Create(INT dlgResId, CWnd* pParent, DLGPROC dlgProc);

};


