// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXEditorFrame.h"
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

// copy paste
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
//#include "BrushSelector.h"

#include "ExceptionInfoDialog.h"


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

    feID_PROPERTIES_VIEWER = feID_BASE + 100,       // wxPropertyGridManager request id greater than 0
    feID_LOCK_BRUSH_SIZE,
    feID_BRUSH_SIZE_X,
    feID_BRUSH_SIZE_Z,
 
    feID_TERRAIN_HEIGHT,
	feID_TERRAIN_HEIGHT_INCREMENT,
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
    feID_SET_TERRAIN_HEIGHT_ACTION,
    feID_TERRAIN_LAYER_ONE_ERASER_ACTION,
    feID_MANIP_OBJECT_ACTION,
    feID_FLIP_DIAGONAL_ACTION,
    feID_DISABLE_DECAL_ACTION,
    feID_TERRAIN_SELECTION_ACTION,	
    feID_ACTION_LAST,

	feID_ERASE_BY_TEXTURE_NAME,

    feID_ENTIRE_TERRAIN_ENABLE_DECAL,

	feID_TERRAIN_LIGHTMAP_OPTION,		// 先入驻这里，找到更好的位置再挪过去
	feID_TERRAIN_LIGHTMAP_HIDE,
	feID_TERRAIN_LIGHTMAP_LOW_QUALITY,
	feID_TERRAIN_LIGHTMAP_HIGH_QUALITY,

    feID_TERRAIN_SURFACE_PARAMETERS,
    feID_SCENE_ANIMATION_QUALITY,
    feID_OBJECT_CREATE_LEVEL,

    feID_CREATE_MINI_MAP,

	feID_BOX_BRUSH,
	feID_ELLIPSE_BRUSH,

    feID_ALIGN_POSITION,

    feID_PREFERSETTING,
	feID_GPU_PROGRAM_PARAMETER_SETTING,

    feID_SHOW_STANDARD_MODEL,
    feID_DELETESTATICOBJECT,
    feID_SAVETERRAININFO,
    feID_LOADTERRAININFO,
	feID_SAVEMANIPINFO,
	feID_LOADMANIPINFO,
	feID_SAVEENVIROMENTINFO,
	feID_LOADENVIROMENTINFO,

    feID_SCENE_INFOMATION,
    
	feID_CREATE_LIQUID,
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

	feID_RESHAPE_TERRAIN,
    feID_ENABLE_32BIT_TEXTURES,
    feID_RELOAD_TEXTURES,
    feID_UNLOAD_USELESS_RESOURCES,
    feID_SHOW_INDICATORS,

	// 显示和跳转游戏逻辑坐标
	feID_COORDINATE_FIRST   = feID_BASE + 1000,
	feID_STATIC_SHOW_X,
	feID_STATIC_SHOW_Y,
	feID_SHOW_X,
	feID_SHOW_Y,
	feID_STATIC_GOTO_X,
	feID_STATIC_GOTO_Y,
	feID_GOTO_X,
	feID_GOTO_Y,
	feID_GOTO_BN,


	feID_USE_GAME_CAMERA,
	feID_USE_SNAP_ROTATION,
	feID_NOTE_BOOK,
	feID_FILE_NOTE_BOOK,
	feID_FILE_OPEN,
	feID_FILE_SAVE,
	feID_OBJ_SAVE,
	feID_RES_UPDATE,

	feID_SHOW_SHORTCUT_KEYS,
	feID_SHOW_GRID,
	feID_SHOW_AXIS,
	feID_SHOW_TERRAIN,
	feID_SHOW_BOUNDINGBOX,
	feID_ROT_FIRST,
};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(ActorEditorFrame, wxFrame)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ActorEditorFrame, wxFrame)

    WX_EVENT_TABLE_CONTROL_CONTAINER(ActorEditorFrame)
    EVT_MENU(feID_FILE_OPEN, ActorEditorFrame::OnFileOpen)
    EVT_MENU(feID_FILE_SAVE, ActorEditorFrame::OnFileSave)
	EVT_MENU(feID_OBJ_SAVE, ActorEditorFrame::OnSaveObjFile)

	EVT_UPDATE_UI(feID_FILE_OPEN, ActorEditorFrame::OnUpdateFileMenu)
	EVT_UPDATE_UI(feID_FILE_SAVE, ActorEditorFrame::OnUpdateFileMenu)
	EVT_UPDATE_UI(feID_OBJ_SAVE, ActorEditorFrame::OnUpdateFileMenu)

	EVT_MENU(feID_RES_UPDATE, ActorEditorFrame::OnResUpdate)
    EVT_MENU(wxID_EXIT, ActorEditorFrame::OnExit)
	EVT_NOTEBOOK_PAGE_CHANGED(feID_NOTE_BOOK, ActorEditorFrame::NoteBookPageChanged)
	EVT_NOTEBOOK_PAGE_CHANGED(feID_FILE_NOTE_BOOK, ActorEditorFrame::FileNoteBookPageChanged)
	EVT_MENU(feID_USE_GAME_CAMERA, ActorEditorFrame::OnUseGameCamera)
	EVT_UPDATE_UI(feID_USE_GAME_CAMERA, ActorEditorFrame::OnUpdateUseGameCamera)

	EVT_MENU(feID_SHOW_SHORTCUT_KEYS,ActorEditorFrame::OnShowShortcutKeys)
	EVT_MENU(feID_SHOW_GRID,ActorEditorFrame::OnCheckBtns)
	EVT_MENU(feID_SHOW_AXIS,ActorEditorFrame::OnCheckBtns)
	EVT_MENU(feID_SHOW_TERRAIN,ActorEditorFrame::OnCheckBtns)
	EVT_MENU(feID_SHOW_BOUNDINGBOX,ActorEditorFrame::OnCheckBtns)
	EVT_MENU(feID_ROT_FIRST,ActorEditorFrame::OnCheckBtns)

	EVT_UPDATE_UI(feID_SHOW_GRID, ActorEditorFrame::OnUpdateCheckBtns)
	EVT_UPDATE_UI(feID_SHOW_AXIS, ActorEditorFrame::OnUpdateCheckBtns)
	EVT_UPDATE_UI(feID_SHOW_TERRAIN, ActorEditorFrame::OnUpdateCheckBtns)
	EVT_UPDATE_UI(feID_SHOW_BOUNDINGBOX,ActorEditorFrame::OnUpdateCheckBtns)
	EVT_UPDATE_UI(feID_ROT_FIRST,ActorEditorFrame::OnUpdateCheckBtns)

END_EVENT_TABLE()
	WX_DELEGATE_TO_CONTROL_CONTAINER(ActorEditorFrame)

ActorEditorFrame::ActorEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style,
                                   const wxString& name)
    : wxFrame(parent, id, title, pos, size, style | wxTAB_TRAVERSAL, name)
    , m_Canvas(NULL)
	, m_pToolbar(NULL)
	, m_Layout(NULL)
	, m_WindowMenu(NULL)
	, m_LogWindow(NULL)
	, m_strlastTitle(wxEmptyString)
	, m_strlastSceneDirectory(wxEmptyString)
	, m_strsceneFilename(wxEmptyString)
	, m_pActorSelector(NULL)
	, m_pSceneManipulator(NULL)
	, m_pAnimationSelector(NULL)
	, m_strlastObjDirectory(wxEmptyString)
	, m_pDataManipulator(NULL)
	, m_pObjFileEditorPanel(NULL)
	, m_pObjSelector(NULL)
	, mUseGameCamera(false)
	, m_NotebookActor(NULL)
{
    m_container.SetContainerWindow(this);

    m_Canvas = new ActorSceneCanvas(this, wxID_ANY);

    m_container.SetDefaultItem(m_Canvas);

	SetIcon(wxIcon(_("APPICON")));

	wxToolBar* toolbar = CreateToolBar();
	// 记录工具栏
	m_pToolbar = toolbar;
    wxStatusBar* statusbar = CreateStatusBar(2, wxSB_NORMAL);

	InitMenuBar();
	InitToolBar(toolbar);

    //////////////////////////////////////////////////////////////////////////

    m_Layout = new wxLayoutManager(this);
	if(m_Layout)
	{
		m_Layout->AddDefaultHosts();

		m_Layout->SetLayout(
			wxDWF_LIVE_UPDATE | wxDWF_SPLITTER_BORDERS,
			m_Canvas);

		// the wxLayoutManager can be used to handle automatically a "window menu";
		// that is, it can show/hide all the windows under its control through a menu	
		m_Layout->SetWindowMenu(m_WindowMenu);
	}

    wxSize bestSize;
    wxHostInfo hostInfo;

    //////////////////////////////////////////////////////////////////////////
	hostInfo = m_Layout->GetDockHost(wxDEFAULT_BOTTOM_HOST);
	wxTextCtrl* ptextCtrl = new wxTextCtrl(this, wxID_ANY, /*wxEmptyString*/wxT("lantis"),
										   wxDefaultPosition, wxDefaultSize,
										   wxTE_MULTILINE);
	delete wxLog::SetActiveTarget(NULL);
    wxLogTextCtrl* plogTextCtrl = new wxLogTextCtrl(ptextCtrl);
    wxLogChain* plogChain = new wxLogChain(plogTextCtrl);
    CreateDockWindow(ptextCtrl,
        /*_("Log Window")*/wxT("日志窗口"),
        wxT("日志窗口"),
        hostInfo);
    bestSize = ptextCtrl->GetBestSize();

    hostInfo.GetHost()->SetAreaSize(bestSize.y);

    m_LogWindow = ptextCtrl;				//给日志窗口指针赋值


    hostInfo = m_Layout->GetDockHost(wxDEFAULT_RIGHT_HOST);

    wxNotebook* notebookActor = new wxNotebook(this, feID_FILE_NOTE_BOOK, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);
	
	m_pActorSelector = new ActorSelector(notebookActor);
	m_pActorSelector->SetParentFrame(this);
	notebookActor->AddPage(m_pActorSelector, wxT("mesh文件"));

	m_pObjSelector = new ObjSelector(notebookActor);
	m_pObjSelector->SetParentFrame(this);
	notebookActor->AddPage(m_pObjSelector,wxT("obj文件"));

	notebookActor->SetSelection(1);

	hostInfo.GetHost()->SetAreaSize(200);

	CreateDockWindow(notebookActor,
        wxT("文件选择"),
        wxT("选择"),
        hostInfo);
    bestSize = notebookActor->GetBestSize();

    hostInfo = m_Layout->GetDockHost(wxDEFAULT_LEFT_HOST);

    m_NotebookActor = new wxNotebook(this, feID_NOTE_BOOK, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);
	
	m_pAnimationSelector = new AnimationSelector(m_NotebookActor);
	if(m_pAnimationSelector)
		m_NotebookActor->AddPage(m_pAnimationSelector, wxT("动画信息"));

	m_pObjFileEditorPanel = new ObjFileEditorPanel(m_NotebookActor);
	if(m_pObjFileEditorPanel)
		m_NotebookActor->AddPage(m_pObjFileEditorPanel,wxT("新的obj文件编辑"));

	hostInfo.GetHost()->SetAreaSize(260);
	CreateDockWindow(m_NotebookActor,
        wxT("工作区"),
        wxT("工作"),
        hostInfo);

	LoadSettings(wxConfigBase::Get(false));

	m_strsceneFilename.clear();

	wxLogMessage(_("%s Startup"), wxTheApp->GetAppName().c_str());
}

ActorEditorFrame::~ActorEditorFrame()
{
    SaveSettings(wxConfigBase::Get(false));

	if(m_pDataManipulator)
	{
		delete m_pDataManipulator;
		m_pDataManipulator = NULL;
	}

	if(m_pObjFileEditorPanel)
	{
		delete m_pObjFileEditorPanel;
		m_pObjFileEditorPanel = NULL;
	}
}

void ActorEditorFrame::InitMenuBar()
{
    //// Make a menubar
	wxMenuBar *menu_bar = new wxMenuBar;
    //// Associate the menu bar with the frame
    SetMenuBar(menu_bar);

    //// Make file menu
    wxMenu *file_menu = new wxMenu;
    menu_bar->Append(file_menu, /*_("&File")*/wxT("[&F]文件"));
    //file_menu->Append(wxID_NEW, wxGetStockLabel(wxID_NEW, true, wxT("Ctrl-N")),wxT("新建"));//新建
    file_menu->Append(feID_FILE_OPEN, _("&Open\tCtrl+O"),_("Open an obj file"));					//打开
	file_menu->Append(feID_FILE_SAVE,_("&Save\tCtrl+S"),_("Save current obj file"));				//保存
    file_menu->Append(feID_OBJ_SAVE,_("Save &As\tCtrl+A"),_("Save current obj file as..."));		//另存为
	file_menu->Append(feID_RES_UPDATE,_("&Update\tF5"),_("Update the resourses"));					//刷新
	file_menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT));										//退出

    //wxMenu* edit_menu = new wxMenu;
    //menu_bar->Append(edit_menu, /*_("&Edit")*/_("[&E]编辑"));
    //edit_menu->Append(wxID_UNDO, wxGetStockLabel(wxID_UNDO, true, wxT("Ctrl-Z")));
    //edit_menu->Append(wxID_REDO, wxGetStockLabel(wxID_REDO, true, wxT("Ctrl-Y")));
    //edit_menu->AppendSeparator();
    //edit_menu->Append(wxID_CUT, wxGetStockLabel(wxID_CUT, true, wxT("Ctrl-X")));
    //edit_menu->Append(wxID_COPY, wxGetStockLabel(wxID_COPY, true, wxT("Ctrl-C")));
    //edit_menu->Append(wxID_PASTE, wxGetStockLabel(wxID_PASTE, true, wxT("Ctrl-V")));

    wxMenu* window_menu = new wxMenu;
    menu_bar->Append(window_menu, _("[&W]窗口"));
	//window_menu->AppendCheckItem(feID_FULL_SCREEN,
	//    _("[&F]全屏显示\tCtrl-Enter"),
	//    _("切换全屏显示模式"));
	//window_menu->Append(feID_TOGGLE_TOOL_WINDOWS,
	//    wxEmptyString);   // Will setup the correct display text on UI update
	//window_menu->AppendCheckItem(feID_USE_GAME_CAMERA,_("使用游戏相机"));
	//window_menu->AppendSeparator();
	
	//// Make help menu
	wxMenu *help_menu = new wxMenu;
	help_menu->Append(wxID_ABOUT,/* _("&About\tF1")*/wxT("[&A]关于"));
	// For shortcut
	help_menu->Append(feID_SHOW_SHORTCUT_KEYS, _("[&K]快捷键\tCtrl+K"));
	menu_bar->Append(help_menu,/* _("&Help")*/wxT("[&H]帮助"));

	m_WindowMenu = window_menu;
}

void ActorEditorFrame::InitToolBar(wxToolBar* toolbar)
{	
	toolbar->AddCheckTool(feID_SHOW_GRID,wxEmptyString,wxBITMAP(GRID),wxBITMAP(GRID),
		wxT("隐藏坐标网格"),wxT("隐藏坐标网格"));

	toolbar->AddCheckTool(feID_SHOW_AXIS,wxEmptyString,wxBITMAP(AXIS),wxBITMAP(AXIS),
		wxT("隐藏偏移操作器"),wxT("隐藏偏移操作器"));
	toolbar->AddCheckTool(feID_SHOW_TERRAIN,wxEmptyString,wxBITMAP(TERRAIN),wxBITMAP(TERRAIN),
		wxT("隐藏地形"),wxT("隐藏地形"));
	toolbar->AddCheckTool(feID_SHOW_BOUNDINGBOX,wxEmptyString,wxBITMAP(BOUNDINGBOX),wxBITMAP(BOUNDINGBOX),
	wxT("隐藏包围盒"),wxT("隐藏包围盒"));
	toolbar->AddCheckTool(feID_ROT_FIRST,wxEmptyString,wxBITMAP(ROTFIRST),wxBITMAP(ROTFIRST),
			wxT("旋转优先"),wxT("旋转优先"));
	toolbar->Realize();
}

void ActorEditorFrame::InitScene(Fairy::SceneManipulator* sceneManipulator)
{
    wxASSERT(m_Canvas);
	assert (sceneManipulator);
	m_Canvas->SetParentFrame(this);
    m_Canvas->InitScene(sceneManipulator);

	m_pAnimationSelector->SetSceneManipulator(sceneManipulator);	
	m_pAnimationSelector->Reload();

	m_pObjFileEditorPanel->SetSceneManipulator(sceneManipulator);

	m_pDataManipulator = new Fairy::CDataManipulator(sceneManipulator);
	if(m_pDataManipulator)
	{
		m_pDataManipulator->Build();		
		m_pDataManipulator->SetAnimationSelector(m_pAnimationSelector);
		m_pDataManipulator->SetObjFileEditorPanel(m_pObjFileEditorPanel);
		m_pDataManipulator->OnInitObjTemplate();
		m_pDataManipulator->OnInitHintsTable();
		m_pDataManipulator->OnInitMatTable();
		m_pDataManipulator->OnInitEffectTable();
		m_pDataManipulator->OnInitSkeltonTable();
		m_pDataManipulator->OnInitMeshTable();
	}

	//进行角色信息的载入
	m_pActorSelector->SetSceneManipulator(sceneManipulator);
	m_pActorSelector->Reload();

	m_pObjSelector->SetSceneManipulator(sceneManipulator);
	m_pObjSelector->Reload();

	//装载地图
	LoadScene();
}

void ActorEditorFrame::SetCurSceneLogicPos(float fx, float fz)
{
	if( !m_pToolbar ) return;

    wxTextCtrl* pTexX = (wxTextCtrl*)m_pToolbar->FindControl(feID_SHOW_X);
    if (pTexX)
    {
        pTexX->SetLabel(wxString::Format("%.2f", fx));
    }

    wxTextCtrl* pTexY = (wxTextCtrl*)m_pToolbar->FindControl(feID_SHOW_Y);
    if (pTexX)
    {
        pTexY->SetLabel(wxString::Format("%.2f", fz));
    }

}

bool ActorEditorFrame::ProcessEvent(wxEvent& e)
{
	return wxFrame::ProcessEvent(e);
	return true;
}

void ActorEditorFrame::OpenSelFile(const wxString& fileName, bool bIsMesh)
{
	if(NULL == m_pDataManipulator)
		return;

	if(!GetDataManipulator()->m_CurObjFile.empty() && GetDataManipulator()->isModified())
	{
		// wxMessage box
		if (bIsMesh)
		{
			wxMessageDialog dialog( NULL,wxT("obj文件有改动，需要保存吗?"),
			wxT("确认!"),
			wxYES_DEFAULT|wxYES_NO|wxICON_INFORMATION);
			switch ( dialog.ShowModal() )
			{
			case wxID_NO:
				break;
			case wxID_YES:	
				{
					SaveObjFile();
					break;
				}
			default:
				{
					SaveObjFile();
					break;
				}	
			}
		}
		else
		{
			wxMessageDialog dialog( NULL,wxT("obj文件有改动，需要保存吗?"),
				wxT("确认!"),
				wxYES_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);
			switch ( dialog.ShowModal() )
			{
			case wxID_NO:
				break;
			case wxID_YES:	
				{
					SaveObjFile();
					break;
				}			
			case wxID_CANCEL:
				return;	
			default:
				{
					SaveObjFile();
					break;
				}	
			}
		}
		
	}	

	if (!fileName.empty())
	{
		bool checkFile = Ogre::ResourceGroupManager::getSingleton().resourceExists(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,AS_STRING(fileName));
		if (!checkFile)
		{
			wxMessageBox(wxString::Format("该文件不在obj编辑器的资源目录下!"));
			return;
		}
	}	

	if (bIsMesh)
	{
		GetDataManipulator()->LoadMeshRes(AS_STRING(fileName));
		GetDataManipulator()->UpdateMeshAnimationListUI();
	}
	else
	{
		GetDataManipulator()->LoadObjTemplateRes(AS_STRING(fileName));
		GetDataManipulator()->UpdateAnimationListUI();
		GetDataManipulator()->UpdateObjFileEditDlgUI2();	
	}
}

void ActorEditorFrame::OnFileOpen(wxCommandEvent& e)
{	
	if(NULL == m_pDataManipulator)
		return;

	if(!GetDataManipulator()->m_CurObjFile.empty() && GetDataManipulator()->isModified())
	{
		// wxMessage box
		wxMessageDialog dialog( NULL,wxT("文件有改动，需要保存吗?"),
			wxT("确认!"),
			wxYES_DEFAULT|wxYES_NO|wxCANCEL|wxICON_INFORMATION);
		switch ( dialog.ShowModal() )
		{
		case wxID_NO:
			break;
		case wxID_YES:	
			{
				SaveObjFile();
				break;
			}			
		case wxID_CANCEL:
			return;
		default:
			{
				SaveObjFile();
				break;
			}	
		}
	}

	wxFileDialog fileDialog(this,
		_("Open a obj file"),
		m_strlastObjDirectory,
		wxEmptyString,
		wxString::Format(
		_("模型文件 (*.obj)|*.obj|模型 XML 文件 (*.obj.xml)|*.obj.xml|全部文件 (%s)|%s"),
		wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
		wxOPEN );

	if (fileDialog.ShowModal() == wxID_OK)
	{
		wxString filename = fileDialog.GetPath();
		m_strlastObjDirectory = wxPathOnly(filename);

		wxString pathname = filename;
		wxString name;
		wxFileName::SplitPath(pathname, NULL, &name, NULL);

		wxString objectname = name + ".obj";

// 		if (GetDataManipulator()->m_CurObjFile.compare(objectname) == 0)
// 		{
// 			return;
// 		}

		// Check the template list
		bool checkFile = Ogre::ResourceGroupManager::getSingleton().resourceExists(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,AS_STRING(objectname));
		if (!checkFile)
		{
			wxMessageBox(wxString::Format("该文件不在obj编辑器的资源目录下!"));
			return;
		}

		GetDataManipulator()->LoadObjTemplateRes(AS_STRING(objectname));
		GetDataManipulator()->UpdateAnimationListUI();
		GetDataManipulator()->UpdateObjFileEditDlgUI2();
		if (m_pObjSelector)
		{
			m_pObjSelector->selItemByName(objectname);
		}
	}	
}

void ActorEditorFrame::OnUpdateWindow(wxUpdateUIEvent& e)
{

}

void ActorEditorFrame::CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo)
{
    wxDockWindow *pDockWindow = new wxDockWindow(this, wxID_ANY,
        title,
        wxDefaultPosition, wxDefaultSize,
        name);
    pDockWindow->SetClient(dockClient);
    m_Layout->AddDockWindow(pDockWindow);
    m_Layout->DockWindow(pDockWindow, hostInfo);

}

void ActorEditorFrame::LoadSettings(wxConfigBase* cfg)
{
	wxASSERT(cfg);
	if(cfg == NULL)
		return;
	cfg->SetPath(wxT("Main"));
	cfg->Read(wxT("Last Scene Directory"),&m_strlastSceneDirectory,wxT("../Scene"));
	cfg->Read(wxT("Last Scene File"),&m_strsceneFilename,wxEmptyString);
	cfg->SetPath(wxT(".."));
}

void ActorEditorFrame::SaveSettings(wxConfigBase* cfg) const
{
	wxASSERT(cfg);
	if(cfg == NULL)
		return;
	cfg->SetPath("Main");
	cfg->Write(wxT("Last Scene Directory"),m_strlastSceneDirectory);
	cfg->Write(wxT("Last Scene File"),m_strsceneFilename);
	cfg->SetPath("..");
}

void ActorEditorFrame::SaveAvatarObjForEditor(wxCommandEvent& e)
{
}


void ActorEditorFrame::OnExit(wxCommandEvent& e)
{
    Close();
}


bool ActorEditorFrame::SaveModified(bool force)
{
	return true;
}

void ActorEditorFrame::NoteBookPageChanged(wxBookCtrlEvent& e)
{
	int sel = e.GetSelection();

	if (sel != 0)
	{
		// Stop the animation
		if (m_pAnimationSelector)
		{
			m_pAnimationSelector->pauseAnimation(true);
		}		
		m_Canvas->mCanManipulateAxis = true;
	}
	else
	{
		if (m_pAnimationSelector)
		{
			m_pAnimationSelector->pauseAnimation(false);
		}		
		m_Canvas->mCanManipulateAxis = false;
	}
}

void ActorEditorFrame::FileNoteBookPageChanged(wxBookCtrlEvent& e)
{
	int sel = e.GetSelection();
	if (sel == 0)
	{
		// Mesh file
		if (m_NotebookActor && m_NotebookActor->GetPageCount()>0)
		{
			m_NotebookActor->SetSelection(0);
		}
		if (m_pObjFileEditorPanel)
		{
			m_pObjFileEditorPanel->Enable(false);
		}
		if (m_pAnimationSelector)
		{
			m_pAnimationSelector->mIsMeshAnim = true;
			m_pAnimationSelector->Reload();
		}
		if (GetDataManipulator())
		{
			Ogre::String name = GetDataManipulator()->m_CurMeshFile;
			OpenSelFile(name.c_str(), true);
		}	
	}
	else
	{
		// obj file	
		if (m_pObjFileEditorPanel)
		{
			m_pObjFileEditorPanel->Enable(true);
		}
		if (m_pAnimationSelector)
		{
			m_pAnimationSelector->mIsMeshAnim = false;
			m_pAnimationSelector->Reload();
		}

		if (GetDataManipulator())
		{
			Ogre::String name = GetDataManipulator()->m_CurObjFile;
			OpenSelFile(name.c_str(), false);
		}		
	}
}

void ActorEditorFrame::OnUseGameCamera(wxCommandEvent& e)
{
	if(NULL == m_pSceneManipulator)
		return;

	Fairy::SceneManipulator *manipulator = m_pSceneManipulator;

	assert (manipulator);

	mUseGameCamera = !mUseGameCamera;
	manipulator->setUseRealCameraHeight(mUseGameCamera);
	manipulator->setUseRealCameraAngle(mUseGameCamera);

	if (mUseGameCamera)
	{
		manipulator->setRealCameraHeight(manipulator->getRealCameraHeight());
		manipulator->setRealCameraAngle(manipulator->getRealCameraAngle());

		Ogre::Vector3 currentCameraPos = manipulator->getCamera()->getPosition();
		manipulator->setRealCamera(currentCameraPos.x, currentCameraPos.z);
	}
}

void ActorEditorFrame::OnUpdateUseGameCamera(wxUpdateUIEvent& e)
{
	if(NULL == m_pSceneManipulator)
		return;

	Fairy::SceneManipulator *manipulator = m_pSceneManipulator;
	e.Enable(manipulator != 0);
	if (e.GetEnabled())
		e.Check(manipulator->getUseRealCameraHeight() || manipulator->getUseRealCameraAngle());
}

void ActorEditorFrame::OnShowShortcutKeys(wxCommandEvent& e)
{
	Ogre::StringVector infos;
	infos.push_back("[w或W] 相机前移\n");
	infos.push_back("[s或S] 相机后退\n");
	infos.push_back("[a或A] 相机左旋\n");
	infos.push_back("[d或D] 相机右旋\n");
	infos.push_back("[h或H] 显示/隐藏挂点偏移操作器\n");
	infos.push_back("[b或B] 显示/隐藏包围盒\n");
	infos.push_back("[t或T] 显示/隐藏地形\n");
	infos.push_back("[g或G] 显示/隐藏网格");
	infos.push_back("[r或R] 旋转是否优先");

	wxString showInfo;
	for (int i = 0; i <(int)infos.size(); ++i)
	{
		showInfo += infos[i].c_str();
	}

	wxMessageBox(showInfo, _("快捷键列表"), wxOK | wxICON_INFORMATION|wxCENTRE, this);
}

void ActorEditorFrame::OnCheckBtns(wxCommandEvent& e)
{
	int id = e.GetId();
	bool checked = e.IsChecked();
	if (id == feID_SHOW_GRID)
	{
		if (m_pSceneManipulator)
		{
			m_pSceneManipulator->setGridVisible(!m_pSceneManipulator->getGridVisible());
		}		
	}
	else if (id == feID_SHOW_AXIS)
	{
		if (m_Canvas && GetDataManipulator())
		{
			m_Canvas->mHideAxis = !m_Canvas->mHideAxis;		
		
			Fairy::CDataManipulator::m_baseNode->setVisible(!m_Canvas->mHideAxis);
			Fairy::CDataManipulator::mAxisNode_x->setVisible(!m_Canvas->mHideAxis);
			Fairy::CDataManipulator::mAxisNode_y->setVisible(!m_Canvas->mHideAxis);
			Fairy::CDataManipulator::mAxisNode_z->setVisible(!m_Canvas->mHideAxis);
		}
	}
	else if (id == feID_SHOW_TERRAIN)
	{
		if (m_pSceneManipulator)
		{
			m_pSceneManipulator->setTerrainVisible(!m_pSceneManipulator->getTerrainVisible());
		}		
	}
	else if (id == feID_ROT_FIRST)
	{
		if (m_Canvas)
		{
			m_Canvas->mRotFirst = !m_Canvas->mRotFirst;
		}
	}
	else if (id == feID_SHOW_BOUNDINGBOX)
	{
		if (GetDataManipulator())
		{
			GetDataManipulator()->switchBoundingBox(true);
		}
	}
}

void ActorEditorFrame::OnUpdateFileMenu(wxUpdateUIEvent& e)
{
	int id = e.GetId();
	if(id == feID_FILE_OPEN || id == feID_FILE_SAVE || feID_OBJ_SAVE)
	{
		if (m_pAnimationSelector)
		{
			e.Enable(!m_pAnimationSelector->mIsMeshAnim);
		}
		else
		{
			e.Enable(true);
		}
	}
}

void ActorEditorFrame::OnUpdateCheckBtns(wxUpdateUIEvent& e)
{
	int id = e.GetId();
	if (id == feID_SHOW_GRID)
	{
		if (m_pSceneManipulator)
		{
			e.Check(!m_pSceneManipulator->getGridVisible());
		}		
	}
	else if (id == feID_SHOW_AXIS)
	{
		if (m_Canvas)
		{
			e.Check(m_Canvas->mHideAxis);
		}
	}
	else if (id == feID_SHOW_TERRAIN)
	{
		if (m_pSceneManipulator)
		{
			e.Check(!m_pSceneManipulator->getTerrainVisible());
		}		
	}
	else if (id == feID_ROT_FIRST)
	{
		if (m_Canvas)
		{
			e.Check(m_Canvas->mRotFirst);
		}
	}
	else if (id == feID_SHOW_BOUNDINGBOX)
	{
		if (GetDataManipulator())
		{
			e.Check(!GetDataManipulator()->mShowWorldBoundingBox);
		}
	}
}

void SplitString(int nFloor,char* szString)
{
	size_t nNum = strlen(szString);
	int nCount = 0;
	for(size_t i=nNum;i>0;--i)
	{		
		if(szString[i] == '\\')
		{
			nCount++;
			if(nCount == nFloor)
				return;
			else
				szString[i] = '\0';
		}
		else
		{
			szString[i] = '\0';
		}
	}
	if(szString[0] != '\\')
		szString[0] = '\0';
}

void ActorEditorFrame::LoadScene()
{
    {
		//地图文件先写死
	/*	char path[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path));
		SplitString(2,path);
		strcat_s(path,sizeof(path),"resource\\scene\\xinshoucun_01\\xinshoucun_01.Scene");
		m_strsceneFilename = wxT(path);
		m_strlastSceneDirectory = wxPathOnly(m_strsceneFilename);*/

		


        if (m_LogWindow)
            m_LogWindow->Clear();

		wxBusyInfo busyInfo(_("正在装载场景，请等待..."), this);
		if(m_Canvas == NULL)
		{
			return;
		}
		else
		{
			if(m_Canvas->GetSceneManipulator())
				m_pSceneManipulator = m_Canvas->GetSceneManipulator();
		}
	
/*		char path[MAX_PATH];
		GetModuleFileName(NULL,path,sizeof(path));
		SplitString(2,path);
		strcat_s(path,sizeof(path),"resource\\brushes\\丘陵\\丘陵草地.jpg");
		Ogre::String terrainTex =path;	*/	
		Ogre::String terrainTex = "default_terrain_texture.jpg";

		m_pSceneManipulator->newScene(terrainTex);
		m_pSceneManipulator->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));

		m_pSceneManipulator->getCamera()->setPosition(Ogre::Vector3(0,180,600));
		m_pSceneManipulator->getCamera()->lookAt(Ogre::Vector3(0,0,0));
		
		//m_pSceneManipulator->loadScene(AS_STRING(m_strsceneFilename));      
		//if (m_pSceneManipulator->hasExceptionWhenLoadScene())
		//{
		//	ExceptionInfoDialog dlg(this, wxID_ANY,
  //              _("异常信息对话框"), m_pSceneManipulator);
		//
		//	if (dlg.ShowModal() != wxID_OK)
		//	{
		//		// 如果异常列表中仍有内容，就给出提示
		//		if (dlg.mListBox->GetCount() > 0)
		//		{
		//			wxMessageBox(_("场景文件中仍存在一些错误，请修复！"));
		//		}
		//		else
		//		{
		//			wxMessageBox(_("场景中的所有错误已修正，请重新保存场景使该改动生效！"));
		//		}
		//	}
		//}
	}
}

bool ActorEditorFrame::SaveCurFile()
{
 	if(!GetDataManipulator())
 		return false;
	
	Ogre::String curFileName = GetDataManipulator()->m_CurObjFile;
	if (curFileName.empty())
	{
		return false;
	}

	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		curFileName);
	
	Ogre::FileInfoList::const_iterator it = fileInfoList->begin();
	if(it == fileInfoList->end())
	{
		wxMessageBox(wxString::Format("保存失败!资源目录下没有找到该文件。"));
		return false;
	}

	Ogre::String dir = it->archive->getName()+"/";
	m_strlastObjDirectory = dir.c_str();

	Ogre::String fullName = m_strlastObjDirectory +curFileName;

	bool bResult = GetDataManipulator()->SaveToObj(AS_STRING(fullName));

	if(bResult)
	{
		wxMessageBox(wxString::Format("保存文件<%s>成功!",fullName.c_str()));

		GetDataManipulator()->setModified(false);
	}
	else
	{
		wxMessageBox(wxString::Format("保存失败!"));
	}
	return true;
}

void ActorEditorFrame::OnFileSave(wxCommandEvent& e)
{
	SaveCurFile();
}

void ActorEditorFrame::OnSaveObjFile(wxCommandEvent& e)
{
	SaveObjFile();
}

void ActorEditorFrame::OnResUpdate(wxCommandEvent& e)
{
	//Reload the obj list and update
	wxBusyInfo busyinfo("刷新中，请等待...");

	if (GetDataManipulator())
	{
		GetDataManipulator()->UpdateAllResource();
	}
	if ( m_pObjSelector)
	{
		m_pObjSelector->Reload();
		wxString curFile = GetDataManipulator()->m_CurObjFile.c_str();
		m_pObjSelector->selItemByName(curFile);
	}
	if ( m_pActorSelector)
	{
		m_pActorSelector->Reload();
	}
}

bool ActorEditorFrame::SaveObjFile()
{
	if(NULL == m_pDataManipulator)
		return false;

	Fairy::LogicModel* pObj = GetDataManipulator()->m_pObjTemplate;

 	if(NULL == pObj)
 	{
 		return false;
 	}
	 
 	wxString objectname;
 	objectname = GetDataManipulator()->m_CurObjFile.c_str();
 
 	wxFileDialog fileDialog(this,
 		_("保存模型"),
 		m_strlastObjDirectory,
 		objectname,
 		wxString::Format(
 		_("模型文件 (*.obj)|*.obj|模型 XML 文件 (*.obj.xml)|*.obj.xml|全部文件 (%s)|%s"),
 		wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
 		wxSAVE | wxOVERWRITE_PROMPT);
 
 	if (fileDialog.ShowModal() != wxID_OK)
 		return false;
 
 	wxString filename;
 
 	filename = fileDialog.GetPath();
 
 	m_strlastObjDirectory = wxPathOnly(filename);	
 
 	bool bResult = GetDataManipulator()->SaveToObj(AS_STRING(filename));
 
 	if(bResult)
 	{
		wxMessageBox(wxString::Format("保存文件<%s>成功!",filename.c_str()));

		GetDataManipulator()->setModified(false);
		
  		wxString pathname = filename;
  		wxString name;
  		wxFileName::SplitPath(pathname, NULL, &name, NULL);
  		wxString newObjectname = name + ".obj";
  		if (newObjectname != objectname)
  		{
  			// Reload the obj file
  			GetDataManipulator()->LoadObjTemplateRes(AS_STRING(newObjectname));
			GetDataManipulator()->UpdateAnimationListUI();
  			GetDataManipulator()->UpdateObjFileEditDlgUI2();
			if (m_pObjSelector)
			{
				m_pObjSelector->selItemByName(objectname);
			}
  		}
 	}
 	else
 	{
 		wxMessageBox(wxString::Format("保存失败!"));
 	}
	return true;
}
