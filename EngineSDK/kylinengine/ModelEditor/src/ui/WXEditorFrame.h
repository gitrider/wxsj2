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

//提前声明
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
	
	// 获取Canvas类指针
    ActorSceneCanvas* GetCanvas(void) const
    {
        return m_Canvas;
    }

	void InitScene(Fairy::SceneManipulator* sceneManipulator);
	// 设置当前的场景逻辑坐标
	void SetCurSceneLogicPos(float fx, float fz);
	bool ProcessEvent(wxEvent& e);
public:
	// 工具栏
	wxToolBar*		m_pToolbar;
	// 保存场景管理器
	Fairy::SceneManipulator* m_pSceneManipulator;
	
 protected:
    ActorSceneCanvas* m_Canvas;
	wxLayoutManager* m_Layout;									//布局管理
	wxMenu* m_WindowMenu;										//窗口菜单
	wxTextCtrl* m_LogWindow;									//日志窗口

	wxNotebook* m_NotebookActor;

    wxString m_strlastTitle;									//标题
    wxString m_strlastSceneDirectory;							//场景文件路径名
	wxString m_strlastObjDirectory;								//obj文件路径名
    wxString m_strsceneFilename;								//场景文件名

	ActorSelector* m_pActorSelector;							//角色选择
	AnimationSelector* m_pAnimationSelector;					//动画选择
	ObjFileEditorPanel* m_pObjFileEditorPanel;					//新的obj文件编辑对话框
	ObjSelector* m_pObjSelector;								//obj文件选择

	bool mUseGameCamera;										//是否使用游戏相机
public:
	Fairy::CDataManipulator* m_pDataManipulator;				//数据操作类指针
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
	void OnUpdateWindow(wxUpdateUIEvent& e);			//更新窗口
	void OnFileOpen(wxCommandEvent& e);					//文件打开
	void OnFileSave(wxCommandEvent& e);				//保存文件
	void OnSaveObjFile(wxCommandEvent& e);
	void OnResUpdate(wxCommandEvent& e);
	void SaveAvatarObjForEditor(wxCommandEvent& e);
	void OnExit(wxCommandEvent& e);
	void NoteBookPageChanged(wxBookCtrlEvent& e);
	void FileNoteBookPageChanged(wxBookCtrlEvent& e);
	void OnUseGameCamera(wxCommandEvent& e);			//使用游戏相机
	void OnUpdateUseGameCamera(wxUpdateUIEvent& e);
	void OnShowShortcutKeys(wxCommandEvent& e);
	void OnCheckBtns(wxCommandEvent& e);
	void OnUpdateCheckBtns(wxUpdateUIEvent& e);
	void OnUpdateFileMenu(wxUpdateUIEvent& e);

};

#endif // 
