#ifndef __UNREACHREGION_H__
#define __UNREACHREGION_H__

#include "Core/FairyPrerequisites.h"
#include <OgreMaterial.h>
#include <OgreVector3.h>
#include "CustomShape.h"

namespace Fairy 
{
	class SceneManipulator;
	class CCustomShape;

	class CRegionPoint
	{

	public:
		CRegionPoint()
		{
			x = 0;
			y = 0;
		}

		CRegionPoint(float x, float y) : x(x), y(y)
		{
		}

		~CRegionPoint()
		{

		}

	public:
		float x;
		float y;
	};


	//////////////////////////////////////////////////////////////////////////////////////
	//
	// ���ߵ��ࡣ
	//
	class CPointInfo
	{

	public:

		CPointInfo() : m_pSceneNode(NULL)
					 , m_pEntity(NULL)
					 , m_Position(Ogre::Vector3::ZERO)
					 , m_pParentSceneNode(NULL)
					 , m_pSceneManipulator(NULL)
					 , m_ulPointId(0)
					 , m_ulParentId(0)
					 , m_bIfYValued(true)
					
		{
		
		}

		CPointInfo(
					Ogre::Vector3		position, 
					Ogre::SceneNode*	pSceneNode,
					SceneManipulator*	pSceneManipulator,
					unsigned long		ulPointId,
					unsigned long       ulParentId

					) 
					: m_pSceneNode(NULL)
					, m_pEntity(NULL)
					, m_Position(position)
					, m_pParentSceneNode(pSceneNode)
					, m_pSceneManipulator(pSceneManipulator)
					, m_ulPointId(ulPointId)
					, m_ulParentId(ulParentId)
					, m_bIfYValued(true)
					
		{
			CreatePointShape();
		}


		~CPointInfo()
		{

		}

	public:

		SceneManipulator*  m_pSceneManipulator;	    // ������ogre�����ľ����

		Ogre::SceneNode*	m_pParentSceneNode;		// ������ʾ����༭���ܵĽڵ�.
		Ogre::SceneNode*	m_pSceneNode;			// ��ĳ����ڵ�.
		Ogre::Vector3		m_Position;				// λ�á�
		Ogre::Entity*		m_pEntity;				// �볡�����������Խڵ�

		Ogre::MaterialPtr	m_pMaterialLine;		// �����ĵ�Ĳ���	


		unsigned long       m_ulPointId;			// ���id
		unsigned long       m_ulParentId;			// ��ĸ�����id
		bool				m_bIfYValued;			// Yֵ�Ƿ�����Ч��

		///////////////////////////////////////////////////////////////////////////////
		//
		// ������������ 
		//

		// ����λ��
		void SetPos(Ogre::Vector3 position);

		// ����Y�Ƿ�����Ч��
		void SetValuedY( bool b ) { m_bIfYValued = b; }

		// �õ�Y�Ƿ�����Ч��
		bool GetValuedY( void ) { return m_bIfYValued; }

		// �õ�λ��
		Ogre::Vector3 GetPosition();

		// ���ò���
		void SetMaterial(String strMaterialName);

		// ���ø������ڵ�.
		void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

		// ����SceneManipulator���
		void SetSceneManipulator(SceneManipulator*  SceneManipulator);

		// ���ø������id
		void SetParentRegionId(unsigned long ulParentId);

		// ������ģ��.
		unsigned long CreatePointShape();
	
		// ���������ڵ�ģ��.
		unsigned long CreateSceneNode();
	
		// ����ʵ��ģ��.
		unsigned long CreateEntity();

		// ������ʵ��
		unsigned long CreateLineShape();

		// �ͷ���ʵ����Դ
		void ClearlineShape();

		// ���ͷų���֮ǰ���ͷ�����ʵ��
		void DetachEntity();

		// ���ͷų���֮ǰ���ͷų����ڵ�
		void DelSceneNode();

		// ����ѡ�еĲ���
		void SetSelMaterial();

	};
	
	

	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��������������
	//
	//
	typedef std::map<unsigned long, CPointInfo*> POINT_MAP;

	class TerrainData;
	class CUnreachRegion
	{

	public:
		unsigned long		m_ulId;					// ��������id.
		POINT_MAP			m_pointMap;				// ���б�.
		POINT_MAP			m_pointMap1;			// ���б�.
													
													// ��Ҫ�����任����ʹ��
													// ����ѡ��һ��������ߣ� Ҫ��ǰһ��ѡ�еĲ������ó�
													// ����״̬�Ĳ���.
		unsigned long		m_ulCurEditPointId;		// �������ǰһ�����id.
		unsigned long		m_ulCurEditLineId;		// �������ǰһ���ߵ�id.

		unsigned long		m_ulCurMaxPointId;		// �������ĵ�������id.
		unsigned long		m_ulCurMaxLineId;		// ���������ߵ������id.

		int				m_bFlyEnable;			// �Ƿ����ͨ�����÷ɷɵ�Ŷ

		Ogre::SceneNode*	m_pParentSceneNode;		// ������ʾ����༭���ܵĽڵ�.
		Ogre::SceneNode*	m_pSceneNode;			// ��ĳ����ڵ�.
		
		Ogre::SceneNode*	m_pLineSceneNode;		// �ߵĳ����ڵ�.
		Ogre::SceneNode*	m_pAreaSceneNode;		// ��������ڵ�.
		Ogre::SceneNode*	m_pLineSceneNode1;		// �ߵĳ����ڵ�.
		Ogre::SceneNode*	m_pAreaSceneNode1;		// ��������ڵ�.
		//Ogre::Entity*		m_pLineEntity;			// �볡�����������Խڵ�
		CCustomShape*		m_pLineShape;  
		CCustomShape*		m_pLineShape1;
		CCustomShape		m_AreaShape;  
		CCustomShape		m_AreaShape1;  

		SceneManipulator*   m_pSceneManipulator;	// ������ogre�����ľ����


		/////////////////////////////////////////////////////////////////////////////////
		//
		// ����������
		//
	public:
		// �Ƿ�������Ṧͨ��
		void SetFlyable( int bFlag ) { m_bFlyEnable = bFlag; }

		// �Ƿ�������Ṧͨ��
		int GetFlyable( void ) { return m_bFlyEnable; }

		// ���ø������ڵ�.
		void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

		// ����SceneManipulator���
		void SetSceneManipulator(SceneManipulator*  SceneManipulator);

		// ���������ڵ�ģ��.
		unsigned long CreateSceneNode();

		// ����µĵ�
		unsigned long AddNewPoint(Ogre::Vector3 position);

		// ����µĵ�
		unsigned long AddNewPoint1(Ogre::Vector3 position);

		// ͨ���±꣬ �õ�һ�������Ϣ.
		CPointInfo* GetPointInfoByIndex(int iIndex);
		CPointInfo* GetPointInfoByIndex1(int iIndex);

		// ͨ��id�� �õ�һ�������Ϣ.
		CPointInfo* GetPointInfoById(unsigned long ulId);
		CPointInfo* GetPointInfoById1(unsigned long ulId);

		// �õ���ĸ���
		int GetPointCount();
		int GetPointCount1();

		// ����ǰһ��С��Ĳ���.
		void  SetPrePointMaterial();

		// �������εĲ���
		Ogre::MaterialPtr	_createIntersectGridMaterial(void);

		// ������������
		unsigned long CreateLineShape();
		unsigned long CreateLineShape1();

		// �����������
		void ClearlineShape();
	
		// ��յ������
		void ClearPointInfo();

		// �����������
		void SetRegionMaterial(String strMatrerial);

		// ������������״̬���ʲ���.
		void SetRegionNorMaterial();
		void SetRegionNorMaterial1();

		// ��������ѡ��״̬����.
		void SetRegionSelMaterial();
		void SetRegionSelMaterial1();

		// �����ߵ���ɫ
		void SetLineColor( Ogre::ColourValue color);
		void SetLineColor1( Ogre::ColourValue color);

		// ������ѡ�е���ɫ
		void SetLineSelColor();

		// �����ߵ���������ɫ
		void SetLineNorColor();

		// ���ݵ��θı����е�ĸ߶�
		void SetPointHeight( TerrainData* pTerrainData);
		void SetPointHeight1( TerrainData* pTerrainData);

		// ���ݵ����޸������ߵĸ߶�λ��
		void SetLineHeight( TerrainData* pTerrainData);
		void SetLineHeight1( TerrainData* pTerrainData);

		// ���ͷų���֮ǰ���ͷ�����ʵ��
		void DetachAllEntity();

		// ɾ�������ӽڵ�
		void DestroyAllChildSceneNode();


		// �ж��������Ƿ��ཻ.
		bool CheckCross(CRegionPoint &fvMS, CRegionPoint &fvME, CRegionPoint &fvNS, CRegionPoint &fvNE);

		// �ж��߶θ��������Ƿ��ཻ������Y��
		bool CheckCross(Ogre::Vector3 fvmStart, Ogre::Vector3 fvmEnd);

		// �ж���ֱ���Ƿ��ཻ������Y��
		bool CheckCross(Ogre::Vector3 fvmStart, Ogre::Vector3 fvmEnd, Ogre::Vector3 fvmStart1, Ogre::Vector3 fvmEnd1);

		// �������Ƿ���ȣ�����Y��
		bool fvEuqual(const Ogre::Vector3& fv1, const Ogre::Vector3& fv2);

		// �Ƿ�������һ���µ�.
		bool CanAddNewPoint(Ogre::Vector3 position);
		
		// ��һ����ά����
		CRegionPoint fvSub(CRegionPoint fv1, CRegionPoint fv2);

		Ogre::Vector3 fvSub(Ogre::Vector3& fv1, Ogre::Vector3& fv2);

		// ���ά����������ʽ.
		float fvMulti(CRegionPoint fv1, CRegionPoint fv2);

		float fvMulti(Ogre::Vector3& fv1, Ogre::Vector3& fv2);

		// �����ֵ��
		float max(float f1, float f2);

		// ����Сֵ
		float min(float f1, float f2);

		// �ж��Ƿ���Ըı�һ��С���λ�á�
		bool CanChangePointPos(int iIndex, Ogre::Vector3 position);


		// ָ��idָ��λ�����һ��С��
		void AddNewPoint(unsigned long ulId, Ogre::Vector3 position);

		void AddNewPoint1(unsigned long ulId, Ogre::Vector3 position);

		// ͨ��id ɾ��һ��С��
		void DelPointById(unsigned long ulId);
		void DelPointById1(unsigned long ulId);

		// ��β�� ɾ��һ��С��
		void DelPointFromTail(unsigned long ulId);
		void DelPointFromTail1(unsigned long ulId);

		// ����ܷ�ѡ���������
		bool IsMouseSel(float x, float y);

		// �Ƿ�ѡ��һ��������
		bool IsSelTri(Ogre::Vector3 dir, Ogre::Vector3 orig, Ogre::Vector3 v1, Ogre::Vector3 v2, Ogre::Vector3 v3);


		// �����������
		void ReShapeArea();
		void ReShapeArea1();

		// �����Ƿ�Ϸ�
		bool IsRegionValidate();

		// ��������ڵ㡣
		void HideAreaNode(bool bHide);
		void HideAreaNode1(bool bHide);

		// �����߽ڵ㡣
		void HideLineNode(bool bHide);

		// �ı�û����Ȼ������Ĳ���
		void ChangeNoDeepMaterial(bool bOpenDeepBuf = false);
		



	public:
		CUnreachRegion(void);

		CUnreachRegion(unsigned long ulId, Ogre::SceneNode* pParentSceneNode, SceneManipulator* pSceneManipulator);
		
		~CUnreachRegion(void);
	};
}

#endif //__UNREACHREGION_H__