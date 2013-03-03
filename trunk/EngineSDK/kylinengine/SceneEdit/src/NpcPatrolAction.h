#pragma once

#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>


namespace Fairy 
{
	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004
	

	
	class CUnreachRegion;

	typedef std::map<std::string, Ogre::MaterialPtr> OGRE_MATERIAL_MAP;
	typedef std::map<unsigned long, CUnreachRegion*> UNREACHREGIONMAP;

	
	class CNpcPatrolAction : public Action
	{
	public:

		// �����б�
		OGRE_MATERIAL_MAP m_materialMap;

		//npcѲ�����б�
		UNREACHREGIONMAP  m_UnreachRegionMap;

		// ��¼���̵İ���״̬.
		unsigned long     m_FuncKeyState;

		// �Ƿ���ʾ����.
		bool  m_bIsShow;

	public:

		 CNpcPatrolAction(SceneManipulator* sceneManipulator);
      	 ~CNpcPatrolAction(void);

	public:

		HitInfo mTerrainHitInfo;

	public:
		////////////////////////////////////////////////////////////////////////////////////
		//
		// ���̺Ͷ��̲���
		//

		// �������ȡ�ļ�
		unsigned long ReadRegionFromFile(std::string& strFileName);
		
		// ������洢���ļ�
		unsigned long SaveRegionToFile(std::string strFileName);
		
		//ֻ���,������ȡ
		int* mWorld;
		int mWidth,mHeight;
		struct NavMapHeader
		{
			unsigned short width;
			unsigned short height;
		};
		unsigned long SaveRegionToNavMapFile(std::string& strFileName);
		void calculateNavInfo(std::vector<ScanLine>& lines);
		int dumpToNavMapFile(std::string& strFileName);
	
	public:
		
		////////////////////////////////////////////////////////////////////////////////////
		//
		// �༭�����������������
		//
		Ogre::MaterialPtr	m_pMaterialPoint;		// �����ĵ�Ĳ���			
		Ogre::MaterialPtr	m_pMaterialPointSel;	// ѡ�е�Ĳ���
		Ogre::MaterialPtr	m_pMaterialPointTemp;	// ��ʱ����
		Ogre::MaterialPtr	m_pMaterialLine;		// �ߵĲ���
		Ogre::MaterialPtr	m_pMaterialLineSel;		// ѡ���ߵĲ���
		Ogre::MaterialPtr	m_pMaterialRegion;		// �������
		Ogre::MaterialPtr	m_pMaterialRegionSel;	// ѡ������Ĳ��ʡ�

		Ogre::SceneNode*	m_pSceneNode;			// ������ʾ����༭���ܵĽڵ�.

	
		unsigned long m_ulCurMaxId;					// ��ǰ��������ID
		unsigned long m_ulCurEditRegionId;			// ��ǰ���ڱ༭�������id
		unsigned long m_ulCurEditPointId;			// ��ǰ���ڱ༭�ĵ��id
		unsigned long m_ulCurEditLineId;			// ��ǰ���ڱ༭���ߵ�id

		// �õ���ǰ���ڱ༭�������id
		int GetCurEditRegionId()
		{
			return m_ulCurEditRegionId;
		}

		//���õ�ǰ����id
		void SetCurEditRegionId(unsigned long ulRegionId)
		{
			m_ulCurEditRegionId = ulRegionId;
		}

		// �õ���ǰ���ڱ༭�ĵ��id
		int GetCurEditPointId()
		{
			return m_ulCurEditPointId;
		}
		//���õ�ǰ��id
		void SetCurEditPointId(unsigned long ulPointId)
		{
			m_ulCurEditPointId = ulPointId;
		}

		// �õ���ǰ���ڱ༭���ߵ�id
		int GetCurEditLineId()
		{
			return m_ulCurEditLineId;
		}//

		//���õ�ǰ��id
		void SetCurEditLineId(unsigned long ulLineId)
		{
			m_ulCurEditLineId = ulLineId;
		}


		

	public:

		////////////////////////////////////////////////////////////////////////////////////////
		//
		// �������
		//

		// ���һ���µ�����
		unsigned long AddNewRegion();

		// ͨ��ָ����id�� ɾ��һ������
		unsigned long DelRegion(unsigned long ulId);

		// ���������������.
		void ClearRegionData();

		// ����ָ����id�� �õ�����
		CUnreachRegion* GetRegionById(unsigned long ulId);

		// ѡ��һ������
		void SelRegion(unsigned long ulId, bool bIsChangeCamera = true);

		// ��ѡ��һ������.
		void UnSelRegion(unsigned long ulId);

		// ����һ����������
		void HideRegionArea(unsigned long ulRegionId, bool bHide);

		// ��ǰ�������Ƿ�Ϸ�
		bool IsRegionValidate(unsigned long ulRegionId);

		// ���´�����ʾ���������
		void ReshapeArea(unsigned long ulRegionId);

		/////////////////////////////////////////////////////////////////////////////////////////
		//
		// �����
		//

		// ��ǰ����֮ǰ�ĵ��λ��.
        Ogre::Vector3 m_preDoPointPos;

		// ��ǰ����֮ǰ�ĵ��λ��.
        Ogre::Vector3 m_curPointPos;

		// �ڵ�ǰ�༭�����������һ���µĵ�
		unsigned long AddNewPoint(Ogre::Vector3 position);

		// ָ������id�� ָ�����id�� ָ��λ�ã� ���һ��С��.
		void AddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position);

		// ָ������id�� ָ�����id�� ָ��λ�ã� ɾ��һ��С��.
		void DelPoint(unsigned long ulRegionId, unsigned long ulPointId);


		// ������
		bool EditPoint(const Point& pt);

		// ѡ��һ����.
		void SelPoint(unsigned long ulPointId);

		// �ı䵱ǰ�༭�ĵ��λ��, �������������
		void ChangeCurEditPointPos(const Point& pt);

		// �ı�ָ��������ָ���ĵ��λ��
		void ChangeCurEditPointPos(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position);


		// ��¼��ǰ����ǰһ�ε��λ��.
		void RecordPreDoPointPos();



		/////////////////////////////////////////////////////////////////////////////////////////
		//
		// �߲���
		//

		// �ж��������Ƿ��ཻ.
		//bool CheckCross(fVector2 fvMS, fVector2 fvME,fVector2 fvNS, fVector2 fvNE);



	public:

		/////////////////////////////////////////////////////////////////////////////////////////
		//
		// �����Ĳ���
		//

		// ǰһ������������.

		int m_iPreOperatorType;


		// ��ʼ������༭��Ҫ��һЩ����
		void InitNpcPatrol();

		// ���������༭������С��Ĳ���
		void SetOldHitPointMaterial();

		// ��������༭�ĳ����ڵ�
		void CreateSceneNode();

		// ��������༭�����ڵ�
		void HideScene(bool bHide);

		// ���ݵ��εĸ߶ȸı�༭������
		void SetRegionHeight();
		
		// ���ð��µĹ��ܼ�
		void SetFuncKey(unsigned long ulFuncKey);

		//  ��չ��ܼ�.
		void ClearFuncKey();

		

	public:

		// �õ�������������.
		const String& getName(void) const;

	public:

		// �������λ�ò���.
		void SetCamera(float fx, float fy);
		void SetCamera();

		// ���ò�������.
		void setParameter(const String& name, const String& value);

    protected:
        
		// ���������麯��.
        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

		// ��ǰ����active����.
        void _onActive(bool active);



	public:

		void Test_AddRegion();

	};

}