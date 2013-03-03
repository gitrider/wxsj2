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
#include "BrushSelector.h"
#include "ModelSelector.h"
#include "ActorSelector.h"

#include ".\RegioneditdlgSelector.h"		// ������������༭�Ի���.
#include ".\MonsterEditDlg.h"				// ����༭�Ի���.
#include ".\GrowPointDlg.h"					// ������༭.
#include ".\NpcPatrolDlg.h"					// npcѲ�߱༭.
#include ".\CEventAreaEditDlg.h"			// �¼�����
#include ".\stallinfodlg.h"					// ̯λ��Ϣ
#include ".\PathFindDlg.h"

#include "ParticleSystemSelector.h"
#include "SceneBrowser.h"
#include "ObjectPropertyEditor.h"
#include "TerrainSelections.h"
#include "LiquidCreatorDialog.h"
#include "ObjectArrayCreatorDialog.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"
#include "Action.h"
#include "NewSceneDialog.h"
#include "SceneInfoMapDialog.h"
#include "ExceptionInfoDialog.h"
//#include "SpinnerSettingDialog.h"
#include "SettingDialogs.h"
#include "TerrainLightmapOptionDialog.h"
#include "ReshapeDialog.h"

#include "EraseTexDialog.h"
#include "ui/res/EraseTexDialog_wdr.h"

#include "MonsterEditAction.h"		// ����༭action
#include "GrowPointEditaction.h"		// ������action
#include "PaintAction.h"
#include "AutoTexPaintAction.h"
#include "SimplePaintAction.h"
#include "StallInfoAction.h"

#include "Core/FrameStatsListener.h"
#include "Core/FairySceneInfo.h"
#include "Core/FairyObject.h"

#include "TerrainSurfaceParametersDialog.h"
#include "MiniMapMakerDialog.h"
#include "SceneInfomationDialog.h"
#include "CameraTrackSettingDialog.h"
#include "SoundEditDlg.h"
#include "DatuPointEditDlg.h"

#include "AutoSaveSceneController.h"

#include <PathLibInterface.h>
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

#include "UIColourConverter.h"
#include <wx/colordlg.h>

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

	// hack����ʱ���÷���Ч��
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
	feID_TERRAIN_DIFFUSE_COLOUR_ACTION,
    feID_TERRAIN_LAYER_ONE_ERASER_ACTION,
    feID_MANIP_OBJECT_ACTION,
    feID_FLIP_DIAGONAL_ACTION,
    feID_DISABLE_DECAL_ACTION,
    feID_TERRAIN_SELECTION_ACTION,	
    feID_ACTION_LAST,

	feID_ERASE_BY_TEXTURE_NAME,

    feID_ENTIRE_TERRAIN_ENABLE_DECAL,

	feID_TERRAIN_LIGHTMAP_OPTION,		// ����פ����ҵ����õ�λ����Ų��ȥ
	feID_TERRAIN_LIGHTMAP_HIDE,
	feID_TERRAIN_LIGHTMAP_LOW_QUALITY,
	feID_TERRAIN_LIGHTMAP_HIGH_QUALITY,

    feID_TERRAIN_SURFACE_PARAMETERS,
    feID_SCENE_ANIMATION_QUALITY,
    feID_OBJECT_CREATE_LEVEL,

    feID_CREATE_MINI_MAP,

	//For get info from file
	feID_CONVERT_CBRUSH_INFO,

	feID_BOX_BRUSH,
	feID_ELLIPSE_BRUSH,

    feID_ALIGN_POSITION,

    feID_PREFERSETTING,

    feID_SHOW_STANDARD_MODEL,
    feID_DELETESTATICOBJECT,
    feID_SAVETERRAININFO,
    feID_LOADTERRAININFO,
	feID_SAVEMANIPINFO,
	feID_LOADMANIPINFO,
	feID_SAVEENVIROMENTINFO,
	feID_LOADENVIROMENTINFO,

	feID_IMPORT_HEIGHT_MAP,
	feID_EXPORT_HEIGHT_MAP,

    feID_SCENE_INFOMATION,
    
	feID_CREATE_LIQUID,
	feID_CREATE_LIGHT,
	feID_CREATE_OBJECT_ARRAY,
	
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

	// ��ʾ����ת��Ϸ�߼�����
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


	feID_USE_REAL_CAMERA,
	feID_USE_SNAP_ROTATION,

};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(FairyEditorFrame, wxFrame)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(FairyEditorFrame, wxFrame)
    WX_EVENT_TABLE_CONTROL_CONTAINER(FairyEditorFrame)

    EVT_MENU(wxID_NEW, FairyEditorFrame::OnFileNew)

    EVT_MENU(wxID_OPEN, FairyEditorFrame::OnFileOpen)
    EVT_MENU(wxID_SAVE, FairyEditorFrame::OnFileSave)
    EVT_MENU(wxID_SAVEAS, FairyEditorFrame::OnFileSaveAs)

    EVT_MENU(wxID_CUT, FairyEditorFrame::OnCut)
    EVT_MENU(wxID_COPY, FairyEditorFrame::OnCopy)
    EVT_MENU(wxID_PASTE, FairyEditorFrame::OnPaste)

    EVT_UPDATE_UI(wxID_CUT, FairyEditorFrame::OnUpdateCutCopyPaste)
    EVT_UPDATE_UI(wxID_COPY, FairyEditorFrame::OnUpdateCutCopyPaste)
    EVT_UPDATE_UI(wxID_PASTE, FairyEditorFrame::OnUpdateCutCopyPaste)

    EVT_MENU(wxID_UNDO, FairyEditorFrame::OnUndo)
    EVT_MENU(wxID_REDO, FairyEditorFrame::OnRedo)

    EVT_MENU(feID_LOCK_CAMERA_HEIGHT, FairyEditorFrame::OnLockCameraHeight)
	EVT_MENU(feID_SELECTALLOBJECTS, FairyEditorFrame::OnSelectAllObjects)
    EVT_MENU(feID_SHOW_STAT_PANEL, FairyEditorFrame::OnShowStatPanel)
    EVT_MENU(feID_SHOW_LOGO_PANEL, FairyEditorFrame::OnShowLogoPanel)
    EVT_MENU(feID_PREFERSETTING, FairyEditorFrame::OnPreferSetting)
	EVT_MENU(feID_ERASE_BY_TEXTURE_NAME, FairyEditorFrame::OnEraseTextureByName)

    EVT_MENU(feID_ENTIRE_TERRAIN_ENABLE_DECAL, FairyEditorFrame::OnEntireTerrainEnableDecal)

	EVT_MENU(feID_RESET_CAMERA, FairyEditorFrame::OnResetCamera)

    EVT_MENU(feID_SHOW_STANDARD_MODEL, FairyEditorFrame::OnShowStandardModel)
    EVT_MENU(feID_DELETESTATICOBJECT,FairyEditorFrame::OnDeleteStaticObject)
    EVT_UPDATE_UI(feID_DELETESTATICOBJECT,FairyEditorFrame::OnUpdateDeleteStaticObject)

    EVT_MENU(feID_SAVETERRAININFO,FairyEditorFrame::OnSaveTerrainInfoToFile)
    EVT_UPDATE_UI(feID_SAVETERRAININFO,FairyEditorFrame::OnUpdateSaveTerrainInfoToFile)
    EVT_MENU(feID_LOADTERRAININFO,FairyEditorFrame::OnLoadTerrainInfoFromFile)
    EVT_UPDATE_UI(feID_LOADTERRAININFO,FairyEditorFrame::OnUpdateLoadTerrainInfoFromFile)

	EVT_MENU(feID_SAVEMANIPINFO,FairyEditorFrame::OnSaveManipInfoToFile)
	EVT_UPDATE_UI(feID_SAVEMANIPINFO,FairyEditorFrame::OnUpdateSaveManipInfoToFile)
	EVT_MENU(feID_LOADMANIPINFO,FairyEditorFrame::OnLoadManipInfoFromFile)
	EVT_UPDATE_UI(feID_LOADMANIPINFO,FairyEditorFrame::OnUpdateLoadManipInfoFromFile)

	EVT_MENU(feID_SAVEENVIROMENTINFO, FairyEditorFrame::OnSaveEnviromentInfoToFile)
	EVT_UPDATE_UI(feID_SAVEENVIROMENTINFO, FairyEditorFrame::OnUpdateSaveEnviromentInfoToFile)
	EVT_MENU(feID_LOADENVIROMENTINFO, FairyEditorFrame::OnLoadEnviromentInfoFromFile)
	EVT_UPDATE_UI(feID_LOADENVIROMENTINFO, FairyEditorFrame::OnUpdateLoadEnviromentInfoFromFile)

	EVT_MENU(feID_IMPORT_HEIGHT_MAP,FairyEditorFrame::OnImportHeightMap)
	EVT_UPDATE_UI(feID_IMPORT_HEIGHT_MAP,FairyEditorFrame::OnUpdateImportHeightMap)
	EVT_MENU(feID_EXPORT_HEIGHT_MAP,FairyEditorFrame::OnExportHeightMap)
	EVT_UPDATE_UI(feID_EXPORT_HEIGHT_MAP,FairyEditorFrame::OnUpdateExportHeightMap)

    EVT_MENU(feID_SCENE_INFOMATION, FairyEditorFrame::OnSceneInfomation)

    EVT_MENU(feID_TOGGLE_TOOL_WINDOWS, FairyEditorFrame::OnToggleToolWindows)
    EVT_MENU(feID_FULL_SCREEN, FairyEditorFrame::OnFullScreen)
    EVT_UPDATE_UI(feID_TOGGLE_TOOL_WINDOWS, FairyEditorFrame::OnUpdateToggleToolWindows)
    EVT_UPDATE_UI(feID_FULL_SCREEN, FairyEditorFrame::OnUpdateFullScreen)


    EVT_UPDATE_UI(wxID_NEW, FairyEditorFrame::OnUpdateFileNew)
    EVT_UPDATE_UI(wxID_SAVE, FairyEditorFrame::OnUpdateSave)
    EVT_UPDATE_UI(wxID_SAVEAS, FairyEditorFrame::OnUpdateSaveAs)

    EVT_UPDATE_UI(wxID_UNDO, FairyEditorFrame::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, FairyEditorFrame::OnUpdateRedo)

    EVT_UPDATE_UI(feID_LOCK_CAMERA_HEIGHT, FairyEditorFrame::OnUpdateLockCameraHeight)
    EVT_UPDATE_UI(feID_SELECTALLOBJECTS, FairyEditorFrame::OnUpdateSelectAllObjects)
    EVT_UPDATE_UI(feID_SHOW_STAT_PANEL, FairyEditorFrame::OnUpdateShowStatPanel)
    EVT_UPDATE_UI(feID_SHOW_LOGO_PANEL, FairyEditorFrame::OnUpdateShowLogoPanel)
    EVT_UPDATE_UI(feID_SHOW_STANDARD_MODEL, FairyEditorFrame::OnUpdateShowStandardModel)

    EVT_MENU(feID_LOCK_BRUSH_SIZE, FairyEditorFrame::OnLockBrushSize)
    EVT_UPDATE_UI(feID_LOCK_BRUSH_SIZE, FairyEditorFrame::OnUpdateLockBrushSize)
    EVT_SPINCTRL(feID_BRUSH_SIZE_X, FairyEditorFrame::OnBrushSizeChanged)
    EVT_SPINCTRL(feID_BRUSH_SIZE_Z, FairyEditorFrame::OnBrushSizeChanged)
    EVT_COMMAND(feID_BRUSH_SIZE_X, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnBrushSizeTextChanged)
    EVT_COMMAND(feID_BRUSH_SIZE_Z, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnBrushSizeTextChanged)
    EVT_UPDATE_UI(feID_BRUSH_SIZE_X, FairyEditorFrame::OnUpdateBrushSizeSpin)
    EVT_UPDATE_UI(feID_BRUSH_SIZE_Z, FairyEditorFrame::OnUpdateBrushSizeSpin)

    EVT_SPINCTRL(feID_HEIGHT_ACTION_AFFECT_RANGE, FairyEditorFrame::OnHeightActionaffectRangeChanged)
    EVT_COMMAND(feID_HEIGHT_ACTION_AFFECT_RANGE, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnHeightActionaffectRangeTextChanged)
    EVT_UPDATE_UI(feID_HEIGHT_ACTION_AFFECT_RANGE, FairyEditorFrame::OnUpdateHeightActionaffectRangeSpin)

	EVT_TEXT_ENTER(feID_HEIGHT_ACTION_EXP_VALUE, FairyEditorFrame::OnHeightActionExpValueTextChanged)
	EVT_UPDATE_UI(feID_HEIGHT_ACTION_EXP_VALUE, FairyEditorFrame::OnUpdateHeightActionExpValue)

	EVT_TEXT_ENTER(feID_HEIGHT_ADJUST_SPEED, FairyEditorFrame::OnHeightAdjustSpeedTextChanged)
	EVT_UPDATE_UI(feID_HEIGHT_ADJUST_SPEED, FairyEditorFrame::OnUpdateHeightAdjustSpeed)

	// HACK ���÷���Ч��
	EVT_TEXT_ENTER(feID_BLURAMOUNT, FairyEditorFrame::OnBlurAmountTextChanged)
	EVT_UPDATE_UI(feID_BLURAMOUNT, FairyEditorFrame::OnUpdateFloodlightingControls)
	EVT_TEXT_ENTER(feID_SHINEAMOUNT, FairyEditorFrame::OnShineAmountTextChanged)
	EVT_UPDATE_UI(feID_SHINEAMOUNT, FairyEditorFrame::OnUpdateFloodlightingControls)

    EVT_SPINCTRL(feID_TERRAIN_HEIGHT, FairyEditorFrame::OnTerrainHeightChanged)
    EVT_COMMAND(feID_TERRAIN_HEIGHT, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnTerrainHeightTextChanged)
    EVT_UPDATE_UI(feID_TERRAIN_HEIGHT, FairyEditorFrame::OnUpdateTerrainHeightSpin)

	EVT_SPINCTRL(feID_TERRAIN_HEIGHT_INCREMENT, FairyEditorFrame::OnTerrainHeightIncrementChanged)
	EVT_COMMAND(feID_TERRAIN_HEIGHT_INCREMENT, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnTerrainHeightIncrementTextChanged)
	EVT_UPDATE_UI(feID_TERRAIN_HEIGHT_INCREMENT, FairyEditorFrame::OnUpdateTerrainHeightIncrementSpin)

    EVT_MENU_RANGE(feID_ACTION_FIRST, feID_ACTION_LAST-1, FairyEditorFrame::OnSelectAction)
    EVT_UPDATE_UI_RANGE(feID_ACTION_FIRST, feID_ACTION_LAST-1, FairyEditorFrame::OnUpdateSelectAction)

	EVT_MENU(feID_TERRAIN_LIGHTMAP_OPTION, FairyEditorFrame::OnTerrainLightmapOption)
	EVT_MENU(feID_TERRAIN_LIGHTMAP_HIDE, FairyEditorFrame::OnTerrainLightmapHide)
	EVT_UPDATE_UI(feID_TERRAIN_LIGHTMAP_HIDE, FairyEditorFrame::OnUpdateTerrainLightmapHide)
	EVT_MENU(feID_TERRAIN_LIGHTMAP_LOW_QUALITY, FairyEditorFrame::OnTerrainLightmapLowQuality)
	EVT_UPDATE_UI(feID_TERRAIN_LIGHTMAP_LOW_QUALITY, FairyEditorFrame::OnUpdateTerrainLightmapLowQuality)
	EVT_MENU(feID_TERRAIN_LIGHTMAP_HIGH_QUALITY, FairyEditorFrame::OnTerrainLightmapHighQuality)	
	EVT_UPDATE_UI(feID_TERRAIN_LIGHTMAP_HIGH_QUALITY, FairyEditorFrame::OnUpdateTerrainLightmapHighQuality)

    EVT_MENU(feID_TERRAIN_SURFACE_PARAMETERS, FairyEditorFrame::OnTerrainSurfaceParameters)
    EVT_MENU(feID_SCENE_ANIMATION_QUALITY, FairyEditorFrame::OnSceneAnimationQuality)
    EVT_MENU(feID_OBJECT_CREATE_LEVEL, FairyEditorFrame::OnObjectCreateLevel)

    EVT_MENU(feID_CREATE_MINI_MAP, FairyEditorFrame::OnCreateMiniMap)
	EVT_MENU(feID_CONVERT_CBRUSH_INFO, FairyEditorFrame::OnReadcBrushInfo)

	EVT_MENU_RANGE(feID_BOX_BRUSH, feID_ELLIPSE_BRUSH, FairyEditorFrame::OnSelectBrushShape)
	EVT_UPDATE_UI_RANGE(feID_BOX_BRUSH, feID_ELLIPSE_BRUSH, FairyEditorFrame::OnUpdateSelectBrushShape)

    EVT_MENU(feID_ALIGN_POSITION, FairyEditorFrame::OnAlignPosition)
    EVT_UPDATE_UI(feID_ALIGN_POSITION, FairyEditorFrame::OnUpdateAlignPosition)

    EVT_MENU(wxID_EXIT, FairyEditorFrame::OnExit)

    EVT_CLOSE(FairyEditorFrame::OnCloseWindow)

	EVT_MENU(feID_CREATE_LIQUID,FairyEditorFrame::OnCreateLiquid)
	EVT_MENU(feID_CREATE_LIGHT,FairyEditorFrame::OnCreateLight)
	EVT_MENU(feID_CREATE_OBJECT_ARRAY,FairyEditorFrame::OnCreateObjectArray)

    EVT_MENU_RANGE(feID_POST_FILTER_FIRST, feID_POST_FILTER_LAST-1, FairyEditorFrame::OnSelectPostFilter)
    EVT_UPDATE_UI_RANGE(feID_POST_FILTER_FIRST, feID_POST_FILTER_LAST-1, FairyEditorFrame::OnUpdateSelectPostFilter)

    EVT_MENU_RANGE(feID_SHADOWTYPE_NONE, feID_SHADOWTYPE_NONE+4, FairyEditorFrame::OnSelectShadowTechnique)
    EVT_UPDATE_UI_RANGE(feID_SHADOWTYPE_NONE, feID_SHADOWTYPE_NONE+4, FairyEditorFrame::OnUpdateSelectShadowTechnique)

    EVT_MENU_RANGE(feID_TFO_NONE, feID_TFO_NONE+3, FairyEditorFrame::OnSelectDefaultTextureFiltering)
    EVT_UPDATE_UI_RANGE(feID_TFO_NONE, feID_TFO_NONE+3, FairyEditorFrame::OnUpdateSelectDefaultTextureFiltering)

    EVT_MENU_RANGE(feID_CL_LOW, feID_CL_LOW + 2, FairyEditorFrame::OnSelectSceneCreateLevel)
    EVT_UPDATE_UI_RANGE(feID_CL_LOW, feID_CL_LOW + 2, FairyEditorFrame::OnUpdateSelectSceneCreateLevel)

	EVT_MENU(feID_RESHAPE_TERRAIN,FairyEditorFrame::OnReshapeTerrain)
	EVT_UPDATE_UI(feID_RESHAPE_TERRAIN,FairyEditorFrame::OnUpdateReshapeTerrain)
    EVT_MENU(feID_ENABLE_32BIT_TEXTURES,FairyEditorFrame::OnEnable32BitTextures)
	EVT_UPDATE_UI(feID_ENABLE_32BIT_TEXTURES,FairyEditorFrame::OnUpdateEnable32BitTextures)
    EVT_MENU(feID_RELOAD_TEXTURES,FairyEditorFrame::OnReloadTextures)
	EVT_UPDATE_UI(feID_RELOAD_TEXTURES,FairyEditorFrame::OnUpdateReloadTextures)
    EVT_MENU(feID_UNLOAD_USELESS_RESOURCES,FairyEditorFrame::OnUnloadUselessResources)
	EVT_UPDATE_UI(feID_UNLOAD_USELESS_RESOURCES,FairyEditorFrame::OnUpdateUnloadUselessResources)
    EVT_MENU(feID_SHOW_INDICATORS, FairyEditorFrame::OnShowAllIndicators)
    EVT_UPDATE_UI(feID_SHOW_INDICATORS, FairyEditorFrame::OnUpdateShowAllIndicators)

	// ��ͼ��ת
	EVT_BUTTON(feID_GOTO_BN, FairyEditorFrame::OnSceneGoto)

	EVT_MENU(feID_USE_REAL_CAMERA, FairyEditorFrame::OnUseRealCamera)
	EVT_UPDATE_UI(feID_USE_REAL_CAMERA, FairyEditorFrame::OnUpdateUseRealCamera)

	EVT_MENU(feID_USE_SNAP_ROTATION, FairyEditorFrame::OnUseSnapRotation)
	EVT_UPDATE_UI(feID_USE_SNAP_ROTATION, FairyEditorFrame::OnUpdateUseSnapRotation)

	EVT_TEXT_ENTER(feID_FRAMERATE, FairyEditorFrame::OnFrameRateTextChanged)

END_EVENT_TABLE()

WX_DELEGATE_TO_CONTROL_CONTAINER(FairyEditorFrame)

static wxSize
CombineBestSize(const wxSize& a, const wxSize& b)
{
    return wxSize(wxMax(a.x, b.x), wxMax(a.y, b.y));
}

FairyEditorFrame::FairyEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style,
                                   const wxString& name)
    : wxFrame(parent, id, title, pos, size, style | wxTAB_TRAVERSAL, name)

    , mCanvas(NULL)

	, m_pRegionEditSelector(NULL)				//������������༭�Ի���.
	, m_pMonsterEditDlg(NULL)					// ����༭�Ի���
	, m_pGrowPointEditDlg(NULL)					// �����㡣
	, m_pNpcPatrolEditDlg(NULL)					// ����Ѳ��·��
	, m_pEventAreaEditDlg(NULL)					// �¼�����༭
	, m_pStallInfoDlg(NULL)						// ̯λ��Ϣ.
	, m_pPathFindEditDlg(NULL)
	, mSmartColourDialog(NULL)

    , mBrushSelector(NULL)
    , mModelSelector(NULL)
    , mActorSelector(NULL)
    , mParticleSystemSelector(NULL)
    , mSceneBrowser(NULL)
    , mObjectPropertyEditor(NULL)
    , mSceneInfoDlg(NULL)
	
    , mLogWindow(NULL)

    , mLayout(NULL)
    , mWindowMenu(NULL)

    , mLockBrushSize(true)
    , mSpinnerValue(1)
    , mByAxis("YAxis")
    , mSegment(4)
    , mActiveSnapRotation(false)

	, mIsTerrainLightmapUpdated(false)

	, m_pToolbar(NULL)
	, m_pSceneManipulator(NULL)

	, mUseRealCamera(false)
	, mUseSnapRotation(false)

    , mTextureFilterOptions(1)
    , mCreateLevelOptions(3)

    , mSceneInfomationDialog(NULL)
    , mAutoSaveSceneController(NULL)
    //, mCameraTrackSettingDialog(NULL)
    , mSoundEditDialog(NULL)
    , mDatuPointEditDialog(NULL)
{
    m_container.SetContainerWindow(this);

	SetIcon(wxIcon(_("AMAINICON")));

    mCanvas = new FairySceneCanvas(this, wxID_ANY);

    m_container.SetDefaultItem(mCanvas);

    wxToolBar* toolbar = CreateToolBar();

	// ��¼������
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

    wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE);
    delete wxLog::SetActiveTarget(NULL);
    wxLogTextCtrl* logTextCtrl = new wxLogTextCtrl(textCtrl);
    wxLogChain* logChain = new wxLogChain(logTextCtrl);
    CreateDockWindow(textCtrl,
        /*_("Log Window")*/wxT("��־����"),
        wxT("��־����"),
        hostInfo);
    bestSize = textCtrl->GetBestSize();

    hostInfo.GetHost()->SetAreaSize(bestSize.y);

    mLogWindow = textCtrl;

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_LEFT_HOST);

	wxNotebook* notebookEdit = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_TOP);
    
	// ����༭
	mSceneBrowser = new SceneBrowser(notebookEdit, wxID_ANY);
	//notebookEdit->AddPage(mSceneBrowser, _("SceneBrowser"));
	notebookEdit->AddPage(mSceneBrowser, _("���������"));

	//��Ӳ�����������༭�Ի���.
	m_pRegionEditSelector = new CRegionEditDlgSector(notebookEdit);
    //notebookEdit->AddPage(m_pRegionEditSelector, _("RegionEdit"));
	notebookEdit->AddPage(m_pRegionEditSelector, wxT("������������༭"));

	// ��ӹ���༭����Ի���
	m_pMonsterEditDlg = new CMonsterEditDlg(notebookEdit);
	//notebookEdit->AddPage(m_pMonsterEditDlg, _("MonsterEdit"));
	notebookEdit->AddPage(m_pMonsterEditDlg, wxT("����npc�༭"));


	// ���������༭����Ի���
	m_pGrowPointEditDlg = new CGrowPointEditDlg(notebookEdit);
	//notebookEdit->AddPage(m_pGrowPointEditDlg, _("GrowPointEdit"));
	notebookEdit->AddPage(m_pGrowPointEditDlg, wxT("������༭"));
	// ���ø�����
	m_pGrowPointEditDlg->SetParent(this);



	// ���ncpѲ�߱༭����Ի���
	m_pNpcPatrolEditDlg = new CNpcPatrolEditDlg(notebookEdit);
	//notebookEdit->AddPage(m_pNpcPatrolEditDlg, _("NpcpatrolEdit"));
	notebookEdit->AddPage(m_pNpcPatrolEditDlg, wxT("npcѲ��·�߱༭"));

	// �¼�����༭�Ի���
	m_pEventAreaEditDlg = new CEventAreaEditDlg(notebookEdit);
	notebookEdit->AddPage(m_pEventAreaEditDlg, wxT("�¼�����༭"));
    //notebookEdit->AddPage(m_pEventAreaEditDlg, _("EventAreaEdit"));


	// ̯λ��Ϣ�༭�Ի���
	m_pStallInfoDlg = new CStallInfoDlg(notebookEdit);
	notebookEdit->AddPage(m_pStallInfoDlg, wxT("̯λ��Ϣ�༭"));
    //notebookEdit->AddPage(m_pStallInfoDlg, _("StallInfoEdit"));

    //mCameraTrackSettingDialog = new CameraTrackSettingDialog(notebookEdit);
	//notebookEdit->AddPage(mCameraTrackSettingDialog, _("CameraTrackSetting"));
	//notebookEdit->AddPage(mCameraTrackSettingDialog, wxT("������켣����"));

	// ������Ϣ�༭�Ի���
    mSoundEditDialog = new SoundEditDialog(notebookEdit);
	notebookEdit->AddPage(mSoundEditDialog, wxT("�����༭"));
	//notebookEdit->AddPage(mSoundEditDialog, _("Sound Edit"));


	// ���Ѱ·��Ի���
	m_pPathFindEditDlg = new CPathFindEditDlg(notebookEdit);
	notebookEdit->AddPage(m_pPathFindEditDlg, wxT("Ѱ··���༭"));

	mDatuPointEditDialog = new DatuPointEditDialog(notebookEdit);
    notebookEdit->AddPage(mDatuPointEditDialog, _("Datu Point Edit"));

	CreateDockWindow(
						notebookEdit,
						/*_("Scene Objects"),*/ wxT("��������"),
						wxT("Object Edit"),
						hostInfo
						);

    bestSize = notebookEdit->GetBestSize();

    mObjectPropertyEditor = new ObjectPropertyEditor(this, wxID_ANY);
    CreateDockWindow(mObjectPropertyEditor,
        /*_("Object Properties")*/ wxT("��������"),
        wxT("�������Ա༭"),
        hostInfo);
    bestSize = CombineBestSize(bestSize, mObjectPropertyEditor->GetBestSize());

    //hostInfo.pHost->SetAreaSize(bestSize.x);
    hostInfo.GetHost()->SetAreaSize(128);

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_RIGHT_HOST);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);
	
    mModelSelector = new ModelSelector(notebook);
    notebook->AddPage(mModelSelector, /*_("Models")*/wxT("ģ��"));
    mActorSelector = new ActorSelector(notebook);
    notebook->AddPage(mActorSelector, /*_("Actors")*/wxT("����"));
    mParticleSystemSelector = new ParticleSystemSelector(notebook);
    notebook->AddPage(mParticleSystemSelector, /*_("ParticleSystems")*/wxT("����ϵͳ"));
    mBrushSelector = new BrushSelector(notebook);
    notebook->AddPage(mBrushSelector, /*_("��ˢ")*/wxT("��ˢ"));

	mSmartColourDialog = new wxSmartColourDialog(notebook);
	notebook->AddPage(mSmartColourDialog, wxT("����ˢ��ɫ"));
	CreateDockWindow(notebook,
        /*_("Selectors")*/wxT("ѡ����"),
        wxT("ѡ����"),
        hostInfo);
    bestSize = notebook->GetBestSize();

    mSceneInfoDlg = new SceneInfoMapDialog(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxCLIP_CHILDREN | wxTAB_TRAVERSAL);
    CreateDockWindow(mSceneInfoDlg,
        /*_("Navigation Picture")*/wxT("����ͼ"),
        wxT("����ͼ"),
        hostInfo);
    bestSize = CombineBestSize(bestSize, mSceneInfoDlg->GetBestSize());

    //hostInfo.pHost->SetAreaSize(bestSize.x);
    hostInfo.GetHost()->SetAreaSize(128);

    //////////////////////////////////////////////////////////////////////////

    this->Connect(GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(FairyEditorFrame::OnUpdateWindow));

    //////////////////////////////////////////////////////////////////////////

    LoadSettings(wxConfigBase::Get(false));

    m_sceneFilename.clear();

    wxLogMessage(_("%s Startup"), wxTheApp->GetAppName().c_str());


	m_pGrowPointEditDlg->m_pNavigationDlg = mSceneInfoDlg;
	// ��ȡ���������������Ϣ.
	GetBuildingCollisionInfo();
}

FairyEditorFrame::~FairyEditorFrame()
{
    delete mAutoSaveSceneController;

    if (mSceneInfomationDialog)
    {
        mSceneInfomationDialog->Destroy();
        delete mSceneInfomationDialog;
        mSceneInfomationDialog = NULL;
    }

    wxLogMessage(_("%s Shutdown"), wxTheApp->GetAppName().c_str());

    SaveSettings(wxConfigBase::Get(false));

    delete mLayout;
}

void
FairyEditorFrame::LoadSettings(wxConfigBase* cfg)
{
    wxASSERT(cfg != NULL);
    if (!cfg) return;

    cfg->SetPath(wxT("Main"));
    cfg->Read(wxT("Last Scene Directory"), &m_lastSceneDirectory, wxT("../Scene"));
    cfg->Read(wxT("Last Scene File"), &m_sceneFilename, wxEmptyString);
    cfg->SetPath(wxT(".."));
}

void
FairyEditorFrame::SaveSettings(wxConfigBase* cfg) const
{
    wxASSERT(cfg != NULL);
    if (!cfg) return;

    cfg->SetPath(wxT("Main"));
    cfg->Write(wxT("Last Scene Directory"), m_lastSceneDirectory);
    cfg->Write(wxT("Last Scene File"), m_sceneFilename);
    cfg->SetPath(wxT(".."));
}

void
FairyEditorFrame::CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo)
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
FairyEditorFrame::InitMenuBar(void)
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
    menu_bar->Append(file_menu, /*_("&File")*/wxT("[&F]�ļ�"));
    file_menu->Append(wxID_NEW, wxGetStockLabel(wxID_NEW, true, wxT("Ctrl-N")));
    file_menu->Append(wxID_OPEN, wxGetStockLabel(wxID_OPEN, true, wxT("Ctrl-L")));
    file_menu->Append(wxID_SAVE, wxGetStockLabel(wxID_SAVE, true, wxT("Ctrl-S")));
    file_menu->Append(wxID_SAVEAS, wxGetStockLabel(wxID_SAVEAS));

	file_menu->AppendSeparator();

	file_menu->Append(feID_SAVETERRAININFO,_("���泡����Ϣ\tCtrl-T"),_("�ѳ�����Ϣ���浽�ļ�"));
	//file_menu->Append(feID_SAVETERRAININFO,_("Save &TerrainInfo\tCtrl-T"),_("Save terrainInfo to file"));
	file_menu->Append(feID_LOADTERRAININFO,_("��ȡ������Ϣ\tCtrl-R"),_("���ļ���ȡ������Ϣ"));
	//file_menu->Append(feID_LOADTERRAININFO,_("Load &TerrainInfo\tCtrl-R"),_("Load terrainInfo from file"));
	file_menu->AppendSeparator();

	file_menu->Append(feID_SAVEMANIPINFO,_("���������Ϣ\tCtrl-M"),_("Save manipObjInfo to file"));
	//file_menu->Append(feID_SAVEMANIPINFO,_("Save &ManipObjInfo\tCtrl-M"),_("Save manipObjInfo to file"));
	file_menu->Append(feID_LOADMANIPINFO,_("��ȡ������Ϣ\tCtrl-I"),_("Load manipObjInfo from file"));
	//file_menu->Append(feID_LOADMANIPINFO,_("Load &ManipObjInfo\tCtrl-I"),_("Load manipObjInfo from file"));

    file_menu->AppendSeparator();

	//file_menu->Append(feID_SAVEENVIROMENTINFO,_("Save scene enviroment infomation"),
	file_menu->Append(feID_SAVEENVIROMENTINFO,_("���泡��������Ϣ"),
		_("Save Save scene enviroment infomation to a file,incluing two main lights,enviroment light,fog."));
	//file_menu->Append(feID_LOADENVIROMENTINFO,_("Load scene enviroment infomation"),
	file_menu->Append(feID_LOADENVIROMENTINFO,_("��ȡ����������Ϣ"),
		_("Save Save scene enviroment infomation to a file,incluing two main lights,enviroment light,fog."));

    file_menu->AppendSeparator();

	file_menu->Append(feID_IMPORT_HEIGHT_MAP,_("����߶�ͼ"),
		_("Import height map."));
	file_menu->Append(feID_EXPORT_HEIGHT_MAP,_("�����߶�ͼ"),
		_("Export heitht map."));

	file_menu->AppendSeparator();

    //file_menu->Append(feID_SCENE_INFOMATION,_("Scene Infomation"),
	file_menu->Append(feID_SCENE_INFOMATION,_("������Ϣ"),
		_("�򿪳�����Ϣ�Ի���"));

    file_menu->AppendSeparator();

    file_menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT, true, wxT("Alt-X")));

    wxMenu* edit_menu = new wxMenu;
    menu_bar->Append(edit_menu, /*_("&Edit")*/_("[&E]�༭"));
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
        _("ɾ�����о�̬���壨����actor��"),
        _("ɾ�����о�̬���壨����actor��"));
	edit_menu->Append(feID_RESHAPE_TERRAIN,
		//_("&Reshape the Terrain"),
		//_("Reshape the Terrain "));
		_("��������"),
		_("��������"));
    edit_menu->AppendCheckItem(feID_ENABLE_32BIT_TEXTURES,
        //_("Enable &32 bits textures"),
        //_("Toggle 32 or 16 bits textures"));
        _("����32λ����"),
        _("ǿ��ʹ��32��16λ����"));
    edit_menu->Append(feID_RELOAD_TEXTURES,
        //_("Reload all te&xtures"),
        //_("Unload and reload all textures"));
        _("ˢ��ģ������"),
        _("����ˢ�����������"));
    edit_menu->Append(feID_UNLOAD_USELESS_RESOURCES,
        //_("&Unload useless resources"),
        //_("Unload all useless resources"));
        _("ж�����õ���Դ"),
        _("ж���������õ���Դ"));
    edit_menu->AppendCheckItem(feID_SHOW_INDICATORS,
        //_("Show all indicators"),
        //_("Toggle showing all indicators"));
        _("��ʾ����ָʾ��"),
        _("��ʾ����ָʾ��"));

    wxMenu* tools_menu = new wxMenu;
    menu_bar->Append(tools_menu, _("[&T]����"));

    tools_menu->AppendCheckItem(feID_DRAG_HEIGHT_ACTION,
        //_("&Drag Height"),
        _("[&D]�����������θ߶�"),
        _("Drag mouse to modify terrain height base on drag distance"));
    tools_menu->AppendCheckItem(feID_RAISE_HEIGHT_ACTION,
        /*_("&Raise Height"),*/_("[&R]�������θ߶�"),
        _("Raise height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_LOWER_HEIGHT_ACTION,
        /*_("&Lower Height"),*/_("[&L]���͵��θ߶�"),
        _("Lower height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_SMOOTH_HEIGHT_ACTION,
        /*_("&Smooth Height"),*/_("[&S]ƽ�����θ߶�"),
        _("Smooth height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_LEVEL_OFF_HEIGHT_ACTION,
        /*_("Level &Off Height"),*/_("[&O]ƽ�����θ߶�"),
        _("Level off height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_SET_TERRAIN_HEIGHT_ACTION,
        /*_("Set Terrain &Height"),*/_("[H]���õ��θ߶�"),
        _("set the height of the terrain based on the height setting"));
    tools_menu->AppendSeparator();

    tools_menu->AppendCheckItem(feID_TERRAIN_LAYER_ONE_ERASER_ACTION,
        /*_("&Erase the texture of layer one"),*/_("[&E]ɾ��͸������ͼ"),
        _("Erase the texture of layer one on the mouse drag area"));
    tools_menu->AppendSeparator();

    tools_menu->AppendCheckItem(feID_MANIP_OBJECT_ACTION,
        /*_("&Manip Object"),*/_("[&M]�ڷ�����"),
        /*_("Select, delete, placement objects"));*/_("ѡ��ɾ�����ڷ�����"));

    tools_menu->AppendCheckItem(feID_FLIP_DIAGONAL_ACTION,
        /*_("&Flip Grid Diagonal"),*/_("[&F]��ת�Խ���"),
        _("Flip the diagonal of the selected grids"));

    tools_menu->AppendCheckItem(feID_DISABLE_DECAL_ACTION,
        _("Disable Decal"),
        _("disable decal of the selected grids"));

    tools_menu->AppendCheckItem(feID_TERRAIN_SELECTION_ACTION,
        /*_("Select &Terrain"),*/_("[&T]ѡ�����"),
        _("Select the grids that you want to manipulate"));

	tools_menu->AppendSeparator();
	tools_menu->Append(feID_ERASE_BY_TEXTURE_NAME,
		/*_("Erase Terrain Texture"),*/_("ɾ��ָ���ĵ�������"),
		/*_("Erase Terrain Texture By Texture Name"));*/_("ɾ����ָ��������"));

    tools_menu->Append(feID_ENTIRE_TERRAIN_ENABLE_DECAL,
        _("Enable decal on entire terrain"),
        _("Enable decal on entire terrain"));

	tools_menu->AppendSeparator();
	tools_menu->Append(feID_TERRAIN_LIGHTMAP_OPTION,
		/*_("Terr&ain shadowmap Option"),*/_("[&A]������Ӱͼѡ��"),
		/*_("Update, show, hide terrain shadowmap"));*/_("���¡���ʾ�����ص�����Ӱͼ"));
	tools_menu->AppendCheckItem(feID_TERRAIN_LIGHTMAP_HIDE,
		/*_("Hide shadow&map"),*/_("[&M]����ʾ������Ӱͼ"),
		/*_("Hide shadowmap"));*/_("����ʾ������Ӱͼ"));
	tools_menu->AppendCheckItem(feID_TERRAIN_LIGHTMAP_LOW_QUALITY,
		/*_("Show lo&w quality shadowmap"),*/_("��ʾ�;��ȵ�����Ӱͼ"),
		/*_("Show low quality shadowmap"));*/_("��ʾ�;��ȵ�����Ӱͼ"));
	tools_menu->AppendCheckItem(feID_TERRAIN_LIGHTMAP_HIGH_QUALITY,
		/*_("Show high &quality shadowmap"),*/_("��ʾ�߾��ȵ�����Ӱͼ"),
		/*_("Show high quality shadowmap"));*/_("��ʾ�߾��ȵ�����Ӱͼ"));

    tools_menu->AppendSeparator();
    tools_menu->Append(feID_TERRAIN_SURFACE_PARAMETERS,
        /*_("Terrain Surface Parameters"),*/_("���β��ʲ���"),
        /*_("Setting the surface parameters of terrain"));*/_("���õ��β��ʲ���"));
    tools_menu->Append(feID_SCENE_ANIMATION_QUALITY,
        /*_("Scene animation quality..."),*/_("�������ü���"),
        _("Sets scene animations quality"));

    tools_menu->Append(feID_OBJECT_CREATE_LEVEL,
        /*_("Ojbect create level..."),*/_("���󴴽�����"),
        _("Sets object create level"));

    tools_menu->AppendSeparator();
    tools_menu->Append(feID_CREATE_MINI_MAP,
        /*_("Create Mini Map"),*/_("����С��ͼ"),
        /*_("Create Mini Map"));*/_("����С��ͼ"));

	tools_menu->AppendSeparator();
	tools_menu->Append(feID_CONVERT_CBRUSH_INFO, _("��ȡcbrush�ļ�����"), _("Ϊ����֮ǰ�İ汾����ȡcbrush�ļ��������ڱ���"));


	wxMenu* creator_menu = new wxMenu;
	menu_bar->Append(creator_menu, /*_("&Creator")*/_("��������"));				

	// Һ�崴�����˵�
	creator_menu->Append(feID_CREATE_LIQUID,
		/*_("Create &Liquid"),*/_("����Һ������"),
		/*_("Show the liquid creator dialog"));*/_("��ʾҺ�����崴���Ի���"));

	// ��Դ�����˵�
	creator_menu->Append(feID_CREATE_LIGHT,
		/*_("Create Light Object"),*/_("�������Դ"),
		/*	_("Create a light object"));*/_("�������Դ����"));

	// ���д����˵�
	creator_menu->Append(feID_CREATE_OBJECT_ARRAY,
		/*_("Create Light Object"),*/_("�����������"),
		/*	_("Create a light object"));*/_("�����������"));

    wxMenu* options_menu = new wxMenu;
    menu_bar->Append(options_menu, /*_("&Options")*/_("[&O]ѡ��"));
    options_menu->Append(feID_PREFERSETTING,
        /*_("&Configuration..."),*/_("[&C]����..."),
        /*_("Global sonfiguration"));*/_("ѡ������"));
    options_menu->AppendCheckItem(feID_LOCK_CAMERA_HEIGHT,
        /*_("&Lock camera height"),*/_("[&L]������ͷ�߶�"),
        _("Lock the camera relative height to terrain when drag move"));
	options_menu->Append(feID_RESET_CAMERA,
		/*_("Reset Camera"),*/_("���������"),
		_("Reset Camera to default height and default angle."));

	// �ڳ����н�������ļ�ȡʱ��ѡ��ȫ�����壨������ֻѡ����Щ���ܿ����ĵ�ѡȡ�����壩
	options_menu->AppendCheckItem(feID_SELECTALLOBJECTS,
        /*_("Select all objects"),*/_("����ѡ����������"),
        //_("If you check this, the object Select action will select all objects, otherwise only select the objects that can not center selected."));
        _("����ѡ���������壨�ڲ��������״̬�£�"));

	options_menu->AppendSeparator();
    options_menu->AppendCheckItem(feID_ALIGN_POSITION,
        /*_("&Align to grid"),*/_("[&A]�������"),
        //_("Align to terrain grid when move objects"));
        _("�������ƶ�����ʱ�Ե�������Ϊ��λ"));
    options_menu->AppendSeparator();
    options_menu->AppendCheckItem(feID_SHOW_STANDARD_MODEL,
        /*_("&Show the standard model"),*/_("[&S]��ʾ�ο�ģ��"),
        _("Show the standard model when mouse move into the scene"));

    wxMenu* debug_panel_menu = new wxMenu;
    options_menu->Append(wxID_ANY, /*_("&Debug Panels")*/_("�������"), debug_panel_menu);
    debug_panel_menu->AppendCheckItem(feID_SHOW_STAT_PANEL,
        /*_("Show &statistics"),*/_("��ʾ��Ⱦͳ��"),
        _("Show the renderer statistics on render window"));
    debug_panel_menu->AppendCheckItem(feID_SHOW_LOGO_PANEL,
        /*_("Show lo&go"),*/_("��ʾ Logo"),
        _("Show the logo on render window"));

	// ��ˢ��״ѡ��˵�
	wxMenu* brush_select_menu = new wxMenu;
	options_menu->Append(wxID_ANY, /*_("&Select Brush")*/_("ѡ��ˢ����"), brush_select_menu);
	brush_select_menu->AppendCheckItem(feID_BOX_BRUSH,
		/*_("Box Brush"),*/_("���λ�ˢ"),
		_("Select the box shape brush"));
	brush_select_menu->AppendCheckItem(feID_ELLIPSE_BRUSH,
		/*_("Ellipse Brush"),*/_("��Բ�λ�ˢ"),
		_("Select the ellipse shape brush"));

//*/
    wxMenu* shadowtype_menu = new wxMenu;
    options_menu->Append(wxID_ANY, /*_("&Dynamic shadow technique")*/_("����ѧ��Ӱ����"), shadowtype_menu);
/*/
    wxMenu* shadowtype_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_NONE,
        /*_("None"),*/_("�ر�"),
        _("Disable dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_TEXTURE_ADDITIVE,
        /*_("Texture additive"),*/_("���ʵ���"),
        _("Use texture additive dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_TEXTURE_MODULATIVE,
        /*_("Texture modulative"),*/_("���ʷ������"),
        _("Use texture modulative dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_STENCIL_ADDITIVE,
        /*_("Stencil additive"),*/_("ģ�����"),
        _("Use stencil additive dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_STENCIL_MODULATIVE,
        /*_("Stencil modulative"),*/_("ģ�巽�����"),
        _("Use stencil modulative dynamic shadows"));

//*/
    wxMenu* tfo_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("�������"), tfo_menu);
/*/
    wxMenu* tfo_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    tfo_menu->AppendRadioItem(feID_TFO_NONE,
        _("�رչ���"),
        _("Disable texture filtering"));
    tfo_menu->AppendRadioItem(feID_TFO_BILINEAR,
        _("˫���Թ���"),
        _("Use bi-linear texture filtering"));
    tfo_menu->AppendRadioItem(feID_TFO_TRILINEAR,
        _("�����Թ���"),
        _("Use tri-linear texture filtering"));
    tfo_menu->AppendRadioItem(feID_TFO_ANISOTROPIC,
        _("�������Թ���"),
        _("Use anisotropic texture filtering"));

    wxMenu* clo_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("������������"), clo_menu);
    /*/
    wxMenu* tfo_menu = options_menu;
    options_menu->AppendSeparator();
    //*/
    clo_menu->AppendRadioItem(feID_CL_LOW,
        _("��"),
        _("Use Low Level"));
    clo_menu->AppendRadioItem(feID_CL_MID,
        _("��"),
        _("Use Mid Level"));
    clo_menu->AppendRadioItem(feID_CL_HIGH,
        _("��"),
        _("Use High Level"));

/*/
    wxMenu* post_filter_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("&Post Filter"), post_filter_menu);
/*/
    wxMenu* post_filter_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    post_filter_menu->AppendCheckItem(feID_PF_HEATVISION,
        _("ҹ�Ӿ�Ч��") + wxString(wxT("\tCtrl-1")),
        _("Use heat vision post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_BLOOM,
        _("�ι�Ч��") + wxString(wxT("\tCtrl-2")),
        _("Use bloom post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_FLOODLIGHTING,
        _("ȫ�ַ���Ч��") + wxString(wxT("\tCtrl-3")),
        _("Use floodlighting post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_FADE,
        _("����Ч��") + wxString(wxT("\tCtrl-4")),
        _("Use fade post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_MOTION_BLUR,
        _("��ӰЧ��") + wxString(wxT("\tCtrl-5")),
        _("Use motion blur filter"));

    wxMenu* window_menu = new wxMenu;
    menu_bar->Append(window_menu, _("[&W]����"));
    window_menu->AppendCheckItem(feID_FULL_SCREEN,
        _("[&F]ȫ����ʾ\tCtrl-Enter"),
        _("�л�ȫ����ʾģʽ"));
    window_menu->Append(feID_TOGGLE_TOOL_WINDOWS,
        wxEmptyString);   // Will setup the correct display text on UI update
    window_menu->AppendSeparator();

    mWindowMenu = window_menu;
}

void
FairyEditorFrame::InitToolBar(wxToolBar* toolbar)
{
    toolbar->AddCheckTool(feID_LOCK_BRUSH_SIZE, _("������ˢ��С"),
        wxBITMAP(locker), wxNullBitmap, _("������ˢ��С"));

	wxControl *control = NULL;

	control = new wxSpinCtrl(toolbar, feID_BRUSH_SIZE_X,
		wxString::Format(wxT("%d"), 3),
		wxDefaultPosition, wxSize(40, -1),
		wxSP_ARROW_KEYS,
		1, 9, 3);
	control->SetToolTip(_("X����ˢ��С"));
    toolbar->AddControl(control);

	control = new wxSpinCtrl(toolbar, feID_BRUSH_SIZE_Z,
		wxString::Format(wxT("%d"), 3),
		wxDefaultPosition, wxSize(40, -1),
		wxSP_ARROW_KEYS,
		1, 9, 3);
	control->SetToolTip(_("Z����ˢ��С"));
    toolbar->AddControl(control);

	control = new wxSpinCtrl(toolbar, feID_TERRAIN_HEIGHT,
		wxString::Format(wxT("%d"), 1000),
		wxDefaultPosition, wxSize(60, -1),
		wxSP_ARROW_KEYS,
		-99999, 99999, 1000);
	control->SetToolTip(_("���õ��εĸ߶�"));
    toolbar->AddControl(control);

	control = new wxSpinCtrl(toolbar, feID_TERRAIN_HEIGHT_INCREMENT,
		wxString::Format(wxT("%d"), 100),
		wxDefaultPosition, wxSize(60, -1),
		wxSP_ARROW_KEYS,
		-999, 999, 100);
	control->SetToolTip(_("���õ��θ߶ȵ���ֵ"));
	toolbar->AddControl(control);

	// ����Ϊ�ı���θ߶ȵ�Ȩ�����õ�ui
	toolbar->AddSeparator();

	control = new wxSpinCtrl(toolbar, feID_HEIGHT_ACTION_AFFECT_RANGE,
		wxString::Format(wxT("%d"), 2),
		wxDefaultPosition, wxSize(40, -1),
		wxSP_ARROW_KEYS,
		0, 10, 2);
	control->SetToolTip(_("���õ��λ�ˢ�Ķ���Ӱ�췶Χ"));
    toolbar->AddControl(control);

	control = new wxTextCtrl(toolbar, feID_HEIGHT_ACTION_EXP_VALUE,
		wxString::Format(wxT("%f"), 0.5),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER);
	control->SetToolTip(_("���õ��λ�ˢ���ⷶΧ��Expֵ����ΧԽԶ��Ӱ��ԽС��"));
	toolbar->AddControl(control);

	control = new wxTextCtrl(toolbar, feID_HEIGHT_ADJUST_SPEED,
		wxString::Format(wxT("%f"), 3.0),
		wxDefaultPosition, wxSize(30, -1), wxTE_PROCESS_ENTER);
	control->SetToolTip(_("���õ��θı���ٶ�"));
	toolbar->AddControl(control);

	// hack�����÷������
	toolbar->AddSeparator();

	toolbar->AddControl(new wxStaticText(toolbar, feID_BLURAMOUNTSTATICTEXT,
		wxT("YH"),	wxDefaultPosition));//��

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_BLURAMOUNT,
		wxString::Format(wxT("%f"), 0.2),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER));

	toolbar->AddControl(new wxStaticText(toolbar, feID_SHINEAMOUNTSTATICTEXT,
		wxT("GL"),	wxDefaultPosition));/*����*/

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_SHINEAMOUNT,
		wxString::Format(wxT("%f"), 0.7),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER));


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// ��ʾ�������(��Ϸ�е��߼�����)
	//

	toolbar->AddSeparator();

	toolbar->AddControl(new wxStaticText(toolbar, feID_STATIC_SHOW_X,
		"��ǰX",	wxDefaultPosition));

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_SHOW_X,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(60, -1), wxTE_PROCESS_ENTER));

	toolbar->AddControl(new wxStaticText(toolbar, feID_STATIC_SHOW_Y,
		"��ǰZ",	wxDefaultPosition));

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_SHOW_Y,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(60, -1), wxTE_PROCESS_ENTER));

	
	toolbar->AddControl(new wxTextCtrl(toolbar, feID_GOTO_X,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER));
	toolbar->AddControl(new wxTextCtrl(toolbar, feID_GOTO_Y,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER));


	// ��ť
	toolbar->AddControl(new wxButton(toolbar, feID_GOTO_BN, _("��ת"), wxDefaultPosition, wxSize(30, -1), wxTE_PROCESS_ENTER));
		

	//
	//
	// ��ʾ�������(��Ϸ�е��߼�����)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	



	toolbar->AddCheckTool(feID_USE_REAL_CAMERA, _("ʹ����ʵ��Ϸ��ͷ"),
		wxBITMAP(USEREALCAMERA), wxNullBitmap, _("ʹ����ʵ��Ϸ��ͷ"));

	toolbar->AddCheckTool(feID_USE_SNAP_ROTATION, _("ʹ�����Ƕ���ת"),
		wxBITMAP(USESNAPROTATE), wxNullBitmap, _("ʹ�����Ƕ���ת"));

	control = new wxTextCtrl(toolbar, feID_FRAMERATE,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER);
	control->SetToolTip(_("֡����"));
	toolbar->AddControl(control);

    // TODO
    toolbar->Realize();
}

// ���õ�ǰ�ĳ����߼�����
void FairyEditorFrame::SetCurSceneLogicPos(float fx, float fz)
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


void
FairyEditorFrame::InitStatusBar(wxStatusBar* statusbar)
{
    // TODO
}

//////////////////////////////////////////////////////////////////////////

Fairy::SceneManipulator*
FairyEditorFrame::GetSceneManipulator(void) const
{
    return mCanvas ? mCanvas->GetSceneManipulator() : NULL;
}

void
FairyEditorFrame::InitScene(Fairy::SceneManipulator* sceneManipulator)
{
    wxASSERT(mCanvas);

	assert (sceneManipulator);
    //laim22 ����ط���װ����Դ
	mCanvas->SetParentFrame(this);
    mCanvas->InitScene(sceneManipulator);
	mSmartColourDialog->SetSceneManipulator(sceneManipulator);
    mBrushSelector->SetSceneManipulator(sceneManipulator);
    mBrushSelector->ReloadTextureList();
    mModelSelector->SetSceneManipulator(sceneManipulator);
    mModelSelector->Reload();
    mActorSelector->SetSceneManipulator(sceneManipulator);
    mActorSelector->Reload();
    mParticleSystemSelector->SetSceneManipulator(sceneManipulator);
    mParticleSystemSelector->Reload();
    mSceneBrowser->SetSceneManipulator(sceneManipulator);
    mObjectPropertyEditor->SetSceneManipulator(sceneManipulator);
    mSceneInfoDlg->SetSceneManipulator(sceneManipulator);
    //mCameraTrackSettingDialog->SetSceneManipulator(sceneManipulator);

	// ������༭
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->SetSceneManipulator(sceneManipulator);
		g_pRegionEdit = m_pRegionEditSelector;
	}

	// ����༭
	if (m_pMonsterEditDlg)
	{
		m_pMonsterEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// ������༭
	if (m_pGrowPointEditDlg)
	{
		m_pGrowPointEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// �����¼�
	if (m_pEventAreaEditDlg)
	{
		m_pEventAreaEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// npc�༭
	if (m_pNpcPatrolEditDlg)
	{
		m_pNpcPatrolEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// ̯λ�༭
	if (m_pStallInfoDlg)

	{
		m_pStallInfoDlg->SetSceneManipulator(sceneManipulator);
	}


	//�������༭
	if(mSoundEditDialog)
	{
		mSoundEditDialog->SetSceneManipulator(sceneManipulator);
	}

	if( m_pPathFindEditDlg )
	{
		m_pPathFindEditDlg->SetSceneManipulator(sceneManipulator);
	}

    // ��ͼ��
    if(mDatuPointEditDialog)
    {
        mDatuPointEditDialog->SetSceneManipulator(sceneManipulator);
    }

    mAutoSaveSceneController = new AutoSaveSceneController(this, 300.0f);

}

bool
FairyEditorFrame::Save(void)
{
	// �����ǰ��action�ǻ�ˢ�����ȸ�����ʾ����Ϊ��������ڻ�ˢaction�±������Ϊpixmap�����ԭ�����������
	/*if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxMessageBox(_("It is not safe to save the scene when the active action is paint action!"));
			return false;
		}
	}*/

    if (m_sceneFilename.empty())
    {
        if(!SaveAs())
		{
			return false;
		}
    }
    else
    {
		// �����ǰ��action�ǻ�ˢ�����ȸ�����ʾ����Ϊ��������ڻ�ˢaction�±������Ϊpixmap�����ԭ�����������
		resetPaintAction();

        // �洢������������
        SaveRegion();

        // �洢�����������档
        SaveBuildingWalk();

        // �洢npc.
        SaveNpc();

        // �洢������
        SaveGrowPoint();

        // �洢npcѲ��·��
        SaveNpcPatrol();

		// �洢Ѱ·��
		SavePathFind();

		// �洢�¼�����
        SaveEventArea();

        // �洢̯λ��Ϣ
        SaveStallInfo();

		// �洢̯λ��Ϣ
		SaveStallInfo();

		// �洢����
		SaveSoundInfo();

        SaveDatuPointInfo();
        // ���泡���༭����Ķ�����Ϣ
        GetSceneManipulator()->getSceneEditingData().saveData(AS_STRING(m_sceneFilename));

		mIsTerrainLightmapUpdated = false;

         
		SaveImpl();

        // �洢������������
        if(!SaveCRegion())
		{
			return false;
		}
		onMotionPaintAction();
    }
	return true;
}

void FairyEditorFrame::resetPaintAction()
{
	if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxPoint pt =mCanvas->mCurrentMousePos;
			Fairy::PaintAction* pAction = dynamic_cast<Fairy::PaintAction*>(GetSceneManipulator()->getActiveAction());
			if(pAction)
			{
				pAction->resetHintModified();				
				pAction->onEnd(pt.x, pt.y, true);
			}

		}
	}
}

void FairyEditorFrame::onMotionPaintAction()
{
	if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxPoint pt =mCanvas->mCurrentMousePos;
			Fairy::PaintAction* pAction = dynamic_cast<Fairy::PaintAction*>(GetSceneManipulator()->getActiveAction());
			if(pAction)
			{
				pAction->onMotion(pt.x,pt.y);
			}

		}
	}
}


bool
FairyEditorFrame::SaveAs(void)
{
	// �����ǰ��action�ǻ�ˢ�����ȸ�����ʾ����Ϊ��������ڻ�ˢaction�±������Ϊpixmap�����ԭ�����������
	resetPaintAction();

    wxFileDialog fileDialog(this,
        _("���泡��"),
        m_lastSceneDirectory,
        m_sceneFilename,
        wxString::Format(
            _("�����ļ� (*.Scene)|*.Scene|���� XML �ļ� (*.Scene.xml)|*.Scene.xml|ȫ���ļ� (%s)|%s"),
            wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (fileDialog.ShowModal() != wxID_OK)
        return false;

    m_sceneFilename = fileDialog.GetPath();
    m_lastSceneDirectory = wxPathOnly(m_sceneFilename);

    // �洢������������
    SaveRegion();

	// �洢Ѱ·��
	SavePathFind();

    // �洢�����������档
    SaveBuildingWalk();

    // �洢npc.
    SaveNpc();

    // �洢������
    SaveGrowPoint();

    // �洢npcѲ��·��
    SaveNpcPatrol();

    // �洢�¼�����
    SaveEventArea();

    // �洢̯λ��Ϣ
    SaveStallInfo();

    // �洢����
    SaveSoundInfo();

    SaveDatuPointInfo();

    
	SaveImpl();

   // �洢������������
    if(SaveCRegion())
	{
		return false;
	}
	onMotionPaintAction();
	return true;
}

bool 
FairyEditorFrame::SaveImpl(bool autoSave)
{
    if (autoSave)
    {
        if (!m_sceneFilename.empty())
        {
            Ogre::String baseName, path;
            Ogre::StringUtil::splitFilename(AS_STRING(m_sceneFilename), baseName, path);

            GetSceneManipulator()->saveScene(path + "AutoSave_" + baseName);    

            // ���泡���༭����Ķ�����Ϣ
            Fairy::SceneEditingData& sed = GetSceneManipulator()->getSceneEditingData();

            if (sed.isDataChanged())
            {
                sed.saveData(path + "AutoSave_" + baseName);
            }

			// ���泡���༭����Ķ�����Ϣ
			GetSceneManipulator()->getSceneEditingData().saveData(AS_STRING(m_sceneFilename));
            mIsTerrainLightmapUpdated = false;
        }
    }
    else
    {
        GetSceneManipulator()->saveScene(AS_STRING(m_sceneFilename));    


        // ���泡���༭����Ķ�����Ϣ
        Fairy::SceneEditingData& sed = GetSceneManipulator()->getSceneEditingData();

        if (sed.isDataChanged())
        {
            sed.saveData(AS_STRING(m_sceneFilename));
        }

        mIsTerrainLightmapUpdated = false;
    }

    mAutoSaveSceneController->reset();

    return true;
}

bool
FairyEditorFrame::SaveModified(bool force)
{
    if ((!GetSceneManipulator() || !GetSceneManipulator()->isModified()) && !mIsTerrainLightmapUpdated)
        return +1;

    wxString name;
    if (!m_sceneFilename.empty())
        wxFileName::SplitPath(m_sceneFilename, NULL, &name, NULL);
    else
        name = _("δ����");

    wxString msgTitle;
    if (!wxTheApp->GetAppName().empty())
        msgTitle = wxTheApp->GetAppName();
    else
        msgTitle = /*_("Warning")*/wxT("����");

    wxString prompt;
    prompt.Printf(_("���뱣����ĵ�%s���޸�?"), name.c_str());
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
FairyEditorFrame::ProcessEvent(wxEvent& e)
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
FairyEditorFrame::OnExit(wxCommandEvent& e)
{
    Close();
}

void
FairyEditorFrame::OnUpdateWindow(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);

    if (GetSceneManipulator())
    {
        wxString name;
        if (!m_sceneFilename.empty())
            wxFileName::SplitPath(m_sceneFilename, NULL, &name, NULL);
        else
            name = _("20100531");//�汾��

        wxString modified;
        if (GetSceneManipulator()->isModified() || mIsTerrainLightmapUpdated)
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
FairyEditorFrame::OnCloseWindow(wxCloseEvent& e)
{
    if (SaveModified(!e.CanVeto()))
        wxFrame::OnCloseWindow(e);
    else
        e.Veto();
}

void
FairyEditorFrame::OnFileNew(wxCommandEvent& e)
{
    if (!SaveModified())
        return;	

	if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxPoint pt =mCanvas->mCurrentMousePos;
			Fairy::PaintAction* pAction = dynamic_cast<Fairy::PaintAction*>(GetSceneManipulator()->getActiveAction());
			if(pAction)
			{
				pAction->resetHintModified(false);
				pAction->resetCurrentGrids();
				GetSceneManipulator()->setActiveAction(NULL);
			}					
		}
	}

    NewSceneDialog dlg(this, wxID_ANY, _("�³���"));
    dlg.Centre();
    if (dlg.ShowModal() == wxID_OK)
    {
        int xsize,zsize,seed,scale,tileSize,height;
        Ogre::String alogrithm;
        
        alogrithm = dlg.mAlogrithmCtrl->GetLabel();
        xsize = atoi(dlg.mXsizeTextCtrl->GetLabel());
        zsize = atoi(dlg.mZsizeTextCtrl->GetLabel());
        Ogre::String baseTexture  = Ogre::String(dlg.mCmbTexture->GetLabel().GetData());
        seed = atoi(dlg.mSeedTextCtrl->GetLabel());
        scale = atoi(dlg.mScaleTextCtrl->GetLabel());
        height = atoi(dlg.mHeightScaleTextCtrl->GetLabel());
        tileSize = atoi(dlg.mTileSizeTextCtrl->GetLabel());

        wxBusyInfo busyInfo(_("�������ɳ�������ȴ�..."), this);

        NewSceneDialog::PathNameMap::const_iterator iter = dlg.pathNameMap.find(baseTexture);
        if (iter!= (dlg.pathNameMap.end()))
        {
            GetSceneManipulator()->newScene(alogrithm,xsize,zsize,tileSize,iter->second,seed,scale,height);
        }
        else
        {
            GetSceneManipulator()->newScene(alogrithm,xsize,zsize,tileSize,baseTexture,seed);
        }
        m_sceneFilename.clear();

		//// �������������.
		//m_pRegionEditSelector->ClearData();
		ClearRegion();

		// ���npc
		ClearNpc();

		// ���������
		ClearGrowPoint();

		// ���npcѲ��
		ClearNpcPatrol();

		// ���Ѱ·��
		ClearPathFind();

		// ����¼�����
		ClearEventArea();

		// ����̯λ
		CreateStallInfo();

		// �������
		ClearSoundInfo();

        ClearDatuPointInfo();

		mIsTerrainLightmapUpdated = false;
		mSceneInfoDlg->mScrWin->Enable(true);
    }

    mAutoSaveSceneController->reset();
}

void
FairyEditorFrame::OnUpdateFileNew(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
FairyEditorFrame::OnReshapeTerrain(wxCommandEvent& e)
{
    // ��ֹ��ǰ����
    GetSceneManipulator()->setActiveAction(NULL);

	ReshapeDialog dlg(this, wxID_ANY, _("��������"));
	dlg.Centre();
	if (dlg.ShowModal() == wxID_OK)
	{
		int left,top,right,bottom;
        int rLeft, rRight, rTop, rBottom;
		int pixmap;
		wxString textureName;

		textureName = dlg.mCmbTexture->GetLabel();
		left = atoi(dlg.mLeftCtrl->GetLabel());
		top = atoi(dlg.mTopCtrl->GetLabel());	
		right = atoi(dlg.mRightCtrl->GetLabel());
		bottom = atoi(dlg.mBottomCtrl->GetLabel());

        Fairy::TerrainData* terrainData = GetSceneManipulator()->getTerrainData();
        assert (terrainData);

        int xSize = terrainData->getXSize();
        int zSize = terrainData->getZSize();
	
        /**
        �Ի������д����Ϊ���������С����������С�ĸ����ĸ�����������С4�����-4
        �����������Σ�������������ĸ�������������4�����4
        ��������Ҫ����Ӧ��ת���������������ת����_reshape��Ҫ�Ĳ���
        */
        rLeft = -left;
        rRight = xSize + right;
        rTop = -top;
        rBottom = zSize + bottom;

        if ((rLeft >= rRight) || (rTop >= rBottom))
        {
            wxMessageBox(_("���������!"));
            return;
        }

		m_pStallInfoDlg->m_pEditAction->m_vOldFirstGrid = GetSceneManipulator()->getTerrainData()->mPosition;

		NewSceneDialog::PathNameMap::const_iterator iter = dlg.pathNameMap.find(Ogre::String(textureName));
		if (iter!= (dlg.pathNameMap.end()))
		{
			pixmap = GetSceneManipulator()->getTerrainData()->_registerPixmap(iter->second,0,0,1,1);
		}
		else
		{
			pixmap = GetSceneManipulator()->getTerrainData()->_registerPixmap(Ogre::String(textureName),0,0,1,1);
		}
		
		wxBusyInfo busyInfo(_("�������� ..."), this);        
        
		terrainData->_reshape(rLeft,rTop,rRight,rBottom,pixmap);

        // д��reshape��size
        GetSceneManipulator()->getSceneEditingData().setTerrainReshapeSize(left, right, top, bottom);

		GetSceneManipulator()->getTerrain()->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
		GetSceneManipulator()->_fireSceneReset();
		m_pStallInfoDlg->ReCreateStallInfo();
	
	}
}

void
FairyEditorFrame::OnUpdateReshapeTerrain(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

void
FairyEditorFrame::OnEnable32BitTextures(wxCommandEvent& e)
{
    Ogre::TextureManager *tm = Ogre::TextureManager::getSingletonPtr();
    if (tm)
    {
        tm->setPreferredIntegerBitDepth(tm->getPreferredIntegerBitDepth() == 16 ? 0 : 16);
    }
}

void
FairyEditorFrame::OnUpdateEnable32BitTextures(wxUpdateUIEvent& e)
{
    Ogre::TextureManager *tm = Ogre::TextureManager::getSingletonPtr();
	e.Enable(tm != 0);
    e.Check(tm && tm->getPreferredIntegerBitDepth() != 16);
}

void
FairyEditorFrame::OnReloadTextures(wxCommandEvent& e)
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
FairyEditorFrame::OnUpdateReloadTextures(wxUpdateUIEvent& e)
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
FairyEditorFrame::OnUnloadUselessResources(wxCommandEvent& e)
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
FairyEditorFrame::OnUpdateUnloadUselessResources(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

void
FairyEditorFrame::OnShowAllIndicators(wxCommandEvent& e)
{
    Fairy::SceneManipulator* manipulator = GetSceneManipulator();

    if (manipulator)
    {
        manipulator->setIndicatorNodeVisible(!manipulator->getIndicatorNodeVisible());
    }    
}

void
FairyEditorFrame::OnUpdateShowAllIndicators(wxUpdateUIEvent& e)
{
    Fairy::SceneManipulator* manipulator = GetSceneManipulator();

    e.Enable(manipulator != 0);
    e.Check(manipulator && manipulator->getIndicatorNodeVisible());    
}

void
FairyEditorFrame::OnUpdateSave(wxUpdateUIEvent& e)
{
    e.Enable((GetSceneManipulator() && GetSceneManipulator()->isModified()) || mIsTerrainLightmapUpdated);
}

void
FairyEditorFrame::OnUpdateSaveAs(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() && !GetSceneManipulator()->isEmpty());
}

void
FairyEditorFrame::OnFileOpen(wxCommandEvent& e)
{
    if (!SaveModified())
        return;

	if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxPoint pt =mCanvas->mCurrentMousePos;
			Fairy::PaintAction* pAction = dynamic_cast<Fairy::PaintAction*>(GetSceneManipulator()->getActiveAction());
			if(pAction)
			{
				pAction->resetHintModified(false);
				pAction->resetCurrentGrids();
				GetSceneManipulator()->setActiveAction(NULL);
			}					
		}
	}

    wxFileDialog fileDialog(this,
        _("װ�س���"),
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

        wxBusyInfo busyInfo(_("����װ�س�������ȴ�..."), this);
      
		GetSceneManipulator()->loadScene(AS_STRING(m_sceneFilename));      

		if (GetSceneManipulator()->hasExceptionWhenLoadScene())
		{
			ExceptionInfoDialog dlg(this, wxID_ANY,
                _("�쳣��Ϣ�Ի���"), GetSceneManipulator());
		
			if (dlg.ShowModal() != wxID_OK)
			{
				// ����쳣�б����������ݣ��͸�����ʾ
				if (dlg.mListBox->GetCount() > 0)
				{
					wxMessageBox(_("�����ļ����Դ���һЩ�������޸���"));
				}
				else
				{
					wxMessageBox(_("�����е����д����������������±��泡��ʹ�øĶ���Ч��"));
				}
			}
		}

		char szDebugInfo[200];
		DWORD dwTickCount = GetTickCount();
		// ��ȡ������������
		ReadRegion();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadRegion cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );
		
		// ��ȡ������
		dwTickCount = GetTickCount();
		ReadGrowPoint();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadGrowPoint cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// ��ȡnpc
		dwTickCount = GetTickCount();
		ReadNpc();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadNpc cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// ��ȡnpcѲ��
		dwTickCount = GetTickCount();
		ReadNpcPatrol();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadNpcPatrol cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		//Ѱ·�༭
		dwTickCount = GetTickCount();
		ReadPathFind();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadPathFind cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// ��ȡ�¼�����
		dwTickCount = GetTickCount();
		ReadEventArea();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadEventArea cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// ��ȡ̯λ��Ϣ
		dwTickCount = GetTickCount();
		ReadStallInfo();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadStallInfo cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// ��ȡ������Ϣ
		dwTickCount = GetTickCount();
		ReadSoundInfo();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadSoundInfo cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

        ReadDatuPointInfo();

        // ��ȡ�����༭����Ķ�����Ϣ
		dwTickCount = GetTickCount();
        GetSceneManipulator()->getSceneEditingData().readData(AS_STRING(m_sceneFilename));
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadOtherInfo cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		mSceneInfoDlg->mScrWin->Enable(true);

    }

    mAutoSaveSceneController->reset();
}

void
FairyEditorFrame::OnFileSave(wxCommandEvent& e)
{
    Save();
}

void
FairyEditorFrame::OnFileSaveAs(wxCommandEvent& e)
{
    SaveAs();
}

void
FairyEditorFrame::OnUndo(wxCommandEvent& e)
{
    GetSceneManipulator()->getOperatorManager()->undo();
}

void
FairyEditorFrame::OnRedo(wxCommandEvent& e)
{
    GetSceneManipulator()->getOperatorManager()->redo();
}

void
FairyEditorFrame::OnUpdateUndo(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() &&
        !GetSceneManipulator()->getOperatorManager()->getUndoOperators().empty());
}

void
FairyEditorFrame::OnUpdateRedo(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() &&
        !GetSceneManipulator()->getOperatorManager()->getRedoOperators().empty());
}

void
FairyEditorFrame::OnLockCameraHeight(wxCommandEvent& e)
{
    GetSceneManipulator()->setCameraHeightLocked(!GetSceneManipulator()->getCameraHeightLocked());
}

void
FairyEditorFrame::OnUpdateLockCameraHeight(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getCameraHeightLocked());
}

void 
FairyEditorFrame::OnShowStandardModel(wxCommandEvent& e)
{
    GetSceneManipulator()->setShowStandardModel(!GetSceneManipulator()->getShowStandardModel());
}

void 
FairyEditorFrame::OnDeleteStaticObject(wxCommandEvent& e)
{
    int answer = wxMessageBox(_("����ɾ���� '%s'"), _("ȷ��"),
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
FairyEditorFrame::OnUpdateDeleteStaticObject(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() !=0);
}

void
FairyEditorFrame::OnUpdateShowStandardModel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getShowStandardModel());
}

void
FairyEditorFrame::OnLockBrushSize(wxCommandEvent& e)
{
    mLockBrushSize = !mLockBrushSize;
    if (mLockBrushSize)
    {
        size_t brushSize = static_cast<size_t>(
            sqrt((double)GetSceneManipulator()->getBrushXSize() * GetSceneManipulator()->getBrushZSize()));
        GetSceneManipulator()->setBrush(
            GetSceneManipulator()->getBrushType(), brushSize, brushSize);
    }
}

void
FairyEditorFrame::OnUpdateLockBrushSize(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(mLockBrushSize);
}

void
FairyEditorFrame::OnBrushSizeChanged(wxSpinEvent& e)
{
    // �ֶ������ͷ�����ڻ�ˢ��С
    if (mLockBrushSize)
    {
        GetSceneManipulator()->setBrush(
            GetSceneManipulator()->getBrushType(), e.GetPosition(), e.GetPosition());
    }
    else
    {
        if (e.GetId() == feID_BRUSH_SIZE_X)
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), e.GetPosition(), GetSceneManipulator()->getBrushZSize());
        else
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), GetSceneManipulator()->getBrushXSize(), e.GetPosition());
    }
}

void
FairyEditorFrame::OnBrushSizeTextChanged(wxCommandEvent& e)
{
    // �ֶ���д���������ڻ�ˢ��С
    if (mLockBrushSize)
    {
        GetSceneManipulator()->setBrush(
            GetSceneManipulator()->getBrushType(), e.GetInt(), e.GetInt());
    }
    else
    {
        if (e.GetId() == feID_BRUSH_SIZE_X)
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), e.GetInt(), GetSceneManipulator()->getBrushZSize());
        else
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), GetSceneManipulator()->getBrushXSize(), e.GetInt());
    }
}

void
FairyEditorFrame::OnUpdateBrushSizeSpin(wxUpdateUIEvent& e)
{
    // ɨ�軭ˢ��С����ѡ�������ĸ���
    e.Enable(GetSceneManipulator() != 0);

    if (e.GetEnabled())
    {
        // ���ݵ�ǰ��ˢ�Ӵ�С���޸�ui�����е�����
        // ���ݵ�ǰ��ˢx����Ĵ�С�������ı�������֣�bug���򣩣���������û���õ�OnBrushSizeTextChanged��
        // ������������С��x�����Ϻ�y�����ϲ�һ��ʱ����ʱ�ͻ�ѻ�ˢ��������������϶�����ͬ�ģ�������ʾ��
        // ����ǲ���ȷ�ģ�����̵�һ�߻ᱻ���죩������������ʱ�ĸķ��ǰ�OnBrushSizeTextChanged�����е�����
        // ע���ˣ������������ı���ʱ�ǲ�����Ļ�ˢ��С�ģ���һ��bug�������޹ؽ�Ҫ��
        wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
        if (toolbar)
        {
            wxSpinCtrl* brushSizeSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
            if (brushSizeSpinCtrl)
            {
                if (e.GetId() == feID_BRUSH_SIZE_X)
                    brushSizeSpinCtrl->SetValue(GetSceneManipulator()->getBrushXSize());
                else
                    brushSizeSpinCtrl->SetValue(GetSceneManipulator()->getBrushZSize());
            }
        }
    }
}

void 
FairyEditorFrame::OnTerrainHeightChanged(wxSpinEvent& e)
{
    Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

    activeAction->setParameter("%TerrainHeight", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnTerrainHeightTextChanged(wxCommandEvent& e)
{
    Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

    activeAction->setParameter("%TerrainHeight", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnUpdateTerrainHeightSpin(wxUpdateUIEvent& e)
{
    // ֻ��SetTerrainHeightAction�����ˣ�����˵��ſ���
    e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
        GetSceneManipulator()->getActiveAction()->getName() == "SetTerrainHeightAction");

    if (e.GetEnabled())
    {
        wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
        if (toolbar)
        {
            wxSpinCtrl* terrainHeightSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
            if (terrainHeightSpinCtrl)
            {
                Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();
                assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");
                int height = Ogre::StringConverter::parseInt(activeAction->getParameter("%TerrainHeight"));
                terrainHeightSpinCtrl->SetValue(height);
            }
        }
    }
}

void 
FairyEditorFrame::OnTerrainHeightIncrementChanged(wxSpinEvent& e)
{
	Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

	activeAction->setParameter("%TerrainHeightIncrement", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnTerrainHeightIncrementTextChanged(wxCommandEvent& e)
{
	Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

	activeAction->setParameter("%TerrainHeightIncrement", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnUpdateTerrainHeightIncrementSpin(wxUpdateUIEvent& e)
{
	// ֻ��SetTerrainHeightAction�����ˣ�����˵��ſ���
	e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
		GetSceneManipulator()->getActiveAction()->getName() == "SetTerrainHeightAction");

	if (e.GetEnabled())
	{
		wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
		if (toolbar)
		{
			wxSpinCtrl* terrainHeightSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
			if (terrainHeightSpinCtrl)
			{
				Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();
				assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");
				int heightIncrement = Ogre::StringConverter::parseInt(activeAction->getParameter("%TerrainHeightIncrement"));
				terrainHeightSpinCtrl->SetValue(heightIncrement);
			}
		}
	}
}

void 
FairyEditorFrame::OnHeightActionaffectRangeChanged(wxSpinEvent& e)
{
    Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && 
        (activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

 //   activeAction->setParameter("%AffectRange", Ogre::StringConverter::toString(e.GetInt()));
	GetSceneManipulator()->setJunctionWeightAffectRange(e.GetInt());
}

void 
FairyEditorFrame::OnHeightActionaffectRangeTextChanged(wxCommandEvent& e)
{
    Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && 
        (activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

  //  activeAction->setParameter("%AffectRange", Ogre::StringConverter::toString(e.GetInt()));
	GetSceneManipulator()->setJunctionWeightAffectRange(e.GetInt());

}

void 
FairyEditorFrame::OnUpdateHeightActionaffectRangeSpin(wxUpdateUIEvent& e)
{
    // ֻ��SetTerrainHeightAction�����ˣ�����˵��ſ���
    e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
        (GetSceneManipulator()->getActiveAction()->getName() == "RaiseHeightAction" ||
          GetSceneManipulator()->getActiveAction()->getName() == "LowerHeightAction"));

    if (e.GetEnabled())
    {
        wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
        if (toolbar)
        {
            wxSpinCtrl* terrainHeightSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
            if (terrainHeightSpinCtrl)
            {
                Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();
                assert (activeAction && 
                    (activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

           //     int height = Ogre::StringConverter::parseInt(activeAction->getParameter("%AffectRange"));
				int height = GetSceneManipulator()->getJunctionWeightAffectRange();
                terrainHeightSpinCtrl->SetValue(height);
            }
        }
    }
}

void 
FairyEditorFrame::OnHeightActionExpValueTextChanged(wxCommandEvent& e)
{
	Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && 
		(activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

	//activeAction->setParameter("%ExponentValue", e.GetString().c_str());
	GetSceneManipulator()->setJunctionWeightExpValue(Ogre::StringConverter::parseReal(e.GetString().c_str()));
}

void 
FairyEditorFrame::OnUpdateHeightActionExpValue(wxUpdateUIEvent& e)
{
	// ֻ��SetTerrainHeightAction�����ˣ�����˵��ſ���
	e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
		(GetSceneManipulator()->getActiveAction()->getName() == "RaiseHeightAction" ||
		GetSceneManipulator()->getActiveAction()->getName() == "LowerHeightAction"));	
}

void 
FairyEditorFrame::OnBlurAmountTextChanged(wxCommandEvent& e)
{
	Fairy::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
	pfm->setPostFilterParameter("Floodlighting", "BlurAmount", e.GetString().c_str());
}

void 
FairyEditorFrame::OnShineAmountTextChanged(wxCommandEvent& e)
{
	Fairy::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
	pfm->setPostFilterParameter("Floodlighting", "ShineAmount", e.GetString().c_str());
}

void 
FairyEditorFrame::OnUpdateFloodlightingControls(wxUpdateUIEvent& e)
{
	Fairy::SceneManipulator *manipulator = GetSceneManipulator();
	e.Enable(
        manipulator &&
        manipulator->getPostFilterManager()->hasPostFilter("Floodlighting") &&
        manipulator->getPostFilterManager()->getPostFilterEnabled("Floodlighting"));
}

void FairyEditorFrame::applyCurDiffuseColour()
{
	Ogre::ColourValue curValue = GetSceneManipulator()->getDiffuseColour();
	mSmartColourDialog->applyCurDiffuseColour(curValue);
}

void FairyEditorFrame::copyPaintAction(const Ogre::String& textureName)
{
	if (mBrushSelector)
	{
		mBrushSelector->setBrushByTexName(textureName);
	}
}

void
FairyEditorFrame::OnHeightAdjustSpeedTextChanged(wxCommandEvent& e)
{
	Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && 
		(activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction" ||
		activeAction->getName() == "SmoothHeightAction" || activeAction->getName() == "TrueSmoothHeightAction"||
		activeAction->getName() == "DragTerrainColourAction"));

	GetSceneManipulator()->setHeightAdjustSpeed(Ogre::StringConverter::parseReal(e.GetString().c_str()));
}

void 
FairyEditorFrame::OnUpdateHeightAdjustSpeed(wxUpdateUIEvent& e)
{
	// ֻ��SetTerrainHeightAction�����ˣ�����˵��ſ���
	e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
		(GetSceneManipulator()->getActiveAction()->getName()== "RaiseHeightAction" ||
		GetSceneManipulator()->getActiveAction()->getName() == "LowerHeightAction" ||
		GetSceneManipulator()->getActiveAction()->getName() == "SmoothHeightAction" ||
	    GetSceneManipulator()->getActiveAction()->getName() == "TrueSmoothHeightAction"||
		 GetSceneManipulator()->getActiveAction()->getName() == "DragTerrainColourAction"));	
}

void
FairyEditorFrame::OnShowStatPanel(wxCommandEvent& e)
{
    GetSceneManipulator()->getFrameStatsListener()->showStatPanel(
        !GetSceneManipulator()->getFrameStatsListener()->isStatPanelShown());
}

void
FairyEditorFrame::OnUpdateShowStatPanel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getFrameStatsListener()->isStatPanelShown());
}

void
FairyEditorFrame::OnShowLogoPanel(wxCommandEvent& e)
{
    GetSceneManipulator()->getFrameStatsListener()->showLogoPanel(
        !GetSceneManipulator()->getFrameStatsListener()->isLogoPanelShown());
}

void
FairyEditorFrame::OnUpdateShowLogoPanel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getFrameStatsListener()->isLogoPanelShown());
}

void
FairyEditorFrame::OnToggleToolWindows(wxCommandEvent& e)
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
FairyEditorFrame::OnUpdateToggleToolWindows(wxUpdateUIEvent& e)
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
    e.SetText((shown ? _("[&A]��ʾ���й�����") : _("[&A]�������й�����")) + wxString(wxT("\tAlt-Enter")));
}

void
FairyEditorFrame::OnFullScreen(wxCommandEvent& e)
{
    ShowFullScreen(!IsFullScreen());
}

void
FairyEditorFrame::OnUpdateFullScreen(wxUpdateUIEvent& e)
{
    e.Check(IsFullScreen());
}

static const char *gActionNames[] = {
    "DragHeightAction",
    "RaiseHeightAction",
    "LowerHeightAction",
    "SmoothHeightAction",
	/*"TrueSmoothHeightAction",*/
    "LevelOffHeightAction",
    "SetTerrainHeightAction",
	"DragTerrainColourAction",
    "TerrainLayerOneEraserAction",
    "ManipObjectAction",
    "FlipDiagonalAction",
    "DisableDecalAction",
    "TerrainSelectionAction",
};

void
FairyEditorFrame::OnSelectAction(wxCommandEvent& e)
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
FairyEditorFrame::OnUpdateSelectAction(wxUpdateUIEvent& e)
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
FairyEditorFrame::OnTerrainLightmapOption(wxCommandEvent& e)
{
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	Fairy::TerrainData* pTerrainData = pTerrain->getData();
	TerrainLightmapOptionDialog dialog(this, pTerrain->getLightmapQuality());
	if (dialog.ShowModal() == wxID_OK)
	{		
		if (dialog.mIsLightmapUpdated)
		{
			if (!pTerrainData->mLightmapImage)
				pTerrainData->mLightmapImage = new Ogre::Image;
			*pTerrainData->mLightmapImage = dialog.mLightmapImage;
			mIsTerrainLightmapUpdated = true;
		}

		pTerrain->setLightmapQuality(dialog.mLightmapQuality);		
		pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());

		switch (pTerrain->getLightmapQuality())
		{
		case Fairy::Terrain::LMQ_NONE:
			WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("None"), _T("Startup.ini"));
			break;
		case Fairy::Terrain::LMQ_LOW:
			WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("Low"), _T("Startup.ini"));
			break;
		case Fairy::Terrain::LMQ_HIGH:
			WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("High"), _T("Startup.ini"));
			break;
		}
	}
	dialog.Destroy();
}

void 
FairyEditorFrame::OnTerrainLightmapHide(wxCommandEvent& e)
{	
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	pTerrain->setLightmapQuality(Fairy::Terrain::LMQ_NONE);
	pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
	WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("None"), _T("Startup.ini"));
}
void 
FairyEditorFrame::OnUpdateTerrainLightmapHide(wxUpdateUIEvent& e)
{	
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	e.Check(pTerrain->getLightmapQuality() == Fairy::Terrain::LMQ_NONE);
}
void 
FairyEditorFrame::OnTerrainLightmapLowQuality(wxCommandEvent& e)
{		
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	pTerrain->setLightmapQuality(Fairy::Terrain::LMQ_LOW);
	pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
	WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("Low"), _T("Startup.ini"));
}
void 
FairyEditorFrame::OnUpdateTerrainLightmapLowQuality(wxUpdateUIEvent& e)
{
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	e.Check(pTerrain->getLightmapQuality() == Fairy::Terrain::LMQ_LOW);
}
void 
FairyEditorFrame::OnTerrainLightmapHighQuality(wxCommandEvent& e)
{
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	pTerrain->setLightmapQuality(Fairy::Terrain::LMQ_HIGH);
	pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
	WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("High"), _T("Startup.ini"));
}
void 
FairyEditorFrame::OnUpdateTerrainLightmapHighQuality(wxUpdateUIEvent& e)
{
	Fairy::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	e.Check(pTerrain->getLightmapQuality() == Fairy::Terrain::LMQ_HIGH);
}

void 
FairyEditorFrame::OnTerrainSurfaceParameters(wxCommandEvent& e)
{
    TerrainSurfaceParametersDialog dlg(this, wxID_ANY,
        _("Terrain Surface Parameters"), GetSceneManipulator());
    if (dlg.ShowModal() == wxID_OK)
    {       
    }
}

void
FairyEditorFrame::OnSceneAnimationQuality(wxCommandEvent& e)
{
    long level = static_cast<long>(GetSceneManipulator()->getAnimationQuality()) + 50;
    long newLevel = wxGetNumberFromUser(
        _("Please input scene animation level"),
        _("�����ȼ�:"),
        _("Set scene animation level"),
        level, 0, 100,
        this);
    if (newLevel >= 0 && newLevel != level)
    {
        GetSceneManipulator()->setAnimationQuality(static_cast<Ogre::Real>(newLevel - 50));
    }
}

void
FairyEditorFrame::OnObjectCreateLevel(wxCommandEvent& e)
{
    long level = static_cast<long>(GetSceneManipulator()->getCreateLevel());
    long newLevel = wxGetNumberFromUser(
        _("Please input object create level"),
        _("���崴���ȼ�:"),
        _("Set object create level"),
        level, -50, 50,
        this);
    if (newLevel != level)
    {
        GetSceneManipulator()->setCreateLevel(static_cast<Ogre::Real>(newLevel));
    }
}

void 
FairyEditorFrame::OnSelectBrushShape(wxCommandEvent& e)
{
	int id = e.GetId();

	if (GetSceneManipulator())
	{
		if (id == feID_BOX_BRUSH)
		{
			GetSceneManipulator()->setBrush("BoxBrushShape", 
				GetSceneManipulator()->getBrushXSize(), GetSceneManipulator()->getBrushZSize());
		}
		else if (id == feID_ELLIPSE_BRUSH)
		{
			GetSceneManipulator()->setBrush("EllipseBrushShape", 
				GetSceneManipulator()->getBrushXSize(), GetSceneManipulator()->getBrushZSize());
		}
	}	
}

void 
FairyEditorFrame::OnUpdateSelectBrushShape(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);

	int id = e.GetId();

	if (GetSceneManipulator())
	{
		if (id == feID_BOX_BRUSH)
		{
			e.Check(GetSceneManipulator()->getBrushType() == "BoxBrushShape");
		}
		else if (id == feID_ELLIPSE_BRUSH)
		{
			e.Check(GetSceneManipulator()->getBrushType() == "EllipseBrushShape");
		}
	}
}

void
FairyEditorFrame::OnAlignPosition(wxCommandEvent& e)
{
    if (GetSceneManipulator()->getPositionAlignGranularity() > 0)
        GetSceneManipulator()->setPositionAlignGranularity(0);
    else
        GetSceneManipulator()->setPositionAlignGranularity(1);
}

void
FairyEditorFrame::OnUpdateAlignPosition(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getPositionAlignGranularity() > 0);
}

void 
FairyEditorFrame::OnFrameRateTextChanged(wxCommandEvent& e)
{
	unsigned long frameRate = Ogre::StringConverter::parseUnsignedLong(e.GetString().c_str());

	if (frameRate < 0)
		frameRate = 0;

	if (mCanvas)
		mCanvas->setTargetFPS(frameRate);
}

void 
FairyEditorFrame::OnPreferSetting(wxCommandEvent& e)
{
    SettingsDialog dialog(this);
    dialog.ShowModal();
}


void 
FairyEditorFrame::GetSelectedTerrainInfo(Ogre::String &infoString)
{
    Ogre::Vector3 mousePosition;
    wxPoint currentMousePos = mCanvas->mCurrentMousePos;

    bool intersected = GetSceneManipulator()->getTerrainIntersects(currentMousePos.x, currentMousePos.y, mousePosition);

    if (intersected)
    {        
        if (GetSceneManipulator()->getActiveAction() &&
            GetSceneManipulator()->getActiveAction()->getName() == "TerrainSelectionAction")
        {
            infoString += GetSceneManipulator()->getActiveAction()->getParameter("%JunctionHeightInfo").c_str();
			infoString += GetSceneManipulator()->getActiveAction()->getParameter("%JunctionDiffuseInfo").c_str();
            infoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridTextureInfo").c_str();
            infoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridDiagonalInfo").c_str();			

			mInfoString.clear();

			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteDiagonalInfo") == "true")
				mInfoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridDiagonalInfo").c_str();			
			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteTextureInfo") == "true")
				mInfoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridTextureInfo").c_str();
			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteDiffuseInfo") == "true")
				mInfoString +=GetSceneManipulator()->getActiveAction()->getParameter("%JunctionDiffuseInfo").c_str();
			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteHeightInfo") == "true")
				mInfoString += GetSceneManipulator()->getActiveAction()->getParameter("%JunctionHeightInfo").c_str();
		}
    }  
}

void
FairyEditorFrame::OnCut(wxCommandEvent& event)
{


    Ogre::String infoString;

    Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();

    if (currentAction && currentAction->getName() == "ManipObjectAction")
    {
        GetSceneManipulator()->getSelectedObjectInfo(infoString);

        wxTextDataObject *data = new wxTextDataObject(infoString);
        wxTheClipboard->SetData(data);

        // ɾ��ԭ�е�����
        GetSceneManipulator()->removeSelectedObjects();
    }    
}

void 
FairyEditorFrame::OnCopy(wxCommandEvent& event)
{
    Ogre::String infoString;

    Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();
	mInfoString.clear();

    if (currentAction)
    {
        // �ڲ�ͬ�Ĳ�����ʹ�õĿ������ǲ�ͬ����Ϣ
        if (currentAction->getName() == "ManipObjectAction")
		{
			GetSceneManipulator()->getSelectedObjectInfo(infoString);
			mManipInfoString = infoString;
		}

        else if (currentAction->getName() == "TerrainSelectionAction")
        {
			// �Ȼ�ȡ���ݣ������á������󡱵ı�־
            GetSelectedTerrainInfo(infoString);
			currentAction->setParameter("%AfterCopy", "true");
        }
		else if (currentAction->getName() == "DragTerrainColourAction")
		{
			Fairy::JunctionSelection* selection = static_cast<Fairy::JunctionSelection*>(GetSceneManipulator()->_getSelection("JunctionSelection"));
			const Fairy::JunctionSelection::JunctionMap& junctions = selection->getJunctions();
			if ((int)junctions.size() >0)
			{
				Fairy::JunctionSelection::JunctionMap::const_iterator beginJunction = junctions.begin();
				Fairy::JunctionSelection::JunctionMap::const_iterator endJunction = junctions.end();
				--endJunction;

				int px = (int)((beginJunction->second.x+endJunction->second.x)/2);
				int pz = (int)((beginJunction->second.z+endJunction->second.z)/2); 
				Ogre::ColourValue gridColor = GetSceneManipulator()->getTerrainData()->getDiffuseInfo(px,pz);
				GetSceneManipulator()->setDiffuseColour(gridColor);
				this->applyCurDiffuseColour();
			}
		}
		else if (currentAction->getName() == "SimplePaintAction" || currentAction->getName() == "AutoTexPaintAction")
		{
			Fairy::GridSelection* selection = NULL;				
			if(currentAction->getName() == "SimplePaintAction")
			{
				Fairy::SimplePaintAction* pSimpleAction = dynamic_cast<Fairy::SimplePaintAction*>(GetSceneManipulator()->getActiveAction());
				selection = pSimpleAction->getHintModified();
			}	
			else if (currentAction->getName() == "AutoTexPaintAction")
			{
				Fairy::AutoTexPaintAction* pAutoTexAction = dynamic_cast<Fairy::AutoTexPaintAction*>(GetSceneManipulator()->getActiveAction());
				selection = pAutoTexAction->getHintModified();
			}

			const Fairy::GridSelection::GridMap& grids = selection->getGrids();
			if ((int)grids.size()<=0)
			{
				return;
			}

			std::pair<int, int> MinBindPoint ;
			std::pair<int, int> MaxBindPoint ;
			for (Fairy::GridSelection::GridMap::const_iterator it = grids.begin(); it != grids.end(); ++it)
			{
				const Fairy::GridSelection::Grid& grid = it->second;
				if(it == grids.begin())
				{
					MinBindPoint = std::pair<int, int>(grid.x,grid.z);
					MaxBindPoint = std::pair<int, int>(grid.x,grid.z);
				}
				else
				{
					MinBindPoint.first = MinBindPoint.first < grid.x ? MinBindPoint.first :  grid.x  ; 
					MinBindPoint.second = MinBindPoint.second < grid.z ? MinBindPoint.second :  grid.z  ; 
					MaxBindPoint.first = MaxBindPoint.first > grid.x ? MaxBindPoint.first :  grid.x  ; 
					MaxBindPoint.second = MaxBindPoint.second > grid.z ? MaxBindPoint.second :  grid.z  ; 
				}					
			}

			int px = (int)((MinBindPoint.first+MaxBindPoint.first)/2);
			int pz = (int)((MinBindPoint.second+MaxBindPoint.second)/2); 

			size_t index = GetSceneManipulator()->getTerrainData()->_getGridIndex(px, pz);

			Fairy::GridSelection::GridMap::const_iterator findIter = grids.find(index);
			if (findIter != grids.end())
			{
				Fairy::GridSelection::Grid gridInfo = findIter->second;

				int id = gridInfo.info.layers[0].pixmapId;
				if (mCanvas->IsShiftDown())
				{
					id = gridInfo.info.layers[1].pixmapId;
				}
				if (id<1)
					return;

				Fairy::TerrainData::Pixmap pixmap = GetSceneManipulator()->getTerrainData()->mPixmaps[id-1];
				Ogre::String layerTexName = GetSceneManipulator()->getTerrainData()->mTextures[ pixmap.textureId ];
				this->copyPaintAction(layerTexName);
			}

			wxPoint pt = mCanvas->mCurrentMousePos;
			Fairy::PaintAction* pAction = dynamic_cast<Fairy::PaintAction*>(GetSceneManipulator()->getActiveAction());
			if(pAction)
			{
				pAction->onMotion(pt.x,pt.y);
			}
		}
		// Ctrl + C // ���õ��θ߶�״̬�� // ������ǰ���λ�ô����θ߶� 
		else if(currentAction->getName() == "SetTerrainHeightAction")
		{
			currentAction->setParameter("%CopyCurrentTerrainHeight","true");
		}

        wxTextDataObject *data = new wxTextDataObject(infoString);
        wxTheClipboard->SetData(data);
    }   
}

void 
FairyEditorFrame::OnPaste(wxCommandEvent& event)
{
    // �õ���굱ǰ�����Ľ��ӵ�
    Ogre::Vector3 mousePosition;
    wxPoint currentMousePos = mCanvas->mCurrentMousePos;

    bool intersected = GetSceneManipulator()->getTerrainIntersects(currentMousePos.x, currentMousePos.y, mousePosition);

    if (intersected)
    {
        wxTextDataObject data;
        if (wxTheClipboard->IsSupported(data.GetFormat())
            && wxTheClipboard->GetData(data))
        {
            Ogre::String text = data.GetText().c_str();

            if (!text.empty())
            {
                Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();

                // ��ͬ��actionճ����ͬ�Ķ���
                if (currentAction && currentAction->getName() == "TerrainSelectionAction")
                {
                    currentAction->setParameter("%CurrentMousePosition", Ogre::StringConverter::toString(mousePosition));
                }
                else if (currentAction && currentAction->getName() == "ManipObjectAction")

				{
					mousePosition = GetSceneManipulator()->_alignPosition(mousePosition,1.0f);
                    // ճ��ojbect
                    GetSceneManipulator()->parseStringAndCreateObject(text, mousePosition);
				}

            }
        }
    }    
}

void
FairyEditorFrame::OnSaveTerrainInfoToFile(wxCommandEvent& e)
{
	if (!mInfoString.empty())
	{
		wxFileDialog dialog(this, _("�ѵ�����Ϣ���浽�ļ�"), wxT(""), wxT(""), _("�����ļ� (%s)|%s"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile fileTerrainInfo;
				if (fileTerrainInfo.Create(dialog.GetPath(),true,wxS_IWUSR |wxS_IWGRP |wxS_IWOTH))
				{
					fileTerrainInfo.Write(mInfoString.c_str());
					fileTerrainInfo.Close();
					wxMessageBox(_("�����ļ�����ɹ�"));
				}
				else
					wxMessageBox(_("�����ļ�����ʧ��"));
			}

		}
	}
}

void 
FairyEditorFrame::OnUpdateSaveTerrainInfoToFile(wxUpdateUIEvent& e)
{
    e.Enable(true);
}

void
FairyEditorFrame::OnLoadTerrainInfoFromFile(wxCommandEvent& e)
{
    wxFileDialog dialog(this, _("���ص����ļ�"), wxT(""), wxT(""), _("�����ļ� (%s)|%s"), 0);

    if (dialog.ShowModal() == wxID_OK)
    {
        if (!dialog.GetPath().IsEmpty())
        {
            wxFile fileTerrainInfo;            
            fileTerrainInfo.Open(dialog.GetPath(),wxFile::read);
			wxString infoString;
			
			wxChar* buf = infoString.GetWriteBuf(fileTerrainInfo.Length()+1);
			
            fileTerrainInfo.Read((void*)buf,fileTerrainInfo.Length());
			Ogre::String ifStr = infoString;
            fileTerrainInfo.Close();

            wxMessageBox(_("���ص����ļ��ɹ�"));

			GetSceneManipulator()->setActiveAction("FixPaintAction");
			GetSceneManipulator()->getActiveAction()->setParameter("%InfoString",ifStr);

			mIsTerrainLightmapUpdated = false;
        }

    }
}

void 
FairyEditorFrame::OnUpdateLoadTerrainInfoFromFile(wxUpdateUIEvent& e)
{
    e.Enable(true);
}


void
FairyEditorFrame::OnSaveManipInfoToFile(wxCommandEvent& e)
{
	if (!mManipInfoString.empty())
	{
		wxFileDialog dialog(this, _("�������������Ϣ���ļ�"), wxT(""), wxT(""), _("MANIPOBJINFO files (*.manipobjinfo)|*.manipobjinfo"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile fileManipInfo;
				if (fileManipInfo.Create(dialog.GetPath(),true,wxS_IWUSR |wxS_IWGRP |wxS_IWOTH))
				{
					fileManipInfo.Write(mManipInfoString.c_str());
					fileManipInfo.Close();
					wxMessageBox(_("����������Ϣ�ļ�����ɹ�"));
				}
				else
					wxMessageBox(_("����������Ϣ�ļ�����ʧ��"));
			}

		}
	}
}

void 
FairyEditorFrame::OnUpdateSaveManipInfoToFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
FairyEditorFrame::OnLoadManipInfoFromFile(wxCommandEvent& e)
{
	wxFileDialog dialog(this, _("���ز���������Ϣ�ļ�"), wxT(""), wxT(""), _("MANIPOBJINFO files (*.manipobjinfo)|*.manipobjinfo"), 0);

	if (dialog.ShowModal() == wxID_OK)
	{
		if (!dialog.GetPath().IsEmpty())
		{
			wxFile fileManipInfo;            
			fileManipInfo.Open(dialog.GetPath(),wxFile::read);
			wxString infoString;

			wxChar* buf = infoString.GetWriteBuf(fileManipInfo.Length()+1);

			fileManipInfo.Read((void*)buf,fileManipInfo.Length());
			Ogre::String ifStr = infoString;
			fileManipInfo.Close();

			wxMessageBox(_("���ز���������Ϣ�ļ��ɹ�"));

			GetSceneManipulator()->setActiveAction("FixManipObjAction");
			GetSceneManipulator()->getActiveAction()->setParameter("%InfoString",ifStr);
		}

	}
}

void 
FairyEditorFrame::OnUpdateLoadManipInfoFromFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
FairyEditorFrame::OnSaveEnviromentInfoToFile(wxCommandEvent& e)
{
	// ��ȡ��ǰ�����еĻ�����Ϣ
	Fairy::SceneInfo *sceneInfo = GetSceneManipulator()->getSceneInfo();
	assert (sceneInfo);

	wxString wrongInfoStr;
	wrongInfoStr.Empty();

	wxString enviInfoStr;
	enviInfoStr.Empty();

	// ��ȡ������Ϣ
	const Fairy::ObjectPtr envObject = sceneInfo->findObjectByName(Fairy::FairyAutoEnviromentObjectName);

	if (!envObject)
	{
		wrongInfoStr << _("������������ƴ���\n")
			<< _("�������������ӦΪ#FairyEnviroment\n");  
	}
	else
	{
		enviInfoStr << "enviroment\n";

		const Fairy::PropertyList& properties = envObject->getProperties();
        for (Fairy::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const Fairy::PropertyDef& propertyDef = *it;

            // �����泡����������Ϣ
            if (propertyDef.name != "weather.particle system name" &&
                propertyDef.name != "weather.render weather")
            {
                enviInfoStr << propertyDef.name.c_str() << '=' 
                    << envObject->getPropertyAsString(propertyDef.name).c_str() << '\n';            
            }
        }
	}

	// ��һ����Դ����Ϣ
	const Fairy::ObjectPtr mainLightOneObject = sceneInfo->findObjectByName(Fairy::FairyAutoMainLightOneObjectName);

	if (!mainLightOneObject)
	{
		wrongInfoStr << _("��һ������Դ�����ƴ���\n")
			<< _("��һ������Դ������ӦΪ#FairyMainLightOne\n");  
	}
	else
	{
		enviInfoStr << "main_light_one\n";

		const Fairy::PropertyList& properties = mainLightOneObject->getProperties();
        for (Fairy::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const Fairy::PropertyDef& propertyDef = *it;
            enviInfoStr << propertyDef.name.c_str() << '=' 
				<< mainLightOneObject->getPropertyAsString(propertyDef.name).c_str() << '\n';            
        }
	}

	// ��ȡ�ڶ�����Դ����Ϣ
	const Fairy::ObjectPtr mainLightTwoObject = sceneInfo->findObjectByName(Fairy::FairyAutoMainLightTwoObjectName);

	if (!mainLightTwoObject)
	{
		wrongInfoStr << _("�ڶ�������Դ�����ƴ���\n")
			<< _("�ڶ�������Դ������ӦΪ#FairyMainLightTwo\n");  
	}
	else
	{
		enviInfoStr << "main_light_two\n";

		const Fairy::PropertyList& properties = mainLightTwoObject->getProperties();
        for (Fairy::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const Fairy::PropertyDef& propertyDef = *it;
            enviInfoStr << propertyDef.name.c_str() << '=' 
				<< mainLightTwoObject->getPropertyAsString(propertyDef.name).c_str() << '\n';           
        }
	}	

	if (!wrongInfoStr.IsEmpty())
	{
		wxMessageBox(wrongInfoStr);
	}
	else
	{
		wxFileDialog dialog(this, _T("���滷����Ϣ���ļ�"), wxT(""), wxT(""), _("Enviroment infomation files (*.envinfo)|*.envinfo"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile infoFile;
				if (infoFile.Create(dialog.GetPath(),true,wxS_IWUSR |wxS_IWGRP |wxS_IWOTH))
				{
					infoFile.Write(enviInfoStr.c_str());
					infoFile.Close();
					wxMessageBox(_("���滷����Ϣ�ļ��ɹ�"));
				}
				else
					wxMessageBox(_("���滷����Ϣ�ļ�ʧ��"));
			}

		}
	}	
}

void 
FairyEditorFrame::OnUpdateSaveEnviromentInfoToFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
FairyEditorFrame::OnLoadEnviromentInfoFromFile(wxCommandEvent& e)
{
	wxString wrongInfoStr;
	wrongInfoStr.Empty();

	// ��ȡ��ǰ�����еĻ�����Ϣ
	Fairy::SceneInfo *sceneInfo = GetSceneManipulator()->getSceneInfo();
	assert (sceneInfo);

	Fairy::ObjectPtr enviObject = sceneInfo->findObjectByName(Fairy::FairyAutoEnviromentObjectName);

	if (!enviObject)
	{
		wrongInfoStr << _("�����������ƴ���!\n")
			<< _("�������������ӦΪ #FairyEnviroment\n");  
	}

	Fairy::ObjectPtr mainLightOneObject = sceneInfo->findObjectByName(Fairy::FairyAutoMainLightOneObjectName);

	if (!mainLightOneObject)
	{
		wrongInfoStr << _("��һ������Դ�����ƴ���\n")
			<< _("��һ������Դ������ӦΪ#FairyMainLightOne\n");  
	}

	Fairy::ObjectPtr mainLightTwoObject = sceneInfo->findObjectByName(Fairy::FairyAutoMainLightTwoObjectName);

	if (!mainLightTwoObject)
	{
		wrongInfoStr << _("�ڶ�������Դ�����ƴ���\n")
			<< _("�ڶ�������Դ������ӦΪ#FairyMainLightTwo\n");  
	}

	if (!wrongInfoStr.IsEmpty())
	{
		wxMessageBox(wrongInfoStr);
	}
	else
	{
		wxFileDialog dialog(this, _("���ػ�����Ϣ�ļ�"), wxT(""), wxT(""), _("Enviroment infomation files (*.envinfo)|*.envinfo"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile infoFile;            
				infoFile.Open(dialog.GetPath(),wxFile::read);
				wxString infoString;

				wxChar* buf = infoString.GetWriteBuf(infoFile.Length()+1);

				infoFile.Read((void*)buf,infoFile.Length());

				wxMessageBox(_("���ػ�����Ϣ�ļ��ɹ�"));

				// ���û�����Ϣ
				Ogre::MemoryDataStream stream((void *)(infoString.c_str()), infoFile.Length(), false);
				Ogre::String line;
				Fairy::ObjectPtr currentObj;
				while(!stream.eof())
				{
					line = stream.getLine();

					if (!line.empty())
					{
						if (line == "enviroment")
						{
							currentObj = enviObject;
						}
						else if (line == "main_light_one")
						{
							currentObj = mainLightOneObject;
						}
						else if (line == "main_light_two")
						{
							currentObj = mainLightTwoObject;
						}
						else
						{
							Ogre::StringVector paraVec = Ogre::StringUtil::split(line, "=");
							assert (paraVec.size() == 2);
							currentObj->setPropertyAsString(paraVec[0], paraVec[1]);
						}
					}					
				}

				infoFile.Close();
			}
		}
	}	
}

void 
FairyEditorFrame::OnUpdateLoadEnviromentInfoFromFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

// ����߶�ͼ
void
FairyEditorFrame::OnImportHeightMap(wxCommandEvent& e)
{
    mSceneInfoDlg->ImportHTimage();
}

void 
FairyEditorFrame::OnUpdateImportHeightMap(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

// �����߶�ͼ
void
FairyEditorFrame::OnExportHeightMap(wxCommandEvent& e)
{
	mSceneInfoDlg->ExportHTimage();
}

void 
FairyEditorFrame::OnUpdateExportHeightMap(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void 
FairyEditorFrame::OnUpdateCutCopyPaste(wxUpdateUIEvent& e)
{
    bool enable = false;

    Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();

    switch (e.GetId())
    {
    case wxID_COPY:
		enable = (currentAction && (currentAction->getName() == "ManipObjectAction" || 
			currentAction->getName() == "TerrainSelectionAction" ||
			currentAction->getName() == "DragTerrainColourAction" ||
			currentAction->getName() == "SimplePaintAction" || 
			currentAction->getName() == "AutoTexPaintAction" ||
			currentAction->getName() == "SetTerrainHeightAction")
			);
		break;
    case wxID_PASTE:
        enable = (currentAction && (currentAction->getName() == "ManipObjectAction" || 
            currentAction->getName() == "TerrainSelectionAction"));
        break;

    case wxID_CUT:
        enable = (currentAction && currentAction->getName() == "ManipObjectAction");
        break;
    }

    enable &= (wxWindow::FindFocus() == mCanvas);

    e.Enable(enable);
}

void
FairyEditorFrame::SetSpinnerValue(int value)
{
    mSpinnerValue = value;
}
int 
FairyEditorFrame::GetSpinnerValue()
{
    return mSpinnerValue;
}

wxString
FairyEditorFrame::GetByAxis()
{
    return mByAxis;
}
void 
FairyEditorFrame::SetByAxis(wxString& strAxis)
{
    mByAxis = strAxis;
}

void 
FairyEditorFrame::SetSnapRotationValue(int value)
{
    mSegment = value;
}

int 
FairyEditorFrame::GetSnapRotationValue()
{
    return mSegment;
}

void 
FairyEditorFrame::SetActiveSnapRotation(bool value)
{
    mActiveSnapRotation = value;
}

bool
FairyEditorFrame::GetActiveSnapRotation()
{
    return mActiveSnapRotation;
}

void 
FairyEditorFrame::OnCreateLiquid(wxCommandEvent& e)
{
	// ������ˮ����Ϣ�ĶԻ���
	LiquidCreatorDialog dlg(this, wxID_ANY, _("����Һ������"));
	if (dlg.ShowModal() == wxID_OK)
	{
		Fairy::Action* action = GetSceneManipulator()->_getAction("CreateComplexObjectAction");
		assert (action);
		action->setParameter("type", "TerrainLiquid");

		action->setParameter("%subdivision", AS_STRING(dlg.mSubDivideTextCtrl->GetValue()));

		action->setParameter("%texture scale", AS_STRING(dlg.mTexScaleTextCtrl->GetValue()));

		action->setParameter("%material", AS_STRING(dlg.mComboBox->GetValue()));

		action->setParameter("%diffuse", AS_STRING(dlg.mDiffuseTextCtrl->GetValue()));

		action->setParameter("%adjustDepth", AS_STRING(dlg.mDepthTextCtrl->GetValue()));

		// �����������崴��action
		GetSceneManipulator()->setActiveAction(action);
	}
}

void 
FairyEditorFrame::OnCreateLight(wxCommandEvent& e)
{
	Fairy::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");
	assert (action);
	action->setParameter("type", "Light");
	action->setParameter("has position", "false");

	action->setParameter("%type", "point");
	action->setParameter("has position", "false");

	// �����������崴��action
	GetSceneManipulator()->setActiveAction(action);
}

void FairyEditorFrame::OnCreateObjectArray(wxCommandEvent& e)
{
	Fairy::Action *currentAction = GetSceneManipulator()->getActiveAction();
	if (currentAction && currentAction->getName() == "ManipObjectAction")
	{
		if (GetSceneManipulator())
		{
			const Fairy::SceneManipulator::ObjectSet &objects = GetSceneManipulator()->getSelectedObjects();

			Fairy::SceneManipulator::ObjectSet::const_iterator i = objects.begin();
			if (i != objects.end())
			{
				const Fairy::ObjectPtr& object = *i;
				if(!object->hasProperty("position"))
				{
					return;
				}

				Ogre::Vector3 size = Ogre::Vector3::ZERO;
				if(object->hasProperty("bounding box"))
				{
					const Ogre::AxisAlignedBox& aabb = Fairy::VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));
					size = aabb.getSize();
				}

				if (object->hasProperty("scale"))
				{
					const Ogre::Vector3 scale = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("scale"));
					size*=scale;
				}
				ObjectArrayCreatorDialog dlg(size, this, wxID_ANY);
				if (dlg.ShowModal() == wxID_OK)
				{
					Ogre::Vector3 axis = Ogre::Vector3::UNIT_X;
					int sel = dlg.m_axisCombo->GetSelection();		
					if (sel == 1)
					{
						axis = Ogre::Vector3::UNIT_Y;
					}
					else if (sel == 2)
					{
						axis = Ogre::Vector3::UNIT_Z;
					}
					
					int count = Ogre::StringConverter::parseInt(dlg.m_countTextCtrl->GetValue().c_str());
					if (count == 0)
					{
						return;
					}
					Ogre::Real finalSize = Ogre::StringConverter::parseReal(dlg.m_sizeTextCtrl->GetValue().c_str());
					GetSceneManipulator()->createObjectArray(count, finalSize, axis);
				}
			}			
		}
	}
}

void 
FairyEditorFrame::SetKeyboardShortcuts(void)
{
	wxAcceleratorEntry entries[4];

	// ���û�ˢѡ��Ŀ�ݼ�
	entries[0].Set(wxACCEL_SHIFT, (int) 'Z', feID_BOX_BRUSH);
	entries[1].Set(wxACCEL_SHIFT, (int) 'X', feID_ELLIPSE_BRUSH);

	// �ָ����������ʼ�߶Ⱥͳ�ʼ�Ƕ�
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
FairyEditorFrame::OnSelectPostFilter(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_POST_FILTER_FIRST;
    if (index >= sizeof(gPostFilterTypes) / sizeof(*gPostFilterTypes))
        return;

    Fairy::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
    pfm->setPostFilterEnabled(gPostFilterTypes[index], !pfm->getPostFilterEnabled(gPostFilterTypes[index]));
}

void
FairyEditorFrame::OnUpdateSelectPostFilter(wxUpdateUIEvent& e)
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
FairyEditorFrame::OnSelectShadowTechnique(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_SHADOWTYPE_NONE;
    if (index >= sizeof(gsShadowTechniques) / sizeof(*gsShadowTechniques))
        return;

	Fairy::SceneManipulator* manipulator = GetSceneManipulator();
    manipulator->getSceneManager()->setShadowTechnique(gsShadowTechniques[index]);
}

void
FairyEditorFrame::OnUpdateSelectShadowTechnique(wxUpdateUIEvent& e)
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
FairyEditorFrame::OnSelectDefaultTextureFiltering(wxCommandEvent& e)
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
FairyEditorFrame::OnUpdateSelectDefaultTextureFiltering(wxUpdateUIEvent& e)
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
FairyEditorFrame::OnSelectSceneCreateLevel(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_CL_LOW;
    if (index >= sizeof(gsCreateLevelOptions) / sizeof(*gsCreateLevelOptions))
        return;

    mCreateLevelOptions = index;
    GetSceneManipulator()->setCreateLevel( gsCreateLevelOptions[index] );
}

void
FairyEditorFrame::OnUpdateSelectSceneCreateLevel(wxUpdateUIEvent& e)
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
FairyEditorFrame::OnUseRealCamera(wxCommandEvent& e)
{
	Fairy::SceneManipulator *manipulator = GetSceneManipulator();

	assert (manipulator);

	if (!mUseRealCamera || manipulator->getCamera()->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
	{
		manipulator->changeCameraType();
	}

	mUseRealCamera = !mUseRealCamera;
	manipulator->setUseRealCameraHeight(mUseRealCamera);
	manipulator->setUseRealCameraAngle(mUseRealCamera);

	if (mUseRealCamera)
	{
		manipulator->setRealCameraHeight(manipulator->getRealCameraHeight());
		manipulator->setRealCameraAngle(manipulator->getRealCameraAngle());

		Ogre::Vector3 currentCameraPos = manipulator->getCamera()->getPosition();
		manipulator->setRealCamera(currentCameraPos.x, currentCameraPos.z);
	}
}
void 
FairyEditorFrame::OnUpdateUseRealCamera(wxUpdateUIEvent& e)
{
	Fairy::SceneManipulator *manipulator = GetSceneManipulator();
	e.Enable(manipulator != 0);
	if (e.GetEnabled())
		e.Check(manipulator->getUseRealCameraHeight() || manipulator->getUseRealCameraAngle());
}

void 
FairyEditorFrame::OnUseSnapRotation(wxCommandEvent& e)
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
FairyEditorFrame::OnUpdateUseSnapRotation(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
	e.Check(GetActiveSnapRotation());
}

void
FairyEditorFrame::OnSceneInfomation(wxCommandEvent& e)
{
    if (NULL == mSceneInfomationDialog)
    {
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;

        GetSize(&width, &height);
        GetPosition(&x, &y);

        mSceneInfomationDialog =
            new SceneInfomationDialog(this, wxID_ANY, _("Scene Infomation"), GetSceneManipulator(), wxPoint(x, y), wxSize(width / 3, height));
    }
    if (!mSceneInfomationDialog->IsShown())
        mSceneInfomationDialog->Show();	
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// ������������
//

// �洢������������.
bool FairyEditorFrame::SaveRegion()
{

	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strRegionFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;


	// ��ϲ������������ļ����֡�
	strRegionFile = strFile + ".Region";

	//�洢������������
	std::string strfile = strRegionFile;
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->SaveData(strfile);
	}


	return true;

}


bool FairyEditorFrame::SaveCRegion()
{

	bool result = false;
	wxString strFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	std::string strfile_noExt = strFile;
	
	PathLib::PathLibRoot* gPathLibRoot = NULL;
	PathLib::PathData* gPathData = NULL;

	gPathLibRoot = new PathLib::PathLibRoot();
    gPathData =  gPathLibRoot->getPathData();
	
	if (m_pRegionEditSelector)
	{
		//�Զ�ת��cRegion
		if( gPathData->getDataFromRegion(strfile_noExt, 8) )
		{
			gPathData->exportPathFile(strfile_noExt + ".CRegion");
			result = true;
		}
	}

	delete gPathLibRoot;
	return result;
}


// ��ȡ������������
bool FairyEditorFrame::ReadRegion()
{
	
	// �õ���������������ļ��� Ȼ���ȡ����
	int iLen = m_lastSceneDirectory.Length() + 1;
	wxString strRegionFile = "";

	strRegionFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strRegionFile.find('.');
	strRegionFile = strRegionFile.Left(iPos);

	strRegionFile = "\\" + strRegionFile;
	strRegionFile =  m_lastSceneDirectory + strRegionFile;
	strRegionFile += ".Region";

	std::string strfile = strRegionFile;
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->LoadData(strfile);
	}

	return true;
}


// ��ղ���������������
void FairyEditorFrame::ClearRegion()
{
	// �������������.
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->ClearData();
	}
}

//
// ������������
//
/////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////
//
// npc ����.
//

// �洢npc
bool FairyEditorFrame::SaveNpc()
{
	{
		// �õ�npc���ļ��� Ȼ���ȡ����
		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strNpcFile = "";

		strNpcFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strNpcFile.find('.');

		// �õ��ļ�����
		strNpcFile = strNpcFile.Left(iPos);
		// ͨ��ӳ���õ��ļ�����
		//strNpcFile = GetSceneName();

		strNpcFile = "\\" + strNpcFile;
		strNpcFile =  m_lastSceneDirectory + strNpcFile;
		strNpcFile += "_monster.ini";//

		std::string strfile = strNpcFile;
		if ((m_pMonsterEditDlg)&&(m_pMonsterEditDlg->m_pMonsterEditAction))
		{
			m_pMonsterEditDlg->SaveMonsterToFile(strfile);
		}

	}

	return true;
}


// ��ȡnpc
bool FairyEditorFrame::ReadNpc()
{
	ClearNpc();

	// �õ�npc���ļ��� Ȼ���ȡ����
	int iLen = m_lastSceneDirectory.Length() + 1;
	std::string strNpcFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	strNpcFile = strNpcFile.substr(0,strNpcFile.find('.'));
	strNpcFile = m_lastSceneDirectory + "\\" + strNpcFile + "_monster.ini";

	if ((m_pMonsterEditDlg)&&(m_pMonsterEditDlg->m_pMonsterEditAction))
	{
		m_pMonsterEditDlg->ReadMonsterFromFile(strNpcFile);
	}

	return true;
}


//���npc
void FairyEditorFrame::ClearNpc()
{
	if (m_pMonsterEditDlg)
	{
		m_pMonsterEditDlg->m_MonsterInfoTable.clear();
		m_pMonsterEditDlg->ClearUI();
		m_pMonsterEditDlg->ClearActionData();
	}
}

//
// npc ����.
//
//////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
//
// ������������
//

// �洢������������.
bool FairyEditorFrame::SaveBuildingWalk()
{
	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strCollisionFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;


	// ��Ͻ�������������ײ�ļ�����
	strCollisionFile =  strFile + ".WCollision";

	// ��������������������.
	CreateBuildingCollisionData();

	// �洢ע����������ļ���
	SaveBuildingCollisionInfo(strCollisionFile);
	
	return true;
}


// �������������
void FairyEditorFrame::ClearBuildingWalk()
{

}

//
// ������������
//
///////////////////////////////////////////////////////////////////////////////////////////////

	

	

//////////////////////////////////////////////////////////////////////////////////////////////
//
// ������༭
//

// ��ȡ������
bool FairyEditorFrame::ReadGrowPoint()
{
	ClearGrowPoint();
	if ((m_pGrowPointEditDlg)&&(m_pGrowPointEditDlg->m_pGrowPointEditAction))
	{

		m_pGrowPointEditDlg->InitAddedGrowPointTree();
		// �õ�npc���ļ��� Ȼ���ȡ����
		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strGrowPointFile = "";

		strGrowPointFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strGrowPointFile.find('.');
		
		// �õ��ļ�����
		strGrowPointFile = strGrowPointFile.Left(iPos);
		// ͨ��ӳ���õ��ļ�����
		//strGrowPointFile = GetSceneName();

		strGrowPointFile = "\\" + strGrowPointFile;
		strGrowPointFile =  m_lastSceneDirectory + strGrowPointFile;
		strGrowPointFile += "_growpoint.txt";

		std::string strfile = strGrowPointFile;
		m_pGrowPointEditDlg->m_pGrowPointEditAction->ReadGrowPointFromFile(strfile);
		
	}
	return true;
}


// �洢
bool FairyEditorFrame::SaveGrowPoint()
{
	if ((m_pGrowPointEditDlg)&&(m_pGrowPointEditDlg->m_pGrowPointEditAction))
	{

		// �õ�npc���ļ��� Ȼ���ȡ����
		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strGrowPointFile = "";

		strGrowPointFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strGrowPointFile.find('.');
		
		//�õ��ļ�����
		strGrowPointFile = strGrowPointFile.Left(iPos);
		// ͨ��ӳ���õ��ļ�����
		//strGrowPointFile = GetSceneName();

		strGrowPointFile = "\\" + strGrowPointFile;
		strGrowPointFile =  m_lastSceneDirectory + strGrowPointFile;
		strGrowPointFile += "_growpoint.txt";

		std::string strfile = strGrowPointFile;
		m_pGrowPointEditDlg->m_pGrowPointEditAction->SaveGrowPointToFile(strfile);
		
	}//



	//// �洢�����ļ���Ŀ¼
	//if ((m_pGrowPointEditDlg)&&(m_pGrowPointEditDlg->m_pGrowPointEditAction))
	//{

	//	// �õ�npc���ļ��� Ȼ��洢����
	//	char szServerDataSavePath[512];
	//	memset(szServerDataSavePath, 0, sizeof(szServerDataSavePath));
	//	::GetPrivateProfileString("path", "pathname", "none", szServerDataSavePath, 512, ".\\server_data_save_path.ini");

	//	int iLen = m_lastSceneDirectory.Length() + 1;
	//	wxString strGrowPointFile = "";

	//	strGrowPointFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	//	int iPos = strGrowPointFile.find('.');
	//	
	//	//strGrowPointFile = strGrowPointFile.Left(iPos);
	//	// ͨ��ӳ���õ��ļ�����
	//	strGrowPointFile = GetSceneName();

	//	strGrowPointFile += "_growpoint.txt";

	//	std::string strfile = "";
	//	strfile = szServerDataSavePath;
	//	strfile += strGrowPointFile;

 //       m_pGrowPointEditDlg->m_pGrowPointEditAction->SaveGrowPointToFile(strfile);
	//}//




	return true;
}


//���
void FairyEditorFrame::ClearGrowPoint()
{
	if (m_pGrowPointEditDlg)
	{
		m_pGrowPointEditDlg->ClearUI();
		if (m_pGrowPointEditDlg->m_pGrowPointEditAction)
		{
			m_pGrowPointEditDlg->m_pGrowPointEditAction->ClearGrowPointData();
		}
	}
}

//
// ������༭
//
//////////////////////////////////////////////////////////////////////////////////////////////


void 
FairyEditorFrame::OnSelectAllObjects(wxCommandEvent& e)
{
	GetSceneManipulator()->setSelectAllObjects(!GetSceneManipulator()->getSelectAllObjects());
}

void 
FairyEditorFrame::OnUpdateSelectAllObjects(wxUpdateUIEvent& e)
{	
	e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getSelectAllObjects());
}

void 
FairyEditorFrame::OnResetCamera(wxCommandEvent& e)
{
	if (GetSceneManipulator())
	{
		if(GetSceneManipulator()->getCamera()->getProjectionType() == Ogre::PT_ORTHOGRAPHIC)
		{
			GetSceneManipulator()->changeCameraType();
		}
		// ʹ������ָ�����ʼ�ĸ߶ȼ��Ƕȣ��������xzλ���������������ʱ������������������ཻ��
		Ogre::Vector3 position;

        Ogre::Ray cameraRay(GetSceneManipulator()->getCamera()->getPosition(), GetSceneManipulator()->getCamera()->getDirection());
        bool hit = GetSceneManipulator()->getTerrainIntersects(cameraRay, position);

		GetSceneManipulator()->setRealCameraHeight(GetSceneManipulator()->getDefaultRealCameraHeight());
		GetSceneManipulator()->setRealCameraAngle(GetSceneManipulator()->getDefaultRealCameraAngle());

		GetSceneManipulator()->setRealCamera(position.x, position.z);
	}
}

void 
FairyEditorFrame::OnEraseTextureByName(wxCommandEvent& e)
{	
    EraseTexDialog dlg(this, wxID_ANY,_("Ҫɾ������������"));
    dlg.Centre();
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString prompt =/* _("The operations before can not be undo,Do you want to continue the operation?")*/wxT("������������Իָ�����ȷ��Ҫ�������в�����");

        int res = wxMessageBox(prompt, /*_("Warning")*/wxT("����"),
            wxYES_NO|wxICON_QUESTION,
            this);

        switch (res)
        {
        default:   
        case wxNO:
            return;

        case wxYES:
            {
                bool succeed = false;
                succeed = GetSceneManipulator()->eraseTerrainPixmapByTextureName(dlg.mComboBox->GetStringSelection().c_str());

                if (!succeed)
                {
                    wxMessageBox(_("The Texture you selected was not painted in this terrain."));
                }
            }
        }
    }
}

void 
FairyEditorFrame::OnEntireTerrainEnableDecal(wxCommandEvent& e)
{	
    Fairy::TerrainData* terrainData = GetSceneManipulator()->getTerrainData();

    for (Fairy::TerrainData::GridInfoArray::iterator it = terrainData->mGridInfos.begin(); it != terrainData->mGridInfos.end(); ++it)
    {
        unsigned int originFlags = (*it).flags;
        
        unsigned int flags = 0;

        if (originFlags & Fairy::TerrainData::GridInfo::FlipDiagonal)
            flags ^= Fairy::TerrainData::GridInfo::FlipDiagonal;

        (*it).flags = flags;
    }

    GetSceneManipulator()->getTerrain()->
        notifyGridInfoModified(0,0,GetSceneManipulator()->getTerrainData()->getXSize(),GetSceneManipulator()->getTerrainData()->getZSize());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �༭npcѲ��
//

	// ��ȡ������������
bool FairyEditorFrame::ReadNpcPatrol()
{
	int iLen = m_lastSceneDirectory.Length() + 1;

	std::string strRegionFile = "";
	strRegionFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	strRegionFile = strRegionFile.substr( 0, strRegionFile.find('.') );
	strRegionFile =  m_lastSceneDirectory + "\\" + strRegionFile + "_patrolpoint.ini";

	if (m_pNpcPatrolEditDlg)
	{
		m_pNpcPatrolEditDlg->LoadData(strRegionFile);
	}

	return true;
	
}

// �洢������������.
bool FairyEditorFrame::SaveNpcPatrol()
{
	{
		// ��ȡ���ļ����֡�
		wxString strFile = "";
		wxString strRegionFile = "";

		int iLen = m_lastSceneDirectory.Length() + 1;
		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');
		
		// �õ�����
		strFile = strFile.Left(iPos);
		// ͨ��ӳ���õ��ļ�����
		strFile = GetSceneName();

		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		// ��ϲ������������ļ����֡�
		strRegionFile = strFile + ".Patrolpoint";

		//�洢������������
		std::string strfile = strRegionFile;
		if (m_pNpcPatrolEditDlg)
		{
			m_pNpcPatrolEditDlg->SaveData(strfile);
		}
	}

	return true;
}

// ��ղ���������������
void FairyEditorFrame::ClearNpcPatrol()
{
	// �������������.
	if (m_pNpcPatrolEditDlg)
	{
		m_pNpcPatrolEditDlg->ClearData();
	}
}

//
// �༭npcѲ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Ѱ·�༭
//

// ��ȡѰ·����
bool FairyEditorFrame::ReadPathFind()
{
	int iLen = m_lastSceneDirectory.Length() + 1;

	std::string strRegionFile = "";
	strRegionFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	strRegionFile = strRegionFile.substr( 0, strRegionFile.find('.') );
	strRegionFile =  m_lastSceneDirectory + "\\" + strRegionFile + ".Dijkstra";

	if (m_pPathFindEditDlg)
	{
		m_pPathFindEditDlg->LoadData(strRegionFile);
	}

	return true;

}

// �洢Ѱ·��.
bool FairyEditorFrame::SavePathFind()
{
	{
		// ��ȡ���ļ����֡�
		wxString strFile = "";
		wxString strRegionFile = "";

		int iLen = m_lastSceneDirectory.Length() + 1;
		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');

		// �õ�����
		strFile = strFile.Left(iPos);
		// ͨ��ӳ���õ��ļ�����
		strFile = GetSceneName();

		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		// ��ϲ������������ļ����֡�
		strRegionFile = strFile + ".Dijkstra";

		//�洢������������
		std::string strfile = strRegionFile;
		if (m_pPathFindEditDlg)
		{
			m_pPathFindEditDlg->SaveData(strfile);
		}
	}

	return true;
}

// ��ղ���������������
void FairyEditorFrame::ClearPathFind()
{
	 //�������������.
	if (m_pPathFindEditDlg)
	{
		m_pPathFindEditDlg->ClearData();
	}
}

//
// Ѱ·�༭
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////
//
// �¼�����༭
//


	// ��ȡ������������
bool FairyEditorFrame::ReadEventArea()
{

	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// ��ϲ������������ļ����֡�
	strSaveFile = strFile + ".area";

	//�洢������������
	std::string strfile = strSaveFile;
	if (m_pEventAreaEditDlg)
	{
		m_pEventAreaEditDlg->LoadData(strfile);

		return true;

	}

	return false;
}

// �洢������������.
bool FairyEditorFrame::SaveEventArea()
{
	{
		// ��ȡ���ļ����֡�
		wxString strFile = "";
		wxString strSaveFile = "";

		int iLen = m_lastSceneDirectory.Length() + 1;
		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');
		
		//strFile = strFile.Left(iPos);
		strFile = GetSceneName();


		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		// ��ϲ������������ļ����֡�
		strSaveFile = strFile + ".area";

		//�洢������������
		std::string strfile = strSaveFile;
		if (m_pEventAreaEditDlg)
		{
			m_pEventAreaEditDlg->SaveData(strfile);
	
		}
	}

	//// �洢�����ļ���Ŀ¼
	//{
	//	// ��ȡ���ļ����� Ȼ��洢����
	//	char szServerDataSavePath[512];
	//	memset(szServerDataSavePath, 0, sizeof(szServerDataSavePath));
	//	::GetPrivateProfileString("path", "pathname", "none", szServerDataSavePath, 512, ".\\server_data_save_path.ini");

	//	int iLen = m_lastSceneDirectory.Length() + 1;
	//	wxString strFile = "";

	//	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	//	int iPos = strFile.find('.');
	//	
	//	//strFile = strFile.Left(iPos);
	//	strFile = GetSceneName();

	//	strFile += ".area";
	//	std::string strSavefile = "";
	//	strSavefile = szServerDataSavePath;
	//	strSavefile += strFile;
	//
	//	if (m_pEventAreaEditDlg)
	//	{
	//		m_pEventAreaEditDlg->SaveData(strSavefile);

	//	}
	//}//

	return true;

}

//���npc
void FairyEditorFrame::ClearEventArea()
{
	if (m_pEventAreaEditDlg)
	{
		m_pEventAreaEditDlg->ClearUI();
		if (m_pEventAreaEditDlg->m_pEditAction)
		{
			m_pEventAreaEditDlg->ClearData();
		}

	}
}

void FairyEditorFrame::ClearDatuPointInfo()
{
    if (mDatuPointEditDialog)
    {
        mDatuPointEditDialog->ClearDatuPointInfo();
    }
}

//
//  �¼�����༭
//
///////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ̯λ��Ϣ
//

//����̯λ��Ϣ
void FairyEditorFrame::CreateStallInfo()
{
	if (m_pStallInfoDlg)
	{
		m_pStallInfoDlg->CreateStallInfo();
	}
}

// ��ȡ̯λ��Ϣ
bool FairyEditorFrame::ReadStallInfo()
{
	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// ��ϲ������������ļ����֡�
	strSaveFile = strFile + ".area";

	//�洢������������
	std::string strfile = strSaveFile;
	if (m_pStallInfoDlg)
	{
		m_pStallInfoDlg->LoadData(strfile);
	}

	return true;
}

// �洢̯λ��Ϣ
bool FairyEditorFrame::SaveStallInfo()
{
	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();


	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// ��ϲ������������ļ����֡�
	strSaveFile = strFile + ".stallInfo";

	//�洢������������
	std::string strfile = strSaveFile;
	if (m_pStallInfoDlg)
	{
		m_pStallInfoDlg->SaveData(strfile);
	}

	return true;
}

//
// ̯λ��Ϣ
//
//////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������Ϣ
//

//�������
void FairyEditorFrame::ClearSoundInfo()
{
	if(mSoundEditDialog)
	{
		mSoundEditDialog->ClearSoundData();
	}//
}

// ��ȡ̯λ��Ϣ
bool FairyEditorFrame::ReadSoundInfo()
{
	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// ��ϲ������������ļ����֡�
	strSaveFile = strFile + ".esd";

	//�洢������������
	std::string strfile = strSaveFile;
	if(mSoundEditDialog)
	{
		mSoundEditDialog->LoadSoundInfoFromFile(strfile);
	}//

	return true;
}

bool FairyEditorFrame::ReadDatuPointInfo()
{
    // ��ȡ���ļ����֡�
    wxString strFile = "";
    wxString strSaveFile = "";

    int iLen = m_lastSceneDirectory.Length() + 1;
    strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
    int iPos = strFile.find('.');

    //strFile = strFile.Left(iPos);
    strFile = GetSceneName();

    strFile = "\\" + strFile;
    strFile =  m_lastSceneDirectory + strFile;

    // ��ϲ������������ļ����֡�
    strSaveFile = strFile + ".datu";

    //�洢������������
    std::string strfile = strSaveFile;
    if(mDatuPointEditDialog)
    {
        mDatuPointEditDialog->LoadDatuPointsFromFile(strfile);
    }//

    return true;
}

// �洢̯λ��Ϣ
bool FairyEditorFrame::SaveSoundInfo()
{
	// ��ȡ���ļ����֡�
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();


	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// ��ϲ������������ļ����֡�
	strSaveFile = strFile + ".esd";

	//�洢������������
	std::string strfile = strSaveFile;
	if(mSoundEditDialog)
	{
		mSoundEditDialog->SaveSoundInfoFromFile(strfile);
	}//

	return true;
}

bool FairyEditorFrame::SaveDatuPointInfo()
{
    // ��ȡ���ļ����֡�
    wxString strFile = "";
    wxString strSaveFile = "";

    int iLen = m_lastSceneDirectory.Length() + 1;
    strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
    int iPos = strFile.find('.');

    //strFile = strFile.Left(iPos);
    strFile = GetSceneName();


    strFile = "\\" + strFile;
    strFile =  m_lastSceneDirectory + strFile;

    // ��ϲ������������ļ����֡�
    strSaveFile = strFile + ".datu";

    //�洢������������
    std::string strfile = strSaveFile;
    if(mDatuPointEditDialog)
    {
        mDatuPointEditDialog->SaveDatuPointsToFile(strfile);
    }//

    return true;
}
//
// ������Ϣ
//
//////////////////////////////////////////////////////////////////////////////////////////////////////




// ��ͼ��ת
void FairyEditorFrame::OnSceneGoto(wxCommandEvent &event)
{
	if( !m_pToolbar ) return;

	float fX = 0;
	float fZ = 0;

	if (mCanvas)
	{
		wxTextCtrl* pTexX = (wxTextCtrl*)m_pToolbar->FindControl(feID_GOTO_X);
		wxTextCtrl* pTexY = (wxTextCtrl*)m_pToolbar->FindControl(feID_GOTO_Y);

		if ((pTexX)&&(pTexY))
		{
			fX = atof(pTexX->GetValue().c_str());
			fZ = atof(pTexY->GetValue().c_str());
			mCanvas->GotoScene(fX, fZ);
		}
	}
}

//�õ���ͼ����
std::string FairyEditorFrame::GetSceneName()
{

	std::map<std::string, std::string> strNameMap;
	// ��ȡ���ļ����� Ȼ��洢����
	char szInfo[512];
	char szName1[128];
	char szName2[128];
	
	strNameMap.clear();
	FILE* pFile = NULL;
	pFile = ::fopen(".\\��ͼ��ӳ���.txt", "r");
	if (pFile)
	{
		while(!feof(pFile))
		{
			memset(szInfo,  0,  sizeof(szInfo));
			memset(szName1, 0, sizeof(szName1));
			memset(szName2, 0, sizeof(szName2));

			::fgets(szInfo, sizeof(szInfo), pFile);
			sscanf(szInfo, "%s	%s", szName1, szName2);
			strNameMap[szName1] = szName2;
	
		}

		::fclose(pFile);
	}

	int iLen = m_lastSceneDirectory.Length() + 1;
	wxString strFile = "";

	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	std::string strReturn = strFile.c_str();
	std::string strKey    = strReturn;
	if (strNameMap.count(strKey))
	{
		strReturn = strNameMap[strKey];
	}

	return strReturn;

}

void FairyEditorFrame::OnCreateMiniMap(wxCommandEvent& e)
{
    GetSceneManipulator()->setIndicatorNodeVisible(false);

    MiniMapMakerDialog dlg(this, wxID_ANY, _("С��ͼ������"),
        GetSceneManipulator(), m_sceneFilename);
    dlg.ShowModal();   

    GetSceneManipulator()->setIndicatorNodeVisible(true, false);
}

void FairyEditorFrame::OnReadcBrushInfo(wxCommandEvent& e)
{
	int answer = wxMessageBox(_("����ˢ���ļ��Ḳ�ǵ�ǰ�Ķ���ɫ���ݣ�ȷ��Ҫ������"), _("ȷ��"),
		wxYES_NO, this);
	if (answer != wxYES)
	{    
		return;
	}
	//Diffuse info map
	if (GetSceneManipulator()->getTerrainData())
	{
		wxBusyInfo info(wxT("��ȡ�����У����Ժ򡣡���"));

		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strFile = "";

		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');
		strFile = strFile.Left(iPos);

		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		Ogre::String fileName = strFile.c_str();

		if (fileName.empty())
		{
			return;
		}
		Ogre::String diffuseFile = fileName+".cbrush";
		// Load diffuse info
		GetSceneManipulator()->_loadDiffuseInfo(diffuseFile);

		Ogre::String diffuseApplyFile = fileName+".bbrush";
		// Load diffuse apply info
		GetSceneManipulator()->_loadDiffuseApplyInfo(diffuseApplyFile);

		GetSceneManipulator()->updateAllObjectDiffuseByGrid();		
	}	
}