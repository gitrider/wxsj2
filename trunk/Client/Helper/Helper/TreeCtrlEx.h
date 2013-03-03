#pragma once

#include "CustomCtl/VividTree.h"
// CTreeCtrlEx

class CTreeCtrlEx : public VividTree
{
	DECLARE_DYNAMIC(CTreeCtrlEx)

public:
	// add a icon to this tree.
	INT				AddIcon(HICON hIcon);

protected:
	// all icons and bitmap that used by item.
	HIMAGELIST		m_hImageList;

public:
	CTreeCtrlEx();
	virtual ~CTreeCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()
};


