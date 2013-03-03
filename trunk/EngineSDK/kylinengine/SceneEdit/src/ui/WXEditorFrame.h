#ifndef __FAIRYEDITORFRAME_H__
#define __FAIRYEDITORFRAME_H__

#include <wx/frame.h>
#include "smtcolrdlgg.h"

#include <OgreVector3.h>
#include <OgreString.h>

#include "TriInfoInMap.h"
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class FairySceneCanvas;
class BrushSelector;
class ModelSelector;
class ActorSelector;
class ParticleSystemSelector;
class SceneBrowser;
class ObjectPropertyEditor;

class wxLayoutManager;
struct wxHostInfo;

class wxConfigBase;
class wxSpinEvent;
class wxListEvent;
class SceneInfoMapDialog;

// ������������༭�Ի���
class CRegionEditDlgSector;

// ����༭�Ի���
class CMonsterEditDlg;

// ������༭
class CGrowPointEditDlg;

// npcѲ�߱༭
class CNpcPatrolEditDlg;

// �¼�����༭
class CEventAreaEditDlg;

// ̯λ��Ϣ
class CStallInfoDlg;

// �����༭
class SoundEditDialog;

// Ѱ·�༭
class CPathFindEditDlg;

class DatuPointEditDialog;

class AutoSaveSceneController;
class CameraTrackSettingDialog;

namespace Fairy {
    class SceneManipulator;
}

class SceneInfomationDialog;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new frame
class FairyEditorFrame : public wxFrame
{
    DECLARE_CLASS(FairyEditorFrame)
    WX_DECLARE_CONTROL_CONTAINER();

public:

	// ������������༭�Ի���.
	CRegionEditDlgSector* m_pRegionEditSelector;
	
	// ����༭�Ի���
	CMonsterEditDlg*	  m_pMonsterEditDlg;

	// ������༭�Ի���
	CGrowPointEditDlg*	  m_pGrowPointEditDlg;

	// npcѲ��·�߱༭
	CNpcPatrolEditDlg*    m_pNpcPatrolEditDlg;

	// �¼�����
	CEventAreaEditDlg*	  m_pEventAreaEditDlg;

	// ̯λ��Ϣ
	CStallInfoDlg*		  m_pStallInfoDlg;

	// �����༭			  
    SoundEditDialog*      mSoundEditDialog;

	// Ѱ·�༭
	CPathFindEditDlg*    m_pPathFindEditDlg;

	DatuPointEditDialog*  mDatuPointEditDialog;
	// ������
	wxToolBar*		m_pToolbar;
	
	// ���泡��������
	Fairy::SceneManipulator* m_pSceneManipulator;

	// ���õ�ǰ�ĳ����߼�����
	void SetCurSceneLogicPos(float fx, float fz);

	// ��ͼ��ת
	void OnSceneGoto(wxCommandEvent &event);


protected:
    FairySceneCanvas* mCanvas;
	wxSmartColourDialog* mSmartColourDialog;

    BrushSelector* mBrushSelector;
    ModelSelector* mModelSelector;
    ActorSelector* mActorSelector;
    ParticleSystemSelector* mParticleSystemSelector;
    SceneBrowser* mSceneBrowser;
    ObjectPropertyEditor* mObjectPropertyEditor;
    SceneInfoMapDialog* mSceneInfoDlg;
    //CameraTrackSettingDialog* mCameraTrackSettingDialog;

    wxTextCtrl* mLogWindow;

    wxLayoutManager* mLayout;
    wxMenu* mWindowMenu;

    bool mLockBrushSize;
	bool mIsTerrainLightmapUpdated;

    wxString m_lastTitle;
    wxString m_lastSceneDirectory;
    wxString m_sceneFilename;

    int mSpinnerValue;
    wxString mByAxis;
    int mSegment;
    bool mActiveSnapRotation;

	Ogre::String mInfoString;
	Ogre::String mManipInfoString;

	bool mUseRealCamera;
	bool mUseSnapRotation;

    int mTextureFilterOptions;

    int mCreateLevelOptions;

    SceneInfomationDialog* mSceneInfomationDialog;

    AutoSaveSceneController* mAutoSaveSceneController;

protected:
    void CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo);
    void InitMenuBar(void);
    void InitToolBar(wxToolBar* toolbar);
    void InitStatusBar(wxStatusBar* statusbar);

public:
    FairyEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    ~FairyEditorFrame();

    FairySceneCanvas* GetCanvas(void) const
    {
        return mCanvas;
    }
    SceneBrowser* GetSceneBrowser(void) const
    {
        return mSceneBrowser;
    }

    Fairy::SceneManipulator* GetSceneManipulator(void) const;

    void InitScene(Fairy::SceneManipulator* sceneManipulator);

	void SetKeyboardShortcuts(void);
    //////////////////////////////////////////////////////////////////////////

    void LoadSettings(wxConfigBase* cfg);
    void SaveSettings(wxConfigBase* cfg) const;

    bool Save(void);
    bool SaveAs(void);
    bool SaveModified(bool force = false);

    bool SaveImpl(bool autoSave = false);

	void resetPaintAction();
	void onMotionPaintAction();

    //////////////////////////////////////////////////////////////////////////
    bool ProcessEvent(wxEvent& e);

    void SetSpinnerValue(int value);
    int GetSpinnerValue();

    wxString GetByAxis();
    void SetByAxis(wxString& strAxis);

    void SetSnapRotationValue(int value);
    int GetSnapRotationValue();

    void SetActiveSnapRotation(bool value);
    bool GetActiveSnapRotation();

    AutoSaveSceneController* getAutoSaveController(void)
    {
        return mAutoSaveSceneController;
    }

    // ��ȡ��ǰѡ�еĵ��Σ������ַ���������clipboard��
    //void GetSelectedTerrainInfo( Ogre::String &infoString );

	void applyCurDiffuseColour();
	void copyPaintAction(const Ogre::String& textureName);

    DECLARE_EVENT_TABLE()	
    void OnUpdateWindow(wxUpdateUIEvent& e);
    void OnCloseWindow(wxCloseEvent& e);

    void OnFileNew(wxCommandEvent& e);
    void OnFileOpen(wxCommandEvent& e);
    void OnFileSave(wxCommandEvent& e);
    void OnFileSaveAs(wxCommandEvent& e);

    void OnUpdateFileNew(wxUpdateUIEvent& e);
    void OnUpdateSave(wxUpdateUIEvent& e);
    void OnUpdateSaveAs(wxUpdateUIEvent& e);

    void OnUndo(wxCommandEvent& e);
    void OnRedo(wxCommandEvent& e);
    void OnUpdateUndo(wxUpdateUIEvent& e);
    void OnUpdateRedo(wxUpdateUIEvent& e);

    void OnLockCameraHeight(wxCommandEvent& e);
    void OnUpdateLockCameraHeight(wxUpdateUIEvent& e);

	void OnSelectAllObjects(wxCommandEvent& e);
    void OnUpdateSelectAllObjects(wxUpdateUIEvent& e);

    void OnLockBrushSize(wxCommandEvent& e);
    void OnUpdateLockBrushSize(wxUpdateUIEvent& e);
    void OnBrushSizeChanged(wxSpinEvent& e);
    void OnBrushSizeTextChanged(wxCommandEvent& e);
    void OnUpdateBrushSizeSpin(wxUpdateUIEvent& e);

    void OnTerrainHeightChanged(wxSpinEvent& e);
    void OnTerrainHeightTextChanged(wxCommandEvent& e);
    void OnUpdateTerrainHeightSpin(wxUpdateUIEvent& e);

	void OnTerrainHeightIncrementChanged(wxSpinEvent& e);
	void OnTerrainHeightIncrementTextChanged(wxCommandEvent& e);
	void OnUpdateTerrainHeightIncrementSpin(wxUpdateUIEvent& e);

    void OnHeightActionaffectRangeChanged(wxSpinEvent& e);
    void OnHeightActionaffectRangeTextChanged(wxCommandEvent& e);
    void OnUpdateHeightActionaffectRangeSpin(wxUpdateUIEvent& e);

	void OnHeightActionExpValueTextChanged(wxCommandEvent& e);
	void OnUpdateHeightActionExpValue(wxUpdateUIEvent& e);

	void OnResetCamera(wxCommandEvent& e);

	// hack ��ʱ���÷���Ч��
	void OnBlurAmountTextChanged(wxCommandEvent& e);
	void OnShineAmountTextChanged(wxCommandEvent& e);
	void OnUpdateFloodlightingControls(wxUpdateUIEvent& e);

	void OnHeightAdjustSpeedTextChanged(wxCommandEvent& e);
	void OnUpdateHeightAdjustSpeed(wxUpdateUIEvent& e);

    void OnShowStatPanel(wxCommandEvent& e);
    void OnUpdateShowStatPanel(wxUpdateUIEvent& e);
    void OnShowLogoPanel(wxCommandEvent& e);
    void OnUpdateShowLogoPanel(wxUpdateUIEvent& e);

    void OnToggleToolWindows(wxCommandEvent& e);
    void OnUpdateToggleToolWindows(wxUpdateUIEvent& e);
    void OnFullScreen(wxCommandEvent& e);
    void OnUpdateFullScreen(wxUpdateUIEvent& e);

    void OnDeleteStaticObject(wxCommandEvent& e);
    void OnUpdateDeleteStaticObject(wxUpdateUIEvent& e);

    void OnSaveTerrainInfoToFile(wxCommandEvent& e);
    void OnUpdateSaveTerrainInfoToFile(wxUpdateUIEvent& e);
    void OnLoadTerrainInfoFromFile(wxCommandEvent& e);
    void OnUpdateLoadTerrainInfoFromFile(wxUpdateUIEvent& e);

	// ����߶�ͼ
	void OnImportHeightMap(wxCommandEvent& e);
	void OnUpdateImportHeightMap(wxUpdateUIEvent& e);
	// �����߶�ͼ
	void OnExportHeightMap(wxCommandEvent& e);
	void OnUpdateExportHeightMap(wxUpdateUIEvent& e);

	void OnSaveManipInfoToFile(wxCommandEvent& e);
	void OnUpdateSaveManipInfoToFile(wxUpdateUIEvent& e);
	void OnLoadManipInfoFromFile(wxCommandEvent& e);
	void OnUpdateLoadManipInfoFromFile(wxUpdateUIEvent& e);

	void OnSaveEnviromentInfoToFile(wxCommandEvent& e);
	void OnUpdateSaveEnviromentInfoToFile(wxUpdateUIEvent& e);
	void OnLoadEnviromentInfoFromFile(wxCommandEvent& e);
	void OnUpdateLoadEnviromentInfoFromFile(wxUpdateUIEvent& e);

    void OnShowStandardModel(wxCommandEvent& e);
    void OnUpdateShowStandardModel(wxUpdateUIEvent& e);

    void OnSelectAction(wxCommandEvent& e);
    void OnUpdateSelectAction(wxUpdateUIEvent& e);

	void OnTerrainLightmapOption(wxCommandEvent& e);
	void OnTerrainLightmapHide(wxCommandEvent& e);
	void OnUpdateTerrainLightmapHide(wxUpdateUIEvent& e);
	void OnTerrainLightmapLowQuality(wxCommandEvent& e);
	void OnUpdateTerrainLightmapLowQuality(wxUpdateUIEvent& e);
	void OnTerrainLightmapHighQuality(wxCommandEvent& e);
	void OnUpdateTerrainLightmapHighQuality(wxUpdateUIEvent& e);

    void OnTerrainSurfaceParameters(wxCommandEvent& e);
    void OnSceneAnimationQuality(wxCommandEvent& e);
    void OnObjectCreateLevel(wxCommandEvent& e);

    void OnCreateMiniMap(wxCommandEvent& e);

	void OnReadcBrushInfo(wxCommandEvent& e);

	void OnSelectBrushShape(wxCommandEvent& e);
	void OnUpdateSelectBrushShape(wxUpdateUIEvent& e);

    void OnAlignPosition(wxCommandEvent& e);
    void OnUpdateAlignPosition(wxUpdateUIEvent& e);

    void OnExit(wxCommandEvent& e);

    //void OnCameraSetting(wxCommandEvent& e);
    void OnPreferSetting(wxCommandEvent& e);

	void OnEraseTextureByName(wxCommandEvent& e);

    void OnEntireTerrainEnableDecal(wxCommandEvent& e);

    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);

	void OnCreateLiquid(wxCommandEvent& e);
	void OnCreateLight(wxCommandEvent& e);
	void OnCreateObjectArray(wxCommandEvent& e);

    void OnUpdateCutCopyPaste(wxUpdateUIEvent& e);

	void OnReshapeTerrain(wxCommandEvent& e);
	void OnUpdateReshapeTerrain(wxUpdateUIEvent& e);

	void OnEnable32BitTextures(wxCommandEvent& e);
	void OnUpdateEnable32BitTextures(wxUpdateUIEvent& e);

	void OnReloadTextures(wxCommandEvent& e);
	void OnUpdateReloadTextures(wxUpdateUIEvent& e);

	void OnUnloadUselessResources(wxCommandEvent& e);
	void OnUpdateUnloadUselessResources(wxUpdateUIEvent& e);

    void OnShowAllIndicators(wxCommandEvent& e);
    void OnUpdateShowAllIndicators(wxUpdateUIEvent& e);

    void OnSelectPostFilter(wxCommandEvent& e);
    void OnUpdateSelectPostFilter(wxUpdateUIEvent& e);

    void OnSelectShadowTechnique(wxCommandEvent& e);
    void OnUpdateSelectShadowTechnique(wxUpdateUIEvent& e);

    void OnSelectDefaultTextureFiltering(wxCommandEvent& e);
    void OnUpdateSelectDefaultTextureFiltering(wxUpdateUIEvent& e);

    void OnSelectSceneCreateLevel(wxCommandEvent& e);

    void OnUpdateSelectSceneCreateLevel(wxUpdateUIEvent& e);

	void OnUseRealCamera(wxCommandEvent& e);
	void OnUpdateUseRealCamera(wxUpdateUIEvent& e);

	void OnUseSnapRotation(wxCommandEvent& e);
	void OnUpdateUseSnapRotation(wxUpdateUIEvent& e);

	void OnFrameRateTextChanged(wxCommandEvent& e);

    // ��ȡ��ǰѡ�еĵ��Σ������ַ���������clipboard��
    void GetSelectedTerrainInfo( Ogre::String &infoString );

    void OnSceneInfomation(wxCommandEvent& e);

	///////////////////////////////////////////////////////////////////////////////////
	//
	// �����������������
	//
public:


	//
	// ���������������Ϣ��.
	//
	typedef std::map<std::string, TRI_INFO_VECTOR> BUILDING_COLLISION_MAP;
	BUILDING_COLLISION_MAP	m_buildingCollisionInfoLibMap;

	// ע���ڵ�ͼ�е�������ӳ���
	CTriInfoInMap	m_TriInMapInfoMap;



	//
	// �õ�����������������Ϣ��
	// 
	// return 0 -- �ɹ����� 0 -- ʧ�� ���������Ϣ����.
	//
	DWORD GetBuildingCollisionInfo();


	//
	// ��ע�ᵽ��ͼ�е���������Ϣ�洢���ļ���
	// �ļ��������볡���ļ���������ͬ�� ��չ����Ϊbcol.
	//
	// �������ļ����֣� ������չ���� ��·����Ϣ��
	void SaveBuildingCollisionInfo(wxString strCollisionFile);


	//// ͨ�������ָ��õ�����ģ�͵����֡�
	////
	//// ��������̬����ģ�͵�����.
	//// 
	//// return �����mesh�ļ�����.
	//wxString GetMeshNameByObjectPtr(const Fairy::ObjectPtr& object);//

	//
	// ��������������������ݡ�
	//
	void CreateBuildingCollisionData();


	//
	// ���������ƽ�ƣ� ��ת�� ���ŷ���������λ�ñ任����.
	//
	void BuildTransformMatrix(Ogre::Matrix4& Matrix,  const Ogre::Vector3& position, const Ogre::Quaternion rotate, const  Ogre::Vector3 scale);


	//
	// ���ݽ���������ְѽ������������ע�ᵽ��ͼ��.
	//
	// ����1 const std::string& strMeshName �� ����������֡�
	// 
	// ����2 Ogre::Matrix4& TransformMatrix -  �������ڵ�ͼ�еı任����.
	void RegisterCollisionToMap(const std::string& strMeshName, Ogre::Matrix4& TransformMatrix);


	//
	// �ӽ�����������������ļ��ж�ȡ�����ε���Ϣ��
	//
	// ����1�� std::string strFileName ������������������ļ���
	//
	// ����2�� TRI_INFO_VECTOR &TriInfoVector ���������ε�����.
	//
	// return�� 0 - �ɹ��� ��0 ʧ�ܡ����庬�������Ҫ������
	DWORD LoadCollisionDataFromFile(std::string strFileName, TRI_INFO_VECTOR &TriInfoVector);


	//
	// ����������Ϣд����ͼ����������λ�õ�ӳ����С�
	//
	// ����1�� �������б�
	// 
	// ����2�� �����εı任����
	//
	void WriteCollisionDataToTriMap(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix);


	// 
	// ʹ�þ������������ཻ��������������Ϣд����ͼ����������λ�õ�ӳ����С�
	//
	// ����1�� �������б�
	// 
	// ����2�� �����εı任����
	//
	void WriteCollisionDataToTriMap_Rectangle(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix);


	
	// 
	// �������������Ƿ��ཻ��
	//
	// ����1�� �����ε�1��
	// 
	// ����2�� �����ε�2��
	//
	// ����3�� �����ε�3��
	//
	// ����1�� ���ε�1��
	// 
	// ����2�� ���ε�2��
	//
	// ����3�� ���ε�3��
	//
	// ����4�� ���ε�4��
	//
	// ����ֵ��1 �ཻ�� 0 ���ཻ��
	// 
	bool Is_Rectangle_Triangle_Intersect(
										  POINT_3D triP1, 
										  POINT_3D triP2,
										  POINT_3D triP3,
										  POINT_3D recP1,
										  POINT_3D recP2,
										  POINT_3D recP3,
										  POINT_3D recP4
										  );


	// 
	// �жϵ��Ƿ����������ڲ�
	//
	// ����1�� �������
	//
	// ����1�� �����ε�1��
	// 
	// ����2�� �����ε�2��
	//
	// ����3�� �����ε�3��
	//
	// ����ֵ��1 ���ڲ��� 0 �����ڲ���
	// 
	bool IsPointInTriangle(
							POINT_3D point, 
							POINT_3D triP1,
							POINT_3D triP2,
							POINT_3D triP3
							);


	// �ж�����ֱ���Ƿ��ཻ.
	//
	// ����1�� ��1��ֱ�ߵĿ�ʼ�㡣
	// ����2�� ��1��ֱ�ߵĽ����㡣
    // ����3�� ��2��ֱ�ߵĿ�ʼ�㡣
	// ����4�� ��2��ֱ�ߵĽ����㡣
	// 
	bool CheckCross(POINT_3D fvMS, POINT_3D fvME, POINT_3D fvNS, POINT_3D fvNE);




	//
	// 
	// �ѳ�������ת������Ϸ�߼����ꡣ
	//
	void TranslateSceneToGameLogic(float &x, float &y, float &z, Ogre::Matrix4& TransformMatrix);


	// ����������������ʽ��ֵ
	float fvMulti(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2);

	// �����������ļ���
	POINT_2D_FLOAT fvSub(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	�洢ʱ�õ��Ĳ���.
	//  

	//�õ���ͼ����, �����ӳ�����־Ͷ�ȡӳ������
	std::string GetSceneName();
	


	///////////////////////////////////////////////////////////////////////////////////
	//
	// ������������
	//

	// �洢������������.
	bool SaveBuildingWalk();

	// �������������
	void ClearBuildingWalk();
	

	//
	// �����������������
	//
	///////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////
	//
	// ������������
	//
	
	public:

		// ��ȡ������������
		bool ReadRegion();

		// �洢������������.
		bool SaveRegion();

		// ת��������������.
		bool SaveCRegion();

		// ��ղ���������������
		void ClearRegion();
	
	//
	// ������������
	//
	///////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////
	//
	// ��npcѲ������
	//
	
	public:

		// ��ȡ������������
		bool ReadNpcPatrol();

		// �洢������������.
		bool SaveNpcPatrol();

		// ��ղ���������������
		void ClearNpcPatrol();
	
	//
	// ��npcѲ������
	//
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	//
	// Ѱ·�༭
	//
	
	public:

		// ��ȡѰ·��
		bool ReadPathFind();

		// �洢Ѱ·��
		bool SavePathFind();

		// ���Ѱ·��
		void ClearPathFind();
	
	//
	// Ѱ·�༭
	//
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	//
	// npc�༭
	//
	
	public:

		// ��ȡ������������
		bool ReadNpc();

		// �洢������������.
		bool SaveNpc();

		//���npc
		void ClearNpc();
	
	//
	//  npc�༭
	//
	///////////////////////////////////////////////////////////////////////////////////


	
	///////////////////////////////////////////////////////////////////////////////////
	//
	// ������༭
	//
	
	public:

		// ��ȡ������������
		bool ReadGrowPoint();

		// �洢������������.
		bool SaveGrowPoint();

		//���npc
		void ClearGrowPoint();
	

	//  ������༭
	//
	///////////////////////////////////////////////////////////////////////////////////


		
	///////////////////////////////////////////////////////////////////////////////////
	//
	// �¼�����༭
	//
	
	public:

		// ��ȡ������������
		bool ReadEventArea();

		// �洢������������.
		bool SaveEventArea();

		//���npc
		void ClearEventArea();
	
	//
	//  �¼�����༭
	//
	///////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////
	//
	// ̯λ��Ϣ
	//
		
		//����̯λ��Ϣ
		void CreateStallInfo();

		// ��ȡ̯λ��Ϣ
		bool ReadStallInfo();

		// �洢̯λ��Ϣ
		bool SaveStallInfo();


	//
	// ̯λ��Ϣ
	//
	//////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////
	//
	// ������Ϣ
	//
		//�������
		void ClearSoundInfo();


		// ��ȡ̯λ��Ϣ
		bool ReadSoundInfo();

		// �洢̯λ��Ϣ
		bool SaveSoundInfo();

        bool ReadDatuPointInfo();
        bool SaveDatuPointInfo();
        void ClearDatuPointInfo();
};

#endif // 
