#ifndef __CUSTOMSHAPE_H__
#define __CUSTOMSHAPE_H__

#include <OgreSimpleRenderable.h>
#include <map>

namespace Fairy 
{
	typedef std::vector<Ogre::Vector3> POSITION_ARRAY;
	typedef std::map<int, Ogre::ColourValue> LINE_COLOR_MAP;

	class CUnreachRegion;
	typedef std::map<unsigned long, CUnreachRegion*> UNREACHREGIONMAP;

	class CCustomShape : public Ogre::SimpleRenderable
	{
		public:
			CCustomShape(void);
			~CCustomShape(void);

		public:

			/**************/
			///////////////////////////////////////////////////////////////////////////////////////////////
			//
			// ��Ϊ�Զ������״�࣬ ��Ҫʵ�ֵ��麯��
			//
			//

			///////////////////////////////////////////////////////////////
			// ���Ա���
			//

			// �Զ�����״�İ뾶��С
			float m_fRadius;

			// �õ��󶨵İ뾶
			Ogre::Real getBoundingRadius() const;

			Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;//

			virtual void _updateRenderQueue  (  Ogre::RenderQueue *  queue   );
 


		public:

			/**************/
			//////////////////////////////////////////////////////////////////////////////////////////////
			//
			// ���ݻ�����������
			//

			/////////////////////////////////////////////////
			// ��������
			//

			//  ��������
			Ogre::HardwareVertexBufferSharedPtr m_vbuf;

			// �ߵĵ��б�
			POSITION_ARRAY	m_pointArray;
			UNREACHREGIONMAP* m_pUnreachRegion;
			float*						m_Weights;
			//float*						m_Dist;

			void SetRegion(UNREACHREGIONMAP* pRegion);

			POSITION_ARRAY& GetPointArray() { return m_pointArray; }

			// ���������
			void FillPointData();
			void FillPointData1();

			// �������㻺����
			void CreateVertexBuffer();
			void CreateVertexBuffer1();

			size_t CalcVertexCount1(size_t nPointsCount);

			// �ͷŶ��㻺����
			void ClearVertexBuffer();
			void ClearVertexBuffer1();



			/***************/
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// �Զ�����״�����Բ���
			//

			///////////////////////////////////////
			//���Ա���

			// ��״��Ĭ����ɫ.
			Ogre::ColourValue m_defaultColor;

			// ����ɫ�б�
			LINE_COLOR_MAP m_LineColroMap;


			// ������ʾ������.
			void SetShapeType(Ogre::RenderOperation::OperationType type = Ogre::RenderOperation::OT_LINE_STRIP);

			// ����ͼ�ε�Ĭ����ɫ��
			void SetDefaultColor(Ogre::ColourValue color);

			// ����ָ���ߵ���ɫ
			void SetLineColor(int iIdex, Ogre::ColourValue color);



			/**************/
			////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// �߼����ݲ���
			//
					
			// ���һ����
			void AddNewPoint(Ogre::Vector3 point, bool bRecreateBuf = true);
			void AddNewPoint1(Ogre::Vector3 point, bool bRecreateBuf = true);



			///////////////////////////////////////////////////////////////////////////////////////////////
			//
			// ��ʾƽ�����
			//

			// ƽ��ĵ��б�
			POSITION_ARRAY	m_planePointArray;
			
			// ������ʾ
			void buildPlanePoint();

			// ���������
			void FillAreaPointData();

			// �������㻺����
			void CreateAreaVertexBuffer();

			void AddAreaNewPoint(Ogre::Vector3 point);



			//////////////////////////////////////////////////////////////////////////////////////////////
			//
			//  ������ײ������
			// 
			bool IsPointInRegion(float fx, float fy);


			
			




		
	};


}

#endif //__CUSTOMSHAPE_H__