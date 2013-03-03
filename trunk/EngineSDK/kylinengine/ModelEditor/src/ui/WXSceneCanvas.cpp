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
#include <OgreVector2.h>
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

#include "DataManipulator.h"
#include "Core/FairySystem.h"
#include "Core/FairyRayCollide.h"


// ----------------------------------------------------------------------------
// FairySceneCanvas::UpdateListener class
// ----------------------------------------------------------------------------

class ActorSceneCanvas::UpdateListener : public Ogre::RenderTargetListener
{
protected:
    ActorSceneCanvas* mParent;
    unsigned long m_lLastTime;

public:
    UpdateListener(ActorSceneCanvas* parent)
        : mParent(parent)
        , m_lLastTime(0)
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
        m_lLastTime = Ogre::Root::getSingleton().getTimer()->getMicroseconds();
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
        unsigned long delta = now - m_lLastTime;
        m_lLastTime = now;
        wxPoint pt = wxGetMousePosition();
        pt = mParent->ScreenToClient(pt);
        mParent->GetActiveAction()->onUpdate(pt.x, pt.y, delta / Ogre::Real(1000*1000));
    }

    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
    {
    }
};

// ----------------------------------------------------------------------------
// ActorSceneCanvas class
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(ActorSceneCanvas, wxWindow)
// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ActorSceneCanvas, wxWindow)
    EVT_PAINT               	(ActorSceneCanvas::OnPaint)
    EVT_ERASE_BACKGROUND    	(ActorSceneCanvas::OnEraseBackground)
    EVT_SIZE                	(ActorSceneCanvas::OnSize)
	EVT_TIMER					(FIX_FRAME_TIMTER, ActorSceneCanvas::OnTimer)
    EVT_WINDOW_DESTROY      	(ActorSceneCanvas::OnDestroy)

	EVT_MOTION              	(ActorSceneCanvas::OnMouseMove)
	EVT_MOUSEWHEEL          	(ActorSceneCanvas::OnMouseWheel)
    EVT_ENTER_WINDOW        	(ActorSceneCanvas::OnMouseEnter)
    EVT_LEAVE_WINDOW        	(ActorSceneCanvas::OnMouseLeave)
    EVT_MIDDLE_DOWN				(ActorSceneCanvas::OnMiddleDown)
    EVT_MIDDLE_UP				(ActorSceneCanvas::OnMiddleUp)
    EVT_LEFT_DOWN           	(ActorSceneCanvas::OnLeftDown)
    EVT_LEFT_UP             	(ActorSceneCanvas::OnLeftUp)

	EVT_RIGHT_DOWN           	(ActorSceneCanvas::OnRightDown)
	EVT_RIGHT_UP                (ActorSceneCanvas::OnRightUp)

	EVT_KEY_DOWN            	(ActorSceneCanvas::OnKeyDown)
    EVT_MOUSE_CAPTURE_CHANGED   (ActorSceneCanvas::OnMouseCaptureChanged)

END_EVENT_TABLE()

ActorSceneCanvas::ActorSceneCanvas(wxWindow *parent, wxWindowID id,
                                   const wxPoint& pos, const wxSize& size, long style)
    : wxWindow(parent, id, pos, size, style)
    , m_pSceneManipulator(NULL)
    , m_pUpdateListener(NULL)
    , m_pCameraManip(NULL)
    , mDragButton(wxMOUSE_BTN_NONE)
    , m_pTerrGrid(NULL)
	, m_pParentFrame(NULL)
	, m_lLastTime(0)
	, m_lTargetFPS(0)
{
    objPos.clear();
	mParentFrame = static_cast<wxFrame *>(parent);
	assert (mParentFrame);
	mDragStarted = false;
	mXax=mYax=mZax=false;
	mHideAxis = false;
	mCanManipulateAxis = false;
	mRotFirst = false;

	m_Timer.SetOwner(this, FIX_FRAME_TIMTER);
}

ActorSceneCanvas::~ActorSceneCanvas()
{
    if (m_pSceneManipulator)
    {
        m_pSceneManipulator->getRenderWindow()->setActive(false);
    }
	if(m_pUpdateListener)
	{
		m_pUpdateListener = NULL;
		delete m_pUpdateListener;
	}

    if(m_pTerrGrid)
	{
        delete m_pTerrGrid;   
		m_pTerrGrid = NULL;
	}
}


Fairy::Action*	ActorSceneCanvas::GetActiveAction(void) const
{
    return GetSceneManipulator()->getActiveAction();
}

bool ActorSceneCanvas::frameStarted(const Ogre::FrameEvent& evt)
{
	if(GetDataManipulator()->mAnimParam->animState!=NULL&& GetDataManipulator()->mAnimParam->stateEnabled==true)
	{
		GetDataManipulator()->mAnimParam->animState->addTime(evt.timeSinceLastFrame);
	}

	//if(GetEditorUIMpt())
	//{
	//	Fairy::WXModel* pMainModel = GetEditorUIMpt()->FindModel(GetEditorUIMpt()->m_CurEntFile);
	//	if(NULL == pMainModel)
	//	{
	//		if(GetEditorUIMpt()->m_bAvataring)
	//		{
	//			pMainModel = GetEditorUIMpt()->m_AvatarModel;
	//		}
	//	}

	//	if(pMainModel)
	//	{
	//		pMainModel->Execute(evt.timeSinceLastFrame);
	//	}

	//	Fairy::LogicModel* pLogicModel = Fairy::LogicModelManager::getSingleton().getModelByName(GetEditorUIMpt()->m_CurObjName);
	//	if(pLogicModel)
	//	{
	//		pLogicModel->execute(evt.timeSinceLastFrame);
	//	}
	//}

	//m_lLastTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	return FrameListener::frameStarted(evt);
}

bool ActorSceneCanvas::doUpdate(bool isIdle)
{
	if (m_lTargetFPS)
	{
		unsigned long thisTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		if ((thisTime - m_lLastTime) * m_lTargetFPS < 1000)
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

        if( Fairy::CDataManipulator::GetDataManipulator())
        { 
			Fairy::CDataManipulator::GetDataManipulator()->updateAnimationInfo();
		}		
	}

	return true;
}

void ActorSceneCanvas::setTargetFPS(unsigned long fps)
{
	if (m_lTargetFPS != fps)
	{
		m_lTargetFPS = fps;
		if (m_lTargetFPS)
		{
			m_Timer.Stop();
			m_Timer.Start(1000 / m_lTargetFPS);
		}
	}
}

void ActorSceneCanvas::InitScene(Fairy::SceneManipulator* sceneManipulator)
{
    m_pSceneManipulator = sceneManipulator;
	//在此进行D3D，相机，视口等初始化，并加载资源
    m_pSceneManipulator->setup(reinterpret_cast<Ogre::ulong>(GetHandle()), NULL);
    m_pUpdateListener = new UpdateListener(this);

	Ogre::Root::getSingleton().addFrameListener(this);
	m_lLastTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
}

void ActorSceneCanvas::SetParentFrame(ActorEditorFrame* pParentFrame)
{
	m_pParentFrame = pParentFrame;
}

// 显示当前鼠标在场景中的逻辑坐标
void ActorSceneCanvas::ShowPos(int iX, int iY)
{
	// 进行选中的临时模型的操作.

	if(NULL == m_pSceneManipulator)
	{

		return;
	}

	Ogre::Vector3 position;
	bool hit = m_pSceneManipulator->getTerrainIntersects(iX, iY, position);

	if(hit)
	{
	
		if(m_pParentFrame)
		{
			float fLogicX = 0;
			float fLogicZ = 0;

			// 缩放系数
			float fScale = 1.0;
			float fTerrainDataPosX = 0;
			float fTerrainDataPosZ = 0;
			float fTerrainDataPosY = 0;

			fTerrainDataPosX = m_pSceneManipulator->getTerrainData()->mPosition.x;
			fTerrainDataPosZ = m_pSceneManipulator->getTerrainData()->mPosition.z;
			fTerrainDataPosY = m_pSceneManipulator->getTerrainData()->mPosition.y;//
			fScale = m_pSceneManipulator->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
			
			// 进行场景坐标和逻辑坐标转换
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


// 地图跳转
void ActorSceneCanvas::GotoScene(float fX, float fY)
{
	//// 进行选中的临时模型的操作.

	if(NULL == m_pSceneManipulator)
	{

		return;
	}

	if(m_pParentFrame)
	{
		float fSceneX = 0;
		float fSceneY = 0;

		// 缩放系数
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = m_pSceneManipulator->getTerrainData()->mPosition.x;
		fTerrainDataPosZ = m_pSceneManipulator->getTerrainData()->mPosition.z;
		fTerrainDataPosY = m_pSceneManipulator->getTerrainData()->mPosition.y;//
		fScale = m_pSceneManipulator->getBaseScale();
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

		posLookAt.y =  m_pSceneManipulator->getTerrainData()->getHeightAt(fSceneX, fSceneY);
		posEye = posLookAt;
		posEye.y += 400;
		
		// 得到就的摄像机位置.
		Ogre::Vector3 oldCameraPos = m_pSceneManipulator->getCamera()->getPosition();

		// 得到方向
		Ogre::Vector3 CameraDir = m_pSceneManipulator->getCamera()->getDirection();

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
	
		// 计算摄像机的位置位置
		posEye.x = fx;
		posEye.y = oldCameraPos.y;
		posEye.z = fz;

		m_pSceneManipulator->getCamera()->setPosition(posEye);

		
	}	 
}

//消息函数处理
void ActorSceneCanvas::OnPaint(wxPaintEvent &e)
{

    wxPaintDC dc(this);
    PrepareDC(dc);

    if (GetSceneManipulator())
    {
		doUpdate();
		

    }
}

void ActorSceneCanvas::OnEraseBackground(wxEraseEvent &e)
{
    if (!GetSceneManipulator())
    {
        wxWindow::OnEraseBackground(e);
    }
}

void ActorSceneCanvas::OnSize(wxSizeEvent &e)
{
    if (GetSceneManipulator())
    {
        wxSize size = e.GetSize();
        GetSceneManipulator()->resizeWindow(size.x, size.y);
    }

}


void ActorSceneCanvas::OnTimer(wxTimerEvent& event)
{
	if (GetSceneManipulator())
	{
		GetSceneManipulator()->renderOneFrame();
	}


}

void ActorSceneCanvas::OnDestroy(wxWindowDestroyEvent& e)
{
    if (GetSceneManipulator())
    {
        GetSceneManipulator()->getRenderWindow()->setActive(false);
    }
}

void ActorSceneCanvas::OnMouseMove(wxMouseEvent& e)
{

	ShowPos(e.GetX(), e.GetY());

    if (!GetSceneManipulator())
        return;

    if (m_pCameraManip)
    {
        wxASSERT(mDragButton != wxMOUSE_BTN_NONE);
        m_pCameraManip->onMotion(e.GetX(), e.GetY());
    }

	if (mDragStarted  && e.LeftIsDown())
	{
		mDragCurrent =Ogre::Vector2(e.GetX(), e.GetY()) ;
		mDragDelta = mDragCurrent - mDragOrigin;
		mDragOrigin = mDragCurrent;

		if (Fairy::CDataManipulator::GetDataManipulator() && mCanManipulateAxis)
		{	
			Ogre::Camera* camera = GetSceneManipulator()->getCamera();
			assert (camera);
			Ogre::Vector3 oldPos = camera->getPosition();
			Fairy::LogicModel* pModel = GetDataManipulator()->m_pObjTemplate;
			if (pModel)
			{
				Ogre::Vector3 objPos = pModel->getPosition();
				Ogre::Real distance = oldPos.distance(objPos);
				Ogre::Real factor = distance*0.1/150.0;

				Ogre::Vector3 pos=Fairy::CDataManipulator::m_baseNode->getPosition();
				Ogre::Vector3 fdeltaxi = Ogre::Vector3::ZERO;
				Ogre::Quaternion qRot = Fairy::CDataManipulator::m_baseNode->getOrientation();
				//david-<<
				 if(mXax)
					 fdeltaxi = /*qRot**/(mDragDelta.x*0.1*Ogre::Vector3::UNIT_X);
				 if(mYax)
					 fdeltaxi = /*qRot**/(mDragDelta.x*0.1*Ogre::Vector3::UNIT_Y);
				 if(mZax)
					 fdeltaxi = /*qRot**/(mDragDelta.x*0.1*Ogre::Vector3::UNIT_Z);
				 //david->>

				Fairy::CDataManipulator::GetDataManipulator()->_updateCurLocatorTrans(fdeltaxi,Ogre::Quaternion::IDENTITY,true);
			}			
		}	
	}

	if(mDragRightStarted &&  e.RightIsDown())
	{
	
		mDragCurrent =Ogre::Vector2(e.GetX(), e.GetY()) ;
		mDragDelta = mDragCurrent - mDragOrigin;
		mDragOrigin = mDragCurrent;

// 		Ogre::Radian x = Ogre::Degree(mDragDelta.val[0]);
// 		Ogre::Radian y = Ogre::Degree(mDragDelta.val[1]);
// 		Fairy::CDataManipulator::m_axex->yaw(y);
// 		Fairy::CDataManipulator::m_axex->pitch(x);

		if ( Fairy::CDataManipulator::GetDataManipulator() &&(mXax || mYax || mZax) && mCanManipulateAxis)
		{			
			Ogre::Vector3 fBaseAxis = Ogre::Vector3::ZERO;
			Ogre::Quaternion fBaseRot = Fairy::CDataManipulator::m_baseNode->getOrientation();

			if(mXax)
				fBaseAxis =/* fBaseRot**/Ogre::Vector3::UNIT_X;
			if(mYax)
				fBaseAxis =/* fBaseRot**/Ogre::Vector3::UNIT_Y;
			if(mZax)
				fBaseAxis =/* fBaseRot**/Ogre::Vector3::UNIT_Z;

			//david-<<
			Ogre::Radian angle = Ogre::Degree(mDragDelta.y);
			//david->>

			Ogre::Quaternion rot(angle, fBaseAxis);			

			if(mRotFirst)
			{
				Fairy::CDataManipulator::GetDataManipulator()->_updateCurLocatorTrans(Ogre::Vector3::ZERO, rot, false);	
			}
			else
			{
				Fairy::CDataManipulator::GetDataManipulator()->_updateCurLocatorRot(rot);	
			}				
		}
	}



    if (GetActiveAction())
    {
        //GetActiveAction()->onMotion(e.GetX(), e.GetY());
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
	else
	{
        //GetSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(e.GetX(), e.GetY());
	}
    // 显示标准模型
    //if ( GetSceneManipulator()->getShowStandardModel() )
    //    GetSceneManipulator()->getHitIndicator("StandardModelIndicator")->setHitPoint(0.5,0.5);
    //else
    //    GetSceneManipulator()->getHitIndicator("StandardModelIndicator")->hide();

    m_CurrentMousePos.x = e.GetX();
    m_CurrentMousePos.y = e.GetY();

	Ogre::Vector3 position;
	bool hit = GetSceneManipulator()->getTerrainIntersects(m_CurrentMousePos.x, m_CurrentMousePos.y, position);

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

void ActorSceneCanvas::OnMouseWheel(wxMouseEvent& e)
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
    else
    {
        //GetSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
    }
}


void ActorSceneCanvas::OnMouseEnter(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

   // / GetSceneManipulator()->showHitPoint(true);
}


void ActorSceneCanvas::OnMouseLeave(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

   // GetSceneManipulator()->showHitPoint(false);
	GetSceneManipulator()->setActiveAction(NULL);
}

void ActorSceneCanvas::OnKeyDown(wxKeyEvent& e)
{
	switch(e.GetKeyCode())
	{
	case 'w':
	case 'W':
		{
			Ogre::Camera* camera = GetSceneManipulator()->getCamera();
			assert (camera);

			Ogre::Vector3 oldPos = camera->getPosition();

			Ogre::Vector3 offsetPos = camera->getDirection() * GetSceneManipulator()->getMoveSpeed();

			Ogre::Vector3 newPos = oldPos;
            newPos.x += offsetPos.x;
            newPos.z += offsetPos.z;

			GetSceneManipulator()->setCameraPosition(newPos);
			e.Skip(false);
		}
		break;
	case 's':
	case 'S':
		{
			Ogre::Camera* camera = GetSceneManipulator()->getCamera();
			assert (camera);

			Ogre::Vector3 oldPos = camera->getPosition();

			Ogre::Vector3 offsetPos = camera->getDirection() * -(GetSceneManipulator()->getMoveSpeed());

			Ogre::Vector3 newPos = oldPos;
            newPos.x += offsetPos.x;
            newPos.z += offsetPos.z;

			GetSceneManipulator()->setCameraPosition(newPos);
			e.Skip(false);
		}
		break;
	case 'a':
	case 'A':
		{
			Ogre::Camera* camera = GetSceneManipulator()->getCamera();
			assert (camera);

			Ogre::Radian yawAngle( GetSceneManipulator()->getRotateSpeed() / 360.0f );

			camera->yaw(yawAngle);
			e.Skip(false);
		}
		break;
	case 'd':
	case 'D':
		{
			Ogre::Camera* camera = GetSceneManipulator()->getCamera();
			assert (camera);

			Ogre::Radian yawAngle( -(GetSceneManipulator()->getRotateSpeed() / 360.0f) );

			camera->yaw(yawAngle);
			e.Skip(false);
		}
		break;
	case 'h':
	case 'H':
		{
			mHideAxis = !mHideAxis;
			Fairy::CDataManipulator::m_baseNode->setVisible(!mHideAxis);
			Fairy::CDataManipulator::mAxisNode_x->setVisible(!mHideAxis);
			Fairy::CDataManipulator::mAxisNode_y->setVisible(!mHideAxis);
			Fairy::CDataManipulator::mAxisNode_z->setVisible(!mHideAxis);
		}
		break;
 	case 'b':
	case 'B':
		{
			GetDataManipulator()->switchBoundingBox(true);
		}
		break;
	case 't':
	case 'T':
		{
			GetSceneManipulator()->setTerrainVisible(!GetSceneManipulator()->getTerrainVisible());
		}
		break;
	case 'g':
	case 'G':
		{
			GetSceneManipulator()->setGridVisible(!GetSceneManipulator()->getGridVisible());
		}
		break;
	case 'r':
	case 'R':
		{
			mRotFirst = !mRotFirst;
		}
		break;
	//case 'C':
 //	case 'c':
 //		{
	//		Fairy::LogicModelManager::getSingleton().showModelBoundingBox(!Fairy::LogicModelManager::getSingleton().isShowBoundingBox()); 			
 //		}
 //		break;
// 
// 	case 'y':
// 	case 'Y':
// 		{
// 			mYax = !mYax;
// 			mXax = mZax = false;
// 		}
// 		break;
// 
// 	case 'z':
// 	case 'Z':
// 		{
// 			mZax = !mZax;
// 			mXax = mYax = false;
// 		}
		//break;
	//case 'z':
	//case 'Z':
 //       {
	//		GetSceneManipulator()->setActiveAction("ManipObjectAction");
 //           GetSceneManipulator()->setUseAxis(!GetSceneManipulator()->getUseAxis());
 //       }
 //       e.Skip(false);
 //       break;
	}
}

void ActorSceneCanvas::OnMouseCaptureChanged(wxMouseCaptureChangedEvent& e)
{
    if (e.GetEventObject() == this && e.GetCapturedWindow() != this)
    {
        switch (mDragButton)
        {
        case wxMOUSE_BTN_MIDDLE:
            {
                wxASSERT(m_pCameraManip);
                wxPoint pt = wxGetMousePosition();
                pt = ScreenToClient(pt);
                m_pCameraManip->onEnd(pt.x, pt.y, true);
                m_pCameraManip = NULL;
                break;
            }

        case wxMOUSE_BTN_LEFT:
            {
                //wxASSERT(GetActiveAction());
                //wxPoint pt = wxGetMousePosition();
                //pt = ScreenToClient(pt);
                //GetActiveAction()->onEnd(pt.x, pt.y, true);
                m_pUpdateListener->disable();
            }
            break;
        }

        mDragButton = wxMOUSE_BTN_NONE;
    }
}

void ActorSceneCanvas::OnMiddleDown(wxMouseEvent& e)
{
    SetFocus();

    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

    wxASSERT(!m_pCameraManip);
    wxASSERT(mDragButton == wxMOUSE_BTN_NONE);

    if (!e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
    {
        // (None)
        m_pCameraManip = GetSceneManipulator()->getManip("CameraDragMove");
    }
    else if (!e.ControlDown() && !e.AltDown() && e.ShiftDown() && !e.MetaDown())
    {
        // Shift
        m_pCameraManip = GetSceneManipulator()->getManip("CameraDragRotate");
    }
    else if (e.ControlDown() && !e.AltDown() && !e.ShiftDown() && !e.MetaDown())
    {
        // Ctrl
        m_pCameraManip = GetSceneManipulator()->getManip("CameraPan");
    }
    else if (!e.ControlDown() && e.AltDown() && !e.ShiftDown() && !e.MetaDown())
    {
        // Alt
        m_pCameraManip = GetSceneManipulator()->getManip("CameraRotate");
    }

    if (m_pCameraManip)
    {
        mDragButton = e.GetButton();
        m_pCameraManip->onBegin(e.GetX(), e.GetY());
        CaptureMouse();
    }

}

void ActorSceneCanvas::OnMiddleUp(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;

    if (!HasCapture())
        return;

    if (mDragButton == wxMOUSE_BTN_NONE || !e.Button(mDragButton))
        return;

    wxASSERT(m_pCameraManip);

    m_pCameraManip->onEnd(e.GetX(), e.GetY(), false);
    m_pCameraManip = NULL;

    mDragButton = wxMOUSE_BTN_NONE;
    ReleaseCapture();

}


void ActorSceneCanvas::OnRightDown(wxMouseEvent& e)
{

	mDragRightStarted = true;
	mDragOrigin = Ogre::Vector2(e.GetX(),e.GetY());
	mDragDelta = Ogre::Vector2(0.0f,0.0f);
}

void ActorSceneCanvas::OnRightUp(wxMouseEvent& e)
{
	mDragRightStarted = false;
	mDragOrigin  =Ogre::Vector2(e.GetX(), e.GetY()) ;
}

void ActorSceneCanvas::OnLeftDown(wxMouseEvent& e)
{
    SetFocus();
	

	mDragStarted = true;
	mDragOrigin = Ogre::Vector2(e.GetX(),e.GetY());
	mDragDelta = Ogre::Vector2(0.0f,0.0f);
	
    if (!GetSceneManipulator())
        return;

    if (HasCapture())
        return;

    wxASSERT(!m_pCameraManip);
    wxASSERT(mDragButton == wxMOUSE_BTN_NONE);

    if (!GetActiveAction())
        return;
	//GetSceneManipulator()->setActiveAction("ManipObjectAction");
	//GetActiveAction()->setParameter("%CurrentMode", "");

    m_pUpdateListener->enable();
    mDragButton = e.GetButton();
    GetActiveAction()->onBegin(e.GetX(), e.GetY());
    CaptureMouse();


}

void ActorSceneCanvas::_resetAxisMaterial()
{
	if (mXax)
	{
		GetDataManipulator()->m_pMaterialX->setSelfIllumination(1,0,0);
	}
	else if (mYax)
	{
		GetDataManipulator()->m_pMaterialY->setSelfIllumination(0,1,0);
	}
	else if(mZax)
	{
		GetDataManipulator()->m_pMaterialZ->setSelfIllumination(0,0,1);
	}
}


void ActorSceneCanvas::OnLeftUp(wxMouseEvent& e)
{
    if (!GetSceneManipulator())
        return;
	mDragStarted = false;
	mDragOrigin  =Ogre::Vector2(e.GetX(), e.GetY()) ;

	Ogre::Ray ray = GetSceneManipulator()->getWindowToViewportRay(e.GetX(),e.GetY());
	bool bAxis = false;
	std::pair<bool, Ogre::Real> result = Fairy::rayCollide(ray, ( Ogre::MovableObject*) Fairy::CDataManipulator::mAxis_x, true);
	if (result.first)
	{
		_resetAxisMaterial();

		mXax = !mXax;
		mYax = mZax = false;

		if (mXax)
		{
			GetDataManipulator()->m_pMaterialX->setSelfIllumination(1.0,1.0,0);
		}

		bAxis = true;
	}
	if (!bAxis)
	{
		result = Fairy::rayCollide(ray, ( Ogre::MovableObject*) Fairy::CDataManipulator::mAxis_y, true);
		if (result.first)
		{
			_resetAxisMaterial();

			mYax = !mYax;
			mXax = mZax = false;			

			if (mYax)
			{
				GetDataManipulator()->m_pMaterialY->setSelfIllumination(1.0,1.0,0);
			}

			bAxis = true;
		}
	}
	if (!bAxis)
	{
		result = Fairy::rayCollide(ray, ( Ogre::MovableObject*) Fairy::CDataManipulator::mAxis_z, true);
		if (result.first)
		{
			_resetAxisMaterial();

			mZax = !mZax;
			mXax = mYax = false;

			if (mZax)
			{
				GetDataManipulator()->m_pMaterialZ->setSelfIllumination(1.0,1.0,0);
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

    m_pUpdateListener->disable();
    mDragButton = wxMOUSE_BTN_NONE;
    ReleaseCapture();


}
