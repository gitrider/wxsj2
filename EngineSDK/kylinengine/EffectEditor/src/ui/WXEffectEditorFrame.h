#ifndef __WXEFFECTFRAME_H__
#define __WXEFFECTFRAME_H__

#include <wx/frame.h>

#include <OgreVector3.h>
#include <OgreString.h>
#include "WXSceneListener.h"
// forward declares
// ----------------------------------------------------------------------------

class FairySceneCanvas;
class AutoSaveSceneController;
class ObjectPropertyEditor;
class ParticleSystemSelector;
class EffectSelector;
class SkillSelector;
class MaterialSelector;
class ActorSelector;
class BulletSelector;
class EffectObjectPropertyEditor;
class EffectObjectEditor;
class SkillObjectEditor;
class ActorAnimationSettingEditor;
class ParticleObjectEditor;
class MaterialEditor;
class BulletEditor;
class wxLayoutManager;
struct wxHostInfo;
class wxConfigBase;
class wxSpinEvent;
class wxListEvent;
class wxGraphDialog;
namespace Fairy {
    class SceneManipulator;
	class LogicModel;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new frame
class EffectEditorFrame : public wxFrame
{
    DECLARE_CLASS(EffectEditorFrame)
    WX_DECLARE_CONTROL_CONTAINER();

public:

	ParticleSystemSelector* mParticleSystemSelector;
	EffectSelector* m_EffectSelector;
	SkillSelector* m_SkillSelector;
	MaterialSelector* m_MaterialSelector;
	wxGraphDialog* mGraphDialog;
	EffectObjectPropertyEditor *mEffectObjectProperty; 
	EffectObjectEditor* mEffectObjectEditor;
	SkillObjectEditor* mSkillObjectEditor;
	MaterialEditor* mMaterialEditor;
	ActorAnimationSettingEditor* mActorAnimSettingEditor;
	ActorSelector* mActorSelector;
	BulletEditor* mBulletEditor;
	BulletSelector* mBulletSelector;


	//WXEffectEditDialog * m_pEffectEditDlg;
	// 工具栏
	wxToolBar*		m_pToolbar;
	
	// 保存场景管理器
	Fairy::SceneManipulator* m_pSceneManipulator;


protected:
    FairySceneCanvas* mCanvas;
    ObjectPropertyEditor* mObjectPropertyEditor;


    wxTextCtrl* mLogWindow;

	wxNotebook* mEditorNotebook;
	wxNotebook* mBottomNotebook;
	wxNotebook* mPropertyNotebook;

    wxLayoutManager* mLayout;
    wxMenu* mWindowMenu;

    bool mLockBrushSize;

    wxString m_lastTitle;
    wxString m_lastSceneDirectory;
    wxString m_sceneFilename;

    int mSpinnerValue;
    wxString mByAxis;
    int mSegment;
    bool mActiveSnapRotation;

	Ogre::String mInfoString;
	Ogre::String mManipInfoString;

	bool mUseRealCamera;
	bool mUseSnapRotation;

    int mTextureFilterOptions;

    int mCreateLevelOptions;

	AutoSaveSceneController* mAutoSaveSceneController;


protected:
    void CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo);
    void InitMenuBar(void);
    void InitToolBar(wxToolBar* toolbar);
    void InitStatusBar(wxStatusBar* statusbar);

public:
    EffectEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    ~EffectEditorFrame();


    FairySceneCanvas* GetCanvas(void) const
    {
        return mCanvas;
    }
    Fairy::SceneManipulator* GetSceneManipulator(void) const;

	EffectObjectPropertyEditor* GetEffectObjectProperty(){return mEffectObjectProperty;}
	void setEffectObjectProperty(EffectObjectPropertyEditor* editor){mEffectObjectProperty  = editor;}

	EffectObjectEditor* GetEffectObjectEditor(){return mEffectObjectEditor;}
	void setEffectObjectEditor(EffectObjectEditor* editor){mEffectObjectEditor  = editor;}

	SkillObjectEditor* GetSkillObjectEidtor(){ return mSkillObjectEditor;}
	void SetSkillObjectEditor(SkillObjectEditor* editor){mSkillObjectEditor = editor;}

	MaterialEditor* GetMaterialEditor(){return mMaterialEditor;}
	void setMaterialEditor(MaterialEditor* editor){mMaterialEditor  = editor;}

	wxGraphDialog* GetGraphDialog(){return mGraphDialog;}
	void SetGraphDialog(wxGraphDialog* dlg){mGraphDialog  = dlg;}

	ParticleSystemSelector* GetParticleSystemSelector(){return mParticleSystemSelector;}
	EffectSelector* GetEffectSelector(){return m_EffectSelector;}
	SkillSelector* GetSkillSelector(){return m_SkillSelector;}

	ActorAnimationSettingEditor* GetActorSettingEditor() {return mActorAnimSettingEditor;}
	ActorSelector* GetActorSelector(){return mActorSelector;}

	BulletEditor* GetBulletEditor() {return mBulletEditor;}
	BulletSelector* GetBulletSelector(){return mBulletSelector;}


	wxNotebook* GetEditorNotebook(){return mEditorNotebook;}
	void SetEditorNotebook(wxNotebook* notebook){mEditorNotebook = notebook;}
	wxNotebook* GetBottomNotebook(){return mBottomNotebook;}
	void SetBottomNotebook(wxNotebook* notbook){mBottomNotebook = notbook;}

    void InitScene(Fairy::SceneManipulator* sceneManipulator);

	void SetKeyboardShortcuts(void);

	//隐藏所有正在编辑的效果物体
	void HideAllEditorObject();
	//清理所有选择器浏览的效果物体
	void ClearAllSelectorObject();
    //////////////////////////////////////////////////////////////////////////

    void LoadSettings(wxConfigBase* cfg);
    void SaveSettings(wxConfigBase* cfg) const;

    bool Save(void);
    bool SaveAs(void);
    bool SaveModified(bool force = false);

    bool SaveImpl(bool autoSave = false);

    //////////////////////////////////////////////////////////////////////////
    bool ProcessEvent(wxEvent& e);

    void SetSpinnerValue(int value);
    int GetSpinnerValue();

    wxString GetByAxis();
    void SetByAxis(wxString& strAxis);

    void SetSnapRotationValue(int value);
    int GetSnapRotationValue();

    void SetActiveSnapRotation(bool value);
    bool GetActiveSnapRotation();

	AutoSaveSceneController* getAutoSaveController(void)
	{
		return mAutoSaveSceneController;
	}

    DECLARE_EVENT_TABLE()

    void OnUpdateWindow(wxUpdateUIEvent& e);
    void OnCloseWindow(wxCloseEvent& e);

	//编辑页改变
	void OnEditorPageChanged(wxNotebookEvent& event);

    void OnFileNew(wxCommandEvent& e);
    void OnFileOpen(wxCommandEvent& e);
    void OnFileSave(wxCommandEvent& e);
    void OnFileSaveAs(wxCommandEvent& e);

    void OnUpdateFileNew(wxUpdateUIEvent& e);
    void OnUpdateSave(wxUpdateUIEvent& e);
    void OnUpdateSaveAs(wxUpdateUIEvent& e);

    void OnUndo(wxCommandEvent& e);
    void OnRedo(wxCommandEvent& e);
    void OnUpdateUndo(wxUpdateUIEvent& e);
    void OnUpdateRedo(wxUpdateUIEvent& e);

    void OnLockCameraHeight(wxCommandEvent& e);
    void OnUpdateLockCameraHeight(wxUpdateUIEvent& e);

	void OnSelectAllObjects(wxCommandEvent& e);
    void OnUpdateSelectAllObjects(wxUpdateUIEvent& e);


	void OnHeightActionExpValueTextChanged(wxCommandEvent& e);

	void OnResetCamera(wxCommandEvent& e);

	// hack 临时设置泛光效果
	void OnBlurAmountTextChanged(wxCommandEvent& e);
	void OnShineAmountTextChanged(wxCommandEvent& e);
	void OnUpdateFloodlightingControls(wxUpdateUIEvent& e);


    void OnShowStatPanel(wxCommandEvent& e);
    void OnUpdateShowStatPanel(wxUpdateUIEvent& e);
    void OnShowLogoPanel(wxCommandEvent& e);
    void OnUpdateShowLogoPanel(wxUpdateUIEvent& e);

    void OnToggleToolWindows(wxCommandEvent& e);
    void OnUpdateToggleToolWindows(wxUpdateUIEvent& e);
    void OnFullScreen(wxCommandEvent& e);
    void OnUpdateFullScreen(wxUpdateUIEvent& e);

    void OnDeleteStaticObject(wxCommandEvent& e);
    void OnUpdateDeleteStaticObject(wxUpdateUIEvent& e);


	void OnSaveEnviromentInfoToFile(wxCommandEvent& e);
	void OnUpdateSaveEnviromentInfoToFile(wxUpdateUIEvent& e);
	void OnLoadEnviromentInfoFromFile(wxCommandEvent& e);
	void OnUpdateLoadEnviromentInfoFromFile(wxUpdateUIEvent& e);

    void OnShowStandardModel(wxCommandEvent& e);
    void OnUpdateShowStandardModel(wxUpdateUIEvent& e);

    void OnSelectAction(wxCommandEvent& e);
    void OnUpdateSelectAction(wxUpdateUIEvent& e);


    void OnSceneAnimationQuality(wxCommandEvent& e);
    void OnObjectCreateLevel(wxCommandEvent& e);

    void OnAlignPosition(wxCommandEvent& e);
    void OnUpdateAlignPosition(wxUpdateUIEvent& e);

    void OnExit(wxCommandEvent& e);

    //void OnCameraSetting(wxCommandEvent& e);
    void OnPreferSetting(wxCommandEvent& e);

	void OnEraseTextureByName(wxCommandEvent& e);

    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);

	void OnCreateLight(wxCommandEvent& e);

    void OnUpdateCutCopyPaste(wxUpdateUIEvent& e);


	void OnEnable32BitTextures(wxCommandEvent& e);
	void OnUpdateEnable32BitTextures(wxUpdateUIEvent& e);

	void OnReloadTextures(wxCommandEvent& e);
	void OnUpdateReloadTextures(wxUpdateUIEvent& e);

	void OnUnloadUselessResources(wxCommandEvent& e);
	void OnUpdateUnloadUselessResources(wxUpdateUIEvent& e);

    void OnShowAllIndicators(wxCommandEvent& e);
    void OnUpdateShowAllIndicators(wxUpdateUIEvent& e);

    void OnSelectPostFilter(wxCommandEvent& e);
    void OnUpdateSelectPostFilter(wxUpdateUIEvent& e);

    void OnSelectShadowTechnique(wxCommandEvent& e);
    void OnUpdateSelectShadowTechnique(wxUpdateUIEvent& e);

    void OnSelectDefaultTextureFiltering(wxCommandEvent& e);
    void OnUpdateSelectDefaultTextureFiltering(wxUpdateUIEvent& e);

    void OnSelectSceneCreateLevel(wxCommandEvent& e);

    void OnUpdateSelectSceneCreateLevel(wxUpdateUIEvent& e);

	void OnUseSnapRotation(wxCommandEvent& e);
	void OnUpdateUseSnapRotation(wxUpdateUIEvent& e);

	void OnFrameRateTextChanged(wxCommandEvent& e);

	//显示隐藏object
	void OnHideActor(wxCommandEvent& e);
	//显示目标object
	void OnHidTargetActor(wxCommandEvent& e);
	//显示隐藏网格
	void OnHideGrid(wxCommandEvent& e);
	//显示隐藏坐标轴
	void OnHideAxis(wxCommandEvent& e);
	//显示隐藏地形
	void OnHideTerrain(wxCommandEvent& e);
	//设置背景颜色
	void OnSelectBackgroundColour(wxCommandEvent& e);

	void OnUpdateHideActor(wxUpdateUIEvent& e);
	void OnUpdateHideTargetActor(wxUpdateUIEvent& e);
	void OnUpdateHideGrid(wxUpdateUIEvent& e);
	void OnUpdateHideAxis(wxUpdateUIEvent& e);
	void OnUpdateHideTerrain(wxUpdateUIEvent& e);

	

	bool m_bHideActor;
	bool m_bHideTargetActor;
	bool m_bHideGrid;
	bool m_bHideAxis;
	bool m_bHideTerrain;

	///////////////////////////////////////////////////////////////////////////////////
	//
	// 建筑物可行走面数据
	//
public:



	//
	// 
	// 把场景坐标转换成游戏逻辑坐标。
	//
	void TranslateSceneToGameLogic(float &x, float &y, float &z, Ogre::Matrix4& TransformMatrix);


	
	public:

		// 读取不可行走区域
		///bool ReadNpc();


};

#endif // 
