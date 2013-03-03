#ifndef __ACTORSCENECANVAS_H__
#define __ACTORSCENECANVAS_H__

#include <wx/window.h>
#include <OgreVector3.h>
#include <OgreFrameListener.h>


#include "WXEditorFrame.h"
#include "Core/FairyObject.h"
#include "SceneManipulator.h"
#include "Core/Fairystaticentityobject.h"


// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

namespace Fairy {
    class SceneManipulator;
    class Manip;
    class Action;
    class TerrainGrid;
}

const int FIX_FRAME_TIMTER = 10100;

class ActorEditorFrame;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a canvas to render scene
class ActorSceneCanvas : public wxWindow, public Ogre::FrameListener
{
    DECLARE_CLASS(FairySceneCanvas)

    typedef std::map<const Fairy::ObjectPtr,Ogre::Vector3> ObjectPosition;
    typedef std::map<const Fairy::ObjectPtr,Ogre::Quaternion> ObjectOrientation;
protected:
    class UpdateListener;
    friend UpdateListener;

    Fairy::SceneManipulator *m_pSceneManipulator;
    UpdateListener* m_pUpdateListener;
    Fairy::Manip* m_pCameraManip;
    int mDragButton;

    ObjectPosition objPos;
    ObjectOrientation objOrient;
    Fairy::TerrainGrid* m_pTerrGrid;

	wxFrame *mParentFrame;

	unsigned long m_lLastTime;
	unsigned long m_lTargetFPS;

	wxTimer   m_Timer;
	Ogre::Entity* m_pEntity;
public:
    //////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 显示坐标
	//
	bool mDragStarted;
	Ogre::Vector2 mDragOrigin;
	Ogre::Vector2 mDragCurrent;
	Ogre::Vector2 mDragDelta;
	bool mDragRightStarted;
	bool mXax;
	bool mYax;
	bool mZax;
	bool mHideAxis;
	bool mCanManipulateAxis;
	bool mRotFirst;

	ActorEditorFrame* m_pParentFrame;
	void SetParentFrame(ActorEditorFrame* pParentFrame);
	void ShowPos(int iX, int iY);
	
	// 地图跳转
	void GotoScene(float fX, float fY);

	//
	// 显示坐标
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

    ActorSceneCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                     long style = wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS);
    ~ActorSceneCanvas();

    wxPoint m_CurrentMousePos;

    void InitScene(Fairy::SceneManipulator* sceneManipulator);
						  
    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return m_pSceneManipulator;
    }

	Fairy::Action* GetActiveAction(void) const;

	virtual bool frameStarted(const Ogre::FrameEvent& evt);

	bool doUpdate(bool isIdle = false);
	void _resetAxisMaterial();

	void setTargetFPS(unsigned long fps);

    DECLARE_EVENT_TABLE()
//消息函数处理
    void OnPaint(wxPaintEvent &e);					//绘图
    void OnEraseBackground(wxEraseEvent &e);		//清理背景
	void OnSize(wxSizeEvent &e);					//尺寸
	void OnTimer(wxTimerEvent& event);
	void OnDestroy(wxWindowDestroyEvent& e);
	void OnMouseMove(wxMouseEvent& e);
	void OnMouseWheel(wxMouseEvent& e);
    void OnMouseEnter(wxMouseEvent& e);
    void OnMouseLeave(wxMouseEvent& e);
	void OnMiddleDown(wxMouseEvent& e);
	void OnMiddleUp(wxMouseEvent& e);
	void OnKeyDown(wxKeyEvent& e);
    void OnMouseCaptureChanged(wxMouseCaptureChangedEvent& e);
	void OnLeftDown(wxMouseEvent& e);
	void OnLeftUp(wxMouseEvent& e);
	void OnRightDown(wxMouseEvent& e);
	void OnRightUp(wxMouseEvent& e);

};

#endif // 
