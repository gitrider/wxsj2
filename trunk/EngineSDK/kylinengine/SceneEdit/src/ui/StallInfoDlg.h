/////////////////////////////////////////////////////////////////////////////
// Name:       StallInfoDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////
#ifndef __STALLINFODLG_H__
#define __STALLINFODLG_H__
#pragma once

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "StallInfoDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/StallInfoDlg_wdr.h"

// WDR: class declarations

#include <wx/panel.h>
#include <wx/image.h>
#include "WXSceneListener.h"

namespace Fairy 
{
    
	class SceneManipulator;
	class CStallInfoAction;
	
}
class CStallInfoDlg: public wxPanel, Fairy::SceneListener
{
public:


///////////////////////////////////////////////////////////////////////////////////////////
//
// ���캯����
//
public:

	CStallInfoDlg(const wxString& resourceGroup = wxT("StallInfoEdit"));

	CStallInfoDlg(
						wxWindow *parent, 
						const wxString& resourceGroup = wxT("StallInfoEdit"),
						wxWindowID winid = wxID_ANY,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxTAB_TRAVERSAL | wxNO_BORDER,
						const wxString& name = "StallInfoEdit"
						);


	~CStallInfoDlg();


//
// ���캯����
//
///////////////////////////////////////////////////////////////////////////////////////////



	//****************************************************************************************************/
	//
	// ��ʼ������ ���������
	//

	
	 // �Ƿ��̯
	wxTextCtrl* m_pTextCanStall;		

	// ˰��
	wxTextCtrl* m_pTextTax;		

	// ̯λ��
	wxTextCtrl* m_pTextPosTax;		

	// ������Ϣ
	wxTextCtrl* m_pExtraInfo;		

	// ��ǰ����������
	int        m_iCurOperateType;

	unsigned char	m_iTradeTax;	//����˰
	unsigned long	m_dwPosTax;		//̯λ��
	unsigned char	m_iExtraInfo;	//������Ϣ

	// ����
    bool Create(
				wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "StallInfoEdit");



	// �õ��ؼ���Ϣ
	void GetAllControl();

	
	//
	// ��ʼ������ ���������
	//
	//****************************************************************************************************/


	
	// ��ȡ����
	void LoadData(std::string strFile);

	// �洢����
	void SaveData(std::string strFile);

	// ����̯λ��Ϣ
	void CreateStallInfo();

	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������
	//

	 // ��������
	 Fairy::SceneManipulator* m_pSceneManipulator;

	 // ��Ӧ��action
	 Fairy::CStallInfoAction* m_pEditAction;

	// �õ�action
	void GetAction();

	// ��������༭����
	void SetActive(bool bShow);

	// ���ó����������.
	void SetSceneManipulator(Fairy::SceneManipulator* sceneManipulator);

	//�ؽ�̯λ��С
	void ReCreateStallInfo();

	// ��ʾ����.
	bool Show(bool show);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �¼�����
	//

	// ��ť������ ����̯λ��Ϣ
	void OnSetStallInfo( wxCommandEvent &event );

	// ��ť������ ѡ��
	void OnDelStallInfo( wxCommandEvent &event );

	// ��ť������ ɾ��һ������
	void OnViewStallInfo( wxCommandEvent &event );

	// ˰�ʸı�
	void OnTradeTaxChange(wxCommandEvent& event);

	// ̯λ�Ѹı�
	void OnPosTaxChange(wxCommandEvent& event);

	// ������Ϣ�ı�
	void OnExtraInfoChange(wxCommandEvent& event);


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����֪ͨ�ص�����
	//

public:

		virtual void onViewStallInfo(
								unsigned char	bCanStall,	//�Ƿ���԰�̯
								unsigned char	iTradeTax,	//����˰
								unsigned long	dwPosTax,	//̯λ��
								unsigned char	iExtraInfo	//������Ϣ 
								);
		

	
	//
	// ����֪ͨ�ص�����
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	DECLARE_EVENT_TABLE()

};

#endif //__STALLINFODLG_H__