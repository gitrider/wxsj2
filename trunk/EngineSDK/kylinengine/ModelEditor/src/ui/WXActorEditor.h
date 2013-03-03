#ifndef __ACTORITORAPP_H__
#define __ACTORITORAPP_H__

#include <wx/app.h>
#include "WXEditorFrame.h"
#pragma warning(disable:4099)

namespace Fairy {
    class SceneManipulator;
}

namespace Ogre
{
    class Exception;
}

//class FairyEditorFrame;

class ActorEditorApp : public wxApp
{
public:
	ActorEditorApp();
	~ActorEditorApp();
public:
	bool	m_bIsRenderEnable;
	bool	m_bRenderUserDXorGL;				//use d3dx when true, use gl when false.
protected:
	ActorEditorFrame* m_pMainFrame;			//主对话框
    Fairy::SceneManipulator* mSceneManipulator;
    wxLocale m_locale;	
public:
	virtual bool OnInit();
    virtual int OnExit();
	virtual void OnFatalException();	//记录异常使用
    DECLARE_EVENT_TABLE()
    void OnIdle(wxIdleEvent &e);
protected:
    /// 初始化实现
    bool InitImpl(void);
    void DisplayExceptionMessageBox(const Ogre::Exception& e);
    /// 异常处理函数
    void _ExceptionHandler(PEXCEPTION_POINTERS pException);
};
DECLARE_APP(ActorEditorApp)
#endif // 
