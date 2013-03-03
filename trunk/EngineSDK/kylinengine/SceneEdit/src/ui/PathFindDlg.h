#ifndef __PathFindDLG_H__
#define __PathFindDLG_H__
/////////////////////////////////////////////////////////////////////////////
// Name:        PathFindDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "PathFindDlg.cpp"
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

namespace Fairy 
{

	class SceneManipulator;
	class CPathFindAction;

}

class CPathFindEditDlg : public wxPanel, Fairy::SceneListener
{

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���캯����
	//
public:


public:

	// �������������б�
	wxListBox* m_pListboxPointInRegion;

	// ɾ����İ�ť.
	wxButton*  m_pDelPointBn;

	wxTextCtrl* m_pPointX;
	wxTextCtrl* m_pPointZ;


	// ��������
	Fairy::SceneManipulator* m_pSceneManipulator;

	Fairy::CPathFindAction* m_pAction;

public:

	CPathFindEditDlg(const wxString& resourceGroup = wxT("PathFindEdit"));


	CPathFindEditDlg(
		wxWindow *parent, 
		const wxString& resourceGroup = wxT("PathFindEdit"),
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = "PathFindEdit");


	~CPathFindEditDlg(void);


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
		const wxString& name = "PathFindEdit");


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
	//void OnDelRegion( wxCommandEvent &event );

	// ��ť������ ɾ��һ��������.
	//void OnAddNewRegion( wxCommandEvent &event );

	// ���һ������ı༭
	//void OnFinishRegion( wxCommandEvent &event );

	// �¼�����,  �������弤���ʱ�����
	bool Show(bool show = true);

	// ��ѡ��listbox��Ӧ���¼�����
	void OnPointListBox(wxCommandEvent &event);

	// ɾ�����һ����, ��ʵ���õ��ǻ��˲�����
	void OnDelPointInRegion( wxCommandEvent &event );

	void OnEditXYZ( wxCommandEvent &event );

	/*******/
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// �߼����ݲ���
	//

	// ���һ������.
	void AddNewRegion();

	// �ڵ�ǰ���������һ���µĵ�
	void RegionAddNewPoint(unsigned long ulId, bool bRefresh = false);


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

	virtual void onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position);

	virtual void onDelDijkPoint(unsigned long ulPointId);

	virtual void onSelDijkPoint(unsigned long ulPointId);

	DECLARE_EVENT_TABLE()

};




#endif

