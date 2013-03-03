// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXEffectEditorFrame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/pen.h>
    #include <wx/dcscreen.h>
    #include <wx/dcmemory.h>
    #include <wx/scrolwin.h>
    #include <wx/toolbar.h>
    #include <wx/statusbr.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
#endif

#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/busyinfo.h>
#include <wx/config.h>
#include <wx/stockitem.h>
#include <wx/filename.h>
#include <wx/colordlg.h>
// copy paste
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

#include "WXEffectEditorFrame.h"
#include "WXSceneCanvas.h"
#include "WXOperatorManager.h"
#include "ObjectPropertyEditor.h"
#include "SceneManipulator.h"
#include "ExceptionInfoDialog.h"
#include "AutoSaveSceneController.h"
#include "SettingDialogs.h"
#include "Core/FrameStatsListener.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyObject.h"
#include "ParticleSystemSelector.h"
#include "EffectSelector.h"
#include "SkillSelector.h"
#include "ActorSelector.h"
#include "BulletSelector.h"
#include "MaterialSelector.h"
#include "EffectObjectEditor.h"
#include "SkillObjectEditor.h"
#include "BulletEditor.h"
#include "WXGraphDialog.h"
#include "WXGraphItem.h"
#include "EffectObjectPropertyEditor.h"
#include "MaterialEditor.h"
#include "Core/FairyObjectFactoryManager.h"
#include "ModelSystem/FairyLogicModel.h"
#include "Core/FairyActorObject.h"
#include "Core/FairyActor.h"
#include <PathLibInterface.h>
#include "ActorAnimationSettingEditor.h"
#include "OgreManualObject.h"
#include "OgreEntity.h"
#include "OgreColourValue.h"
// ----------------------------------------------------------------------------
// wxDockIt headers
// ----------------------------------------------------------------------------

#include <wx/layoutmanager.h>
#include <wx/dockwindow.h>
#include <wx/dockhost.h>
#include <wx/accel.h>

//-----------------------------------------------------------------------------
// File header
//-----------------------------------------------------------------------------
#include <wx/file.h>

//-----------------------------------------------------------------------------
// Dialogs
//-----------------------------------------------------------------------------
#include <wx/numdlg.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreException.h>
#include <OgreRenderWindow.h>
#include "Action.h"
#include "PostFilter/FairyPostFilterManager.h"
#include "PostFilter/FairyPostFilter.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

enum
{
    feID_BASE = wxID_HIGHEST + 1001,

    feID_LOCK_BRUSH_SIZE,
 
    feID_HEIGHT_ACTION_AFFECT_RANGE,
	feID_HEIGHT_ACTION_EXP_VALUE,
	feID_HEIGHT_ADJUST_SPEED,

	// hack，临时设置泛光效果
	feID_BLURAMOUNT,
	feID_SHINEAMOUNT,
	feID_FRAMERATE,

	feID_BLURAMOUNTSTATICTEXT,
	feID_SHINEAMOUNTSTATICTEXT,

    feID_LOCK_CAMERA_HEIGHT = feID_BASE + 200,
	feID_SELECTALLOBJECTS,
    feID_SHOW_STAT_PANEL,
    feID_SHOW_LOGO_PANEL,
    feID_TOGGLE_TOOL_WINDOWS,
    feID_FULL_SCREEN,
	feID_RESET_CAMERA,

    feID_ACTION_FIRST   = feID_BASE + 300,

    feID_DRAG_HEIGHT_ACTION = feID_ACTION_FIRST,
    feID_RAISE_HEIGHT_ACTION,
    feID_LOWER_HEIGHT_ACTION,
    feID_SMOOTH_HEIGHT_ACTION,
    feID_LEVEL_OFF_HEIGHT_ACTION,
    feID_MANIP_OBJECT_ACTION,
    feID_FLIP_DIAGONAL_ACTION,
    feID_DISABLE_DECAL_ACTION,
    feID_ACTION_LAST,
	feID_ERASE_BY_TEXTURE_NAME,

    feID_SCENE_ANIMATION_QUALITY,
    feID_OBJECT_CREATE_LEVEL,

    feID_CREATE_MINI_MAP,

    feID_ALIGN_POSITION,

    feID_PREFERSETTING,
	feID_GPU_PROGRAM_PARAMETER_SETTING,

    feID_SHOW_STANDARD_MODEL,
    feID_DELETESTATICOBJECT,

	feID_SAVEENVIROMENTINFO,
	feID_LOADENVIROMENTINFO,


	feID_CREATE_LIGHT,
	
    feID_POST_FILTER_FIRST = feID_BASE + 500,
    feID_PF_HEATVISION = feID_POST_FILTER_FIRST,
    feID_PF_BLOOM,
    feID_PF_FLOODLIGHTING,
    feID_PF_FADE,
    feID_PF_MOTION_BLUR,
    feID_POST_FILTER_LAST,

    feID_SHADOWTYPE_NONE,
    feID_SHADOWTYPE_TEXTURE_ADDITIVE,
    feID_SHADOWTYPE_TEXTURE_MODULATIVE,
    feID_SHADOWTYPE_STENCIL_ADDITIVE,
    feID_SHADOWTYPE_STENCIL_MODULATIVE,

    feID_TFO_NONE,
    feID_TFO_BILINEAR,
    feID_TFO_TRILINEAR,
    feID_TFO_ANISOTROPIC,

    feID_CL_LOW,
    feID_CL_MID,
    feID_CL_HIGH,

    feID_ENABLE_32BIT_TEXTURES,
    feID_RELOAD_TEXTURES,
    feID_UNLOAD_USELESS_RESOURCES,
    feID_SHOW_INDICATORS,

	feID_USE_SNAP_ROTATION,
	feID_SHOW_ACTOR,
	feId_SHOW_TARGET_ACTOR,
	feID_SHOW_GRID,
	feID_SHOW_AXIS,
	feID_SHOW_TERRAIN,
	feID_SET_BACKGROUND,

};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(EffectEditorFrame, wxFrame)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(EffectEditorFrame, wxFrame)
    WX_EVENT_TABLE_CONTROL_CONTAINER(EffectEditorFrame)

    EVT_MENU(wxID_NEW, EffectEditorFrame::OnFileNew)

    EVT_MENU(wxID_OPEN, EffectEditorFrame::OnFileOpen)
    EVT_MENU(wxID_SAVE, EffectEditorFrame::OnFileSave)
    EVT_MENU(wxID_SAVEAS, EffectEditorFrame::OnFileSaveAs)

    EVT_MENU(wxID_CUT, EffectEditorFrame::OnCut)
    EVT_MENU(wxID_COPY, EffectEditorFrame::OnCopy)
    EVT_MENU(wxID_PASTE, EffectEditorFrame::OnPaste)

    EVT_UPDATE_UI(wxID_CUT, EffectEditorFrame::OnUpdateCutCopyPaste)
    EVT_UPDATE_UI(wxID_COPY, EffectEditorFrame::OnUpdateCutCopyPaste)
    EVT_UPDATE_UI(wxID_PASTE, EffectEditorFrame::OnUpdateCutCopyPaste)

    EVT_MENU(wxID_UNDO, EffectEditorFrame::OnUndo)
    EVT_MENU(wxID_REDO, EffectEditorFrame::OnRedo)

    EVT_MENU(feID_LOCK_CAMERA_HEIGHT, EffectEditorFrame::OnLockCameraHeight)
	EVT_MENU(feID_SELECTALLOBJECTS, EffectEditorFrame::OnSelectAllObjects)
    EVT_MENU(feID_SHOW_STAT_PANEL, EffectEditorFrame::OnShowStatPanel)
    EVT_MENU(feID_SHOW_LOGO_PANEL, EffectEditorFrame::OnShowLogoPanel)
    EVT_MENU(feID_PREFERSETTING, EffectEditorFrame::OnPreferSetting)
	EVT_MENU(feID_ERASE_BY_TEXTURE_NAME, EffectEditorFrame::OnEraseTextureByName)

	EVT_MENU(feID_RESET_CAMERA, EffectEditorFrame::OnResetCamera)

    EVT_MENU(feID_SHOW_STANDARD_MODEL, EffectEditorFrame::OnShowStandardModel)
    EVT_MENU(feID_DELETESTATICOBJECT,EffectEditorFrame::OnDeleteStaticObject)
    EVT_UPDATE_UI(feID_DELETESTATICOBJECT,EffectEditorFrame::OnUpdateDeleteStaticObject)

	EVT_MENU(feID_SAVEENVIROMENTINFO, EffectEditorFrame::OnSaveEnviromentInfoToFile)
	EVT_UPDATE_UI(feID_SAVEENVIROMENTINFO, EffectEditorFrame::OnUpdateSaveEnviromentInfoToFile)
	EVT_MENU(feID_LOADENVIROMENTINFO, EffectEditorFrame::OnLoadEnviromentInfoFromFile)
	EVT_UPDATE_UI(feID_LOADENVIROMENTINFO, EffectEditorFrame::OnUpdateLoadEnviromentInfoFromFile)


    EVT_MENU(feID_TOGGLE_TOOL_WINDOWS, EffectEditorFrame::OnToggleToolWindows)
    EVT_MENU(feID_FULL_SCREEN, EffectEditorFrame::OnFullScreen)
    EVT_UPDATE_UI(feID_TOGGLE_TOOL_WINDOWS, EffectEditorFrame::OnUpdateToggleToolWindows)
    EVT_UPDATE_UI(feID_FULL_SCREEN, EffectEditorFrame::OnUpdateFullScreen)


    EVT_UPDATE_UI(wxID_NEW, EffectEditorFrame::OnUpdateFileNew)
    EVT_UPDATE_UI(wxID_SAVE, EffectEditorFrame::OnUpdateSave)
    EVT_UPDATE_UI(wxID_SAVEAS, EffectEditorFrame::OnUpdateSaveAs)

    EVT_UPDATE_UI(wxID_UNDO, EffectEditorFrame::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, EffectEditorFrame::OnUpdateRedo)

    EVT_UPDATE_UI(feID_LOCK_CAMERA_HEIGHT, EffectEditorFrame::OnUpdateLockCameraHeight)
    EVT_UPDATE_UI(feID_SELECTALLOBJECTS, EffectEditorFrame::OnUpdateSelectAllObjects)
    EVT_UPDATE_UI(feID_SHOW_STAT_PANEL, EffectEditorFrame::OnUpdateShowStatPanel)
    EVT_UPDATE_UI(feID_SHOW_LOGO_PANEL, EffectEditorFrame::OnUpdateShowLogoPanel)
    EVT_UPDATE_UI(feID_SHOW_STANDARD_MODEL, EffectEditorFrame::OnUpdateShowStandardModel)


	EVT_MENU(feID_SHOW_ACTOR,EffectEditorFrame::OnHideActor)
	EVT_MENU(feId_SHOW_TARGET_ACTOR,EffectEditorFrame::OnHidTargetActor)
	EVT_MENU(feID_SHOW_GRID,EffectEditorFrame::OnHideGrid)
	EVT_MENU(feID_SHOW_AXIS,EffectEditorFrame::OnHideAxis)
	EVT_MENU(feID_SHOW_TERRAIN,EffectEditorFrame::OnHideTerrain)
	EVT_MENU(feID_SET_BACKGROUND,EffectEditorFrame::OnSelectBackgroundColour)

	EVT_UPDATE_UI(feID_SHOW_ACTOR, EffectEditorFrame::OnUpdateHideActor)
	EVT_UPDATE_UI(feId_SHOW_TARGET_ACTOR, EffectEditorFrame::OnUpdateHideTargetActor)
	EVT_UPDATE_UI(feID_SHOW_GRID, EffectEditorFrame::OnUpdateHideGrid)
	EVT_UPDATE_UI(feID_SHOW_AXIS, EffectEditorFrame::OnUpdateHideAxis)
	EVT_UPDATE_UI(feID_SHOW_TERRAIN, EffectEditorFrame::OnUpdateHideTerrain)
	
	// HACK 设置泛光效果
	EVT_TEXT_ENTER(feID_BLURAMOUNT, EffectEditorFrame::OnBlurAmountTextChanged)
	EVT_UPDATE_UI(feID_BLURAMOUNT, EffectEditorFrame::OnUpdateFloodlightingControls)
	EVT_TEXT_ENTER(feID_SHINEAMOUNT, EffectEditorFrame::OnShineAmountTextChanged)
	EVT_UPDATE_UI(feID_SHINEAMOUNT, EffectEditorFrame::OnUpdateFloodlightingControls)

    EVT_MENU_RANGE(feID_ACTION_FIRST, feID_ACTION_LAST-1, EffectEditorFrame::OnSelectAction)
    EVT_UPDATE_UI_RANGE(feID_ACTION_FIRST, feID_ACTION_LAST-1, EffectEditorFrame::OnUpdateSelectAction)


    EVT_MENU(feID_ALIGN_POSITION, EffectEditorFrame::OnAlignPosition)
    EVT_UPDATE_UI(feID_ALIGN_POSITION, EffectEditorFrame::OnUpdateAlignPosition)

    EVT_MENU(wxID_EXIT, EffectEditorFrame::OnExit)

    EVT_CLOSE(EffectEditorFrame::OnCloseWindow)
	EVT_MENU(feID_CREATE_LIGHT,EffectEditorFrame::OnCreateLight)

    EVT_MENU_RANGE(feID_POST_FILTER_FIRST, feID_POST_FILTER_LAST-1, EffectEditorFrame::OnSelectPostFilter)
    EVT_UPDATE_UI_RANGE(feID_POST_FILTER_FIRST, feID_POST_FILTER_LAST-1, EffectEditorFrame::OnUpdateSelectPostFilter)

    EVT_MENU_RANGE(feID_SHADOWTYPE_NONE, feID_SHADOWTYPE_NONE+4, EffectEditorFrame::OnSelectShadowTechnique)
    EVT_UPDATE_UI_RANGE(feID_SHADOWTYPE_NONE, feID_SHADOWTYPE_NONE+4, EffectEditorFrame::OnUpdateSelectShadowTechnique)

    EVT_MENU_RANGE(feID_TFO_NONE, feID_TFO_NONE+3, EffectEditorFrame::OnSelectDefaultTextureFiltering)
    EVT_UPDATE_UI_RANGE(feID_TFO_NONE, feID_TFO_NONE+3, EffectEditorFrame::OnUpdateSelectDefaultTextureFiltering)

    EVT_MENU_RANGE(feID_CL_LOW, feID_CL_LOW + 2, EffectEditorFrame::OnSelectSceneCreateLevel)
    EVT_UPDATE_UI_RANGE(feID_CL_LOW, feID_CL_LOW + 2, EffectEditorFrame::OnUpdateSelectSceneCreateLevel)

    EVT_MENU(feID_ENABLE_32BIT_TEXTURES,EffectEditorFrame::OnEnable32BitTextures)
	EVT_UPDATE_UI(feID_ENABLE_32BIT_TEXTURES,EffectEditorFrame::OnUpdateEnable32BitTextures)
    EVT_MENU(feID_RELOAD_TEXTURES,EffectEditorFrame::OnReloadTextures)
	EVT_UPDATE_UI(feID_RELOAD_TEXTURES,EffectEditorFrame::OnUpdateReloadTextures)
    EVT_MENU(feID_UNLOAD_USELESS_RESOURCES,EffectEditorFrame::OnUnloadUselessResources)
	EVT_UPDATE_UI(feID_UNLOAD_USELESS_RESOURCES,EffectEditorFrame::OnUpdateUnloadUselessResources)
    EVT_MENU(feID_SHOW_INDICATORS, EffectEditorFrame::OnShowAllIndicators)
    EVT_UPDATE_UI(feID_SHOW_INDICATORS, EffectEditorFrame::OnUpdateShowAllIndicators)


	EVT_MENU(feID_USE_SNAP_ROTATION, EffectEditorFrame::OnUseSnapRotation)
	EVT_UPDATE_UI(feID_USE_SNAP_ROTATION, EffectEditorFrame::OnUpdateUseSnapRotation)

	EVT_TEXT_ENTER(feID_FRAMERATE, EffectEditorFrame::OnFrameRateTextChanged)

END_EVENT_TABLE()

WX_DELEGATE_TO_CONTROL_CONTAINER(EffectEditorFrame)

static wxSize
CombineBestSize(const wxSize& a, const wxSize& b)
{
    return wxSize(wxMax(a.x, b.x), wxMax(a.y, b.y));
}

EffectEditorFrame::EffectEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style,
                                   const wxString& name)
    : wxFrame(parent, id, title, pos, size, style | wxTAB_TRAVERSAL, name)

    , mCanvas(NULL)
    , mObjectPropertyEditor(NULL)
	, mParticleSystemSelector(NULL)
	, m_EffectSelector(NULL)
	, m_SkillSelector(NULL)
	, mEffectObjectEditor(NULL)
	, mSkillObjectEditor(NULL)
	, mMaterialEditor(NULL)
	, mBulletEditor(NULL)
	, mBulletSelector(NULL)
	, mGraphDialog(NULL)
    , mLogWindow(NULL)
    , mLayout(NULL)
    , mWindowMenu(NULL)
    , mLockBrushSize(true)
    , mSpinnerValue(1)
    , mByAxis("YAxis")
    , mSegment(4)
    , mActiveSnapRotation(false)
	, m_pToolbar(NULL)
	, m_pSceneManipulator(NULL)
	, mUseRealCamera(false)
	, mUseSnapRotation(false)
    , mTextureFilterOptions(1)
    , mCreateLevelOptions(3)
	, mBottomNotebook(NULL)
	, mEditorNotebook(NULL)
	, mPropertyNotebook(NULL)
	, mActorAnimSettingEditor(NULL)
	, m_bHideActor(false)
	, m_bHideTargetActor(true)
	, m_bHideGrid(false)
	, m_bHideAxis(false)
	, m_bHideTerrain(true)
{
    m_container.SetContainerWindow(this);

	SetIcon(wxIcon(_("AMAINICON")));

    mCanvas = new FairySceneCanvas(this, wxID_ANY);

    m_container.SetDefaultItem(mCanvas);

    wxToolBar* toolbar = CreateToolBar();

	// 记录工具栏
	m_pToolbar = toolbar;
    wxStatusBar* statusbar = CreateStatusBar(2, wxSB_NORMAL);


    InitMenuBar();
    InitToolBar(toolbar);
    InitStatusBar(statusbar);

    //////////////////////////////////////////////////////////////////////////

    mLayout = new wxLayoutManager(this);
    mLayout->AddDefaultHosts();

    mLayout->SetLayout(
        wxDWF_LIVE_UPDATE | wxDWF_SPLITTER_BORDERS,
        mCanvas);

	// the wxLayoutManager can be used to handle automatically a "window menu";
	// that is, it can show/hide all the windows under its control through a menu	
    mLayout->SetWindowMenu(mWindowMenu);

    wxSize bestSize;
    wxHostInfo hostInfo;

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_BOTTOM_HOST);
	mBottomNotebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);

    wxTextCtrl* textCtrl = new wxTextCtrl(mBottomNotebook, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE);
    delete wxLog::SetActiveTarget(NULL);
    wxLogTextCtrl* logTextCtrl = new wxLogTextCtrl(textCtrl);
    wxLogChain* logChain = new wxLogChain(logTextCtrl);


	mGraphDialog = new wxGraphDialog(mBottomNotebook,wxID_ANY,
		wxDefaultPosition, wxSize(-1,180));
	mGraphDialog->SetScrollRate( 5, 5 );
	mGraphDialog->SetScrollbars( 10, 10, 50, 50 );
	mGraphDialog->SetParentFrame(this);


	//m_pPaticleDialog = new EffectObjectPropertyEditor(this,wxID_ANY,_T("粒子属性"));



	mBottomNotebook->AddPage(textCtrl,wxT("日志"));
    bestSize = textCtrl->GetBestSize();
    hostInfo.GetHost()->SetAreaSize(bestSize.y);

    mLogWindow = textCtrl;

	mBottomNotebook->AddPage(mGraphDialog,wxT("图表"));
	bestSize = mGraphDialog->GetBestSize();
	hostInfo.GetHost()->SetAreaSize(120);

	CreateDockWindow(mBottomNotebook,
		/*_("Log Window")*/wxT("图表日志"),
		wxT("图表日志窗口"),
		hostInfo);

	
    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_LEFT_HOST);

	mEditorNotebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);

	mEffectObjectEditor = new EffectObjectEditor(mEditorNotebook,wxID_ANY);
	mEffectObjectEditor->SetParentFrame(this);
	mEditorNotebook->AddPage(mEffectObjectEditor,wxT("效果"));

	mSkillObjectEditor = new SkillObjectEditor(mEditorNotebook,wxID_ANY);
	mSkillObjectEditor->SetParentFrame(this);
	mEditorNotebook->AddPage(mSkillObjectEditor,wxT("技能"));

	mMaterialEditor = new MaterialEditor(mEditorNotebook,wxID_ANY);
	mMaterialEditor->SetParentFrame(this);
	mEditorNotebook->AddPage(mMaterialEditor,wxT("材质"));

	mBulletEditor = new BulletEditor(mEditorNotebook,wxID_ANY);
	mBulletEditor->SetParentFrame(this);
	mEditorNotebook->AddPage(mBulletEditor,wxT("子弹"));

	CreateDockWindow(   mEditorNotebook,
						wxT("编辑"),
						wxT("Object Edit"),
						hostInfo
						);
	mEditorNotebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( EffectEditorFrame::OnEditorPageChanged ), NULL, this );


	mPropertyNotebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);
    
	// 添加粒子编辑区域对话框。
	mEffectObjectProperty = new EffectObjectPropertyEditor(mPropertyNotebook,wxID_ANY);
	mEffectObjectProperty->SetParentFrame(this);
	mPropertyNotebook->AddPage(mEffectObjectProperty,wxT("效果属性"));
	mObjectPropertyEditor = new ObjectPropertyEditor(mPropertyNotebook, wxID_ANY);
	mObjectPropertyEditor->SetParentFrame(this);
	mPropertyNotebook->AddPage(mObjectPropertyEditor,wxT("物体属性"));

    CreateDockWindow(mPropertyNotebook,
        wxT("属性"),
        wxT("属性编辑"),
        hostInfo);
    bestSize = CombineBestSize(bestSize, mPropertyNotebook->GetBestSize());

    hostInfo.GetHost()->SetAreaSize(180);

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_RIGHT_HOST);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);

	mParticleSystemSelector = new ParticleSystemSelector(notebook);
	mParticleSystemSelector->SetParentFrame(this);
	notebook->AddPage(mParticleSystemSelector,wxT("粒子"));

	m_EffectSelector = new EffectSelector(notebook);
	m_EffectSelector->SetParentFrame(this);
	notebook->AddPage(m_EffectSelector,wxT("效果"));

	m_SkillSelector = new SkillSelector(notebook);
	m_SkillSelector->SetParentFrame(this);
	notebook->AddPage(m_SkillSelector,wxT("技能"));

	mActorSelector = new ActorSelector(notebook);
	mActorSelector->SetParentFrame(this);
	notebook->AddPage(mActorSelector,wxT("角色"));

	m_MaterialSelector = new MaterialSelector(notebook);
	m_MaterialSelector->SetParentFrame(this);
	notebook->AddPage(m_MaterialSelector,wxT("材质"));

	mBulletSelector = new BulletSelector(notebook);
	mBulletSelector->SetParentFrame(this);
	notebook->AddPage(mBulletSelector,wxT("子弹"));

	CreateDockWindow(notebook,
        wxT("浏览"),
        wxT("浏览"),
        hostInfo);
    bestSize = notebook->GetBestSize();

	mActorAnimSettingEditor = new ActorAnimationSettingEditor(this,wxID_ANY);
	mActorAnimSettingEditor->SetParentFrame(this);

	CreateDockWindow(mActorAnimSettingEditor,
		wxT("角色设置"),
		wxT("角色设置"),
		hostInfo);


    //hostInfo.pHost->SetAreaSize(bestSize.x);
    hostInfo.GetHost()->SetAreaSize(180);

    //////////////////////////////////////////////////////////////////////////

    this->Connect(GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(EffectEditorFrame::OnUpdateWindow));

    //////////////////////////////////////////////////////////////////////////

    LoadSettings(wxConfigBase::Get(false));

    m_sceneFilename.clear();

    wxLogMessage(_("%s Startup"), wxTheApp->GetAppName().c_str());


}

EffectEditorFrame::~EffectEditorFrame()
{

	mEditorNotebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( EffectEditorFrame::OnEditorPageChanged ), NULL, this );
    wxLogMessage(_("%s Shutdown"), wxTheApp->GetAppName().c_str());

    SaveSettings(wxConfigBase::Get(false));

    delete mLayout;
}
void EffectEditorFrame::OnEditorPageChanged(wxNotebookEvent& event)
{
	int iSelected = event.GetSelection();
	if(iSelected == 0)
	{
		ClearAllSelectorObject();
		HideAllEditorObject();
		mEffectObjectEditor->OnPageSelect();
	}
	else if(iSelected == 1)
	{
		ClearAllSelectorObject();
		HideAllEditorObject();
		mSkillObjectEditor->OnPageSelect();
	}
	else if(iSelected == 2)
	{
		mMaterialEditor->OnPageSelect();
	}
	event.Skip();
}
void
EffectEditorFrame::LoadSettings(wxConfigBase* cfg)
{
    wxASSERT(cfg != NULL);
    if (!cfg) return;

    cfg->SetPath(wxT("Main"));
    cfg->Read(wxT("Last Scene Directory"), &m_lastSceneDirectory, wxT("../Scene"));
    cfg->Read(wxT("Last Scene File"), &m_sceneFilename, wxEmptyString);
    cfg->SetPath(wxT(".."));
}

void
EffectEditorFrame::SaveSettings(wxConfigBase* cfg) const
{
    wxASSERT(cfg != NULL);
    if (!cfg) return;

    cfg->SetPath(wxT("Main"));
    cfg->Write(wxT("Last Scene Directory"), m_lastSceneDirectory);
    cfg->Write(wxT("Last Scene File"), m_sceneFilename);
    cfg->SetPath(wxT(".."));
}

void
EffectEditorFrame::CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo)
{
    wxDockWindow *dockWindow = new wxDockWindow(this, wxID_ANY,
        title,
        wxDefaultPosition, wxDefaultSize,
        name);
    dockWindow->SetClient(dockClient);
    mLayout->AddDockWindow(dockWindow);
    mLayout->DockWindow(dockWindow, hostInfo);
}

void
EffectEditorFrame::InitMenuBar(void)
{
    //// Make a menubar
    wxMenuBar *menu_bar = new wxMenuBar;
#ifdef __WXMAC__
    wxMenuBar::MacSetCommonMenuBar(menu_bar);
#endif //def __WXMAC__
    //// Associate the menu bar with the frame
    SetMenuBar(menu_bar);

    //// Make file menu
    wxMenu *file_menu = new wxMenu;
    menu_bar->Append(file_menu, /*_("&File")*/wxT("[&F]文件"));
    file_menu->Append(wxID_NEW, wxGetStockLabel(wxID_NEW, true, wxT("Ctrl-N")));
    file_menu->Append(wxID_OPEN, wxGetStockLabel(wxID_OPEN, true, wxT("Ctrl-L")));
    file_menu->Append(wxID_SAVE, wxGetStockLabel(wxID_SAVE, true, wxT("Ctrl-S")));
    file_menu->Append(wxID_SAVEAS, wxGetStockLabel(wxID_SAVEAS));

	file_menu->AppendSeparator();


	//file_menu->Append(feID_SAVEENVIROMENTINFO,_("Save scene enviroment infomation"),
	file_menu->Append(feID_SAVEENVIROMENTINFO,_("保存场景环境信息"),
		_("Save Save scene enviroment infomation to a file,incluing two main lights,enviroment light,fog."));
	//file_menu->Append(feID_LOADENVIROMENTINFO,_("Load scene enviroment infomation"),
	file_menu->Append(feID_LOADENVIROMENTINFO,_("读取场景环境信息"),
		_("Save Save scene enviroment infomation to a file,incluing two main lights,enviroment light,fog."));

    file_menu->AppendSeparator();

    file_menu->AppendSeparator();

    file_menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT, true, wxT("Alt-X")));

    wxMenu* edit_menu = new wxMenu;
    menu_bar->Append(edit_menu, /*_("&Edit")*/_("[&E]编辑"));
    edit_menu->Append(wxID_UNDO, wxGetStockLabel(wxID_UNDO, true, wxT("Ctrl-Z")));
    edit_menu->Append(wxID_REDO, wxGetStockLabel(wxID_REDO, true, wxT("Ctrl-Y")));
    edit_menu->AppendSeparator();
    edit_menu->Append(wxID_CUT, wxGetStockLabel(wxID_CUT, true, wxT("Ctrl-X")));
    edit_menu->Append(wxID_COPY, wxGetStockLabel(wxID_COPY, true, wxT("Ctrl-C")));
    edit_menu->Append(wxID_PASTE, wxGetStockLabel(wxID_PASTE, true, wxT("Ctrl-V")));
    edit_menu->AppendSeparator();
    edit_menu->Append(feID_DELETESTATICOBJECT,
        //_("&Delete All staticobject and actor"),
        //_("Delete All staticobject and actor"));
        _("删除所有静态物体（包括actor）"),
        _("删除所有静态物体（包括actor）"));
    edit_menu->AppendCheckItem(feID_ENABLE_32BIT_TEXTURES,
        //_("Enable &32 bits textures"),
        //_("Toggle 32 or 16 bits textures"));
        _("启用32位纹理"),
        _("强制使用32或16位纹理"));
    edit_menu->Append(feID_RELOAD_TEXTURES,
        //_("Reload all te&xtures"),
        //_("Unload and reload all textures"));
        _("刷新模型纹理"),
        _("重新刷新物体的纹理"));
    edit_menu->Append(feID_UNLOAD_USELESS_RESOURCES,
        //_("&Unload useless resources"),
        //_("Unload all useless resources"));
        _("卸载无用的资源"),
        _("卸载所有无用的资源"));
    edit_menu->AppendCheckItem(feID_SHOW_INDICATORS,
        //_("Show all indicators"),
        //_("Toggle showing all indicators"));
        _("显示所有指示器"),
        _("显示所有指示器"));

	wxMenu* display_menu = new wxMenu;
	menu_bar->Append(display_menu, _("[&D]显示"));
	display_menu->AppendCheckItem(feID_SHOW_ACTOR,
		wxT("隐藏角色模型"),wxT("隐藏角色模型"));

	display_menu->AppendCheckItem(feId_SHOW_TARGET_ACTOR,
		wxT("隐藏目标角色模型"),wxT("隐藏目标角色模型"));

	display_menu->AppendCheckItem(feID_SHOW_GRID,
		wxT("隐藏坐标网格"),wxT("隐藏坐标网格"));

	display_menu->AppendCheckItem(feID_SHOW_AXIS,
		wxT("隐藏坐标轴"),wxT("隐藏坐标轴"));

	display_menu->AppendCheckItem(feID_SHOW_TERRAIN,
		wxT("隐藏地形"),wxT("隐藏地形"));

	display_menu->Append(feID_SET_BACKGROUND, wxT("设置背景颜色"),wxT("设置背景颜色"));
	

	wxMenu* options_menu = new wxMenu;

    wxMenu* tools_menu = new wxMenu;
    menu_bar->Append(tools_menu, _("[&T]工具"));


    tools_menu->AppendCheckItem(feID_MANIP_OBJECT_ACTION,
        /*_("&Manip Object"),*/_("[&M]摆放物体"),
        /*_("Select, delete, placement objects"));*/_("选择、删除、摆放物体"));

    tools_menu->AppendCheckItem(feID_FLIP_DIAGONAL_ACTION,
        /*_("&Flip Grid Diagonal"),*/_("[&F]反转对角线"),
        _("Flip the diagonal of the selected grids"));

    tools_menu->AppendCheckItem(feID_DISABLE_DECAL_ACTION,
        _("Disable Decal"),
        _("disable decal of the selected grids"));


//*/
    wxMenu* shadowtype_menu = new wxMenu;
    options_menu->Append(wxID_ANY, /*_("&Dynamic shadow technique")*/_("动力学阴影技术"), shadowtype_menu);
/*/
    wxMenu* shadowtype_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_NONE,
        /*_("None"),*/_("关闭"),
        _("Disable dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_TEXTURE_ADDITIVE,
        /*_("Texture additive"),*/_("材质叠加"),
        _("Use texture additive dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_TEXTURE_MODULATIVE,
        /*_("Texture modulative"),*/_("材质方向调整"),
        _("Use texture modulative dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_STENCIL_ADDITIVE,
        /*_("Stencil additive"),*/_("模板叠加"),
        _("Use stencil additive dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_STENCIL_MODULATIVE,
        /*_("Stencil modulative"),*/_("模板方向调整"),
        _("Use stencil modulative dynamic shadows"));

/*/
    wxMenu* post_filter_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("&Post Filter"), post_filter_menu);
/*/
    wxMenu* post_filter_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    post_filter_menu->AppendCheckItem(feID_PF_HEATVISION,
        _("夜视镜效果") + wxString(wxT("\tCtrl-1")),
        _("Use heat vision post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_BLOOM,
        _("晕光效果") + wxString(wxT("\tCtrl-2")),
        _("Use bloom post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_FLOODLIGHTING,
        _("全局泛光效果") + wxString(wxT("\tCtrl-3")),
        _("Use floodlighting post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_FADE,
        _("死亡效果") + wxString(wxT("\tCtrl-4")),
        _("Use fade post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_MOTION_BLUR,
        _("拖影效果") + wxString(wxT("\tCtrl-5")),
        _("Use motion blur filter"));

    wxMenu* window_menu = new wxMenu;
    menu_bar->Append(window_menu, _("[&W]窗口"));
    window_menu->AppendCheckItem(feID_FULL_SCREEN,
        _("[&F]全屏显示\tCtrl-Enter"),
        _("切换全屏显示模式"));
    window_menu->Append(feID_TOGGLE_TOOL_WINDOWS,
        wxEmptyString);   // Will setup the correct display text on UI update
    window_menu->AppendSeparator();

    mWindowMenu = window_menu;
}

void
EffectEditorFrame::InitToolBar(wxToolBar* toolbar)
{

	toolbar->AddCheckTool(feID_SHOW_ACTOR,wxEmptyString,wxBITMAP(SETTINGACTOR),wxBITMAP(SETTINGACTOR),
		wxT("显示/隐藏角色模型"),wxT("显示/隐藏角色模型"));
	toolbar->AddCheckTool(feId_SHOW_TARGET_ACTOR,wxEmptyString,wxBITMAP(SETTINGACTOR),wxBITMAP(SETTINGACTOR),
		wxT("显示/隐藏目标角色模型"),wxT("显示/隐藏目标角色模型"));
	toolbar->AddCheckTool(feID_SHOW_GRID,wxEmptyString,wxBITMAP(GRID),wxBITMAP(GRID),
		wxT("显示/隐藏坐标网格"),wxT("显示/隐藏坐标网格"));
	toolbar->AddCheckTool(feID_SHOW_AXIS,wxEmptyString,wxBITMAP(AXIS),wxBITMAP(AXIS),
		wxT("显示/隐藏坐标轴"),wxT("显示/隐藏坐标轴"));
	toolbar->AddCheckTool(feID_SHOW_TERRAIN,wxEmptyString,wxBITMAP(TERRAIN),wxBITMAP(TERRAIN),
		wxT("显示/隐藏地形"),wxT("显示/隐藏地形"));
	toolbar->AddTool(feID_SET_BACKGROUND,
		wxBITMAP(BACKGROUND),wxBITMAP(BACKGROUND),false,
		4,wxDefaultCoord,NULL, wxT("设置背景颜色"), wxT("设置背景颜色"));

	


    // TODO
    toolbar->Realize();
}


void
EffectEditorFrame::InitStatusBar(wxStatusBar* statusbar)
{
    // TODO
}

//////////////////////////////////////////////////////////////////////////

Fairy::SceneManipulator*
EffectEditorFrame::GetSceneManipulator(void) const
{
    return mCanvas ? mCanvas->GetSceneManipulator() : NULL;
}

void
EffectEditorFrame::InitScene(Fairy::SceneManipulator* sceneManipulator)
{
    wxASSERT(mCanvas);

	assert (sceneManipulator);
    //laim22 这个地方在装载资源
	mCanvas->SetParentFrame(this);
	wxBusyInfo* busyInfo = new wxBusyInfo(_("正在生成场景 ..."), this);
    mCanvas->InitScene(sceneManipulator);
	
	sceneManipulator->newScene();

	Ogre::Entity* pAxes = sceneManipulator->getSceneManager()->getEntity("axes");
	if(pAxes)
		pAxes->setVisible(false);
	m_bHideAxis = true;

	sceneManipulator->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));

	sceneManipulator->getCamera()->setPosition(Ogre::Vector3(0,200,800));
	sceneManipulator->getCamera()->lookAt(Ogre::Vector3(0,0,0));

	mObjectPropertyEditor->SetSceneManipulator(sceneManipulator);
	mEffectObjectProperty->SetSceneManipulator(sceneManipulator);

	mEffectObjectEditor->SetSceneManipulator(sceneManipulator);
	mMaterialEditor->SetSceneManipulator(sceneManipulator);
	mActorAnimSettingEditor->SetSceneManipulator(sceneManipulator);
	mBulletEditor->SetSceneManipulator(sceneManipulator);
	mBulletEditor->SetBulletCallBack();

	mParticleSystemSelector->SetSceneManipulator(sceneManipulator);
	m_EffectSelector->SetSceneManipulator(sceneManipulator);
	m_SkillSelector->SetSceneManipulator(sceneManipulator);
	m_MaterialSelector->SetSceneManipulator(sceneManipulator);
	mActorSelector->SetSceneManipulator(sceneManipulator);
	mBulletSelector->SetSceneManipulator(sceneManipulator);
	sceneManipulator->setTerrainVisible(false);

	delete  busyInfo;
	busyInfo = new wxBusyInfo(_("正在读取粒子数据 ..."), this);
	mParticleSystemSelector->Reload();

	delete  busyInfo;
	busyInfo = new wxBusyInfo(_("正在读取效果数据 ..."), this);
	m_EffectSelector->Reload();

	delete  busyInfo;
	busyInfo = new wxBusyInfo(_("正在读取技能数据 ..."), this);
	m_SkillSelector->Reload();
	delete  busyInfo;
	busyInfo = NULL;

	//mActorSelector->Reload();
	mActorAnimSettingEditor->SetCurrentObject(Ogre::String("丐帮女.obj"));
	mActorAnimSettingEditor->SetTargetObject(Ogre::String("丐帮女.obj"));
	

	Fairy::LogicModel* pDModel = mActorAnimSettingEditor->GetTargetObject();
	if(!pDModel)
		return;
	pDModel->setVisible(false);
	pDModel->createSkill("棒_马下_站立_01");

}

bool
EffectEditorFrame::Save(void)
{     
	SaveImpl();
	return true;
}

bool
EffectEditorFrame::SaveAs(void)
{

	SaveImpl();
	return true;


}

bool 
EffectEditorFrame::SaveImpl(bool autoSave)
{
    if (autoSave)
    {
	}
    return true;
}

bool
EffectEditorFrame::SaveModified(bool force)
{
    if ((!GetSceneManipulator() || !GetSceneManipulator()->isModified()))
        return +1;

    wxString name;
    if (!m_sceneFilename.empty())
        wxFileName::SplitPath(m_sceneFilename, NULL, &name, NULL);
    else
        name = _("未命名");

    wxString msgTitle;
    if (!wxTheApp->GetAppName().empty())
        msgTitle = wxTheApp->GetAppName();
    else
        msgTitle = /*_("Warning")*/wxT("警告");

    wxString prompt;
    prompt.Printf(_("你想保存对文档%s的修改?"), name.c_str());
    int res = wxMessageBox(prompt, msgTitle,
        force ? wxYES_NO|wxICON_QUESTION : wxYES_NO|wxCANCEL|wxICON_QUESTION,
        this);
    switch (res)
    {
    default:
    case wxCANCEL:
        return false;
    case wxNO:		
        return true;
    case wxYES:		
        return Save();
    }
}

// Extend event processing to search the canvas's event table
bool
EffectEditorFrame::ProcessEvent(wxEvent& e)
{
//*/ 
    
    return wxFrame::ProcessEvent(e);
/*/
    static wxEvent *activeEvent = NULL;

    // Break recursion loops
    if (activeEvent == &e)
        return false;

    activeEvent = &e;

    bool ret;
    // Only hand down to the canvas if it's a menu command
    if (!e.IsKindOf(CLASSINFO(wxCommandEvent)) || !mCanvas || !mCanvas->ProcessEvent(e))
        ret = wxFrame::ProcessEvent(e);
    else
        ret = true;

    activeEvent = NULL;
    return ret;
//*/
}

void
EffectEditorFrame::OnExit(wxCommandEvent& e)
{
    Close();
}

void
EffectEditorFrame::OnUpdateWindow(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);

    if (GetSceneManipulator())
    {
        wxString name;
        if (!m_sceneFilename.empty())
            wxFileName::SplitPath(m_sceneFilename, NULL, &name, NULL);
        else
            name = _("20100610");//版本号

        wxString modified;
        if (GetSceneManipulator()->isModified())
            modified = _("*");

        wxString title = wxTheApp->GetAppName() + _(" - ") + name + modified;
        if (m_lastTitle != title)
        {
            m_lastTitle = title;
            e.SetText(title);
        }
    }
}

void
EffectEditorFrame::OnCloseWindow(wxCloseEvent& e)
{
    if (SaveModified(!e.CanVeto()))
        wxFrame::OnCloseWindow(e);
    else
        e.Veto();
}

void
EffectEditorFrame::OnFileNew(wxCommandEvent& e)
{
    if (!SaveModified())
        return;	

}

void
EffectEditorFrame::OnUpdateFileNew(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}


void
EffectEditorFrame::OnEnable32BitTextures(wxCommandEvent& e)
{
    Ogre::TextureManager *tm = Ogre::TextureManager::getSingletonPtr();
    if (tm)
    {
        tm->setPreferredIntegerBitDepth(tm->getPreferredIntegerBitDepth() == 16 ? 0 : 16);
    }
}

void
EffectEditorFrame::OnUpdateEnable32BitTextures(wxUpdateUIEvent& e)
{
    Ogre::TextureManager *tm = Ogre::TextureManager::getSingletonPtr();
	e.Enable(tm != 0);
    e.Check(tm && tm->getPreferredIntegerBitDepth() != 16);
}

void
EffectEditorFrame::OnReloadTextures(wxCommandEvent& e)
{
    if (Ogre::TextureManager::getSingletonPtr())
    {
        // Make sure doesn't unload render texture, which is unnecessary,
        // and will cause issues in OGL render system.
        // Now we trust that all render texture just create as manually without loader,
        // in other words, it doesn't reloadable.
        Ogre::TextureManager::getSingleton().unloadAll(true);
        // The texture will reload on demand
    }
}

void
EffectEditorFrame::OnUpdateReloadTextures(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

static void
removeUselessResources(Ogre::ResourceManager& rm)
{
    static const unsigned int usReferenceCountByResourceManager = 3;

    // Collect useless resources first because we can't remove it while
    // iterate through the resource map
    std::list<Ogre::ResourceHandle> uselessResources;

    Ogre::ResourceManager::ResourceMapIterator it = rm.getResourceIterator();
    while (it.hasMoreElements())
    {
        Ogre::ResourceHandle handle = it.peekNextKey();
        Ogre::ResourcePtr res = it.getNext();
        if (!res->isManuallyLoaded() &&
            res.useCount() <= usReferenceCountByResourceManager + 1)
        {
            uselessResources.push_back(handle);
        }
    }

    // Now remove useless resources
    std::list<Ogre::ResourceHandle>::const_iterator it2;
    for (it2 = uselessResources.begin(); it2 != uselessResources.end(); ++it2)
    {
        Ogre::ResourceHandle handle = *it2;
        rm.remove(handle);
    }
}

void
EffectEditorFrame::OnUnloadUselessResources(wxCommandEvent& e)
{
    Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
    if (rgm)
    {
        static char const * const unloadableResourceManagerNames[] =
        {
            "Font",
            "GpuProgram",
            "HighLevelGpuProgram",
            "Material",
            0,
        };

        static char const * const removableResourceManagerNames[] =
        {
            "Mesh",
            "Skeleton",
            // Texture can't remove because has exists weak reference to it,
            // comment out until we fixed all weak reference to textures.
            //"Texture",
            0,
        };

        char const * const * p;

        for (p = unloadableResourceManagerNames; *p; ++p)
        {
            rgm->_getResourceManager(*p)->unloadUnreferencedResources();
        }

        for (p = removableResourceManagerNames; *p; ++p)
        {
            removeUselessResources(*rgm->_getResourceManager(*p));
        }
    }
}

void
EffectEditorFrame::OnUpdateUnloadUselessResources(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

void
EffectEditorFrame::OnShowAllIndicators(wxCommandEvent& e)
{
    Fairy::SceneManipulator* manipulator = GetSceneManipulator();

    if (manipulator)
    {
        manipulator->setIndicatorNodeVisible(!manipulator->getIndicatorNodeVisible());
    }    
}

void
EffectEditorFrame::OnUpdateShowAllIndicators(wxUpdateUIEvent& e)
{
    Fairy::SceneManipulator* manipulator = GetSceneManipulator();

    e.Enable(manipulator != 0);
    e.Check(manipulator && manipulator->getIndicatorNodeVisible());    
}

void
EffectEditorFrame::OnUpdateSave(wxUpdateUIEvent& e)
{
    e.Enable((GetSceneManipulator() && GetSceneManipulator()->isModified()));
}

void
EffectEditorFrame::OnUpdateSaveAs(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() && !GetSceneManipulator()->isEmpty());
}

void
EffectEditorFrame::OnFileOpen(wxCommandEvent& e)
{
    if (!SaveModified())
        return;

	wxFileDialog fileDialog(this,
		_("装载场景"),
		m_lastSceneDirectory,
		m_sceneFilename,
		wxString::Format(
		_("Scene files (*.Scene)|*.Scene|Scene XML files (*.Scene.xml)|*.Scene.xml|All files (%s)|%s"),
		wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
		wxOPEN | wxFILE_MUST_EXIST);

	if (fileDialog.ShowModal() == wxID_OK)
	{
		m_sceneFilename = fileDialog.GetPath();
		m_lastSceneDirectory = wxPathOnly(m_sceneFilename);

		if (mLogWindow)
			mLogWindow->Clear();

		wxBusyInfo busyInfo(_("正在装载场景，请等待..."), this);

		GetSceneManipulator()->loadScene(AS_STRING(m_sceneFilename));      

		if (GetSceneManipulator()->hasExceptionWhenLoadScene())
		{
			ExceptionInfoDialog dlg(this, wxID_ANY,
				_("异常信息对话框"), GetSceneManipulator());

			if (dlg.ShowModal() != wxID_OK)
			{
				// 如果异常列表中仍有内容，就给出提示
				if (dlg.mListBox->GetCount() > 0)
				{
					wxMessageBox(_("场景文件中仍存在一些错误，请修复！"));
				}
				else
				{
					wxMessageBox(_("场景中的所有错误已修正，请重新保存场景使该改动生效！"));
				}
			}
		}
	}

}

void
EffectEditorFrame::OnFileSave(wxCommandEvent& e)
{
    Save();
}

void
EffectEditorFrame::OnFileSaveAs(wxCommandEvent& e)
{
    SaveAs();
}

void
EffectEditorFrame::OnUndo(wxCommandEvent& e)
{
    GetSceneManipulator()->getOperatorManager()->undo();
}

void
EffectEditorFrame::OnRedo(wxCommandEvent& e)
{
    GetSceneManipulator()->getOperatorManager()->redo();
}

void
EffectEditorFrame::OnUpdateUndo(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() &&
        !GetSceneManipulator()->getOperatorManager()->getUndoOperators().empty());
}

void
EffectEditorFrame::OnUpdateRedo(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() &&
        !GetSceneManipulator()->getOperatorManager()->getRedoOperators().empty());
}

void
EffectEditorFrame::OnLockCameraHeight(wxCommandEvent& e)
{
    GetSceneManipulator()->setCameraHeightLocked(!GetSceneManipulator()->getCameraHeightLocked());
}

void
EffectEditorFrame::OnUpdateLockCameraHeight(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getCameraHeightLocked());
}

void 
EffectEditorFrame::OnShowStandardModel(wxCommandEvent& e)
{
    GetSceneManipulator()->setShowStandardModel(!GetSceneManipulator()->getShowStandardModel());
}

void 
EffectEditorFrame::OnDeleteStaticObject(wxCommandEvent& e)
{
    int answer = wxMessageBox(_("不能删除键 '%s'"), _("确认"),
        wxYES_NO | wxCANCEL, this);
    if (answer == wxYES)
    {    
        const Fairy::Scene::Objects& mObjects = GetSceneManipulator()->getSceneInfo()->getObjects();
        std::vector<Fairy::ObjectPtr> objVec;
        for (Fairy::Scene::Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
        {
            const Fairy::ObjectPtr& object = *it;

            if (object->getCategory() == "StaticObject" || object->getCategory() == "Actor")
            {
                objVec.push_back(object);
            }
        }

        for(unsigned int i=0; i<objVec.size();i++)
        {
            GetSceneManipulator()->getSceneInfo()->removeObject(objVec[i]);
        }
        GetSceneManipulator()->_fireSceneReset();
    }
}

void 
EffectEditorFrame::OnUpdateDeleteStaticObject(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() !=0);
}

void
EffectEditorFrame::OnUpdateShowStandardModel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getShowStandardModel());
}


void 
EffectEditorFrame::OnBlurAmountTextChanged(wxCommandEvent& e)
{
	Fairy::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
	pfm->setPostFilterParameter("Floodlighting", "BlurAmount", e.GetString().c_str());
}

void 
EffectEditorFrame::OnShineAmountTextChanged(wxCommandEvent& e)
{
	Fairy::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
	pfm->setPostFilterParameter("Floodlighting", "ShineAmount", e.GetString().c_str());
}

void 
EffectEditorFrame::OnUpdateFloodlightingControls(wxUpdateUIEvent& e)
{
	Fairy::SceneManipulator *manipulator = GetSceneManipulator();
	e.Enable(
        manipulator &&
        manipulator->getPostFilterManager()->hasPostFilter("Floodlighting") &&
        manipulator->getPostFilterManager()->getPostFilterEnabled("Floodlighting"));
}



void
EffectEditorFrame::OnShowStatPanel(wxCommandEvent& e)
{
    GetSceneManipulator()->getFrameStatsListener()->showStatPanel(
        !GetSceneManipulator()->getFrameStatsListener()->isStatPanelShown());
}

void
EffectEditorFrame::OnUpdateShowStatPanel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getFrameStatsListener()->isStatPanelShown());
}

void
EffectEditorFrame::OnShowLogoPanel(wxCommandEvent& e)
{
    GetSceneManipulator()->getFrameStatsListener()->showLogoPanel(
        !GetSceneManipulator()->getFrameStatsListener()->isLogoPanelShown());
}

void
EffectEditorFrame::OnUpdateShowLogoPanel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getFrameStatsListener()->isLogoPanelShown());
}

void
EffectEditorFrame::OnToggleToolWindows(wxCommandEvent& e)
{
    bool shown = true;
    for (const DockWindowList::Node* node = mLayout->GetDockHost(wxDEFAULT_LEFT_HOST)->GetDockWindowList().GetFirst(); node; node = node->GetNext())
    {
        wxDockWindowBase* dockWindow = node->GetData();
        if (dockWindow->IsVisible())
        {
            shown = false;
            break;
        }
    }

    for (const DockWindowList::Node* node = mLayout->GetDockHost(wxDEFAULT_RIGHT_HOST)->GetDockWindowList().GetFirst(); node; node = node->GetNext())
    {
        wxDockWindowBase* dockWindow = node->GetData();
        dockWindow->Show(shown);
    }
}

void
EffectEditorFrame::OnUpdateToggleToolWindows(wxUpdateUIEvent& e)
{
    bool shown = true;
    for (const DockWindowList::Node* node = mLayout->GetDockHost(wxDEFAULT_LEFT_HOST)->GetDockWindowList().GetFirst(); node; node = node->GetNext())
    {
        wxDockWindowBase* dockWindow = node->GetData();
        if (dockWindow->IsVisible())
        {
            shown = false;
            break;
        }
    }
	for (const DockWindowList::Node* node = mLayout->GetDockHost(wxDEFAULT_RIGHT_HOST)->GetDockWindowList().GetFirst(); node; node = node->GetNext())
	{
		wxDockWindowBase* dockWindow = node->GetData();
		if (dockWindow->IsVisible())
		{
			shown = false;
			break;
		}
	}
    e.SetText((shown ? _("[&A]显示所有工具栏") : _("[&A]隐藏所有工具栏")) + wxString(wxT("\tAlt-Enter")));
}

void
EffectEditorFrame::OnFullScreen(wxCommandEvent& e)
{
    ShowFullScreen(!IsFullScreen());
}

void
EffectEditorFrame::OnUpdateFullScreen(wxUpdateUIEvent& e)
{
    e.Check(IsFullScreen());
}

static const char *gActionNames[] = {
	""
};

void
EffectEditorFrame::OnSelectAction(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_ACTION_FIRST;
    if (index >= sizeof(gActionNames) / sizeof(*gActionNames))
        return;

    if (GetSceneManipulator()->getActiveAction() &&
        GetSceneManipulator()->getActiveAction()->getName() == gActionNames[index])
        GetSceneManipulator()->setActiveAction(NULL);
    else
        GetSceneManipulator()->setActiveAction(gActionNames[index]);
}

void
EffectEditorFrame::OnUpdateSelectAction(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_ACTION_FIRST;
    if (index >= sizeof(gActionNames) / sizeof(*gActionNames))
    {
        e.Enable(false);
        return;
    }

    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getActiveAction() &&
            GetSceneManipulator()->getActiveAction()->getName() == gActionNames[index]);
}


void
EffectEditorFrame::OnSceneAnimationQuality(wxCommandEvent& e)
{
    long level = static_cast<long>(GetSceneManipulator()->getAnimationQuality()) + 50;
    long newLevel = wxGetNumberFromUser(
        _("Please input scene animation level"),
        _("动作等级:"),
        _("Set scene animation level"),
        level, 0, 100,
        this);
    if (newLevel >= 0 && newLevel != level)
    {
        GetSceneManipulator()->setAnimationQuality(static_cast<Ogre::Real>(newLevel - 50));
    }
}

void
EffectEditorFrame::OnObjectCreateLevel(wxCommandEvent& e)
{
    long level = static_cast<long>(GetSceneManipulator()->getCreateLevel());
    long newLevel = wxGetNumberFromUser(
        _("Please input object create level"),
        _("物体创建等级:"),
        _("Set object create level"),
        level, -50, 50,
        this);
    if (newLevel != level)
    {
        GetSceneManipulator()->setCreateLevel(static_cast<Ogre::Real>(newLevel));
    }
}


void
EffectEditorFrame::OnAlignPosition(wxCommandEvent& e)
{
    if (GetSceneManipulator()->getPositionAlignGranularity() > 0)
        GetSceneManipulator()->setPositionAlignGranularity(0);
    else
        GetSceneManipulator()->setPositionAlignGranularity(1);
}

void
EffectEditorFrame::OnUpdateAlignPosition(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getPositionAlignGranularity() > 0);
}

void 
EffectEditorFrame::OnFrameRateTextChanged(wxCommandEvent& e)
{
	unsigned long frameRate = Ogre::StringConverter::parseUnsignedLong(e.GetString().c_str());

	if (frameRate < 0)
		frameRate = 0;

	if (mCanvas)
		mCanvas->setTargetFPS(frameRate);
}

void 
EffectEditorFrame::OnPreferSetting(wxCommandEvent& e)
{
    SettingsDialog dialog(this);
    dialog.ShowModal();
}

void
EffectEditorFrame::OnCut(wxCommandEvent& event)
{


    Ogre::String infoString;

    Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();

    if (currentAction && currentAction->getName() == "ManipObjectAction")
    {
        GetSceneManipulator()->getSelectedObjectInfo(infoString);

        wxTextDataObject *data = new wxTextDataObject(infoString);
        wxTheClipboard->SetData(data);

        // 删除原有的物体
        GetSceneManipulator()->removeSelectedObjects();
    }    
}

void 
EffectEditorFrame::OnCopy(wxCommandEvent& event)
{
    Ogre::String infoString;

    Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();
	mInfoString.clear();

    if (currentAction)
    {
        // 在不同的操作中使用的拷贝的是不同的信息
        if (currentAction->getName() == "ManipObjectAction")
		{
			GetSceneManipulator()->getSelectedObjectInfo(infoString);
			mManipInfoString = infoString;
		}

        wxTextDataObject *data = new wxTextDataObject(infoString);
        wxTheClipboard->SetData(data);
    }   
}

void 
EffectEditorFrame::OnPaste(wxCommandEvent& event)
{
    // 得到鼠标当前与地面的交接点
    Ogre::Vector3 mousePosition;
    wxPoint currentMousePos = mCanvas->mCurrentMousePos;
  
}

void
EffectEditorFrame::OnSaveEnviromentInfoToFile(wxCommandEvent& e)
{

}

void 
EffectEditorFrame::OnUpdateSaveEnviromentInfoToFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
EffectEditorFrame::OnLoadEnviromentInfoFromFile(wxCommandEvent& e)
{
	
}

void 
EffectEditorFrame::OnUpdateLoadEnviromentInfoFromFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void 
EffectEditorFrame::OnUpdateCutCopyPaste(wxUpdateUIEvent& e)
{
    bool enable = false;

    Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();

    switch (e.GetId())
    {
    case wxID_COPY:
    case wxID_PASTE:
        enable = (currentAction && (currentAction->getName() == "ManipObjectAction"));
        break;

    case wxID_CUT:
        enable = (currentAction && currentAction->getName() == "ManipObjectAction");
        break;
    }

    enable &= (wxWindow::FindFocus() == mCanvas);

    e.Enable(enable);
}
//设置显示技能的角色模型
void EffectEditorFrame::OnHideActor(wxCommandEvent& e)
{
	m_bHideActor = e.IsChecked();
	Fairy::LogicModel* pDModel = mActorAnimSettingEditor->GetActorObject();
	if(!pDModel)
		return;
	pDModel->setVisible(!m_bHideActor);
}
void EffectEditorFrame::OnHidTargetActor(wxCommandEvent& e)
{
	m_bHideTargetActor = e.IsChecked();
	Fairy::LogicModel* pDModel = mActorAnimSettingEditor->GetTargetObject();
	if(!pDModel)
		return;
	pDModel->setVisible(!m_bHideTargetActor);
}
void EffectEditorFrame::OnHideGrid(wxCommandEvent& e)
{
	m_bHideGrid = e.IsChecked();
	Ogre::ManualObject* pGrid = GetSceneManipulator()->getSceneManager()->getManualObject("gridObject");
	if(pGrid)
		pGrid->setVisible(!m_bHideGrid);

}
//显示隐藏坐标轴
void EffectEditorFrame::OnHideAxis(wxCommandEvent& e)
{
	m_bHideAxis = e.IsChecked();
	Ogre::Entity* pAxes = GetSceneManipulator()->getSceneManager()->getEntity("axes");
	if(pAxes)
		pAxes->setVisible(!m_bHideAxis);
}
void EffectEditorFrame::OnHideTerrain(wxCommandEvent& e)
{
	m_bHideTerrain =  e.IsChecked();
	GetSceneManipulator()->setTerrainVisible(!m_bHideTerrain);
}
void EffectEditorFrame::OnSelectBackgroundColour(wxCommandEvent& e)
{
	Ogre::ColourValue backgroundColor = GetSceneManipulator()->getBackgroundColour();
	wxColor back_color;
	back_color.Set(backgroundColor.r* 255,backgroundColor.g * 255,backgroundColor.b * 255);
	wxColourData data;
	data.SetColour(back_color);
	wxColourDialog dialog(this, &data);
	wxColourData& colourData = dialog.GetColourData();
	//colourData.SetColour());
	dialog.SetTitle(wxString::Format(wxT("背景颜色 %2.2f %2.2f %2.2f"),backgroundColor.r,
		backgroundColor.g,backgroundColor.b));
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColor col = colourData.GetColour();
		GetSceneManipulator()->setBackgroundColour(Ogre::ColourValue((float)col.Red()/255.0f,
			(float)col.Green()/255.0f,(float)col.Blue()/255.0f));
	}

}
void EffectEditorFrame::OnUpdateHideActor(wxUpdateUIEvent& e)
{
	e.Check(m_bHideActor);
}
void EffectEditorFrame::OnUpdateHideTargetActor(wxUpdateUIEvent& e)
{
	e.Check(m_bHideTargetActor);
}
void EffectEditorFrame::OnUpdateHideGrid(wxUpdateUIEvent& e)
{
	e.Check(m_bHideGrid);
}
void EffectEditorFrame::OnUpdateHideAxis(wxUpdateUIEvent& e)
{
	e.Check(m_bHideAxis);
}
void EffectEditorFrame::OnUpdateHideTerrain(wxUpdateUIEvent& e)
{
	e.Check(m_bHideTerrain);
}
void
EffectEditorFrame::SetSpinnerValue(int value)
{
    mSpinnerValue = value;
}
int 
EffectEditorFrame::GetSpinnerValue()
{
    return mSpinnerValue;
}

wxString
EffectEditorFrame::GetByAxis()
{
    return mByAxis;
}
void 
EffectEditorFrame::SetByAxis(wxString& strAxis)
{
    mByAxis = strAxis;
}

void 
EffectEditorFrame::SetSnapRotationValue(int value)
{
    mSegment = value;
}

int 
EffectEditorFrame::GetSnapRotationValue()
{
    return mSegment;
}

void 
EffectEditorFrame::SetActiveSnapRotation(bool value)
{
    mActiveSnapRotation = value;
}

bool
EffectEditorFrame::GetActiveSnapRotation()
{
    return mActiveSnapRotation;
}

void 
EffectEditorFrame::OnCreateLight(wxCommandEvent& e)
{
	Fairy::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");
	assert (action);
	action->setParameter("type", "Light");

	action->setParameter("%type", "point");

	// 启动复杂物体创建action
	GetSceneManipulator()->setActiveAction(action);
}

void 
EffectEditorFrame::SetKeyboardShortcuts(void)
{
	wxAcceleratorEntry entries[4];

	// 恢复摄像机到初始高度和初始角度
	entries[2].Set(wxACCEL_SHIFT, (int) 'Q', feID_RESET_CAMERA);

	wxAcceleratorTable accel(3, entries);

	this->SetAcceleratorTable(accel);	
}

static const char * const gPostFilterTypes[] = {
    "HeatVision",
    "Bloom_",
    "Floodlighting",
    "Fade",
    "MotionBlur",
};

void
EffectEditorFrame::OnSelectPostFilter(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_POST_FILTER_FIRST;
    if (index >= sizeof(gPostFilterTypes) / sizeof(*gPostFilterTypes))
        return;

    Fairy::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
    pfm->setPostFilterEnabled(gPostFilterTypes[index], !pfm->getPostFilterEnabled(gPostFilterTypes[index]));
}

void
EffectEditorFrame::OnUpdateSelectPostFilter(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_POST_FILTER_FIRST;
    if (index >= sizeof(gPostFilterTypes) / sizeof(*gPostFilterTypes))
    {
        e.Enable(false);
        return;
    }

	Fairy::SceneManipulator *manipulator = GetSceneManipulator();
    bool enabled = manipulator &&
        manipulator->getPostFilterManager()->hasPostFilter(gPostFilterTypes[index]) &&
        manipulator->getPostFilterManager()->getPostFilter(gPostFilterTypes[index])->getCompositorInstance();
    e.Enable(enabled);
    e.Check(enabled && GetSceneManipulator()->getPostFilterManager()->getPostFilterEnabled(gPostFilterTypes[index]));
}

static const Ogre::ShadowTechnique gsShadowTechniques[] =
{
    Ogre::SHADOWTYPE_NONE,
    Ogre::SHADOWTYPE_TEXTURE_ADDITIVE,
    Ogre::SHADOWTYPE_TEXTURE_MODULATIVE,
    Ogre::SHADOWTYPE_STENCIL_ADDITIVE,
    Ogre::SHADOWTYPE_STENCIL_MODULATIVE,
};

void
EffectEditorFrame::OnSelectShadowTechnique(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_SHADOWTYPE_NONE;
    if (index >= sizeof(gsShadowTechniques) / sizeof(*gsShadowTechniques))
        return;

	Fairy::SceneManipulator* manipulator = GetSceneManipulator();
    manipulator->getSceneManager()->setShadowTechnique(gsShadowTechniques[index]);
}

void
EffectEditorFrame::OnUpdateSelectShadowTechnique(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_SHADOWTYPE_NONE;
    if (index >= sizeof(gsShadowTechniques) / sizeof(*gsShadowTechniques))
    {
        e.Enable(false);
        return;
    }

	Fairy::SceneManipulator* manipulator = GetSceneManipulator();
    e.Enable(manipulator != 0);
    e.Check(
        manipulator &&
        manipulator->getSceneManager()->getShadowTechnique() == gsShadowTechniques[index]);
}

static const Ogre::TextureFilterOptions gsTextureFilterOptions[] =
{
    Ogre::TFO_NONE,
    Ogre::TFO_BILINEAR,
    Ogre::TFO_TRILINEAR,
    Ogre::TFO_ANISOTROPIC,
};

void
EffectEditorFrame::OnSelectDefaultTextureFiltering(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_TFO_NONE;
    if (index >= sizeof(gsTextureFilterOptions) / sizeof(*gsTextureFilterOptions))
        return;

    Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
    if (mm)
    {
        mTextureFilterOptions = index;
        mm->setDefaultTextureFiltering(gsTextureFilterOptions[index]);
        mm->setDefaultAnisotropy(1 << index);
    }
}

void
EffectEditorFrame::OnUpdateSelectDefaultTextureFiltering(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_TFO_NONE;
    if (index >= sizeof(gsTextureFilterOptions) / sizeof(*gsTextureFilterOptions))
    {
        e.Enable(false);
        return;
    }

    Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
    e.Enable(mm != 0);
    e.Check(index == mTextureFilterOptions);
}

static const int gsCreateLevelOptions[] =
{
    0.0f,
    40.0f,
    50.0f,
};

void
EffectEditorFrame::OnSelectSceneCreateLevel(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_CL_LOW;
    if (index >= sizeof(gsCreateLevelOptions) / sizeof(*gsCreateLevelOptions))
        return;

    mCreateLevelOptions = index;
    GetSceneManipulator()->setCreateLevel( gsCreateLevelOptions[index] );
}

void
EffectEditorFrame::OnUpdateSelectSceneCreateLevel(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_CL_LOW;
    if (index >= sizeof(gsCreateLevelOptions) / sizeof(*gsCreateLevelOptions))
    {
        e.Enable(false);
        return;
    }
    
    e.Check(index == mCreateLevelOptions);
}


void 
EffectEditorFrame::OnUseSnapRotation(wxCommandEvent& e)
{
	mUseSnapRotation = !mUseSnapRotation;
	SetActiveSnapRotation(mUseSnapRotation);

	if (mUseSnapRotation)
	{
		int rotation = GetSnapRotationValue();
		if (rotation <= 0)
			rotation = 1;
		GetSceneManipulator()->setRotationAlignGranularity(360.0f / rotation);
	}
	else
		GetSceneManipulator()->setRotationAlignGranularity(0.0f);
}
void 
EffectEditorFrame::OnUpdateUseSnapRotation(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
	e.Check(GetActiveSnapRotation());
}



void 
EffectEditorFrame::OnSelectAllObjects(wxCommandEvent& e)
{
	GetSceneManipulator()->setSelectAllObjects(!GetSceneManipulator()->getSelectAllObjects());
}

void 
EffectEditorFrame::OnUpdateSelectAllObjects(wxUpdateUIEvent& e)
{	
	e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getSelectAllObjects());
}

void 
EffectEditorFrame::OnResetCamera(wxCommandEvent& e)
{
	if (GetSceneManipulator())
	{
		// 使摄像机恢复到初始的高度及角度，摄像机的xz位置是启用这个命令时摄像机的视线与地面的相交点
		Ogre::Vector3 position;

		GetSceneManipulator()->setRealCameraHeight(GetSceneManipulator()->getDefaultRealCameraHeight());
		GetSceneManipulator()->setRealCameraAngle(GetSceneManipulator()->getDefaultRealCameraAngle());

		//GetSceneManipulator()->setRealCamera(position.x, position.z);
	}
}

void 
EffectEditorFrame::OnEraseTextureByName(wxCommandEvent& e)
{	

}


//清理所有正在编辑的效果物体
void EffectEditorFrame::HideAllEditorObject()
{
	mEffectObjectEditor->DestroyEffectObject();
	mSkillObjectEditor->DestroySkillObject();

}
//清理所有选择器浏览的效果物体
void EffectEditorFrame::ClearAllSelectorObject()
{
	mParticleSystemSelector->DestroySelectObject();
	m_EffectSelector->DestroySelectObject();

}