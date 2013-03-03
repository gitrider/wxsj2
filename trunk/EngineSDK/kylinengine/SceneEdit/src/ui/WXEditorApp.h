#ifndef __FAIRYEDITORAPP_H__
#define __FAIRYEDITORAPP_H__

#include <wx/app.h>

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------
//����ϵͳ��ʼ��
typedef bool (__stdcall* FUNC_INITSOUND)(const char* szSndPath, const char* szDBCFile);
//��ѯ�����б�
typedef bool (__stdcall* FUNC_ENUMSOUNDFILE)(int nIndex, char* szSoundFileBuf, int nBufSize, int& id);
//�����ṩ���������Žӿ�,��������ԴHANDLE
typedef int (__stdcall*  FUNC_PLAYSOUND)(const char* szSoundFile, float* fvPos, bool bLoop);
//�����ṩ������ֹͣ�ӿ�
typedef void (__stdcall* FUNC_STOPSOUND)(int nHandle);
//�����ṩ����ϵͳ���ͷŽӿ�
typedef void (__stdcall* FUNC_DEINIT)(void);

typedef void (__stdcall* FUNC_SETSOUNDVOLUME)(int handle, float gain);

class FairyEditorFrame;

namespace Fairy {
    class SceneManipulator;
}

namespace Ogre
{
    class Exception;
}
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class FairyEditorApp : public wxApp
{
public:
	bool	mIsRenderEnable;
	bool	mRenderUserDXorGL;//use d3dx when true, use gl when false.

	FUNC_INITSOUND funcInit;
	FUNC_ENUMSOUNDFILE funcEnumSoundFile;
	FUNC_PLAYSOUND funcPlay;
	FUNC_STOPSOUND funcStop;
    FUNC_DEINIT funcDeInit;
    FUNC_SETSOUNDVOLUME funcSetSoundVolume;

protected:
    FairyEditorFrame* mMainFrame;
    Fairy::SceneManipulator* mSceneManipulator;

    wxLocale _locale;	

public:
    FairyEditorApp();
    ~FairyEditorApp();

    FairyEditorFrame* GetMainFrame(void) const
    {
        return mMainFrame;
    }

    Fairy::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();

    // This is only called if OnInit() returned true so it's a good place to do
    // any cleanup matching the initializations done there.
    virtual int OnExit();

    virtual bool OnExceptionInMainLoop();
    virtual void OnFatalException();
    virtual void OnUnhandledException();

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnIdle(wxIdleEvent &e);

protected:
	void SetKeyboardShortcuts(void);

    /// �쳣������
    void _ExceptionHandler(PEXCEPTION_POINTERS pException);

    /// ��ʼ��ʵ��
    bool _InitImpl(void);

    void _displayExceptionMessageBox(const Ogre::Exception& e);

};

DECLARE_APP(FairyEditorApp)

#endif // 
