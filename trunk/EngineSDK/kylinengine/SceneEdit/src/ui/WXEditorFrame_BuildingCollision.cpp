
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXEditorFrame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/pen.h>
    #include <wx/dcscreen.h>
    #include <wx/dcmemory.h>
    #include <wx/scrolwin.h>
    #include <wx/toolbar.h>
    #include <wx/statusbr.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
#endif

#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/busyinfo.h>
#include <wx/config.h>
#include <wx/stockitem.h>
#include <wx/filename.h>

// copy paste
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include "BrushSelector.h"
#include "ModelSelector.h"
#include "ActorSelector.h"
#include "RegionEditDlgSelector.h"					//������������༭�Ի���.
#include "ParticleSystemSelector.h"
#include "SceneBrowser.h"
#include "ObjectPropertyEditor.h"
#include "TerrainSelections.h"
#include "LiquidCreatorDialog.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"
#include "Action.h"
#include "NewSceneDialog.h"
#include "SceneInfoMapDialog.h"
#include "SettingDialogs.h"

#include "RegionEditAction.h"

#include "Core/FairyObject.h"
#include "Core/FairySceneInfo.h"
#include "Core/FrameStatsListener.h"
#include "Core/FairyObjectFactoryManager.h"
#include "Core/FairyStaticEntityObject.h"
#include "Core/FairyModelObject.H"

#include <OgreMatrix4.h>
#include <OgreString.h>


// ----------------------------------------------------------------------------
// wxDockIt headers
// ----------------------------------------------------------------------------

#include <wx/layoutmanager.h>
#include <wx/dockwindow.h>
#include <wx/dockhost.h>
#include <wx/accel.h>

//-----------------------------------------------------------------------------
// File header
//-----------------------------------------------------------------------------
#include <wx/file.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include "OgreStringConverter.h"
#include "OgreCamera.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

#define OFFEET 0.9f

///////////////////////////////////////////////////////////////////////////////////
//
// �����������������
//


//
// �õ�����������������Ϣ��
// 
// return 0 -- �ɹ����� 0 -- ʧ�� ���������Ϣ����.
//
DWORD FairyEditorFrame::GetBuildingCollisionInfo()
{

	FILE* pFile = NULL;
	std::string strBuildingCollisionName = "";


	pFile = ::fopen("..\\building_collision\\data.ini", "r+");
	if(NULL == pFile)
	{
		return 1;
	}

	m_buildingCollisionInfoLibMap.clear();

	char bufInfo[512];
	TRI_INFO_VECTOR triVector;
	while(!feof(pFile))
	{
		memset(bufInfo, 0, sizeof(bufInfo));
		::fgets(bufInfo, sizeof(bufInfo), pFile);
		strBuildingCollisionName = bufInfo;
		//m_buildingCollisionInfoLibMap[strBuildingCollisionName] = 1;
		m_buildingCollisionInfoLibMap[bufInfo] = triVector;
	}
	::fclose(pFile);
	return 0;
}


//
// ��ע�ᵽ��ͼ�е���������Ϣ�洢���ļ���
// �ļ��������볡���ļ���������ͬ�� ��չ����Ϊbcol.
//
// �������ļ����֣� ������չ���� ��·����Ϣ��
void FairyEditorFrame::SaveBuildingCollisionInfo(wxString strCollisionFile)
{
	FILE* pFile = NULL;

	std::string strStlFileName = strCollisionFile;
	
	pFile = ::fopen(strStlFileName.c_str(), "wb");
	if(NULL == pFile)
	{
		return;
	}

	// ��д�汾��
	DWORD dwVersion = 0xff000001;
	::fwrite(&dwVersion, sizeof(dwVersion), 1, pFile);


	// д�������б�ĸ�����
	int iCount = m_TriInMapInfoMap.m_triInfoInMap.size();
	::fwrite(&iCount, sizeof(iCount), 1, pFile);


	TRI_INFO_MAP::iterator itEnd;
	TRI_INFO_MAP::iterator it;


	POINT_2D	pos;				// �������б�ע���λ�á�
	int			iTriCountInVector;	// �������б��������εĸ�����
	TRI_INFO	triInfo;			// �����εĽṹ��
	
	itEnd  = m_TriInMapInfoMap.m_triInfoInMap.end();
	for(it = m_TriInMapInfoMap.m_triInfoInMap.begin(); it != itEnd; it++)
	{
		// д������ע���λ�á�
		pos = it->first;
		::fwrite(&pos, sizeof(pos), 1, pFile);


		// дע�������λ���ϵ������εĸ�����
		TRI_INFO_VECTOR &triVector = it->second;
		iTriCountInVector = triVector.size();
		::fwrite(&iTriCountInVector, sizeof(iTriCountInVector), 1, pFile);

		for(int i = 0; i < iTriCountInVector; i++)
		{
			// дÿһ�������ε���Ϣ��
			triInfo = triVector[i];
			::fwrite(&triInfo, sizeof(triInfo), 1, pFile);
		}
		
	}

	::fclose(pFile);

}

// ͨ�������ָ��õ�����ģ�͵����֡�
//
// ��������̬����ģ�͵�����.
// 
// return �����mesh�ļ�����.
//std::string FairyEditorFrame::GetMeshNameByObjectPtr(const Fairy::ObjectPtr& object)
//{
//	wxString strMeshName = "";
//
//	return strMeshName;
//}//


//
// ��������������������ݡ�
//
void FairyEditorFrame::CreateBuildingCollisionData()
{

   std::string			strTemp;			// 
   Ogre::String			meshName;			// ��̬�����Ӧ��meshģ���ļ�.
   Ogre::String			meshFile;			// ��̬�����Ӧ��meshģ���ļ�.
   Ogre::String			meshPath;			// ��̬�����Ӧ��meshģ���ļ�.
  
   Ogre::Vector3		position;			// ��̬����ڷŵ�λ��.
   Ogre::Quaternion		rotateQuaternion;	// ��ת����.
   Ogre::Vector3		scale;				// ��������.

   int	iStrLen = 0;	// mesh �ļ����ֵĳ���
   int  iPos	= 0;	// mesh �ļ������� �� / ����λ��.

   // �����ǰ�����ݡ�
   m_TriInMapInfoMap.m_triInfoInMap.clear();


   // ����ÿһ���������������ݵ�����
   const Fairy::Scene::Objects& mObjects = GetSceneManipulator()->getSceneInfo()->getObjects();
   for (Fairy::Scene::Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
   {
       const Fairy::ObjectPtr& object = *it;
       //if ( ("StaticEntity" != object->getType())&&("Model" != object->getType()))
       //{
       //   continue;      
       //}//
	   //if ( (Fairy::StaticEntityObject::msType != object->getType())
		  // &&(Fairy::ModelObject::msType !=  object->getType()))
    //   {
    //      continue;      
    //   }//

	   if(Fairy::StaticEntityObject::msType != object->getType())
	   {
          continue;      
       }//

		meshName = Fairy::VariantCast<Ogre::String>(object->getProperty("mesh name"));
	    Ogre::StringUtil::splitFilename(meshName, meshFile, meshPath);
		iPos     = meshFile.find_last_of(".");
   		
		std::string strName;
		strName.empty();
		if(iPos> 0)
		{
			strName  = meshFile.substr(0, iPos);
		}
		else
		{
			strName = meshFile;
		}

		strName  = strName + ".cll";

		//FairyEditorFrame::BUILDING_COLLISION_MAP::iterator it1;
		//FairyEditorFrame::BUILDING_COLLISION_MAP::iterator itEnd1;

		//itEnd1 = m_buildingCollisionInfoLibMap.end();
		//for(it1 = m_buildingCollisionInfoLibMap.begin(); it1 != itEnd1; it1++)
		//{
		//	std::string strCllFileName = it1->first;
		//	if(strName == strCllFileName)
		//	{
		//		break;
		//	}
		//}

		//if(it1 == itEnd1)
		//{
		//	continue;
		//}
		////if(0 == m_buildingCollisionInfoLibMap.count(strName))
		////{
		////	continue;
		////}//

		// �õ�ģ�͵�ƽ�ƣ� ��ת���������ݡ�
		position         = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("position"));
		rotateQuaternion = Fairy::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
		scale			 = Fairy::VariantCast<Ogre::Vector3>(object->getProperty("scale"));

		// �����任����
		Ogre::Matrix4 TransformMatrix;
		BuildTransformMatrix(TransformMatrix, position, rotateQuaternion, scale);

		// �Ȱ�����������ע�ᵽ��ͼ��ȥ��
		RegisterCollisionToMap(strName, TransformMatrix);

    }
}


//
// ���������ƽ�ƣ� ��ת�� ���ŷ���������λ�ñ任����.
//
void FairyEditorFrame::BuildTransformMatrix(Ogre::Matrix4& Matrix,  const Ogre::Vector3& position, const Ogre::Quaternion rotate, const Ogre::Vector3 scale)
{

	Ogre::Matrix4 posMatrix;
	Ogre::Matrix4 scaleMatrix;
	Ogre::Matrix4 rotateMatrix(rotate);

	posMatrix = Ogre::Matrix4::IDENTITY;
	posMatrix.setTrans(position);

	scaleMatrix = Ogre::Matrix4::IDENTITY;
	scaleMatrix.setScale(scale);

	// ���յı任����.	
	Matrix = posMatrix * rotateMatrix * scaleMatrix;


}


//
// ���ݽ���������ְѽ������������ע�ᵽ��ͼ��.
//
void FairyEditorFrame::RegisterCollisionToMap(const std::string& strMeshName, Ogre::Matrix4& TransformMatrix)
{
	std::string strCollisionName = strMeshName;

	//if(0 == m_buildingCollisionInfoLibMap.count(strCollisionName))
	//{
	//	// �������û�������档
	//	return;
	//}//

	std::string strFileName = "..\\resource\\building_collision\\" + strCollisionName;
	FILE* pFile = NULL;
	pFile = ::fopen(strFileName.c_str(), "rb");
	if(NULL == pFile)
	{
		return;
	}
	::fclose(pFile);


	// �õ�����������������ݡ�
	TRI_INFO_VECTOR &TriInfoVector = m_buildingCollisionInfoLibMap[strCollisionName];
	if(0 == TriInfoVector.size())
	{
		//std::string strFileName = "..\\building_collision\\" + strCollisionName;
		if(0 != LoadCollisionDataFromFile(strFileName, TriInfoVector))
		{
			return;
		}
	}

	// ���㽨�����������Ԥ��������
	//WriteCollisionDataToTriMap(TriInfoVector, TransformMatrix);
	WriteCollisionDataToTriMap_Rectangle(TriInfoVector, TransformMatrix);

}



//
// �ӽ�����������������ļ��ж�ȡ�����ε���Ϣ��
//
// ������ std::string strFileName ������������������ļ���
//
// return�� 0 - �ɹ��� ��0 ʧ�ܡ����庬�������Ҫ������
DWORD FairyEditorFrame::LoadCollisionDataFromFile(std::string strFileName, TRI_INFO_VECTOR &TriInfoVector)
{
	
	FILE* pFile = NULL;
	pFile = ::fopen(strFileName.c_str(), "rb");
	if(NULL == pFile)
	{
		return 1;
	}

	DWORD dwVersion = 0;
	int   iTriCount = 0;

	TRI_INFO triInfo;
	TriInfoVector.clear();

	::fread(&dwVersion, sizeof(dwVersion), 1, pFile);
	::fread(&iTriCount, sizeof(iTriCount), 1, pFile);

	for(int i = 0; i < iTriCount; i++)
	{
		::fread(&triInfo, sizeof(triInfo), 1, pFile);
		TriInfoVector.push_back(triInfo);
	}

	::fclose(pFile);

	return 0;
}



//
// 
// �ѳ�������ת������Ϸ�߼����ꡣ
//
void FairyEditorFrame::TranslateSceneToGameLogic(float &x, float &y, float &z, Ogre::Matrix4& TransformMatrix)
{

	Ogre::Vector3 pos;

	pos.x = x;
	pos.y = y;
	pos.z = z;
	pos = TransformMatrix * pos;

	if(GetSceneManipulator())
	{
		pos.x = (pos.x - GetSceneManipulator()->getTerrainData()->mPosition.x) / GetSceneManipulator()->getTerrainData()->mScale.x;
		pos.y = (pos.y - GetSceneManipulator()->getTerrainData()->mPosition.y) / GetSceneManipulator()->getTerrainData()->mScale.y;
		pos.z = (pos.z - GetSceneManipulator()->getTerrainData()->mPosition.z) / GetSceneManipulator()->getTerrainData()->mScale.z;
	}
	
	
	x = pos.x;
	y = pos.y;
	z = pos.z;
}

//
// ����������Ϣд����ͼ����������λ�õ�ӳ����С�
//
// ����1�� �������б�
// 
// ����2�� �����εı任����
//
void FairyEditorFrame::WriteCollisionDataToTriMap(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix)
{

	typedef std::vector<SCANLINE_INFO> SCAN_LINE_VECTOR;

	SCANLINE_INFO    Scanline_info;			// ɨ����Ϣ.
	SCAN_LINE_VECTOR ScanLineVector;		// ÿһ�������ε�ɨ����Ϣ.
	
	int iCount = 0;			//ÿһ������������������θ���
	
	POINT_3D  Info1;		// ������ĵ�һ���㡣
	POINT_3D  Info2;        // ������ĵڶ����㡣
    POINT_3D  Info3;		// ������ĵ�������.

	float       fV1x;		// �����������������ߵĲ������̡� 
	float       fV1z;		//
	float       fV2x;		// fV1x = v1.x - v2.x  fV1z = v1.z - v2.z
	float       fV2z;		// fV2x = v2.x - v3.x  fV2y = v2.z - v3.z
	float       fV3x;		// fV3x = v3.x - v1.x  fV3y = v3.z - v1.z
	float       fV3z;		//

	float       ft;			// �������̱���t

	int iScanStart = 0;		// ɨ���ߵĿ�ʼ�㡣
	int iScanEnd   = 0;		// ɨ���ߵĽ����㡣

	float fFindStart;		// ÿ��ɨ���ҵ��Ŀ�ʼ�㡣
	float fFindEnd;			// ÿ��ɨ���ҵ��Ľ����㡣

	float fMaxX = 0;		// ������x��ȡֵ��Χ.
	float fMinX = 0;		//

	float fValue = 0;		// �ҵ���ֵ��

	POINT_2D point2DKey;

	// ��Ҫɨ��������Ρ�
	TRI_INFO tri;

	int iFaceCount = TriInfoVector.size();

	TRI_INFO Tri;
	for(int i = 0; i < iFaceCount; i++)
	{
		ScanLineVector.clear();
		
		Tri = TriInfoVector[i];
		Info1.fx = TriInfoVector[i].f1x;
		Info1.fy = TriInfoVector[i].f1y;
		Info1.fz = TriInfoVector[i].f1z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);
		fMinX = Info1.fx;
		fMaxX = Info1.fx;
			
		Info2.fx = TriInfoVector[i].f2x;
		Info2.fy = TriInfoVector[i].f2y;
		Info2.fz = TriInfoVector[i].f2z;
		TranslateSceneToGameLogic(Info2.fx, Info2.fy, Info2.fz, TransformMatrix);
		//fMinX = min(fMinX, Info2.fx);
		//fMaxX = max(fMaxX, Info2.fx);
		if(fMinX > Info2.fx)
		{
			fMinX = Info2.fx;
		}
		if(fMaxX < Info2.fx)
		{
			fMaxX = Info2.fx;
		}
				

		Info3.fx = TriInfoVector[i].f3x;
		Info3.fy = TriInfoVector[i].f3y;
		Info3.fz = TriInfoVector[i].f3z;
		TranslateSceneToGameLogic(Info3.fx, Info3.fy, Info3.fz, TransformMatrix);
		//fMinX = min(fMinX, Info3.fx);
		//fMaxX = max(fMaxX, Info3.fx);
		if(fMinX > Info3.fx)
		{
			fMinX = Info3.fx;
		}
		if(fMaxX < Info3.fx)
		{
			fMaxX = Info3.fx;
		}
			
		
		// ����Ҫɨ��������Ρ�
		tri.f1x = Info1.fx;
		tri.f1y = Info1.fy;
		tri.f1z = Info1.fz;
		
		tri.f2x = Info2.fx;
		tri.f2y = Info2.fy;
		tri.f2z = Info2.fz;

		tri.f3x = Info3.fx;
		tri.f3y = Info3.fy;
		tri.f3z = Info3.fz;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//  �ҵ����е�ɨ����.
		//
		//

		// ɨ���һ���ߡ�
		fV1x = Info2.fx - Info1.fx;
		fV1z = Info2.fz - Info1.fz;
		if(fabs((float)fV1x) < 0.0001)
		{

			Scanline_info.iStart    = (int)((Info1.fz < Info2.fy) ? Info1.fz : Info2.fz ) - 1;
			Scanline_info.iEnd      = (int)((Info1.fz > Info2.fy) ? Info1.fz : Info2.fz ) + 1;
			Scanline_info.iScanLine = (int)Info1.fx;
			ScanLineVector.push_back(Scanline_info);

		}

			
		// ɨ��ڶ����ߡ�
		fV2x = Info3.fx - Info2.fx;
		fV2z = Info3.fz - Info2.fz;
		if(fabs((float)fV2x) < 0.0001)
		{

			Scanline_info.iStart    = (int)((Info3.fz < Info2.fz) ? Info3.fz : Info2.fz) - 1;
			Scanline_info.iEnd      = (int)((Info3.fz > Info2.fz) ? Info3.fz : Info2.fz) + 1;
			Scanline_info.iScanLine = (int)Info2.fx;
			ScanLineVector.push_back(Scanline_info);

		}

		// ɨ��������ߡ�
		fV3x = Info1.fx - Info3.fx;
		fV3z = Info1.fz - Info3.fz;
		if(fabs((float)fV3x) < 0.0001)
		{

			Scanline_info.iStart    = (int)((Info1.fz < Info3.fz) ? Info1.fz : Info3.fz) - 1;
			Scanline_info.iEnd      = (int)((Info1.fz > Info3.fz) ? Info1.fz : Info3.fz) + 1;
			Scanline_info.iScanLine = (int)Info3.fx;
			ScanLineVector.push_back(Scanline_info);

		}

		iScanStart = (int)fMinX - 1;			// ɨ���ߵĿ�ʼ�㡣
		iScanEnd   = (int)fMaxX + 1;			// ɨ���ߵĽ����㡣
		
		for(int k = iScanStart; k <= iScanEnd; k++)
		{// ѭ��ÿһ��ɨ����.
			
			///////////////////////////////////////////////////////
			//
			// ��ʼ��������ֵ�������ǵ�һ��ֱ�߶ο��ܲ���ɨ������
			//
			//
			fFindStart = 10000.0f;		// ���ȡֵҪ����ֱ�߷����п���ȡ�������ֵ��
			fFindEnd   = 0;	
			fValue     = -1;

			if(fabs((float)fV1x) > 0.0001)
			{
				ft = (k * 1.0f - Info1.fx) / fV1x;
				if((ft >= 0)&&(ft <= 1.0))
				{
					fValue     = Info1.fz + ft * fV1z; 
					fFindStart = Info1.fz + ft * fV1z; 
					fFindEnd   = Info1.fz + ft * fV1z; 
				}
			}

			if(fabs((float)fV2x) > 0.0001)
			{
				ft = (k * 1.0f - Info2.fx) / fV2x;
				if((ft >= 0)&&(ft <= 1.0))
				{
					fValue = Info2.fz + ft * fV2z;
					if(fValue < fFindStart)
					{
						fFindStart = fValue;
					}

					if(fValue > fFindEnd)
					{
						fFindEnd = fValue;
					}
				}

			}

			if(fabs((float)fV3x) > 0.0001)
			{
				ft = (k * 1.0f - Info3.fx) / fV3x;
				if((ft >= 0)&&(ft <= 1.0))
				{
					fValue = Info3.fz + ft * fV3z;
					if(fValue < fFindStart)
					{
						fFindStart = fValue;
					}

					if(fValue > fFindEnd)
					{
						fFindEnd = fValue;
					}
				}

			}

			if(fValue >= 0)
			{
				Scanline_info.iStart    = (int)(fFindStart) - 1;
				Scanline_info.iEnd      = (int)(fFindEnd) + 1;
				Scanline_info.iScanLine = (int)k;
				ScanLineVector.push_back(Scanline_info);
			}
		
			// ѭ��ÿһ��ɨ���߽���.
		}// for(int k = iScanStart; k <= iScanEnd; k++)

		// �������ε��������㣬 ��ӵ�����
		Scanline_info.iStart    = (int)(Info1.fz);
		Scanline_info.iEnd      = (int)(Info1.fz);
		Scanline_info.iScanLine = (int)(Info1.fx);
		ScanLineVector.push_back(Scanline_info);

		Scanline_info.iStart    = (int)(Info2.fz);
		Scanline_info.iEnd      = (int)(Info2.fz);
		Scanline_info.iScanLine = (int)(Info2.fx);
		ScanLineVector.push_back(Scanline_info);

		Scanline_info.iStart    = (int)(Info3.fz);
		Scanline_info.iEnd      = (int)(Info3.fz);
		Scanline_info.iScanLine = (int)(Info3.fx);
		ScanLineVector.push_back(Scanline_info);
	
		//
		//  �ҵ����е�ɨ����.
		//
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////
	
		// �����ҵ���ɨ���߰�������ע�ᵽ��ͼ�С�
		TRI_INFO_VECTOR newTriInfoVector;
		for(int j = 0; j < (int)ScanLineVector.size(); j++)
		{
			for(int iY = ScanLineVector[j].iStart; iY <= ScanLineVector[j].iEnd; iY++)
			{
				point2DKey.iX = ScanLineVector[j].iScanLine;
				point2DKey.iY = iY;
				if(m_TriInMapInfoMap.m_triInfoInMap.count(point2DKey))
				{
					m_TriInMapInfoMap.m_triInfoInMap[point2DKey].push_back(tri);
					
				}
				else
				{
					newTriInfoVector.clear();
					newTriInfoVector.push_back(tri);
					m_TriInMapInfoMap.m_triInfoInMap[point2DKey] = newTriInfoVector;
				}
			}

		}

	}// for(int i = 0; i < iFaceCount; i++)

}



//
// ����������Ϣд����ͼ����������λ�õ�ӳ����С�
//
// ����1�� �������б�
// 
// ����2�� �����εı任����
//
void FairyEditorFrame::WriteCollisionDataToTriMap_Rectangle(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix)
{

	
	int iCount = 0;			//ÿһ������������������θ���
	
	POINT_3D  Info1;		// ������ĵ�һ���㡣
	POINT_3D  Info2;        // ������ĵڶ����㡣
    POINT_3D  Info3;		// ������ĵ�������.

	POINT_3D  recP1;		// ���ε�һ���㡣
	POINT_3D  recP2;        // ���εڶ����㡣
    POINT_3D  recP3;		// ���ε�������.
	POINT_3D  recP4;		// ���ε��ĸ���.

	float       fV1x = 0;		// �����������������ߵĲ������̡� 
	float       fV1z = 0;		//
	float       fV2x = 0;		// fV1x = v1.x - v2.x  fV1z = v1.z - v2.z
	float       fV2z = 0;		// fV2x = v2.x - v3.x  fV2y = v2.z - v3.z
	float       fV3x = 0;		// fV3x = v3.x - v1.x  fV3y = v3.z - v1.z
	float       fV3z = 0;		//


	float       fMinX = 0;		// �����������������ߵĲ������̡� 
	float       fMinZ = 0;		//
	float       fMaxX = 0;		// fV1x = v1.x - v2.x  fV1z = v1.z - v2.z
	float       fMaxZ = 0;		// fV2x = v2.x - v3.x  fV2y = v2.z - v3.z
	


	float       ft = 0;			// �������̱���t

	int iScanStart = 0;		// ɨ���ߵĿ�ʼ�㡣
	int iScanEnd   = 0;		// ɨ���ߵĽ����㡣

	float fFindStart = 0;		// ÿ��ɨ���ҵ��Ŀ�ʼ�㡣
	float fFindEnd   = 0;		// ÿ��ɨ���ҵ��Ľ����㡣

	float fValue = 0;		// �ҵ���ֵ��

	POINT_2D point2DKey;

	int iFaceCount = TriInfoVector.size();

	TRI_INFO Tri;

	Info1.fx = TriInfoVector[0].f1x;
	Info1.fy = TriInfoVector[0].f1y;
	Info1.fz = TriInfoVector[0].f1z;

	TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

	fMinX = Info1.fx;
	fMinZ = Info1.fz;
	fMaxX = Info1.fx;
	fMaxZ = Info1.fz;

	for(int i = 0; i < iFaceCount; i++)
	{

		// �����һ
		Info1.fx = TriInfoVector[i].f1x;
		Info1.fy = TriInfoVector[i].f1y;
		Info1.fz = TriInfoVector[i].f1z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

		if(fMinX > Info1.fx)
		{
			fMinX = Info1.fx;
		}

		if(fMinZ > Info1.fz)
		{
			fMinZ = Info1.fz;
		}

		if(fMaxX < Info1.fx)
		{
			fMaxX = Info1.fx;
		}

		if(fMaxZ < Info1.fz)
		{
			fMaxZ = Info1.fz;
		}

		// ������		
		Info1.fx = TriInfoVector[i].f2x;
		Info1.fy = TriInfoVector[i].f2y;
		Info1.fz = TriInfoVector[i].f2z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

		if(fMinX > Info1.fx)
		{
			fMinX = Info1.fx;
		}

		if(fMinZ > Info1.fz)
		{
			fMinZ = Info1.fz;
		}

		if(fMaxX < Info1.fx)
		{
			fMaxX = Info1.fx;
		}

		if(fMaxZ < Info1.fz)
		{
			fMaxZ = Info1.fz;
		}


		// �������
		Info1.fx = TriInfoVector[i].f3x;
		Info1.fy = TriInfoVector[i].f3y;
		Info1.fz = TriInfoVector[i].f3z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

		if(fMinX > Info1.fx)
		{
			fMinX = Info1.fx;
		}

		if(fMinZ > Info1.fz)
		{
			fMinZ = Info1.fz;
		}

		if(fMaxX < Info1.fx)
		{
			fMaxX = Info1.fx;
		}

		if(fMaxZ < Info1.fz)
		{
			fMaxZ = Info1.fz;
		}

	}// for(int i = 0; i < iFaceCount; i++)


	int iMinX = 0;
	int iMinZ = 0;
	int iMaxX = 0;
	int iMaxZ = 0;

	iMinX = fMinX - 1;
	iMinZ =	fMinZ - 1;
	iMaxX =	fMaxX + 1;
	iMaxZ =	fMaxZ + 1;

	TRI_INFO_VECTOR newTriInfoVector;
	for(int x = iMinX; x <= iMaxX; x++)
		for(int z = iMinZ; z <= iMaxZ; z++)
		{

			recP1.fx = x;
			recP1.fz = z;
			recP2.fx = x + 1;
			recP2.fz = z;
            recP3.fx = x + 1;
			recP3.fz = z + 1;
			recP4.fx = x;
			recP4.fz = z + 1;

			for(int iFace = 0; iFace < iFaceCount; iFace++)
			{

				// �����һ
				Info1.fx = TriInfoVector[iFace].f1x;
				Info1.fy = TriInfoVector[iFace].f1y;
				Info1.fz = TriInfoVector[iFace].f1z;
				TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

				// ������		
				Info2.fx = TriInfoVector[iFace].f2x;
				Info2.fy = TriInfoVector[iFace].f2y;
				Info2.fz = TriInfoVector[iFace].f2z;
				TranslateSceneToGameLogic(Info2.fx, Info2.fy, Info2.fz, TransformMatrix);

				// �������
				Info3.fx = TriInfoVector[iFace].f3x;
				Info3.fy = TriInfoVector[iFace].f3y;
				Info3.fz = TriInfoVector[iFace].f3z;
				TranslateSceneToGameLogic(Info3.fx, Info3.fy, Info3.fz, TransformMatrix);

				if(Is_Rectangle_Triangle_Intersect(Info1, Info2, Info3, recP1, recP2, recP3, recP4))
				{
					// ��������ע�ᵽ��ͼ��ȥ
					Tri.f1x = Info1.fx;
					Tri.f1y = Info1.fy;
					Tri.f1z = Info1.fz;
					Tri.f2x = Info2.fx;
					Tri.f2y = Info2.fy;
					Tri.f2z = Info2.fz;
					Tri.f3x = Info3.fx;
					Tri.f3y = Info3.fy; 
					Tri.f3z = Info3.fz;

					point2DKey.iX = x;
					point2DKey.iY = z;
					if(m_TriInMapInfoMap.m_triInfoInMap.count(point2DKey))
					{
						m_TriInMapInfoMap.m_triInfoInMap[point2DKey].push_back(Tri);
						
					}
					else
					{
						newTriInfoVector.clear();
						newTriInfoVector.push_back(Tri);
						m_TriInMapInfoMap.m_triInfoInMap[point2DKey] = newTriInfoVector;
					}

				}// if(Is_Rectangle_Triangle_Intersect(Info1, Info2, Info3, recP1, recP2, recP3, recP4))

			}// for(int iFace = 0; iFace < iFaceCount; iFace++)

		}	//for(int x = iMinX; x < iMaxX; x++)
		//for(int z = fMinZ; z < fMaxZ; z++)


}



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
bool FairyEditorFrame::Is_Rectangle_Triangle_Intersect(
										  POINT_3D triP1, 
										  POINT_3D triP2,
										  POINT_3D triP3,
										  POINT_3D recP1,
										  POINT_3D recP2,
										  POINT_3D recP3,
										  POINT_3D recP4
										  )
{

	// ���ж��������Ƿ��ھ����ڲ���

	// �жϵ�1
	if((triP1.fx >= recP1.fx - OFFEET )&&(triP1.fx <= recP2.fx + OFFEET)&&(triP1.fz >= recP1.fz - OFFEET)&&(triP1.fz <= recP3.fz + OFFEET))
	{
		return true;
	}

	// �жϵ�2
    if((triP2.fx >= recP1.fx - OFFEET )&&(triP2.fx <= recP2.fx + OFFEET)&&(triP2.fz >= recP1.fz - OFFEET)&&(triP2.fz <= recP3.fz + OFFEET))
	{
		return true;
	}

	// �жϵ�3
	if((triP3.fx >= recP1.fx - OFFEET )&&(triP3.fx <= recP2.fx + OFFEET)&&(triP3.fz >= recP1.fz - OFFEET)&&(triP3.fz <= recP3.fz + OFFEET))
	{
		return true;
	}

	// �ж��ķ��εĵ��Ƿ�����������

	// �жϵ�1
	if(IsPointInTriangle(recP1, triP1, triP2, triP3))
	{
		return true;
	}

	// �жϵ�2
	if(IsPointInTriangle(recP2, triP1, triP2, triP3))
	{
		return true;
	}

	// �жϵ�3
	if(IsPointInTriangle(recP3, triP1, triP2, triP3))
	{
		return true;
	}

	// �жϵ�4
	if(IsPointInTriangle(recP4, triP1, triP2, triP3))
	{
		return true;
	}

	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP1, recP2, triP1, triP2))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP1, recP2, triP2, triP3))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP1, recP2, triP3, triP1))
	{
		return true;
	}



	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP2, recP3, triP1, triP2))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP2, recP3, triP2, triP3))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP2, recP3, triP3, triP1))
	{
		return true;
	}


	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP3, recP4, triP1, triP2))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP3, recP4, triP2, triP3))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP3, recP4, triP3, triP1))
	{
		return true;
	}


	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP4, recP1, triP1, triP2))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP4, recP1, triP2, triP3))
	{
		return true;
	}
	// �ж�ֱ���Ƿ��ཻ��
	if(CheckCross(recP4, recP1, triP3, triP1))
	{
		return true;
	}


	return false;
}


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
bool FairyEditorFrame::IsPointInTriangle(POINT_3D point, POINT_3D triP1, POINT_3D triP2, POINT_3D triP3)
{
	float a1 = 0;
	float a2 = 0;
	float a3 = 0;

	float b1 = 0;
	float b2 = 0;
	float b3 = 0;

	float fu = 0;
	float fv = 0;

	float fBase = 0;
	
	a1 = point.fx - triP1.fx;
	b1 = point.fz - triP1.fz;

	a2 = triP2.fx - triP1.fx;
	b2 = triP2.fz - triP1.fz;

	a3 = triP3.fx - triP1.fx;
	b3 = triP3.fz - triP1.fz;

	fBase = a2*b3 - a3*b2;
	if(abs(fBase) > 0.0001)
	{
		fu = (a2*b1 - a1*b2) / fBase;
		fv = (a1*b3 - a3*b1) / fBase;

		if((fu > -0.8)&&(fv > -0.8)&&((fu + fv) < 1.8))
		{
			return true;
		}
	}
	else 
	{
		return true;
	}

	return false;
}


// �ж�����ֱ���Ƿ��ཻ.
//
// ����1�� ��1��ֱ�ߵĿ�ʼ�㡣
// ����2�� ��1��ֱ�ߵĽ����㡣
// ����3�� ��2��ֱ�ߵĿ�ʼ�㡣
// ����4�� ��2��ֱ�ߵĽ����㡣
// 
bool FairyEditorFrame::CheckCross(POINT_3D fvMS, POINT_3D fvME, POINT_3D fvNS, POINT_3D fvNE)
{

	POINT_2D_FLOAT	fvMS2;
	POINT_2D_FLOAT	fvME2;
	POINT_2D_FLOAT	fvNS2;
	POINT_2D_FLOAT	fvNE2;
	
	fvMS2.fx = fvMS.fx;
	fvMS2.fy = fvMS.fz;

	fvME2.fx = fvME.fx;
	fvME2.fy = fvME.fz;

	fvNS2.fx = fvNS.fx;
	fvNS2.fy = fvNS.fz;

	fvNE2.fx = fvNE.fx;
	fvNE2.fy = fvNE.fz;

	if ( std::max(fvMS2.fx,fvME2.fx) <= std::min(fvNS2.fx,fvNE2.fx) )
	{
		return false;
	}
	if ( std::min(fvMS2.fx,fvME2.fx) >= std::max(fvNS2.fx,fvNE2.fx) )
	{
		return false;
	}
	if ( std::max(fvMS2.fy,fvME2.fy) <= std::min(fvNS2.fy,fvNE2.fy) )
	{
		return false;
	}
	if ( std::min(fvMS2.fy,fvME2.fy) >= std::max(fvNS2.fy,fvNE2.fy) )
	{
		return false;
	}

	float temp1 = fvMulti(fvSub(fvMS2,fvNS2),fvSub(fvNE2,fvNS2)) * fvMulti( fvSub(fvNE2,fvNS2),fvSub(fvME2,fvNS2));
	float temp2 = fvMulti(fvSub(fvNS2,fvMS2),fvSub(fvME2,fvMS2)) * fvMulti( fvSub(fvME2,fvMS2),fvSub(fvNE2,fvMS2));

	if((temp1>=0)&&(temp2>=0))
	{
		return true;
	}

	return false;
		
}

// ����������������ʽ��ֵ
float FairyEditorFrame::fvMulti(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2)
{
	return (fv1.fx*fv2.fy - fv1.fy*fv2.fx);
}


POINT_2D_FLOAT FairyEditorFrame::fvSub(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2)
{
	POINT_2D_FLOAT fvResult;
	fvResult.fx = fv1.fx-fv2.fx;
	fvResult.fy = fv1.fy-fv2.fy;
	return fvResult;
}


//
// �����������������
//
///////////////////////////////////////////////////////////////////////////////////

