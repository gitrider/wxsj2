// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXSceneCanvas.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/dcclient.h>
#endif

// copy paste
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

#include "WXSceneCanvas.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreTimer.h>
#include <OgreRenderWindow.h>
#include <OgreRenderTargetListener.h>
#include <OgreStringConverter.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

// ----------------------------------------------------------------------------
#include "Core/FrameStatsListener.h"
#include "Core/TerrainData.h"
#include "Core/TerrainData.h"

#include "SceneManipulator.h"
#include "Manip.h"          
#include "Action.h"
#include "HitIndicator.h"
#include "TerrainGrid.h"
#include "d3d9types.h"

#include "ManipObjectAction.h"
#include "WXModifyObjectPropertyOperator.h"
#include "WXOperatorManager.h"
#include "MoveUpDownSettingDialog.h"
#include "WXEditorApp.h"
#include "WXEditorFrame.h"
#include "CEventAreaEditDlg.h"
#include "EventAreaEditAction.h"
#include ".\RegioneditdlgSelector.h"		// ������������༭�Ի���.
#include "SmoothHeightAction.h"
#include "OgreConfigOptionMap.h"
#include "OgreRenderSystem.h"
#include "TerrainSelections.h"
#include "OgreStringConverter.h"
#include "OgrePrerequisites.h"

#include "Core/FairyStaticEntityObject.h"
#include "OgreRectangle.h"
#include "OgreSubEntity.h"
#include "OgreMaterialManager.h"

#include "OgreHardwarePixelBuffer.h"


#include "core/FairyLightObject.h"
#include "Core/FairySceneInfo.h"
// ----------------------------------------------------------------------------
// FairySceneCanvas::UpdateListener class
// ----------------------------------------------------------------------------
using namespace Ogre;
class FairySceneCanvas::UpdateListener : public Ogre::RenderTargetListener
{
protected:
    FairySceneCanvas* mParent;
    unsigned long mLastTime;

public:
    UpdateListener(FairySceneCanvas* parent)
        : mParent(parent)
        , mLastTime(0)
    {
        wxASSERT(parent);
    }

    ~UpdateListener()
    {
        mParent->GetSceneManipulator()->getRenderWindow()->removeListener(this);
    }

    void enable(void)
    {
        mParent->GetSceneManipulator()->getRenderWindow()->addListener(this);
        mLastTime = Ogre::Root::getSingleton().getTimer()->getMicroseconds();
    }

    void disable(void)
    {
        mParent->GetSceneManipulator()->getRenderWindow()->removeListener(this);
    }

    void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
        wxASSERT(mParent->mDragButton == wxMOUSE_BTN_LEFT);
        wxASSERT(mParent->GetActiveAction());
        unsigned long now = Ogre::Root::getSingleton().getTimer()->getMicroseconds();
        unsigned long delta = now - mLastTime;
        mLastTime = now;
        wxPoint pt = wxGetMousePosition();
        pt = mParent->ScreenToClient(pt);
        mParent->GetActiveAction()->onUpdate(pt.x, pt.y, delta / Ogre::Real(1000*1000));
    }

    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
    }
};

// ----------------------------------------------------------------------------
// FairySceneCanvas class
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(FairySceneCanvas, wxWindow)

static const int feID_SNAP_TO_GRID = wxNewId();
static const int feID_MOVE_BY_X =  feID_SNAP_TO_GRID + 1;
static const int feID_MOVE_BY_Y =  feID_SNAP_TO_GRID + 2;
static const int feID_MOVE_BY_Z =  feID_SNAP_TO_GRID + 3;
static const int feID_SNAP_ROTATIONX =  feID_SNAP_TO_GRID + 4;
static const int feID_SNAP_ROTATIONY =  feID_SNAP_TO_GRID + 5;
static const int feID_SNAP_ROTATIONZ =  feID_SNAP_TO_GRID + 6;

static const int feID_TERRAINHEIGHTCHECKBOX = 10000;
static const int feID_TERRAINTEXTURECHECKBOX = 10001;
static const int feID_TERRAINDIAGONALCHECKBOX = 10002;
static const int feID_TERRAINROTATERIGHTANGLE = 10003;
static const int feID_TERRAINDIFFUSECHECKBOX = 10004;

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(FairySceneCanvas, wxWindow)
    EVT_PAINT               (FairySceneCanvas::OnPaint)
    EVT_ERASE_BACKGROUND    (FairySceneCanvas::OnEraseBackground)
    EVT_SIZE                (FairySceneCanvas::OnSize)

    EVT_MOTION              (FairySceneCanvas::OnMouseMove)
    EVT_ENTER_WINDOW        (FairySceneCanvas::OnMouseEnter)
    EVT_LEAVE_WINDOW        (FairySceneCanvas::OnMouseLeave)
    EVT_MIDDLE_DOWN         (FairySceneCanvas::OnMiddleDown)
    EVT_MIDDLE_UP           (FairySceneCanvas::OnMiddleUp)
    EVT_LEFT_DOWN           (FairySceneCanvas::OnLeftDown)
    EVT_LEFT_UP             (FairySceneCanvas::OnLeftUp)
    EVT_MOUSEWHEEL          (FairySceneCanvas::OnMouseWheel)

    EVT_KEY_DOWN            (FairySceneCanvas::OnKeyDown)
    EVT_KEY_UP              (FairySceneCanvas::OnKeyUp)
    EVT_CHAR                (FairySceneCanvas::OnChar)


    EVT_MOUSE_CAPTURE_CHANGED (FairySceneCanvas::OnMouseCaptureChanged)

    EVT_CONTEXT_MENU        (FairySceneCanvas::OnContextMenu)
    EVT_MENU                (feID_SNAP_TO_GRID, OnSnapToGrid)
    EVT_MENU_RANGE          (feID_MOVE_BY_X,feID_MOVE_BY_Z,OnMoveByXYZ)
	EVT_UPDATE_UI_RANGE     (feID_MOVE_BY_X,feID_MOVE_BY_Z,OnUpdateMoveByXYZ)
    EVT_MENU_RANGE          (feID_SNAP_ROTATIONX,feID_SNAP_ROTATIONZ,OnSnapRotation)
	EVT_UPDATE_UI_RANGE     (feID_SNAP_ROTATIONX,feID_SNAP_ROTATIONZ,OnUpdateSnapRotation)
    EVT_MENU                (feID_TERRAINHEIGHTCHECKBOX, OnTerrainPasteSetting)
    EVT_MENU                (feID_TERRAINTEXTURECHECKBOX, OnTerrainPasteSetting)
    EVT_MENU                (feID_TERRAINDIAGONALCHECKBOX, OnTerrainPasteSetting)
	EVT_MENU				(feID_TERRAINROTATERIGHTANGLE, OnTerrainRotateRightAngle)
	EVT_MENU				(feID_TERRAINDIFFUSECHECKBOX, OnTerrainPasteSetting)

    EVT_UPDATE_UI           (feID_TERRAINHEIGHTCHECKBOX, OnUpdateTerrainPasteSetting)
    EVT_UPDATE_UI           (feID_TERRAINTEXTURECHECKBOX, OnUpdateTerrainPasteSetting)
    EVT_UPDATE_UI           (feID_TERRAINDIAGONALCHECKBOX, OnUpdateTerrainPasteSetting)
	EVT_UPDATE_UI			(feID_TERRAINDIFFUSECHECKBOX, OnUpdateTerrainPasteSetting)

    EVT_WINDOW_DESTROY      (FairySceneCanvas::OnDestroy)

	EVT_TIMER(FIX_FRAME_TIMTER, FairySceneCanvas::OnTimer)

END_EVENT_TABLE()

FairySceneCanvas::FairySceneCanvas(wxWindow *parent, wxWindowID id,
                                   const wxPoint& pos, const wxSize& size, long style)
    : wxWindow(parent, id, pos, size, style)
    , mSceneManipulator(NULL)
    , mUpdateListener(NULL)
    , mCameraManip(NULL)
    , mDragButton(wxMOUSE_BTN_NONE)
    , mTerrGrid(NULL)
	, m_pParentFrame(NULL)
	, mLastTime(0)
	, mTargetFPS(0)
{
    objPos.clear();
	mParentFrame = static_cast<wxFrame *>(parent);
	assert (mParentFrame);

	mTimer.SetOwner(this, FIX_FRAME_TIMTER);
}

FairySceneCanvas::~FairySceneCanvas()
{
    if (mSceneManipulator)
    {
        mSceneManipulator->getRenderWindow()->setActive(false);
    }

    delete mUpdateListener;

    if(mTerrGrid)
        delete mTerrGrid;    
}


void FairySceneCanvas::SetParentFrame(FairyEditorFrame* pParentFrame)
{
	m_pParentFrame = pParentFrame;
}


void
FairySceneCanvas::InitScene(Fairy::SceneManipulator* sceneManipulator)
{
	Ogre::RenderSystem* pRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
	const Ogre::ConfigOptionMap& opts = pRenderSystem->getConfigOptions();
	Ogre::NameValuePairList ViewSetting;

	for (Ogre::ConfigOptionMap::const_iterator itopt = opts.begin(); itopt != opts.end(); ++itopt)
	{
		 const Ogre::ConfigOption& option = itopt->second;
		 std::string value = option.currentValue;
		
		 if( option.name == "Anti aliasing" )
		 {
			 if (option.currentValue == "None"){
				 ViewSetting["FSAA"] = Ogre::StringConverter::toString(D3DMULTISAMPLE_NONE);
				 ViewSetting["FSAAQuality"] =StringConverter::toString(0);
			 }
			 else 
			 {
				 D3DMULTISAMPLE_TYPE fsaa = D3DMULTISAMPLE_NONE;
				 DWORD level = 0;

				 if (StringUtil::startsWith(value, "NonMaskable", false))
				 {
					 fsaa = D3DMULTISAMPLE_NONMASKABLE;
					 size_t pos = value.find_last_of(" ");
					 String sNum = value.substr(pos + 1);
					 level = StringConverter::parseInt(sNum);
					 level -= 1;
				 }
				 else if (StringUtil::startsWith(value, "Level", false))
				 {
					 size_t pos = value.find_last_of(" ");
					 String sNum = value.substr(pos + 1);
					 fsaa = (D3DMULTISAMPLE_TYPE)StringConverter::parseInt(sNum);
				 }
				 ViewSetting["FSAA"] = StringConverter::toString(fsaa);
				 ViewSetting["FSAAQuality"] =StringConverter::toString(level);
		
			 }
		 }else
		 ViewSetting[option.name]= option.currentValue;
	}

    mSceneManipulator = sceneManipulator;
    mSceneManipulator->setup(reinterpret_cast<Ogre::ulong>(GetHandle()), &ViewSetting);
    mUpdateListener = new UpdateListener(this);

	Ogre::Root::getSingleton().addFrameListener(this);
	mLastTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
}

Fairy::Action*
FairySceneCanvas::GetActiveAction(void) const
{
    return GetSceneManipulator()->getActiveAction();
}

//////////////////////////////////////////////////////////////////////////

void
FairySceneCanvas::OnPaint(wxPaintEvent &e)
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    if (GetSceneManipulator())
    {
        //GetSceneManipulator()->updateWindow();
		doUpdate();

    }
}

void
FairySceneCanvas::OnEraseBackground(wxEraseEvent &e)
{
    if (!GetSceneManipulator())
    {
        wxWindow::OnEraseBackground(e);
    }
}

void
FairySceneCanvas::OnSize(wxSizeEvent &e)
{
    if (GetSceneManipulator())
    {
        wxSize size = e.GetSize();
        GetSceneManipulator()->resizeWindow(size.x, size.y);
    }
}

int 
FairySceneCanvas::GetAxisIndex(wxString& strAxis)
{
    if(strAxis == "XAxis")
    {
        return 0;
    }
    else if(strAxis == "YAxis")
    {
        return 1;
    }
    else if(strAxis == "ZAxis")
    {
        return 2;
    }

    assert(0);
    return 1;
}

bool
FairySceneCanvas::ProcessArrowKey(int singal)
{
    typedef std::set<Fairy::ObjectPtr> ObjectSet;
    const ObjectSet& objSet = GetSceneManipulator()->getSelectedObjects();

    for(ObjectSet::const_iterator iter = objSet.begin();iter != objSet.end(); iter++)
    {
        Fairy::ObjectPtr object = *iter;
        Ogre::Vector3 oldPosition = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));
        Ogre::Vector3 newPosition = oldPosition;
        FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
        //int index = GetAxisIndex(app->GetMainFrame()->GetByAxis());
        int index = Ogre::Math::Abs(singal) - 1;

		if(!IsCtrlDown())
			newPosition[index] += app->GetMainFrame()->GetSpinnerValue() * singal / Ogre::Math::Abs(singal);
		else
			newPosition[index] += app->GetMainFrame()->GetSpinnerValue() * singal / Ogre::Math::Abs(singal) * GetSceneManipulator()->getBaseScale();

        object->setProperty("position", newPosition);
        GetSceneManipulator()->_fireObjectPropertyChanged(object, "position");

        ObjectPosition::const_iterator it = objPos.find(object);
        if(it == objPos.end())
        {  
            objPos.insert(ObjectPosition::value_type(object,oldPosition));
        }
    }  

	if (GetSceneManipulator()->getActiveAction() && GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction")
	{
		Fairy::ManipObjectAction* manipAction = (Fairy::ManipObjectAction*)(GetSceneManipulator()->getActiveAction());
		if (manipAction)
		{
			manipAction->updateObjectInfo();
			manipAction->updateRangeSphere();
		}
	}

    if (objSet.size() > 0)
        return true;
    else
        return false;
}

void
FairySceneCanvas::OnKeyDown(wxKeyEvent& e)
{
    e.Skip(true);  // default to continue key process

    //POINT pt;
    switch (e.GetKeyCode())
    {
	case WXK_CONTROL:
		if ( mSceneManipulator && mSceneManipulator->getActiveAction())
		{
			const Ogre::String actionName = mSceneManipulator->getActiveAction()->getName();
			if ( actionName == "AutoTexPaintAction" ) //��ˢ��סCTRAL���������漴
			{
				
				mSceneManipulator->getActiveAction()->setParameter("%Random_Texture","ture");
			}
			else if ( IsCapital() && IsLeftCtrlDown() && (actionName == "ManipObjectAction") ) 
			{
				Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
				// ManipObjectAction״̬�£�������ѡ��ѡ״̬
				if((NULL != pAction) && (GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction"))
				{
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->setIsUseDefinedSelecting(true);
				}
			}
		}
		break;
    case WXK_ESCAPE:
		if ( mSceneManipulator && mSceneManipulator->getActiveAction() )
		{
			const Ogre::String actionName = mSceneManipulator->getActiveAction()->getName();
			if ( actionName == "CreateObjectAction" || 
				actionName == "CreateComplexObjectAction" )
			{
				mSceneManipulator->setActiveAction("ManipObjectAction");
			}
			else if ( actionName == "MonsterEdit" )
			{
				mSceneManipulator->getActiveAction()->setParameter("FUNC_KEY", "ESC");
			}
			else if ( actionName == "GrowPointEdit" )
			{
				mSceneManipulator->getActiveAction()->setParameter("FUNC_KEY", "ESC");
			}
			else if ( actionName == "SoundEdit" )
			{
				mSceneManipulator->getActiveAction()->setParameter("FUNC_KEY", "ESC");
			}
			else if ( actionName == "RegionEdit" )
			{
				mSceneManipulator->getActiveAction()->setParameter("FUNC_KEY", "ESC");
				
			}

			e.Skip(false);  // eat the key down event
		}

        if (HasCapture())
        {
            ReleaseCapture();
            e.Skip(false);  // eat the key down event
        }		

        break;

	case 126: //esc ����� ~
		{
			if (GetActiveAction())
			{
				if("MonsterEdit" == GetActiveAction()->getName())
				{
					
				}

				if("GrowPointEdit" == GetActiveAction()->getName())
				{
					
				}

				if("SoundEdit" == GetActiveAction()->getName())
				{
					
				}

				if("RegionEdit" == GetActiveAction()->getName())
				{
					if(e.ControlDown())
					{
						// ��ֱ�ӽ�
						SetVerticalView();
					}
					//else
					//{
					//	//g_pRegionEdit->FinishRegion();
					//}//
				}

				
			}
			break;
		}
	case WXK_SPACE: // �ո�
		{
			
			if (GetActiveAction())
			{
				if("MonsterEdit" == GetActiveAction()->getName())
				{
					
				}

				if("GrowPointEdit" == GetActiveAction()->getName())
				{
					
				}

				if("SoundEdit" == GetActiveAction()->getName())
				{
					
				}

				if("RegionEdit" == GetActiveAction()->getName())
				{
					if(e.ShiftDown())
					{
						g_pRegionEdit->AddNewRegion();
					}
					else
					{
						g_pRegionEdit->FinishRegion();
					}
				}

				
			}
			break;
		}
    case WXK_DELETE:
        GetSceneManipulator()->removeSelectedObjects();

		////////////////////////////////////////////////////////////////////////
		//
		// ɾ���ڷŵĹ���.
		//
		if (GetActiveAction())
		{
			if("MonsterEdit" == GetActiveAction()->getName())
			{
				GetActiveAction()->setParameter("DELETE_MONSTER", "DEL");
			}

			if("GrowPointEdit" == GetActiveAction()->getName())
			{
				GetActiveAction()->setParameter("DELETE_GROWPOINT", "DEL");
			}

			if("SoundEdit" == GetActiveAction()->getName())
			{
				GetActiveAction()->setParameter("DELETE_SOUNDITEM", "DEL");
			}

			if("RegionEdit" == GetActiveAction()->getName())
			{
				GetActiveAction()->setParameter("DELETE_REGION_EDIT", "DEL");
				g_pRegionEdit->DelRegion();
			}

			
		}

        e.Skip(false);
        break;

    case WXK_UP:

        // ֻ������ѡ�����������²����ü����ƶ������
        if ( false == ProcessArrowKey(-3) )
        {
            Ogre::Camera* camera = GetSceneManipulator()->getCamera();
            assert (camera);

            Ogre::Vector3 oldPos = camera->getPosition();

            Ogre::Vector3 offsetPos = camera->getDirection() * GetSceneManipulator()->getMoveSpeed();

            Ogre::Vector3 newPos = oldPos;
            newPos.x += offsetPos.x;
            newPos.z += offsetPos.z;

            GetSceneManipulator()->setCameraPosition(newPos);
        }
        e.Skip(false);
        break;
    case WXK_DOWN:
        
        // ֻ������ѡ�����������²����ü����ƶ������
        if ( false == ProcessArrowKey(3) )
        {
            Ogre::Camera* camera = GetSceneManipulator()->getCamera();
            assert (camera);

            Ogre::Vector3 oldPos = camera->getPosition();

            Ogre::Vector3 offsetPos = camera->getDirection() * -(GetSceneManipulator()->getMoveSpeed());

            Ogre::Vector3 newPos = oldPos;
            newPos.x += offsetPos.x;
            newPos.z += offsetPos.z;

            GetSceneManipulator()->setCameraPosition(newPos);
        }
        e.Skip(false);
        break;
    case WXK_LEFT:     

        // ֻ������ѡ�����������²����ü�����ת�����
        if ( false == ProcessArrowKey(-1) )
        {
            Ogre::Camera* camera = GetSceneManipulator()->getCamera();
            assert (camera);

            Ogre::Radian yawAngle( GetSceneManipulator()->getRotateSpeed() / 360.0f );

            camera->yaw(yawAngle);
        }
        e.Skip(false);
        break;
    case WXK_RIGHT: 

        // ֻ������ѡ�����������²����ü�����ת�����
        if ( false == ProcessArrowKey(1) )
        {
            Ogre::Camera* camera = GetSceneManipulator()->getCamera();
            assert (camera);

            Ogre::Radian yawAngle( -(GetSceneManipulator()->getRotateSpeed() / 360.0f) );

            camera->yaw(yawAngle);
        }
        e.Skip(false);
        break;
    case WXK_HOME:
        ProcessArrowKey(2);
        e.Skip(false);
        break;
    case WXK_END:
        ProcessArrowKey(-2);
        e.Skip(false);
        break;
    case 'H':
    case 'h':
        {
            static unsigned mSceneDetailIndex = 0;
            mSceneDetailIndex = (mSceneDetailIndex+1)%3;
            switch(mSceneDetailIndex) 
            {
            case 0 : 
                GetSceneManipulator()->getCamera()->setPolygonMode(Ogre::PM_SOLID); 
                break;
            case 1 : 
                GetSceneManipulator()->getCamera()->setPolygonMode(Ogre::PM_WIREFRAME); 
                break;
            case 2 : 
                GetSceneManipulator()->getCamera()->setPolygonMode(Ogre::PM_POINTS); 
                break;
            }
        }
        e.Skip(false);
        break;
    case 'F':
    case 'f':
        {
            wxPoint pt = wxGetMousePosition();
            pt = ScreenToClient(pt);
            
            GetSceneManipulator()->setUseBoxCenterSelection(pt.x,pt.y,!GetSceneManipulator()->getUseBoxCenterSelection());
        }
        e.Skip(false);
        break;
    case 'Z':
    case 'z':
        {
            GetSceneManipulator()->setUseAxis(!GetSceneManipulator()->getUseAxis());
        }
        e.Skip(false);
        break;
    case 'M':
    case 'm':
        {
            GetSceneManipulator()->setActiveAction("ManipObjectAction"); 
        }
        e.Skip(false);
        break;

    case 'G':
    case 'g':
        {
			GetSceneManipulator()->getTerrainGrid()->setActive(!(GetSceneManipulator()->getTerrainGrid()->isActive()));
        }
        e.Skip(false);
        break;

	case 'W':
	case 'w':
		{
			GetSceneManipulator()->setActiveAction("RaiseHeightAction"); 
		}
		e.Skip(false);
		break;

	case 'S':
	case 's':
		{
			GetSceneManipulator()->setActiveAction("LowerHeightAction"); 
		}
		e.Skip(false);
		break;

	case '1':
		{
			GetSceneManipulator()->setActiveAction("TrueSmoothHeightAction"); 
		}
		e.Skip(false);
		break;

	case 'Q':
	case 'q':
		{
			GetSceneManipulator()->setActiveAction("SmoothHeightAction"); 
		}
		e.Skip(false);
		break;

	case 'I':
	case 'i':
		{
			GetSceneManipulator()->setActiveAction("DragTerrainColourAction"); 
		}
		e.Skip(false);
		break;

	case 'E':
	case 'e':
		{
			GetSceneManipulator()->setActiveAction("LevelOffHeightAction"); 
		}
		e.Skip(false);
		break;

	case 'D':
	case 'd':
		{
			GetSceneManipulator()->setActiveAction("SetTerrainHeightAction"); 
		}
		e.Skip(false);
		break;

	case 'V':
	case 'v':
		{
			GetSceneManipulator()->setActiveAction("TerrainLayerOneEraserAction"); 
		}
		e.Skip(false);
		break;

	case 'A':
	case 'a':
		{
			GetSceneManipulator()->setActiveAction("TerrainSelectionAction"); 
		}
		e.Skip(false);
		break;

	case 'O':
	case 'o':
		GetSceneManipulator()->getFrameStatsListener()->showDebugOverlay
			(!GetSceneManipulator()->getFrameStatsListener()->isDebugOverlayShown());
		e.Skip(false);
		break;	

	case 'P':
	case 'p':
		GetSceneManipulator()->changeCameraType();
		e.Skip(false);
		break;	

	case WXK_CAPITAL:
		{
			Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
			// ManipObjectAction״̬�£�ȡ����ѡ��ѡ״̬
			if((NULL != pAction) && (GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction") )
			{
				if ( !IsCapital() )
				{
					dynamic_cast<Fairy::ManipObjectAction*>( pAction )->setIsUseDefinedSelecting( false );
					dynamic_cast<Fairy::ManipObjectAction*>( pAction )->setIsObjectMoveByGrid( false );

					GetActiveAction()->setParameter("%CurrentMode", "RotateX");
				}
				else
				{
					dynamic_cast<Fairy::ManipObjectAction*>( pAction )->setIsUseDefinedSelecting( true );
					dynamic_cast<Fairy::ManipObjectAction*>( pAction )->setIsObjectMoveByGrid( true );

					GetActiveAction()->setParameter("%CurrentMode", "MultiSelecting");
				}
			}
			//�ر�ƽ����2������ͨ��CAPITAL����
			if((NULL != pAction) && (GetSceneManipulator()->getActiveAction()->getName() == "SmoothHeightAction") )
			{

				if ( !IsCapital() )
				{
					dynamic_cast<Fairy::SmoothHeightAction*>( pAction )->setTypeSelecting( Fairy::SmoothHeightAction::ENUM_SMOOTH_TYPE::SMOOTH_TYPE_TYPE_SMOOTH );
				}
				else
				{
					dynamic_cast<Fairy::SmoothHeightAction*>( pAction )->setTypeSelecting(  Fairy::SmoothHeightAction::ENUM_SMOOTH_TYPE::SMOOTH_TYPE_TYPE_TRISMOOTH );
				}

			}
		}
		break;
    }
    
}

void 
FairySceneCanvas::OnChar( wxKeyEvent &event )
{
    switch (event.m_keyCode)
    {
    case WXK_PAGEUP:
        ProcessArrowKey(2);
        break;
    case WXK_PAGEDOWN:
        ProcessArrowKey(-2);
        break;
    }

}

static bool disableRender = false;
void
FairySceneCanvas::OnKeyUp(wxKeyEvent& e)
{  
    e.Skip(true);  // default to continue key process

    typedef std::set<Fairy::ObjectPtr> ObjectSet;
    const ObjectSet& objSet = GetSceneManipulator()->getSelectedObjects();
    Fairy::ModifyObjectPropertyOperator* op = NULL;

    switch (e.GetKeyCode())
    {
    case WXK_UP:  
    case WXK_DOWN:
    case WXK_LEFT:
    case WXK_RIGHT:
    case WXK_HOME:
    case WXK_END:
		if ( false == objPos.empty() )
		{
			for(ObjectSet::const_iterator iter = objSet.begin();iter != objSet.end(); iter++)
			{
				Fairy::ObjectPtr object = *iter;
				Ogre::Vector3 newPosition = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));

				ObjectPosition::const_iterator it = objPos.find(object);
				assert(it != objPos.end());

				op = new Fairy::ModifyObjectPropertyOperator(GetSceneManipulator());
				op->add(object->getName(), "position",it->second,newPosition);

				if(op && op->empty())
					delete op;
				else if(op && !op->empty())
					GetSceneManipulator()->getOperatorManager()->addOperator(op);
			}

			
			objPos.clear();
		}
        
        break;
	case WXK_CONTROL:
		if ( mSceneManipulator && mSceneManipulator->getActiveAction())
		{
			const Ogre::String actionName = mSceneManipulator->getActiveAction()->getName();
			if ( actionName == "AutoTexPaintAction" ) //��ˢ��סCTRAL���������漴
			{

				mSceneManipulator->getActiveAction()->setParameter("%Random_Texture","false");
			}
			else if ( IsCapital() && ( actionName == "ManipObjectAction") ) 
			{
				Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
				// ManipObjectAction״̬�£�ȡ����ѡ��ѡ״̬
				if( ( NULL != pAction ) && ( GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction" ) )
				{
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->setIsUseDefinedSelecting( false );
				}
			}
		}
		break;
	case '+':
		{
			Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
			if (pAction)
			{
				//'+'
				int size_x = GetSceneManipulator()->getBrushXSize();
				int size_z = GetSceneManipulator()->getBrushZSize();
				GetSceneManipulator()->setBrush(GetSceneManipulator()->getBrushType(), size_x+1, size_z+1);
				
				wxPoint pt = mCurrentMousePos;
				pAction->onMotion(pt.x,pt.y);
			}
		}
		break;
	case '-':
		{
			Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
			if (pAction)
			{
				//'-'
				int size_x = GetSceneManipulator()->getBrushXSize();
				int size_z = GetSceneManipulator()->getBrushZSize();
				GetSceneManipulator()->setBrush(
					GetSceneManipulator()->getBrushType(), (size_x>1)?(size_x-1):1, (size_z>1)?(size_z-1):1);

				wxPoint pt = mCurrentMousePos;
				pAction->onMotion(pt.x,pt.y);
			}
		}
		break;
	//case '5':
	//{
	//	disableRender  = !disableRender;
	//}
	//break;
	case ',':// '<'
		{
			Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
            // ManipObjectAction״̬�£�������ת����
			if((NULL != pAction) && (GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction"))
			{
				Ogre::Degree degrees(-45);
				//��ϼ�����
				if ( !IsCapital() && IsLeftCtrlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
				{
					// Ctrl
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->roteObject(degrees,Ogre::Vector3::UNIT_X);
				}
				else if ( !e.ControlDown() && !e.AltDown() && IsLeftShiftDown() && !e.MetaDown())
				{
					// Shift
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->roteObject(degrees,Ogre::Vector3::UNIT_Z);
				}
				else if ( !e.ControlDown() && IsLeftAltDown() && !e.ShiftDown() && !e.MetaDown())
				{
					// Alt
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->roteObject(degrees,Ogre::Vector3::UNIT_Y);
				}
			}
		}
		break;

	case '.':// '>'
		{
			Fairy::Action* pAction = GetSceneManipulator()->getActiveAction();
			// ManipObjectAction״̬�£�������ת����
			if((NULL != pAction) && (GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction"))
			{
				Ogre::Degree degrees(45);
				//��ϼ�����
				if ( !IsCapital() && IsLeftCtrlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
				{
					// Ctrl
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->roteObject(degrees,Ogre::Vector3::UNIT_X);
				}
				else if ( !e.ControlDown() && !e.AltDown() && IsLeftShiftDown() && !e.MetaDown())
				{
					// Shift
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->roteObject(degrees,Ogre::Vector3::UNIT_Z);
				}
				else if ( !e.ControlDown() && IsLeftAltDown() && !e.ShiftDown() && !e.MetaDown())
				{
					// Alt
					dynamic_cast<Fairy::ManipObjectAction*>(pAction)->roteObject(degrees,Ogre::Vector3::UNIT_Y);
				}
			}
		}
		break;
   /* case WXK_LEFT:
    case WXK_RIGHT:
        for(ObjectSet::const_iterator iter = objSet.begin();iter != objSet.end(); iter++)
        {
            Fairy::ObjectPtr object = *iter;
            Ogre::Quaternion newOrientation = Fairy::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));

            ObjectOrientation::const_iterator it = objOrient.find(object);
            assert(it != objOrient.end());

            op = new Fairy::ModifyObjectPropertyOperator(GetSceneManipulator());
            op->add(object->getName(), "orientation",it->second,newOrientation);
        }

        if(op && op->empty())
            delete op;
        else if(op && !op->empty())
            GetSceneManipulator()->getOperatorManager()->addOperator(op);

        objOrient.clear();
        break;*/
    
    }
}
void
FairySceneCanvas::OnMouseCaptureChanged(wxMouseCaptureChangedEvent& e)
{
    if (e.GetEventObject() == this && e.GetCapturedWindow() != this)
    {
        switch (mDragButton)
        {
        case wxMOUSE_BTN_MIDDLE:
            {
                wxASSERT(mCameraManip);
                wxPoint pt = wxGetMousePosition();
                pt = ScreenToClient(pt);
                mCameraManip->onEnd(pt.x, pt.y, true);
                mCameraManip = NULL;
                break;
            }

        case wxMOUSE_BTN_LEFT:
            {
                wxASSERT(GetActiveAction());
                wxPoint pt = wxGetMousePosition();
                pt = ScreenToClient(pt);
                GetActiveAction()->onEnd(pt.x, pt.y, true);
                mUpdateListener->disable();
            }
            break;
        }

        mDragButton = wxMOUSE_BTN_NONE;
    }
}

void
FairySceneCanvas::OnMouseEnter(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    GetSceneManipulator()->showHitPoint(true);
}

void
FairySceneCanvas::OnMouseLeave(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    GetSceneManipulator()->showHitPoint(false);
}

void
FairySceneCanvas::OnMouseWheel(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

	if (GetSceneManipulator()->getCamera()->getProjectionType() == Ogre::PT_PERSPECTIVE )
	{
		Ogre::Real distance = - GetSceneManipulator()->_getWheelSpeed() * e.GetWheelRotation() / e.GetWheelDelta();
		GetSceneManipulator()->getCamera()->moveRelative(Ogre::Vector3(0, 0, distance));
	}
	// ��������ͶӰ�������µĴ���
	else
	{
	    const Ogre::Real ZOOM_SPEED = 100;
		const Ogre::Real MIN_NEAR_CLIP = 20;
		const Ogre::Real MAX_NEAR_CLIP = 25000;

		Ogre::Real nearclip = GetSceneManipulator()->getCamera()->getNearClipDistance();
		Ogre::Real offset = - ZOOM_SPEED * (e.GetWheelRotation() / e.GetWheelDelta());
		Ogre::Vector3 pos =  GetSceneManipulator()->getCamera()->getRealPosition();
		Ogre::Real newclip = nearclip + offset;

		if ((newclip > MIN_NEAR_CLIP) && (newclip < MAX_NEAR_CLIP))
		{
			GetSceneManipulator()->getCamera()->setNearClipDistance(newclip);
			GetSceneManipulator()->getCamera()->setPosition(pos.x, pos.y + 2.5 * offset, pos.z);	   
	    }
	}

    GetSceneManipulator()->_fireCameraPosChanged();

    wxPoint pt = ScreenToClient(wxPoint(e.GetX(), e.GetY()));
    if (GetActiveAction())
    {
        GetActiveAction()->onMotion(pt.x, pt.y);
    }
    else
    {
        GetSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
    }
}

void
FairySceneCanvas::OnMouseMove(wxMouseEvent& e)
{
	SetFocus();
	ShowPos(e.GetX(), e.GetY());

    if (!GetSceneManipulator())
        return;

    if (mCameraManip)
    {
        wxASSERT(mDragButton != wxMOUSE_BTN_NONE);
        mCameraManip->onMotion(e.GetX(), e.GetY());
    }

    if (GetActiveAction())
    {
		GetActiveAction()->onMotion(e.GetX(), e.GetY());

		Fairy::String strNameKK = GetActiveAction()->getName();
		if("RegionEdit"    == GetActiveAction()->getName() || 
		   "MonsterEdit"   == GetActiveAction()->getName() ||
		   "GrowPointEdit" == GetActiveAction()->getName() ||
		   "NpcPatrol"     == GetActiveAction()->getName() )
		{
			if (e.ControlDown())
			{
				GetActiveAction()->setParameter("FUNC_KEY", "CTRL");
			}
			
			if(e.AltDown())
			{
				GetActiveAction()->setParameter("FUNC_KEY", "ATL");
			}
			
			if(e.ShiftDown())
			{
				GetActiveAction()->setParameter("FUNC_KEY", "SHIFT");
			}
		}
    }
    else
    {
        GetSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(e.GetX(), e.GetY());
    }

    // ��ʾ��׼ģ��
    if ( GetSceneManipulator()->getShowStandardModel() )
        GetSceneManipulator()->getHitIndicator("StandardModelIndicator")->setHitPoint(0.5,0.5);
    else
        GetSceneManipulator()->getHitIndicator("StandardModelIndicator")->hide();

    mCurrentMousePos.x = e.GetX();
    mCurrentMousePos.y = e.GetY();

	Ogre::Vector3 position;
	bool hit = GetSceneManipulator()->getTerrainIntersects(mCurrentMousePos.x, mCurrentMousePos.y, position);

	std::pair<int, int> gridCoord = GetSceneManipulator()->getTerrainData()->getGrid(position.x, position.z);

	if (hit)
	{
		mParentFrame->SetStatusText("World Coordinate : " + Ogre::StringConverter::toString((int)(position.x)) +
			"  " + Ogre::StringConverter::toString((int)(position.y)) + "  "
			 + Ogre::StringConverter::toString((int)(position.z)), 0);
		mParentFrame->SetStatusText("Grid Coordinate : " + Ogre::StringConverter::toString(gridCoord.first) + " " +
			Ogre::StringConverter::toString(gridCoord.second), 1);
	}
}

void
FairySceneCanvas::OnMiddleDown(wxMouseEvent& e)
{
    SetFocus();

    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

    wxASSERT(!mCameraManip);
    wxASSERT(mDragButton == wxMOUSE_BTN_NONE);

    if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
    {
        // (None)
        mCameraManip = GetSceneManipulator()->getManip("CameraDragMove");
    }
    else if (!e.ControlDown() && !e.AltDown() && e.ShiftDown() && !e.MetaDown())
    {
        // Shift
        mCameraManip = GetSceneManipulator()->getManip("CameraDragRotate");
    }
    else if (e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
    {
        // Ctrl
        mCameraManip = GetSceneManipulator()->getManip("CameraPan");
    }
    else if (!e.ControlDown() && e.AltDown() && !e.ShiftDown() && !e.MetaDown())
    {
        // Alt
        mCameraManip = GetSceneManipulator()->getManip("CameraRotate");
    }

    if (mCameraManip)
    {
        mDragButton = e.GetButton();
        mCameraManip->onBegin(e.GetX(), e.GetY());
        CaptureMouse();
    }
}

void
FairySceneCanvas::OnMiddleUp(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    if (!HasCapture())
        return;

    if (mDragButton == wxMOUSE_BTN_NONE || !e.Button(mDragButton))
        return;

    wxASSERT(mCameraManip);

    mCameraManip->onEnd(e.GetX(), e.GetY(), false);
    mCameraManip = NULL;

    mDragButton = wxMOUSE_BTN_NONE;
    ReleaseCapture();
}

void
FairySceneCanvas::OnLeftDown(wxMouseEvent& e)
{
    SetFocus();

    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

    wxASSERT(!mCameraManip);
    wxASSERT(mDragButton == wxMOUSE_BTN_NONE);

    if (!GetActiveAction())
        return;



	// �����¼�����༭ʹ�õĽű�id
	if(GetActiveAction()->getName() == "EventAreaEdit")
	{
		FairyEditorFrame* pMainFrame = (FairyEditorFrame*)GetParent();
		if((pMainFrame)&&(pMainFrame->m_pEventAreaEditDlg))
		{
			pMainFrame->m_pEventAreaEditDlg->SetActionScriptValue();
		}

	}



     if(GetActiveAction()->getName() == "ManipObjectAction")
    {
        //ͨ�ò���
        if(!e.ControlDown() && e.AltDown() && IsLeftShiftDown() && !e.MetaDown())
        {
            //Alt + Shift
            GetActiveAction()->setParameter("%CurrentMode", "MultiRotate");
        }
        else if ( !IsCapital() && IsLeftCtrlDown() && e.AltDown() && IsLeftShiftDown() && !e.MetaDown())
        {
            // Shift+Ctrl
            GetActiveAction()->setParameter("%CurrentMode", "MultiScale");
        }
        else if ( !e.AltDown() && !e.ShiftDown() && !e.MetaDown() && ( ( !e.ControlDown() && IsSpaceDown() ) || ( IsCapital() && IsLeftCtrlDown() ) ) )
        {
            // space
              GetActiveAction()->setParameter("%CurrentMode", "MultiSelecting");
        }
        else
        {
            GetActiveAction()->setParameter("%CurrentMode", "");
        }

        if(GetSceneManipulator()->getUseAxis())
        {
            //��Max����ģʽ
            if (!e.ControlDown() && IsLeftAltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Alt
                GetActiveAction()->setParameter("%CurrentMode", "Rotate");
            }
            else if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown() && !IsSpaceDown())
            {
                // (None)
                GetActiveAction()->setParameter("%CurrentMode", "Move");
            }
            else if ( !IsCapital() && IsLeftCtrlDown()  && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Ctrl
                GetActiveAction()->setParameter("%CurrentMode", "Scale");
            }
        }
        else
        {
            //��ϼ�����ģʽ
            if ( !IsCapital() && IsLeftCtrlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Ctrl
                GetActiveAction()->setParameter("%CurrentMode", "RotateX");
            }
            else if (!e.ControlDown() && !e.AltDown() && IsLeftShiftDown() && !e.MetaDown())
            {
                // Shift
                GetActiveAction()->setParameter("%CurrentMode", "RotateZ");
            }
            else if (!e.ControlDown() && IsLeftAltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Alt
                GetActiveAction()->setParameter("%CurrentMode", "RotateY");
            }
            else if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown() && 
                (IsKeyDown('R') || IsKeyDown('r') )
                )
            {
                // Ctrl+Alt
                GetActiveAction()->setParameter("%CurrentMode", "Scale");
            }
            else if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown() && !IsSpaceDown())
            {   
                GetActiveAction()->setParameter("%CurrentMode", "Move");
            }
        }
    }
    else if(GetActiveAction()->getName() == "TerrainSelectionAction")
    {
        if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
        {
            // (None)
            GetActiveAction()->setParameter("%CurrentMode", "SingleSelecting");
        }
        else if ( !IsCapital() && IsLeftCtrlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
        {
            // Ctrl
            GetActiveAction()->setParameter("%CurrentMode", "MultiSelecting");
        }        
        else
        {
            GetActiveAction()->setParameter("%CurrentMode", "");
        }
    }

    mUpdateListener->enable();
    mDragButton = e.GetButton();
    GetActiveAction()->onBegin(e.GetX(), e.GetY());
    CaptureMouse();
}

void
FairySceneCanvas::OnLeftUp(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    if (!HasCapture())
        return;

    if (mDragButton == wxMOUSE_BTN_NONE || !e.Button(mDragButton))
        return;

    wxASSERT(GetActiveAction());

    GetActiveAction()->onEnd(e.GetX(), e.GetY(), false);

    mUpdateListener->disable();
    mDragButton = wxMOUSE_BTN_NONE;
    ReleaseCapture();
}

void
FairySceneCanvas::OnContextMenu(wxContextMenuEvent& e)
{
    if (GetSceneManipulator()->hasSelectedObjects() && GetActiveAction() && GetActiveAction()->getName() == "ManipObjectAction")
    {
        wxMenu menu;

        menu.Append(feID_SNAP_TO_GRID,
            _("&Snap to grid"),
            _("Snap select objects to grid"));

        menu.AppendCheckItem(feID_MOVE_BY_X,
            _("&Move ByX"),
            _("Move by X axis"));

        menu.AppendCheckItem(feID_MOVE_BY_Y,
            _("&Move ByY"),
            _("Move by Y axis"));

        menu.AppendCheckItem(feID_MOVE_BY_Z,
            _("&Move ByZ"),
            _("Move by Z axis"));

        menu.AppendCheckItem(feID_SNAP_ROTATIONX,
            _("&X Snap rotation "),
            _("Snap select objects's rotation"));

        menu.AppendCheckItem(feID_SNAP_ROTATIONY,
            _("&Y Snap rotation "),
            _("Snap select objects's rotation"));

        menu.AppendCheckItem(feID_SNAP_ROTATIONZ,
            _("&Z Snap rotation "),
            _("Snap select objects's rotation"));

        wxPoint pos = e.GetPosition();
        if (pos != wxDefaultPosition)
            pos = ScreenToClient(pos);

        PopupMenu(&menu, pos);
        return;
    }
    // TerrainSelectionAction�Ĳ˵� TODO:��һ���ж��������ж��Ƿ���ѡ�е���
    else if (GetActiveAction() && GetActiveAction()->getName() == "TerrainSelectionAction")
    {
        wxMenu menu;       

		// ����Ҽ���������ʱ���ֵĲ˵�
		// �Ƿ񿽱��߶���Ϣ
        menu.AppendCheckItem(feID_TERRAINHEIGHTCHECKBOX,
            _("&Copy Terrain Height Infomation"),
            _("If you want to copy the terrain height infomation,check on this option"));

		// �Ƿ񿽱����ζ���ɫ
		menu.AppendCheckItem(feID_TERRAINDIFFUSECHECKBOX,
			_("&Copy Terrain Diffuse Infomation"),
			_("If you want to copy the terrain Diffuse infomation,check on this option"));

		// �Ƿ񿽱�������Ϣ
        menu.AppendCheckItem(feID_TERRAINTEXTURECHECKBOX,
            _("&Copy Terrain Texture Infomation"),
            _("If you want to copy the terrain texture infomation,check on this option"));

		// �Ƿ񿽱��Խ�����Ϣ
        menu.AppendCheckItem(feID_TERRAINDIAGONALCHECKBOX,
            _("&Copy Terrain Diagonal Infomation"),
            _("If you want to copy the terrain diagonal infomation,check on this option"));

		// �Ƿ�Ҫ��ת���Σ���ʱ��90�ȣ�
		menu.Append(feID_TERRAINROTATERIGHTANGLE,
			_("Rotate Terrain right angle"),
			_("If you want to rotate the terrain right angle,hit this botton"));

        wxPoint pos = e.GetPosition();
        if (pos != wxDefaultPosition)
            pos = ScreenToClient(pos);

        PopupMenu(&menu, pos);
        return;
    }

    e.Skip();   // continue searching for event handle
}

void
FairySceneCanvas::OnSnapToGrid(wxCommandEvent& e)
{
    GetSceneManipulator()->_snapSelectedObjectsPosition(1);
}

void
FairySceneCanvas::OnMoveByXYZ(wxCommandEvent& e)
{
    int index = e.GetId() - feID_MOVE_BY_X;

    MoveUpDownSettingDialog* dlg = new MoveUpDownSettingDialog (this, wxID_ANY,
        _("MoveUpDown Setting"));
    dlg->Centre();
    if(dlg->ShowModal() == wxID_OK)
    {
        int offset = dlg->mSpinCtrl->GetValue();

        if(GetSceneManipulator()->getActiveAction()->getName() == "ManipObjectAction")
        {
            Fairy::ManipObjectAction* action = (Fairy::ManipObjectAction*)(GetSceneManipulator()->getActiveAction());
            typedef std::set<Fairy::ObjectPtr> ObjectSet;
            const ObjectSet& objSet = GetSceneManipulator()->getSelectedObjects();

            Ogre::Vector3 offPos(0,0,0);
            offPos[index] = offset * GetSceneManipulator()->getBaseScale();

            for(ObjectSet::const_iterator iter = objSet.begin();iter != objSet.end(); iter++)
            {
                Fairy::ObjectPtr object = *iter;
                Ogre::Vector3 newPosition = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position")); 
                object->setProperty("position", newPosition + offPos); 
                FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
                GetSceneManipulator()->_fireObjectPropertyChanged(object, "position");  

            }
            Fairy::ModifyObjectPropertyOperator* op = new Fairy::ModifyObjectPropertyOperator(GetSceneManipulator());

            for(ObjectSet::const_iterator iter = objSet.begin();iter != objSet.end(); iter++)
            {
                Fairy::ObjectPtr object = *iter;
                Ogre::Vector3 newPosition = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position")); 

                op->add(object->getName(), "position", 
                    newPosition - offPos,
                    newPosition);
            }

            if(op->empty())
                delete op;
            else
                GetSceneManipulator()->getOperatorManager()->addOperator(op);
        }

    }

}

void 
FairySceneCanvas::OnUpdateMoveByXYZ(wxUpdateUIEvent& e)
{
	e.Enable(true);
	e.Check(false);
}

void
FairySceneCanvas::OnSnapRotation(wxCommandEvent& e)
{
    int index = e.GetId() - feID_SNAP_ROTATIONX;
    FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
    GetSceneManipulator()->_snapSelectedObjectsRotation((float)360/app->GetMainFrame()->GetSnapRotationValue(),index);
}

void
FairySceneCanvas::OnUpdateSnapRotation(wxUpdateUIEvent& e)
{
	e.Enable(true);
	e.Check(false);
}
void
FairySceneCanvas::OnDestroy(wxWindowDestroyEvent& e)
{
    if (GetSceneManipulator())
    {
        GetSceneManipulator()->getRenderWindow()->setActive(false);
    }
}

void 
FairySceneCanvas::OnTerrainPasteSetting(wxCommandEvent& e)
{
    if ( !GetSceneManipulator() )
        return;

    Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

    if ( activeAction && activeAction->getName() == "TerrainSelectionAction" )
    {
        int id = e.GetId();

        switch(id)
        {
        case feID_TERRAINHEIGHTCHECKBOX:
            {
                bool copy = Ogre::StringConverter::parseBool(activeAction->getParameter("%PasteHeightInfo"));
                activeAction->setParameter("%PasteHeightInfo", Ogre::StringConverter::toString(!copy));
            }
            break;
		
		case feID_TERRAINDIFFUSECHECKBOX:
			{
				bool copy = Ogre::StringConverter::parseBool(activeAction->getParameter("%PasteDiffuseInfo"));
				activeAction->setParameter("%PasteDiffuseInfo", Ogre::StringConverter::toString(!copy));
			}
			break;

        case feID_TERRAINTEXTURECHECKBOX:
            {
                bool copy = Ogre::StringConverter::parseBool(activeAction->getParameter("%PasteTextureInfo"));
                activeAction->setParameter("%PasteTextureInfo", Ogre::StringConverter::toString(!copy));
            }
            break;

        case feID_TERRAINDIAGONALCHECKBOX:
            {
                bool copy = Ogre::StringConverter::parseBool(activeAction->getParameter("%PasteDiagonalInfo"));
                activeAction->setParameter("%PasteDiagonalInfo", Ogre::StringConverter::toString(!copy));
            }
            break;

        default:
            break;
        }
    }    
}

void 
FairySceneCanvas::OnUpdateTerrainPasteSetting(wxUpdateUIEvent& e)
{
    if ( !GetSceneManipulator() )
        return;

    Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();
    
    if ( activeAction && activeAction->getName() == "TerrainSelectionAction" )
    {
        int id = e.GetId();

        switch(id)
        {
        case feID_TERRAINHEIGHTCHECKBOX:
            {
                e.Check( activeAction->getParameter("%PasteHeightInfo") == "true" );
            }
            break;

		case feID_TERRAINDIFFUSECHECKBOX:
			{
				e.Check( activeAction->getParameter("%PasteDiffuseInfo") == "true" );
			}
			break;

        case feID_TERRAINTEXTURECHECKBOX:
            {
                e.Check( activeAction->getParameter("%PasteTextureInfo") == "true" );
            }
            break;

        case feID_TERRAINDIAGONALCHECKBOX:
            {
                e.Check( activeAction->getParameter("%PasteDiagonalInfo") == "true" );
            }
            break;

        default:
            break;
        }
    }    
}

void 
FairySceneCanvas::OnTerrainRotateRightAngle(wxCommandEvent& e)
{
	if ( !GetSceneManipulator() )
		return;

	Fairy::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && activeAction->getName()=="TerrainSelectionAction");

	// ���ж��Ƿ��Ѿ������˿���������ֻ�н����˿������������������ת
	if ( activeAction->getParameter("%AfterCopy") == "true" )
	{
		// ֪ͨaction�����¸�ʽ�����ݣ�ʹaction�еĿ������ݶ��Ǿ�����ת��
		activeAction->setParameter("%BeginRotateTerrain", "true");

		Ogre::String infoString;
		infoString += activeAction->getParameter("%JunctionHeightInfo").c_str();
		infoString += activeAction->getParameter("%GridTextureInfo").c_str();
		infoString += activeAction->getParameter("%GridDiagonalInfo").c_str();

		// �õ������ݺ��������õ�clipboard��
		wxTextDataObject *data = new wxTextDataObject(infoString);
		wxTheClipboard->SetData(data);	
	}    
	else
	{
		// ����һ���Ի���
		wxMessageBox(_("Please copy the terrain before rotation."));
	}
}

// ��ʾ��ǰ����ڳ����е��߼�����
void FairySceneCanvas::ShowPos(int iX, int iY)
{
	// ����ѡ�е���ʱģ�͵Ĳ���.

	if(NULL == mSceneManipulator)
	{

		return;
	}

	Ogre::Vector3 position;
	bool hit = mSceneManipulator->getTerrainIntersects(iX, iY, position);

	if(hit)
	{
	
		if(m_pParentFrame)
		{
			float fLogicX = 0;
			float fLogicZ = 0;

			// ����ϵ��
			float fScale = 1.0;
			float fTerrainDataPosX = 0;
			float fTerrainDataPosZ = 0;
			float fTerrainDataPosY = 0;

			fTerrainDataPosX = mSceneManipulator->getTerrainData()->mPosition.x;
			fTerrainDataPosZ = mSceneManipulator->getTerrainData()->mPosition.z;
			fTerrainDataPosY = mSceneManipulator->getTerrainData()->mPosition.y;//
			fScale = mSceneManipulator->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
			
			// ���г���������߼�����ת��
			fLogicX = (position.x - fTerrainDataPosX) / fScale;
			fLogicZ = (position.z - fTerrainDataPosZ) / fScale;
			m_pParentFrame->SetCurSceneLogicPos(fLogicX, fLogicZ);
		}
		
	}
	else
	{
		if(m_pParentFrame)
		{
			m_pParentFrame->SetCurSceneLogicPos(0, 0);
		}
	}
}


// ��ͼ��ת
void FairySceneCanvas::GotoScene(float fX, float fY)
{
	//// ����ѡ�е���ʱģ�͵Ĳ���.

	if(NULL == mSceneManipulator)
	{

		return;
	}

	if(m_pParentFrame)
	{
		float fSceneX = 0;
		float fSceneY = 0;

		// ����ϵ��
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = mSceneManipulator->getTerrainData()->mPosition.x;
		fTerrainDataPosZ = mSceneManipulator->getTerrainData()->mPosition.z;
		fTerrainDataPosY = mSceneManipulator->getTerrainData()->mPosition.y;//
		fScale = mSceneManipulator->getBaseScale();
		if(abs((float)fScale) < 0.00001)
		{
			fScale = 1.0;
		}
		
		fSceneX = fX * fScale + fTerrainDataPosX;
		fSceneY = fY * fScale + fTerrainDataPosZ;

		Ogre::Vector3 posEye;
		Ogre::Vector3 posLookAt;

		posLookAt.x = fSceneX;
		posLookAt.z = fSceneY;

		posLookAt.y =  mSceneManipulator->getTerrainData()->getHeightAt(fSceneX, fSceneY);
		posEye = posLookAt;
		posEye.y += 400;
		
		// �õ��͵������λ��.
		Ogre::Vector3 oldCameraPos = mSceneManipulator->getCamera()->getPosition();

		// �õ�����
		Ogre::Vector3 CameraDir = mSceneManipulator->getCamera()->getDirection();

		float fx = 0;
		float fz = 0;
		float fy = 0;

		if( abs(CameraDir.y) < 0.0001 )
		{
			fx = (oldCameraPos.y - posLookAt.y) * CameraDir.x + posLookAt.x;
			fz = (oldCameraPos.y - posLookAt.y) * CameraDir.z + posLookAt.z;
		}
		else
		{
			fx = ((oldCameraPos.y - posLookAt.y) / CameraDir.y) * CameraDir.x + posLookAt.x;
			fz = ((oldCameraPos.y - posLookAt.y) / CameraDir.y) * CameraDir.z + posLookAt.z;
		}
	
		// �����������λ��λ��
		posEye.x = fx;
		posEye.y = oldCameraPos.y;
		posEye.z = fz;

		mSceneManipulator->getCamera()->setPosition(posEye);

		
	}
		
	

	 
}

bool 
FairySceneCanvas::frameStarted(const Ogre::FrameEvent& evt)
{
	mLastTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	return FrameListener::frameStarted(evt);
}

bool FairySceneCanvas::doUpdate(bool isIdle)
{
	if (mTargetFPS)
	{
		unsigned long thisTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		if ((thisTime - mLastTime) * mTargetFPS < 1000)
		{
			if (!isIdle)
			{
				if (GetSceneManipulator())
				{
					GetSceneManipulator()->updateWindow();
				}
			}

			return false;
		}
		else if (isIdle)
		{
			return false;
		}
	}

	if (GetSceneManipulator()  &&  !disableRender)
	{
		GetSceneManipulator()->renderOneFrame();
	}

	return true;
}

void FairySceneCanvas::OnTimer(wxTimerEvent& event)
{
	if (GetSceneManipulator())
	{
		GetSceneManipulator()->renderOneFrame();
	}
}

void FairySceneCanvas::setTargetFPS(unsigned long fps)
{
	if (mTargetFPS != fps)
	{
		mTargetFPS = fps;
		if (mTargetFPS)
		{
			mTimer.Stop();
			mTimer.Start(1000 / mTargetFPS);
		}
	}
}

void FairySceneCanvas::SetVerticalView()
{
	//// ����ѡ�е���ʱģ�͵Ĳ���.

	if(NULL == mSceneManipulator)
	{

		return;
	}


	mSceneManipulator->setRealCameraAngle(89.9f);
	//if(m_pParentFrame)
	//{
	//	float fSceneX = 0;
	//	float fSceneY = 0;

	//	// ����ϵ��
	//	float fScale = 1.0;
	//	float fTerrainDataPosX = 0;
	//	float fTerrainDataPosZ = 0;
	//	float fTerrainDataPosY = 0;

	//	fTerrainDataPosX = mSceneManipulator->getTerrainData()->mPosition.x;
	//	fTerrainDataPosZ = mSceneManipulator->getTerrainData()->mPosition.z;
	//	fTerrainDataPosY = mSceneManipulator->getTerrainData()->mPosition.y;//
	//	fScale = mSceneManipulator->getBaseScale();
	//	if(abs((float)fScale) < 0.00001)
	//	{
	//		fScale = 1.0;
	//	}
	//	
	//	fSceneX = fX * fScale + fTerrainDataPosX;
	//	fSceneY = fY * fScale + fTerrainDataPosZ;

	//	Ogre::Vector3 posEye;
	//	Ogre::Vector3 posLookAt;

	//	posLookAt.x = fSceneX;
	//	posLookAt.z = fSceneY;

	//	posLookAt.y =  mSceneManipulator->getTerrainData()->getHeightAt(fSceneX, fSceneY);
	//	posEye = posLookAt;
	//	posEye.y += 400;
	//	
	//	// �õ��͵������λ��.
	//	Ogre::Vector3 oldCameraPos = mSceneManipulator->getCamera()->getPosition();

	//	// �õ�����
	//	Ogre::Vector3 CameraDir = mSceneManipulator->getCamera()->getDirection();

	//	float fx = 0;
	//	float fz = 0;
	//	float fy = 0;

	//	if( abs(CameraDir.y) < 0.0001 )
	//	{
	//		fx = (oldCameraPos.y - posLookAt.y) * CameraDir.x + posLookAt.x;
	//		fz = (oldCameraPos.y - posLookAt.y) * CameraDir.z + posLookAt.z;
	//	}
	//	else
	//	{
	//		fx = ((oldCameraPos.y - posLookAt.y) / CameraDir.y) * CameraDir.x + posLookAt.x;
	//		fz = ((oldCameraPos.y - posLookAt.y) / CameraDir.y) * CameraDir.z + posLookAt.z;
	//	}
	//
	//	// �����������λ��λ��
	//	posEye.x = fx;
	//	posEye.y = oldCameraPos.y;
	//	posEye.z = fz;

	//	mSceneManipulator->getCamera()->setPosition(posEye);

	//	
	//}
		
}

