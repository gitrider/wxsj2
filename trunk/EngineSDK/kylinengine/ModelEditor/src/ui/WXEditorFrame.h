#ifndef __ACTOREDITORFRAME_H__
#define __ACTOREDITORFRAME_H__

#include <wx/frame.h>

#include <OgreVector3.h>
#include <OgreString.h>
#include "WXSceneCanvas.h"

#include "DataManipulator.h"
#include "ActorSelector.h"
#include "ObjSelector.h"
#include "AnimationSelector.h"
#include "ObjFileEditDlg.h"
#include "ObjFileEditorPanel.h"

//��ǰ����
class ActorSceneCanvas;
class wxLayoutManager;
class wxConfigBase;
class wxSpinEvent;
class wxListEvent;
class ObjSelector;

class ActorSelector;
struct wxHostInfo;
namespace Fairy {
    class SceneManipulator;
}


class ActorEditorFrame : public wxFrame
{
    DECLARE_CLASS(ActorEditorFrame)
    WX_DECLARE_CONTROL_CONTAINER();
public:
	ActorEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

	~ActorEditorFrame();
	
	// ��ȡCanvas��ָ��
    ActorSceneCanvas* GetCanvas(void) const
    {
        return m_Canvas;
    }

	void InitScene(Fairy::SceneManipulator* sceneManipulator);
	// ���õ�ǰ�ĳ����߼�����
	void SetCurSceneLogicPos(float fx, float fz);
	bool ProcessEvent(wxEvent& e);
public:
	// ������
	wxToolBar*		m_pToolbar;
	// ���泡��������
	Fairy::SceneManipulator* m_pSceneManipulator;
	
 protected:
    ActorSceneCanvas* m_Canvas;
	wxLayoutManager* m_Layout;									//���ֹ���
	wxMenu* m_WindowMenu;										//���ڲ˵�
	wxTextCtrl* m_LogWindow;									//��־����

	wxNotebook* m_NotebookActor;

    wxString m_strlastTitle;									//����
    wxString m_strlastSceneDirectory;							//�����ļ�·����
	wxString m_strlastObjDirectory;								//obj�ļ�·����
    wxString m_strsceneFilename;								//�����ļ���

	ActorSelector* m_pActorSelector;							//��ɫѡ��
	AnimationSelector* m_pAnimationSelector;					//����ѡ��
	ObjFileEditorPanel* m_pObjFileEditorPanel;					//�µ�obj�ļ��༭�Ի���
	ObjSelector* m_pObjSelector;								//obj�ļ�ѡ��

	bool mUseGameCamera;										//�Ƿ�ʹ����Ϸ���
public:
	Fairy::CDataManipulator* m_pDataManipulator;				//���ݲ�����ָ��
protected:
	void InitMenuBar();
	void InitToolBar(wxToolBar* toolbar);
    void CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo);
   
	void LoadSettings(wxConfigBase* cfg);				
    void SaveSettings(wxConfigBase* cfg) const;
   
    bool SaveCurFile();
    bool SaveModified(bool force = false);
	void LoadScene(); 
	bool SaveObjFile();
public:
	void OpenSelFile(const wxString& fileName, bool bIsMesh = false);
public:
    DECLARE_EVENT_TABLE()
	void OnUpdateWindow(wxUpdateUIEvent& e);			//���´���
	void OnFileOpen(wxCommandEvent& e);					//�ļ���
	void OnFileSave(wxCommandEvent& e);				//�����ļ�
	void OnSaveObjFile(wxCommandEvent& e);
	void OnResUpdate(wxCommandEvent& e);
	void SaveAvatarObjForEditor(wxCommandEvent& e);
	void OnExit(wxCommandEvent& e);
	void NoteBookPageChanged(wxBookCtrlEvent& e);
	void FileNoteBookPageChanged(wxBookCtrlEvent& e);
	void OnUseGameCamera(wxCommandEvent& e);			//ʹ����Ϸ���
	void OnUpdateUseGameCamera(wxUpdateUIEvent& e);
	void OnShowShortcutKeys(wxCommandEvent& e);
	void OnCheckBtns(wxCommandEvent& e);
	void OnUpdateCheckBtns(wxUpdateUIEvent& e);
	void OnUpdateFileMenu(wxUpdateUIEvent& e);

};

#endif // 
