#include "CustomShape.h"
#include <OgreHardwareVertexBuffer.h>		// ���㻺����
#include <OgreHardwareBufferManager.h>		// ����������
#include <OgreCamera.h>						// �����
#include <OgreRoot.h>
#include "RegionEditAction.h"
#include "UnreachRegion.h"

namespace Fairy 
{

	CCustomShape::CCustomShape(void)
	{
		
		SetShapeType();

		m_fRadius = 100;
		setPolygonModeOverrideable(false);

		m_defaultColor = Ogre::ColourValue(0, 1.0, 0, 0.5);

		m_pUnreachRegion = NULL;
		m_Weights = NULL;
		//m_Dist = NULL;
	}

	CCustomShape::~CCustomShape(void)
	{
		ClearVertexBuffer();
		ClearVertexBuffer1();

		m_pUnreachRegion = NULL;
	}

	void CCustomShape::SetRegion(UNREACHREGIONMAP* pRegion)
	{
		m_pUnreachRegion = pRegion;
	}


	// �������Ϣ
	void CCustomShape::FillPointData()
	{
		size_t posCount = m_pointArray.size();

		if ( posCount <= 0 )
		{
			return;
		}

		// �������㻺����.
		CreateVertexBuffer();
	
		// ���°�Χ���Ա��ܹ���ʾ�߶�
		Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
        Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

     	for (size_t ii = 0; ii < posCount; ++ii)
		{
			if(vmin.x > m_pointArray[ii].x)
			{
				vmin.x = m_pointArray[ii].x;
			}

			if(vmin.y > m_pointArray[ii].y)
			{
				vmin.y = m_pointArray[ii].y;
			}

			if(vmin.z > m_pointArray[ii].z)
			{
				vmin.z = m_pointArray[ii].z;
			}
			

			if(vmax.x < m_pointArray[ii].x)
			{
				vmax.x = m_pointArray[ii].x;
			}

			if(vmax.y < m_pointArray[ii].y)
			{
				vmax.y = m_pointArray[ii].y;
			}

			if(vmax.z < m_pointArray[ii].z)
			{
				vmax.z = m_pointArray[ii].z;
			}

		}

		vmin.x -= 100.0f;
        vmax.x += 100.0f;

		vmin.y -= 100.0f;
        vmax.y += 100.0f;

		vmin.z -= 100.0f;
        vmax.z += 100.0f;

		// ��뾶
        Ogre::Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
        m_fRadius = Ogre::Math::Sqrt(sqLen);//

		// ���°�Χ��
        mBox.setExtents(vmin,vmax);//
        getParentNode()->needUpdate();//

		// ������������ �������.
		Ogre::RGBA *pCol;
		Ogre::ColourValue color;
		bool bSetColor = false;
		float *vertexPos = static_cast<float*>(m_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (size_t i = 0; i < posCount; ++i)
		{
			*vertexPos++ = m_pointArray[i].x;
			*vertexPos++ = m_pointArray[i].y+10;
			*vertexPos++ = m_pointArray[i].z;
			
			if( !bSetColor)
			{
				// ��䶨����ɫ
				color = m_defaultColor;
			}

			if(m_LineColroMap.count(i))
			{
				color = m_LineColroMap[i];
				bSetColor = true;
			}
			else
			{
				bSetColor = false;
			}

            pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
       		// ��diffuse����Ϊalpha��Դ
            Ogre::Root::getSingleton().convertColourValue(m_defaultColor, pCol++ ) ;
            vertexPos = static_cast<float*>(static_cast<void*>(pCol));
		}

		m_vbuf->unlock();     

		SetShapeType();

	}
	void CCustomShape::FillPointData1()
	{
		size_t posCount = m_pointArray.size();

		if ( posCount <= 1 )
		{
			return;
		}

		// �������㻺����.
		CreateVertexBuffer1();

		if( mRenderOp.vertexData->vertexCount == 0 )
		{
			return;
		}

		// ���°�Χ���Ա��ܹ���ʾ�߶�
		Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
		Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

		for (size_t ii = 0; ii < posCount; ++ii)
		{
			if(vmin.x > m_pointArray[ii].x)
			{
				vmin.x = m_pointArray[ii].x;
			}

			if(vmin.y > m_pointArray[ii].y)
			{
				vmin.y = m_pointArray[ii].y;
			}

			if(vmin.z > m_pointArray[ii].z)
			{
				vmin.z = m_pointArray[ii].z;
			}


			if(vmax.x < m_pointArray[ii].x)
			{
				vmax.x = m_pointArray[ii].x;
			}

			if(vmax.y < m_pointArray[ii].y)
			{
				vmax.y = m_pointArray[ii].y;
			}

			if(vmax.z < m_pointArray[ii].z)
			{
				vmax.z = m_pointArray[ii].z;
			}

		}

		vmin.x -= 100.0f;
		vmax.x += 100.0f;

		vmin.y -= 100.0f;
		vmax.y += 100.0f;

		vmin.z -= 100.0f;
		vmax.z += 100.0f;

		// ��뾶
		Ogre::Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
		m_fRadius = Ogre::Math::Sqrt(sqLen);//

		// ���°�Χ��
		mBox.setExtents(vmin,vmax);//
		getParentNode()->needUpdate();//

		// ������������ �������.
		Ogre::RGBA *pCol;
		Ogre::ColourValue color;
		bool bSetColor = false;
		float *vertexPos = static_cast<float*>(m_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (size_t i = 0; i < posCount-1; ++i)
		{
			for( size_t j = i+1; j < posCount; j ++)
			{
				float w = m_Weights[i + j*posCount];
				if( w != FLT_MAX )
				{
					*vertexPos++ = m_pointArray[i].x;
					*vertexPos++ = m_pointArray[i].y+10;
					*vertexPos++ = m_pointArray[i].z;

					if( !bSetColor)
						color = m_defaultColor;

					if(m_LineColroMap.count(i))
					{
						color = m_LineColroMap[i];
						bSetColor = true;
					}
					else
					{
						bSetColor = false;
					}

					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
					Ogre::Root::getSingleton().convertColourValue(m_defaultColor, pCol++ ) ;
					vertexPos = static_cast<float*>(static_cast<void*>(pCol));


					*vertexPos++ = m_pointArray[j].x;
					*vertexPos++ = m_pointArray[j].y+10;
					*vertexPos++ = m_pointArray[j].z;

					if( !bSetColor)
						color = m_defaultColor;

					if(m_LineColroMap.count(i))
					{
						color = m_LineColroMap[j];
						bSetColor = true;
					}
					else
					{
						bSetColor = false;
					}

					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
					Ogre::Root::getSingleton().convertColourValue(m_defaultColor, pCol++ ) ;
					vertexPos = static_cast<float*>(static_cast<void*>(pCol));
				}
			}
		}

		m_vbuf->unlock();     

		SetShapeType( Ogre::RenderOperation::OT_LINE_LIST );

	}

	
	// �ͷŶ��㻺����
	void CCustomShape::ClearVertexBuffer()
	{
		 // �Ȱ��ϴδ����Ļ�����ɾ��
        if ( mRenderOp.vertexData )
        {
            delete mRenderOp.vertexData;
            mRenderOp.vertexData = 0;
        }
	}

	void CCustomShape::ClearVertexBuffer1()
	{
		// �Ȱ��ϴδ����Ļ�����ɾ��
		if ( mRenderOp.vertexData )
		{
			delete mRenderOp.vertexData;
			mRenderOp.vertexData = 0;
		}

		delete [] m_Weights; m_Weights = NULL;
		//delete [] m_Dist; m_Dist = NULL;
	}

	// ����һ�����㻺����
	void CCustomShape::CreateVertexBuffer()
    {
		if(0 == m_pointArray.size())
		{
			return;
		}

        // �Ȱ��ϴδ����Ļ�����ɾ��
        ClearVertexBuffer();

        mRenderOp.vertexData = new Ogre::VertexData;

        mRenderOp.vertexData->vertexCount = m_pointArray.size();
        mRenderOp.vertexData->vertexStart = 0;
      
		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		// ���������ʽ
        size_t offset = 0;
      	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

	     m_vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            mRenderOp.vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

         mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_vbuf);
    }

	size_t CCustomShape::CalcVertexCount1(size_t nPointsCount)
	{
		if( nPointsCount <= 0 ) return 0;

		return ( nPointsCount + CalcVertexCount1(nPointsCount-1) );
	}

	void CCustomShape::CreateVertexBuffer1()
	{
		if(0 == m_pointArray.size())
		{
			return;
		}

		// �Ȱ��ϴδ����Ļ�����ɾ��
		ClearVertexBuffer1();

		size_t nVertexCount = CalcVertexCount1( m_pointArray.size()-1 )*2;

		if( m_pUnreachRegion )
		{
			nVertexCount = 0;

			m_Weights = new float[m_pointArray.size()*m_pointArray.size()];
			memset( m_Weights, 0, m_pointArray.size()*m_pointArray.size()*sizeof(float) );
			size_t s;
			for( s = 0; s < m_pointArray.size()-1; s ++ )
			{
				for( size_t t = s+1; t < m_pointArray.size(); t ++ )
				{
					Ogre::Vector3& v1 = m_pointArray[s];
					Ogre::Vector3& v2 = m_pointArray[t];
					UNREACHREGIONMAP::iterator it;
					for( it = m_pUnreachRegion->begin(); it != m_pUnreachRegion->end();it ++ )
					{
						CUnreachRegion* pRe = it->second;
						if( pRe->CheckCross( v1, v2 ) )
							break;
					}
					float w = FLT_MAX;
					if( it == m_pUnreachRegion->end() )
					{
						w = (v1.x-v2.x)*(v1.x-v2.x) + (v1.z-v2.z)*(v1.z-v2.z);
						nVertexCount ++;
					}

					m_Weights[(s) + (t)*(m_pointArray.size())] = w;
					m_Weights[(t) + (s)*(m_pointArray.size())] = w;
				}
				m_Weights[(s) + (s)*(m_pointArray.size())] = 0.0f;
			}

			m_Weights[(s) + (s)*(m_pointArray.size())] = 0.0f;
			m_Weights[0] = 0.0f;

			nVertexCount *= 2;
		}

		mRenderOp.vertexData = new Ogre::VertexData;

		mRenderOp.vertexData->vertexCount = nVertexCount;
		mRenderOp.vertexData->vertexStart = 0;

		if( mRenderOp.vertexData->vertexCount == 0 ) return;

		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		// ���������ʽ
		size_t offset = 0;
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

		m_vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(0),
			mRenderOp.vertexData->vertexCount,
			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_vbuf);

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �Զ�����״�����Բ���
	//
	
	//���û�������
	void CCustomShape::SetShapeType(Ogre::RenderOperation::OperationType type)
	{
		mRenderOp.operationType = type; 
        mRenderOp.useIndexes    = false; 
	}

	// ���һ����
	void CCustomShape::AddNewPoint(Ogre::Vector3 point, bool bRecreateBuf)
	{
		m_pointArray.push_back(point);

		if(bRecreateBuf)
		{
			FillPointData();
		}
	}

	// ���һ����
	void CCustomShape::AddNewPoint1(Ogre::Vector3 point, bool bRecreateBuf)
	{
		m_pointArray.push_back(point);

		if(bRecreateBuf)
		{
			FillPointData1();
		}
	}

	// ��������ͼ�ε�Ĭ����ɫ.
	void CCustomShape::SetDefaultColor(Ogre::ColourValue color)
	{
		m_defaultColor = color;
	}

	// ����ָ���ߵ���ɫ
	void CCustomShape::SetLineColor(int iIdex, Ogre::ColourValue color)
	{
		m_LineColroMap[iIdex] = color;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ��Ϊ�Զ������״�࣬ ��Ҫʵ�ֵ��麯��
	//
	//
	Ogre::Real CCustomShape::getBoundingRadius() const
	{
		return m_fRadius;
	}//

	Ogre::Real CCustomShape::getSquaredViewDepth(const Ogre::Camera* cam) const
	{
		Ogre::Vector3 min, max, mid, dist;

        min = mBox.getMinimum();
        max = mBox.getMaximum();
        mid = ((min - max) * 0.5) + min;
        dist = cam->getDerivedPosition() - mid;

			
        return dist.squaredLength(); 
	}





	// ������ʾ
	void CCustomShape::buildPlanePoint()
	{
		POSITION_ARRAY tempPointVector;
		POSITION_ARRAY remainPointVector;

		std::vector<int> remainPointIndexVector;
		std::vector<int> storePointIndexVector;

		// ���ƽ������
		m_planePointArray.clear();

		int iPointCount = m_pointArray.size();
		for(int i = 0; i < iPointCount; i++)
		{
			remainPointIndexVector.push_back(i);
		}

		float fx = 0;
		float fy = 0;
		int	  iReduceCount = 0;

		int iRemainPointCount = remainPointIndexVector.size();

		int iFindCount = 0;
		while(iRemainPointCount > 3)
		{
			iFindCount++;
			iReduceCount = 0;
			storePointIndexVector.clear();
			for(int iIndex = 0; iIndex < iRemainPointCount; iIndex++)
			{
				
				if((iIndex + 2) >= iRemainPointCount)
				{
					// ��Ҫ��ʣ��ĵ���뵽��һ�β��ҵ������С�
					// ���㻹ʣ�¶��ٸ��㡣
					if(iIndex < iRemainPointCount)
					{
						storePointIndexVector.push_back(remainPointIndexVector[iIndex]);
					}
					if(iIndex+1 < iRemainPointCount)
					{
						storePointIndexVector.push_back(remainPointIndexVector[iIndex+1]);
					}

					//iRemainPointCount = iRemainPointCount - iReduceCount;
					remainPointIndexVector.clear();
					remainPointIndexVector.assign(storePointIndexVector.begin(), storePointIndexVector.end());
					iRemainPointCount = remainPointIndexVector.size();
					break;
				}
			
				fx = m_pointArray[remainPointIndexVector[iIndex]].x +  m_pointArray[remainPointIndexVector[iIndex+2]].x;
				fx = fx / 2;

				fy = m_pointArray[remainPointIndexVector[iIndex]].z +  m_pointArray[remainPointIndexVector[iIndex+2]].z;
				fy = fy / 2;

				if(IsPointInRegion(fx, fy))
				{
					//  �ҵ�һ��������
					m_planePointArray.push_back(m_pointArray[remainPointIndexVector[iIndex]]);
					m_planePointArray.push_back(m_pointArray[remainPointIndexVector[iIndex+1]]);
					m_planePointArray.push_back(m_pointArray[remainPointIndexVector[iIndex+2]]);
					storePointIndexVector.push_back(remainPointIndexVector[iIndex]);
					iReduceCount++;
					iIndex++;
				}
				else
				{
					storePointIndexVector.push_back(remainPointIndexVector[iIndex]);
				}
			}

			// ���㻹ʣ�¶��ٸ��㡣
			
			remainPointIndexVector.clear();
			remainPointIndexVector.assign(storePointIndexVector.begin(), storePointIndexVector.end());
			//iRemainPointCount = iRemainPointCount - iReduceCount;
			iRemainPointCount = remainPointIndexVector.size();

			if(10000 < iFindCount)
			{
				break;
			}
		}


		if(3 == iRemainPointCount)
		{
			m_planePointArray.push_back(m_pointArray[remainPointIndexVector[0]]);
			m_planePointArray.push_back(m_pointArray[remainPointIndexVector[1]]);
			m_planePointArray.push_back(m_pointArray[remainPointIndexVector[2]]);
		}
		else
		{
			m_planePointArray.clear();
		}

	}




	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  ������ײ������
	// 

	// �ж�һ�����Ƿ���һ�������ڡ�
	bool CCustomShape::IsPointInRegion(float fx, float fy)
	{
		//float fsx = 0;
		//float fex = 0;
		//float fsy = 0;
		//float fey = 0;

		//float fd1 = 0;
		//float fd2 = 0;
		//float fd3 = 0;

		//int   iInterSectCount = 0;

		//int iPointCount = m_pointArray.size();
		//for(int i = 0; i < iPointCount; i++)
		//{
		//	fsx = m_pointArray[i].x;
		//	fsy = m_pointArray[i].z;

		//	if(i == iPointCount - 1)
		//	{
		//		fex = m_pointArray[0].x;
		//		fey = m_pointArray[0].z;
		//	}
		//	else
		//	{
		//		fex = m_pointArray[i+1].x;
		//		fey = m_pointArray[i+1].z;
		//	}

		//	fd1 = abs(fx - fsx);
		//	fd2 = abs(fex - fx);
		//	fd3 = abs(fex - fsx);

		//	if((fd1 + fd2) <= fd3)
		//	{
		//		iInterSectCount++;
		//	}

		//}

		//if(iInterSectCount % 2)
		//{
		//	return true;
		//}

		//return false;//




		float fsx = 0;
		float fex = 0;
		float fsy = 0;
		float fey = 0;

		float t = 0;
		float y = 0;

		int   iInterSectCount = 0;
		int iPointCount = m_pointArray.size();

		for(int i = 0; i < iPointCount; i++)
		{
			t = 0;
			fsx = m_pointArray[i].x;
			fsy = m_pointArray[i].z;

			if(i == iPointCount - 1)
			{
				fex = m_pointArray[0].x;
				fey = m_pointArray[0].z;
			}
			else
			{
				fex = m_pointArray[i+1].x;
				fey = m_pointArray[i+1].z;
			}

			if((abs(fsx - fex) < 0.00001)&&(abs(fsx - fx) < 0.00001))
			{
				iInterSectCount++;
				continue;
			}

			t = (fx - fsx)/(fex - fsx);

			if((t >= 0 )&&(t <=1))
			{
				y = fsy + t*(fey - fsy);

				if((fy - y) < 0)
				{
					iInterSectCount++;
				}
			}
			

		}

		if(iInterSectCount % 2)
		{
			return true;
		}

		return false;//

	}

	
	// ���������
	void CCustomShape::FillAreaPointData()
	{

		size_t posCount = m_planePointArray.size();

		if ( posCount <= 0 )
		{
			 //ClearVertexBuffer();
			return;
		}

		// �������㻺����.
		CreateAreaVertexBuffer();
	
		// ���°�Χ���Ա��ܹ���ʾ�߶�
		Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
        Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

     	for (size_t ii = 0; ii < posCount; ++ii)
		{
			if(vmin.x > m_planePointArray[ii].x)
			{
				vmin.x = m_planePointArray[ii].x;
			}

			if(vmin.y > m_planePointArray[ii].y)
			{
				vmin.y = m_planePointArray[ii].y;
			}

			if(vmin.z > m_planePointArray[ii].z)
			{
				vmin.z = m_planePointArray[ii].z;
			}
			

			if(vmax.x < m_planePointArray[ii].x)
			{
				vmax.x = m_planePointArray[ii].x;
			}

			if(vmax.y < m_planePointArray[ii].y)
			{
				vmax.y = m_planePointArray[ii].y;
			}

			if(vmax.z < m_planePointArray[ii].z)
			{
				vmax.z = m_planePointArray[ii].z;
			}

		}

		vmin.x -= 100.0f;
        vmax.x += 100.0f;

		vmin.y -= 100.0f;
        vmax.y += 100.0f;

		vmin.z -= 100.0f;
        vmax.z += 100.0f;

		// ��뾶
        Ogre::Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
        m_fRadius = Ogre::Math::Sqrt(sqLen);//

		// ���°�Χ��
        mBox.setExtents(vmin,vmax);//
        getParentNode()->needUpdate();//

		// ������������ �������.
		Ogre::RGBA *pCol;
		Ogre::ColourValue color;
		bool bSetColor = false;
		float *vertexPos = static_cast<float*>(m_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		Ogre::ColourValue Areacolor = Ogre::ColourValue(5.0, 0, 1.0, 0.5);
		for (size_t i = 0; i < posCount; ++i)
		{
			*vertexPos = m_planePointArray[i].x;
			vertexPos++;
			*vertexPos = m_planePointArray[i].y+20;
			vertexPos++;
			*vertexPos = m_planePointArray[i].z;
			vertexPos++;
			
			pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
       		// ��diffuse����Ϊalpha��Դ
            Ogre::Root::getSingleton().convertColourValue(Areacolor, pCol++ ) ;
            vertexPos = static_cast<float*>(static_cast<void*>(pCol));
		}

		m_vbuf->unlock();    


		SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);

	}

	// �������㻺����
	void CCustomShape::CreateAreaVertexBuffer()
	{
		
		if(0 == m_planePointArray.size())
		{
			return;
		}

        // �Ȱ��ϴδ����Ļ�����ɾ��
        ClearVertexBuffer();

        mRenderOp.vertexData = new Ogre::VertexData;

        mRenderOp.vertexData->vertexCount = m_planePointArray.size();
        mRenderOp.vertexData->vertexStart = 0;
      
		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		// ���������ʽ
        size_t offset = 0;
      	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

	     m_vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            mRenderOp.vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

         mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_vbuf);
	}

	// ���һ����
	void CCustomShape::AddAreaNewPoint(Ogre::Vector3 point)
	{
		m_pointArray.push_back(point);
		buildPlanePoint();
		FillAreaPointData();
	}

	void CCustomShape::_updateRenderQueue  (  Ogre::RenderQueue *  queue   )
	{
		if(queue)
		{
			if(mRenderOp.operationType == Ogre::RenderOperation::OT_TRIANGLE_LIST)
			{
				queue->addRenderable(this, Ogre::RENDER_QUEUE_8);
			}
			else
			{
				queue->addRenderable(this, Ogre::RENDER_QUEUE_3);
			}
		}
	}//


}// namespace Fairy 
