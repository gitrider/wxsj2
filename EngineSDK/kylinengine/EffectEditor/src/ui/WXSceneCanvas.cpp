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

#include "Action.h"
#include "Core/FrameStatsListener.h"
#include "SceneManipulator.h"
#include "Manip.h"          
#include "WXModifyObjectPropertyOperator.h"
#include "WXOperatorManager.h"
#include "WXEffectEditorApp.h"
#include "WXEffectEditorFrame.h"
#include "Core/FairyRayCollide.h"
#include "EffectObjectPropertyEditor.h"
#include "ActorAnimationSettingEditor.h"

// ----------------------------------------------------------------------------
// FairySceneCanvas::UpdateListener class
// ----------------------------------------------------------------------------

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
	, m_pParentFrame(NULL)
	, mTargetFPS(0)
{
    objPos.clear();
	mParentFrame = static_cast<wxFrame *>(parent);
	assert (mParentFrame);
	mDragStarted = false;
	mXax=mYax=mZax=false;

	mTimer.SetOwner(this, FIX_FRAME_TIMTER);
}

FairySceneCanvas::~FairySceneCanvas()
{
    if (mSceneManipulator)
    {
        mSceneManipulator->getRenderWindow()->setActive(false);
    }

    delete mUpdateListener;

}


void FairySceneCanvas::SetParentFrame(EffectEditorFrame* pParentFrame)
{
	m_pParentFrame = pParentFrame;
}


void
FairySceneCanvas::InitScene(Fairy::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
    mSceneManipulator->setup(reinterpret_cast<Ogre::ulong>(GetHandle()), NULL);
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
        EffectEditorApp* app = (EffectEditorApp*)(wxTheApp);
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
    case WXK_ESCAPE:
		if ( mSceneManipulator && mSceneManipulator->getActiveAction() )
		{
			const Ogre::String actionName = mSceneManipulator->getActiveAction()->getName();

			e.Skip(false);  // eat the key down event
		}

        if (HasCapture())
        {
            ReleaseCapture();
            e.Skip(false);  // eat the key down event
        }		

        break;


    case WXK_DELETE:
        GetSceneManipulator()->removeSelectedObjects();

        e.Skip(false);
        break;

    case WXK_UP:

        // 只有在无选择物体的情况下才能用键盘移动摄像机
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
        
        // 只有在无选择物体的情况下才能用键盘移动摄像机
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

        // 只有在无选择物体的情况下才能用键盘旋转摄像机
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

        // 只有在无选择物体的情况下才能用键盘旋转摄像机
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
    case 'Z':
    case 'z':
        {
            GetSceneManipulator()->setUseAxis(!GetSceneManipulator()->getUseAxis());
        }
        e.Skip(false);
        break;

	case 'O':
	case 'o':
		GetSceneManipulator()->getFrameStatsListener()->showDebugOverlay
			(!GetSceneManipulator()->getFrameStatsListener()->isDebugOverlayShown());
		e.Skip(false);
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


}

void
FairySceneCanvas::OnMouseLeave(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

}

void
FairySceneCanvas::OnMouseWheel(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

    Ogre::Real distance = - GetSceneManipulator()->_getWheelSpeed() * e.GetWheelRotation() / e.GetWheelDelta();
    GetSceneManipulator()->getCamera()->moveRelative(Ogre::Vector3(0, 0, distance));

    GetSceneManipulator()->_fireCameraPosChanged();

    wxPoint pt = ScreenToClient(wxPoint(e.GetX(), e.GetY()));
    if (GetActiveAction())
    {
        GetActiveAction()->onMotion(pt.x, pt.y);
    }

}

void
FairySceneCanvas::OnMouseMove(wxMouseEvent& e)
{
	SetFocus();

    if (!GetSceneManipulator())
        return;

    if (mCameraManip)
    {
        wxASSERT(mDragButton != wxMOUSE_BTN_NONE);
        mCameraManip->onMotion(e.GetX(), e.GetY());
    }

	if (mDragStarted  && e.LeftIsDown())
	{
		mDragCurrent =Ogre::Vector2(e.GetX(), e.GetY()) ;
		mDragDelta = mDragCurrent - mDragOrigin;
		mDragOrigin = mDragCurrent;

		Ogre::Camera* camera = GetSceneManipulator()->getCamera();
		assert (camera);
		Ogre::Vector3 oldPos = camera->getPosition();
		Fairy::LogicModel* pModel = m_pParentFrame->GetActorSettingEditor()->GetActorObject();
		Ogre::Vector3 objPos = pModel->getPosition();
		Ogre::Real distance = oldPos.distance(objPos);
		Ogre::Real factor = distance*0.1/150.0;

		if (GetSceneManipulator())
		{			
			Ogre::Vector3 pos = GetSceneManipulator()->getLocatorNode()->getPosition();
			Ogre::Vector3 fdeltaxi = Ogre::Vector3::ZERO;
			Ogre::Quaternion qRot = GetSceneManipulator()->getLocatorNode()->getOrientation();

			if(mXax)
				fdeltaxi = /*qRot**/(mDragDelta.x*factor*Ogre::Vector3::UNIT_X);
			if(mYax)
				fdeltaxi = /*qRot**/(mDragDelta.x*factor*Ogre::Vector3::UNIT_Y);
			if(mZax)
				fdeltaxi = /*qRot**/(mDragDelta.x*factor*Ogre::Vector3::UNIT_Z);

			pos +=  fdeltaxi;				

			GetSceneManipulator()->getLocatorNode()->translate(fdeltaxi,Ogre::Node::TS_LOCAL);
			m_pParentFrame->mEffectObjectProperty->UpdatePositionProperty(pos);
		}	
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
  

    mCurrentMousePos.x = e.GetX();
    mCurrentMousePos.y = e.GetY();

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

	mDragStarted = true;
	mDragOrigin = Ogre::Vector2(e.GetX(),e.GetY());
	mDragDelta = Ogre::Vector2(0.0f,0.0f);

    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

    wxASSERT(!mCameraManip);
    wxASSERT(mDragButton == wxMOUSE_BTN_NONE);

    if (!GetActiveAction())
        return;





     if(GetActiveAction()->getName() == "ManipObjectAction")
    {
        //通用操作
        if(!e.ControlDown() && e.AltDown() && e.ShiftDown() && !e.MetaDown())
        {
            //Alt + Shift
            GetActiveAction()->setParameter("%CurrentMode", "MultiRotate");
        }
        else if (e.ControlDown() && e.AltDown() && e.ShiftDown() && !e.MetaDown())
        {
            // Shift+Ctrl
            GetActiveAction()->setParameter("%CurrentMode", "MultiScale");
        }
        else if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown() && IsSpaceDown())
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
            //类Max操作模式
            if (!e.ControlDown() && e.AltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Alt
                GetActiveAction()->setParameter("%CurrentMode", "Rotate");
            }
            else if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown() && !IsSpaceDown())
            {
                // (None)
                GetActiveAction()->setParameter("%CurrentMode", "Move");
            }
            else if (e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Ctrl
                GetActiveAction()->setParameter("%CurrentMode", "Scale");
            }
        }
        else
        {
            //组合键操作模式
            if (e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
            {
                // Ctrl
                GetActiveAction()->setParameter("%CurrentMode", "RotateX");
            }
            else if (!e.ControlDown() && !e.AltDown() && e.ShiftDown() && !e.MetaDown())
            {
                // Shift
                GetActiveAction()->setParameter("%CurrentMode", "RotateZ");
            }
            else if (!e.ControlDown() && e.AltDown() && !e.ShiftDown() && !e.MetaDown())
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

	mDragStarted = false;
	mDragOrigin  =Ogre::Vector2(e.GetX(), e.GetY()) ;

	Ogre::Ray ray = GetSceneManipulator()->getWindowToViewportRay(e.GetX(),e.GetY());
	bool bAxis = false;
	std::pair<bool, Ogre::Real> result = Fairy::rayCollide(ray, ( Ogre::MovableObject*) GetSceneManipulator()->mAxis_x, true);
	if (result.first)
	{
		_resetAxisMaterial();

		mXax = !mXax;
		mYax = mZax = false;

		if (mXax)
		{
			GetSceneManipulator()->m_pMaterialX->setSelfIllumination(1.0,1.0,0);
		}

		bAxis = true;
	}
	if (!bAxis)
	{
		result = Fairy::rayCollide(ray, ( Ogre::MovableObject*)GetSceneManipulator()->mAxis_y, true);
		if (result.first)
		{
			_resetAxisMaterial();

			mYax = !mYax;
			mXax = mZax = false;			

			if (mYax)
			{
				GetSceneManipulator()->m_pMaterialY->setSelfIllumination(1.0,1.0,0);
			}

			bAxis = true;
		}
	}
	if (!bAxis)
	{
		result = Fairy::rayCollide(ray, ( Ogre::MovableObject*)GetSceneManipulator()->mAxis_z, true);
		if (result.first)
		{
			_resetAxisMaterial();

			mZax = !mZax;
			mXax = mYax = false;

			if (mZax)
			{
				GetSceneManipulator()->m_pMaterialZ->setSelfIllumination(1.0,1.0,0);
			}

			bAxis = true;
		}	
	}

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

    e.Skip();   // continue searching for event handle
}

void
FairySceneCanvas::OnSnapToGrid(wxCommandEvent& e)
{

}

void
FairySceneCanvas::OnMoveByXYZ(wxCommandEvent& e)
{
    int index = e.GetId() - feID_MOVE_BY_X;

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
    EffectEditorApp* app = (EffectEditorApp*)(wxTheApp);
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

	if (GetSceneManipulator())
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
	//// 进行选中的临时模型的操作.

	if(NULL == mSceneManipulator)
	{

		return;
	}


	mSceneManipulator->setRealCameraAngle(89.9f);
	
		
}

void FairySceneCanvas::_resetAxisMaterial()
{
	if (mXax)
	{
		GetSceneManipulator()->m_pMaterialX->setSelfIllumination(1,0,0);
	}
	else if (mYax)
	{
		GetSceneManipulator()->m_pMaterialY->setSelfIllumination(0,1,0);
	}
	else if(mZax)
	{
		GetSceneManipulator()->m_pMaterialZ->setSelfIllumination(0,0,1);
	}
}
