#ifndef __REGIONEDITDLGSECTOR_H__
#define __REGIONEDITDLGSECTOR_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "RegionEditDlgSelector.h"
#endif

#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

namespace Fairy 
{
    class SceneManipulator;
	class CRegionEditAction;		// ���������������򳡾��༭����
}

class CRegionEditDlgSector : public wxPanel, Fairy::SceneListener
{

public:

	 // �������������б�
	 wxListBox* m_pListboxRegion;

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

	  // ����༭״̬��ť
	 wxButton*  m_pEditRgionBn;

	  // �Ƿ��ܷɹ��ı��
	 wxTextCtrl* m_pFlyableCB;

	 wxTextCtrl* m_SearchRgionTextCtrl;
	 wxButton* m_SearchRgionBtn;


	 // ��������
	 Fairy::SceneManipulator* m_pSceneManipulator;

	 Fairy::CRegionEditAction* m_pRegionEditAction;

public:
	
	CRegionEditDlgSector(const wxString& resourceGroup = wxT("RegionEdit"));
  

    CRegionEditDlgSector(
							wxWindow *parent, 
							const wxString& resourceGroup = wxT("RegionEdit"),
							wxWindowID winid = wxID_ANY,
							const wxPoint& pos = wxDefaultPosition,
							const wxSize& size = wxDefaultSize,
							long style = wxTAB_TRAVERSAL | wxNO_BORDER,
							const wxString& name = "RegionEdit");
   
	
	~CRegionEditDlgSector(void);


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
                const wxString& name = "RegionEdit");

	
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
	void OnDelRegion( wxCommandEvent &event );

	// ��ť������ ɾ��һ��������.
	void OnAddNewRegion( wxCommandEvent &event );

	//��ť�������༭һ��ѡ�е�����
	void OnEditSelectRegion( wxCommandEvent &event );

	// ���һ������ı༭
	void OnFinishRegion( wxCommandEvent &event );

	// �¼�����,  �������弤���ʱ�����
	bool Show(bool show = true);

	// �������б��ѡ��ı�ʱ����.
	void OnRegionListBox(wxCommandEvent &event);

	// ��ѡ��listbox��Ӧ���¼�����
	void OnPointListBox(wxCommandEvent &event);

	// ɾ�����һ����, ��ʵ���õ��ǻ��˲�����
	void OnDelPointInRegion( wxCommandEvent &event );

	// �����Ƿ��Ṧͨ�����
	void OnFlyable( wxCommandEvent &event );

	void OnSearchRgion( wxCommandEvent& event);

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

	//�༭ѡ�е�����
	void EditSelectRegion();

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

	void DelRegion();





	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������
	//

	// ��������༭����
	void SetActive(bool bShow);

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

	// ѡ��༭�����еĵ�
	virtual void onSelectPoint(unsigned long ulRegionId,unsigned long ulPointId);

	// ɾ��һ����Ļص�����
	virtual void onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId);

	// ѡ��һ������
	virtual void onRegionSel(unsigned long ulRegionId);

	DECLARE_EVENT_TABLE()
};

extern CRegionEditDlgSector* g_pRegionEdit;

#endif //__REGIONEDITDLGSECTOR_H__