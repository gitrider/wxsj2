/////////////////////////////////////////////////////////////////////////////
// Name:        CEventAreaEditDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __CEventAreaEditDlg_H__
#define __CEventAreaEditDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "CEventAreaEditDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "res/EventArea_wdr.h"

// WDR: class declarations

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"
#include <string>

namespace Fairy 
{
    
	class SceneManipulator;
	class CEventAreaEditAction;
	
}

class CEventAreaEditDlg : public wxPanel, Fairy::SceneListener
{

///////////////////////////////////////////////////////////////////////////////////////////
//
// ���캯����
//
public:

	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����ؼ�
//
//

public:

	 // �Ѿ���������б�
	 wxListCtrl*	m_pRectListCtrl;

	 // �����id��ť.
	 wxButton*  m_pAddNewEventAreaIdBn;

	  // ���Select
	 wxButton*  m_pSelectBn;

	  // ���Draw
	 wxButton*  m_pDrawBn;


	 // id�ؼ�
	wxTextCtrl* m_pEventIdText;		

	// �ű��ؼ�
	wxTextCtrl* m_pScriptidText;	

	wxCheckBox* m_pLocalCheck;

	wxGrid* m_pParamsGrid;

	 // ȷ����ɱ༭����ť.
	 //wxButton*  m_pRegionOkBn;

	 // // ɾ����İ�ť.
	 //wxButton*  m_pDelRegionBn;

	 // // ȷ����ɱ༭����ť.
	 //wxButton*  m_pAddNewRegionBn;


	//�õ��ű�id
	std::string GetScriptIdValue();

	// ��ս���
	void ClearUI();

	// ��ս���
	void RefreshDataUI();

	void UpdateParamsInfo();

	 
	 // ��������
	 Fairy::SceneManipulator* m_pSceneManipulator;

	 // ��Ӧ��action
	 Fairy::CEventAreaEditAction* m_pEditAction;

public:
	
	CEventAreaEditDlg(const wxString& resourceGroup = wxT("EventAreaEdit"));
  

    CEventAreaEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup = wxT("EventAreaEdit"),
							wxWindowID winid = wxID_ANY,
							const wxPoint& pos = wxDefaultPosition,
							const wxSize& size = wxDefaultSize,
							long style = wxTAB_TRAVERSAL | wxNO_BORDER,
							const wxString& name = "EventAreaEdit");
   
	
	~CEventAreaEditDlg(void);


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
                const wxString& name = "EventAreaEdit");

	
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
	void LoadData(std::string strFile);

	// �洢����
	void SaveData(std::string strFile);

	// // �õ�����༭�ĳ�������
	void GetEventAreaAction();
	
	
	// �õ�action
	void GetAction();


	

public:

/*******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���������Ӧ����
	//

	
	// ��ť������ ɾ��һ��������.
	void OnAddNewId( wxCommandEvent &event );

	// ��ť������ ѡ��
	void OnSelect( wxCommandEvent &event );

	// ��ť������ ѡ��
	void OnDraw( wxCommandEvent &event );

	// ��ť������ ɾ��һ������
	void OnDeleteRect( wxCommandEvent &event );

	// ѡ��һ�������������
	 void OnSelectEventAreaRect(wxListEvent& event);

	// ��ť������ ����һ����id
	void OnModifyArea( wxCommandEvent &event );

	// �л��������ͣ����ػ���serverʹ��
	void OnCheckLocal( wxCommandEvent & event );

	void OnGridCellChanged(wxGridEvent & event);





	/*******/
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// �߼����ݲ���
	//

	// ��ǰ������id
	int m_CurEventAreaId;

	// ����ǰ�Ľű�id
	int m_iCurScriptId;

	// ��������.
	int m_OperatroType;


	// ���ñ༭action�Ľű�id
	void SetActionScriptValue();



	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������
	//

	// ��������༭����
	void SetActive(bool bShow);

	// ���ó����������.
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	// ��ʾ����.
	bool Show(bool show);


public:
   
	/***********************/
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����ص���������
	//

	virtual void onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId);

	// ɾ��һ������
	virtual void onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
	
	// ѡ��һ������
	virtual void onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
	


	DECLARE_EVENT_TABLE()
	
	
};



#endif
