#ifndef __FAIRYSCENECANVAS_H__
#define __FAIRYSCENECANVAS_H__

#include <wx/window.h>
#include <OgreVector3.h>
#include <OgreFrameListener.h>


#include "WXEditorFrame.h"
#include "Core/FairyObject.h"


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
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a canvas to render scene
class FairySceneCanvas : public wxWindow, public Ogre::FrameListener
{
    DECLARE_CLASS(FairySceneCanvas)

    typedef std::map<const Fairy::ObjectPtr,Ogre::Vector3> ObjectPosition;
    typedef std::map<const Fairy::ObjectPtr,Ogre::Quaternion> ObjectOrientation;
protected:
    class UpdateListener;
    friend UpdateListener;

    Fairy::SceneManipulator *mSceneManipulator;
    UpdateListener* mUpdateListener;
    Fairy::Manip* mCameraManip;
    int mDragButton;

    ObjectPosition objPos;
    ObjectOrientation objOrient;
    Fairy::TerrainGrid* mTerrGrid;

	wxFrame *mParentFrame;

	unsigned long mLastTime;
	unsigned long mTargetFPS;

	wxTimer   mTimer;

public:


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��ʾ����
	//
	FairyEditorFrame* m_pParentFrame;
	void SetParentFrame(FairyEditorFrame* pParentFrame);
	void ShowPos(int iX, int iY);
	
	// ��ͼ��ת
	void GotoScene(float fX, float fY);

	//
	// ��ʾ����
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

    FairySceneCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                     long style = wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS);
    ~FairySceneCanvas();

    wxPoint mCurrentMousePos;

    void InitScene(Fairy::SceneManipulator* sceneManipulator);

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    Fairy::Action* GetActiveAction(void) const;

	virtual bool frameStarted(const Ogre::FrameEvent& evt);

	bool doUpdate(bool isIdle = false);

	void setTargetFPS(unsigned long fps);

	// ShiftDown
	bool IsShiftDown()
	{
		return ::GetKeyState(VK_LSHIFT) < 0 || ::GetKeyState(VK_RSHIFT) < 0;
	}

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnPaint(wxPaintEvent &e);
    void OnEraseBackground(wxEraseEvent &e);
    void OnSize(wxSizeEvent &e);

    void OnMouseCaptureChanged(wxMouseCaptureChangedEvent& e);
    void OnMouseEnter(wxMouseEvent& e);
    void OnMouseLeave(wxMouseEvent& e);
    void OnMouseWheel(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMiddleDown(wxMouseEvent& e);
    void OnMiddleUp(wxMouseEvent& e);
    void OnLeftDown(wxMouseEvent& e);
    void OnLeftUp(wxMouseEvent& e);

    void OnKeyDown(wxKeyEvent& e);
    void OnKeyUp(wxKeyEvent& e);
    void OnChar( wxKeyEvent &event );

    void OnContextMenu(wxContextMenuEvent& e);
    void OnSnapToGrid(wxCommandEvent& e);
    void OnMoveByXYZ(wxCommandEvent& e);
	void OnUpdateMoveByXYZ(wxUpdateUIEvent& e);
    void OnSnapRotation(wxCommandEvent& e);
	void OnUpdateSnapRotation(wxUpdateUIEvent& e);

    void OnDestroy(wxWindowDestroyEvent& e);

    void OnTerrainPasteSetting(wxCommandEvent& e);
    void OnUpdateTerrainPasteSetting(wxUpdateUIEvent& e);

	void OnTerrainRotateRightAngle(wxCommandEvent& e);

    bool IsSpaceDown()
    {
        return ::GetKeyState(VK_SPACE) < 0;
    }

	// CtrlDown
	bool IsCtrlDown()
	{
		   return ::GetKeyState(VK_LCONTROL) < 0 || ::GetKeyState(VK_RCONTROL) < 0;
	}
	bool IsLeftCtrlDown()
	{
		return ::GetKeyState(VK_LCONTROL) < 0;
	}
	bool IsRightCtrlDown()
	{
		return ::GetKeyState(VK_RCONTROL) < 0;
	}
	bool IsLeftShiftDown()
	{
		return ::GetKeyState(VK_LSHIFT) < 0;
	}
	bool IsRightShiftDown()
	{
		return ::GetKeyState(VK_RSHIFT) < 0;
	}

	// AltDown
	bool IsAltDown()
	{
		return ::GetKeyState(VK_LMENU) < 0 || ::GetKeyState(VK_RMENU) < 0;
	}
	bool IsLeftAltDown()
	{
		return ::GetKeyState(VK_LMENU) < 0;
	}
	bool IsRightAltDown()
	{
		return ::GetKeyState(VK_RMENU) < 0;
	}

	// Captial (��д״̬)
	bool IsCapital()
	{
		return (::GetKeyState(VK_CAPITAL) & 0x0001) != 0;
	}

	bool IsKeyDown(int key)
    {
        return ( ::GetAsyncKeyState( key ) & (1<<15) ) != 0;
    }
    int GetAxisIndex(wxString& strAxis);
    bool ProcessArrowKey(int singal);

	void OnTimer(wxTimerEvent& event);

	void SetVerticalView();

};

#endif // 
