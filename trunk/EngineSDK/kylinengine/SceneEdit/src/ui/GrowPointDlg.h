/////////////////////////////////////////////////////////////////////////////
// Name:        GrowPointDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __GrowPointDlg_H__
#define __GrowPointDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "GrowPointDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/GrowPointEdit_wdr.h"

// WDR: class declarations

#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

typedef std::map<int, std::string>	ID_NAME_DLG_MAP;//
typedef std::map<int, wxTreeItemId> ID_TREE_ITEM_MAP;

namespace Fairy 
{
    class SceneManipulator;
	class CGrowPointEditAction;		// ���������������򳡾��༭����
}


class SceneInfoMapDialog;
class FairyEditorFrame;
class CGrowPointEditDlg : public wxPanel, Fairy::SceneListener
{

///////////////////////////////////////////////////////////////////////////////////////////
//
// ���캯����
//
public:

	CGrowPointEditDlg(const wxString& resourceGroup = wxT("GrowPointEdit"));

	CGrowPointEditDlg(
						wxWindow *parent, 
						const wxString& resourceGroup = wxT("GrowPointEdit"),
						wxWindowID winid = wxID_ANY,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxTAB_TRAVERSAL | wxNO_BORDER,
						const wxString& name = "GrowPointEdit"
						);


	~CGrowPointEditDlg()
	{

	}

//
// ���캯����
//
///////////////////////////////////////////////////////////////////////////////////////////



	//****************************************************************************************************/
	//
	// ��ʼ������ ���������
	//

	// ����
    bool Create(
				wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "GrowPointEdit");


	// ��ʾ����
	bool Show(bool show);

	
	//
	// ��ʼ������ ���������
	//
	//****************************************************************************************************/



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �߼�����
//
//
public:

	// id �������б�
	ID_NAME_DLG_MAP		m_IdNameMap;

	// id ��mesh�б�
    ID_NAME_DLG_MAP		m_IdMeshMap;

	//id ��tree item�б�.
	ID_TREE_ITEM_MAP    m_IdTreeItemMap;

	// �Ѿ���ӵĹ���
	ID_TREE_ITEM_MAP    m_AddedTreeItemMap;


	// ���һ��������
	void AddNewGrowPoint(unsigned long ulMonsterId, unsigned long ulInstanceId);

	// ɾ��һ��������
	void DelGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId);

	// ���action��mesh��
	void SetActionMeshType();


//
// �߼�����
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ؼ�����
//

public:

	//����ͼ����
	SceneInfoMapDialog* m_pNavigationDlg;

	// ������ָ��
	FairyEditorFrame* m_pParent;

	// �Ѿ���ӹ������ĸ��ڵ�.
	wxTreeItemId	m_AddedGrowPointRoot;

	// �����б�ؼ���
	wxListCtrl*	m_pGrowPointListCtrl;

	// �Ѿ���ӵĹ����б�
	wxTreeCtrl* m_pAddedGrowPointTreeCtrl;

	// ��ʼ�������б�ı�ͷ.
	void InitGrowPointListCtrlTable();

	// ��ʼ�������б���.
	void InitAddedGrowPointTree();

	// �õ����еĿؼ�.
	void GetAllControl();

	// ���ui
	void ClearUI();

	// ���ø�����
	void SetParent(FairyEditorFrame* pParent);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �ؼ���Ӧ����
	//

	// ѡ��һ�������������
	 void OnGrowPointTypeSelected(wxListEvent& event);

	 // ѡ���Ѿ����������
	 void OnGrowPointAddedSelected(wxTreeEvent& event);

//
// ����ؼ�����
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������
	//

	public:

	Fairy::SceneManipulator*		m_pSceneManipulator;
	Fairy::CGrowPointEditAction*	m_pGrowPointEditAction;

	// �õ�action
	void GetAction();

	// ���ó����������
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);


	// �����
	void SetActive(bool bShow);

	//
	// ��������
	//
	////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����֪ͨ�ص�����
	//

	public:

	virtual void onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId);
	// ɾ��һ��������
	virtual void onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);
	// ѡ��һ��������
	virtual void onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);

	
	//
	// ����֪ͨ�ص�����
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	DECLARE_EVENT_TABLE()
	
};


#endif
