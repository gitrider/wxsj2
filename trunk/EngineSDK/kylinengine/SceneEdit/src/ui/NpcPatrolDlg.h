#ifndef __NPCPATROLDLG_H__
#define __NPCPATROLDLG_H__
/////////////////////////////////////////////////////////////////////////////
// Name:        NpcPatrolDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __NpcPatrolDlg_H__
#define __NpcPatrolDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "NpcPatrolDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"
#include <wx/listctrl.h>

namespace Fairy 
{
    
	class SceneManipulator;
	class CNpcPatrolAction;
	
}

class CNpcPatrolEditDlg : public wxPanel, Fairy::SceneListener
{

///////////////////////////////////////////////////////////////////////////////////////////
//
// ���캯����
//
public:

	
public:

	 // �������������б�
	 wxListCtrl* m_pListCtrlRegion;

	 // �������������б�
	 wxListBox* m_pListboxPointInRegion;

	 // ɾ����İ�ť.
	 wxButton*  m_pDelPointBn;

	 // ȷ����ɱ༭����ť.
	 wxButton*  m_pRegionOkBn;

	 // ɾ����İ�ť.
	 wxButton*  m_pDelRegionBn;

	 // ȷ����ɱ༭����ť.
	 wxButton*  m_pAddNewRegionBn;

	 wxTextCtrl* m_pPointX;
	 wxTextCtrl* m_pPointY;
	 wxTextCtrl* m_pPointZ;
	 wxCheckBox* m_pUseY;
	 wxTextCtrl* m_pDistance;

	 
	 // ��������
	 Fairy::SceneManipulator* m_pSceneManipulator;

	 Fairy::CNpcPatrolAction* m_pAction;

public:
	
	CNpcPatrolEditDlg(const wxString& resourceGroup = wxT("NpcPatrolEdit"));
  

    CNpcPatrolEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup = wxT("NpcPatrolEdit"),
							wxWindowID winid = wxID_ANY,
							const wxPoint& pos = wxDefaultPosition,
							const wxSize& size = wxDefaultSize,
							long style = wxTAB_TRAVERSAL | wxNO_BORDER,
							const wxString& name = "NpcPatrolEdit");
   
	
	~CNpcPatrolEditDlg(void);


public:


	/******/
	/////////////////////////////////////////////////////////////////////////////////
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
                const wxString& name = "NpcPatrolEdit");

	
	// ��ʼ������
	void Init(const wxString& resourceGroup);

	// ��ʼ���߼�����
	void InitData();

	// �������
	void ClearData();

	// ��RegionAction��ˢ������, ���̵�ʱ��ʹ��
	void RefreshData();

	// ���ui���������
	void ClearUiData();

	// ��ȡ����
	void LoadData(std::string& strFile);

	// �洢����
	void SaveData(std::string strFile);

	// // �õ�����༭�ĳ�������
	void GetRegionAction();

	// ��������༭��ʾ
    void UpdateListCtrlRegionDataView();

public:

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����������������.
	//
	unsigned long m_ulCurRegionId;		// ��ǰ�༭������id.
	unsigned long m_ulCurPointId;		// ��ǰ�༭�ĵ��id.
	unsigned long m_ulCurLineId;		// ��ǰ�༭���ߵ�id.

	bool		  m_bIsChangeCamera;	    // �Ƿ�ı������λ�á�
	bool		  m_bIsEditingRegion;		// �Ƿ����ڱ༭����

	int			  m_iCurSelRegionIndex;

	/*******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���������Ӧ����
	//


	// �������ڱ༭�����ǡ�
	void SetEditRegionFlag(bool bIsEdit);

	// ��ť������ ɾ��һ��������.
	void OnDelRegion( wxCommandEvent &event );

	// ��ť������ ɾ��һ��������.
	void OnAddNewRegion( wxCommandEvent &event );

	// ���һ������ı༭
	void OnFinishRegion( wxCommandEvent &event );

	// �¼�����,  �������弤���ʱ�����
	bool Show(bool show = true);

	// �������б�ѡ��ı�ʱ����.
    void OnRegionListCtrlItemSelected(wxListEvent& event);

	// ��ѡ��listbox��Ӧ���¼�����
	void OnPointListBox(wxCommandEvent &event);

	// ɾ�����һ����, ��ʵ���õ��ǻ��˲�����
	void OnDelPointInRegion( wxCommandEvent &event );


	void OnPointUseY( wxCommandEvent &event );

	void OnEditXYZ( wxCommandEvent &event );

	//// ��ť������ ɾ��һ����.
	//void OnDelPointInRegion( wxCommandEvent &event );

	/*******/
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// �߼����ݲ���
	//

	// ɾ��һ������
	void DelRegion(unsigned long ulId);

	// ���һ������.
	void AddNewRegion();

	// �ڵ�ǰ���������һ���µĵ�
	void RegionAddNewPoint(unsigned long ulId, bool bRefresh = false);

	// ����ǰѡ�е�����
	void DoSelRegion();

	// ���һ������ı༭
	void FinishRegion();


	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����߼�����
	//

	// ѡ�е�Ĳ���
	void DoSelPoint();

	// �ѵ����Ϣˢ�µ�������
	void RefreshRegionPointData(unsigned long ulRegionId);

	// ɾ�������е�һ����
	void DelRegionPoint(unsigned long ulPointId);





	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������
	//

	// ��������༭����
	void SetActive(bool bShow);

	// ����
	void ActiveAction(bool bActive);


	// ���ó����������.
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

public:
   
	/***********************/
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����ص���������
	//
	// ���һ�����壬 ��ʱû��
    virtual void onAddObject(const Fairy::ObjectPtr& object);

	//������������ص�����
	virtual void onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position);

	// ɾ��һ����Ļص�����
	virtual void onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId);

	// ѡ��һ������
	virtual void onRegionSel(unsigned long ulRegionId);
    
	DECLARE_EVENT_TABLE()
	
};




#endif

#endif //__NPCPATROLDLG_H__