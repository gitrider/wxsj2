
#include "StdAfx.h"
#include ".\buildingcollisionmng.h"
#include <math.h>
#include "..\Procedure\GameProcedure.h"
#include "GIResourceProvider.h"



const INT BUILD_COLLISION_SEARCH_RANGE	= 30;


CBuildingCollisionMng::CBuildingCollisionMng(VOID)
{
}

CBuildingCollisionMng::~CBuildingCollisionMng(VOID)
{
}


//
// �������������ļ��ж�ȡ���ݡ�
//
// �����������������ļ��� ����·����
UINT CBuildingCollisionMng::LoadCollisionTriInfoFromFile(const char* szFileName)
{

	// ��ȡ�µ�����֮ǰ�� Ҫ�������.
	ClearData();

	if(!szFileName || szFileName[0] == '\0') return 1;

	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szFileName, lpAddress, "Scene");

	if(dwSize > 0)
	{
		char* pPoint = lpAddress;

		//FILE* pFile = NULL;
		//pFile = ::fopen(szFileName, "rb");
		//if(NULL == pFile)
		//{
		//	return 1;
		//}

		TRI_INFO	triInfo;
		POINT_2D	pos;
		UINT		dwVersion = 0;
		INT         iPosCount = 0;
		INT         iTriCount = 0;

		TRI_INFO_VECTOR triInfoVector;

		// ��ȡ�汾��
		//::fread(&dwVersion, sizeof(dwVersion), 1, pFile);
		memcpy(&dwVersion, pPoint, sizeof(dwVersion)); pPoint+=sizeof(dwVersion);

		// ��ȡע����������ε�λ�õĸ���
		//::fread(&iPosCount, sizeof(iPosCount), 1, pFile);
		memcpy(&iPosCount, pPoint, sizeof(iPosCount)); pPoint+=sizeof(iPosCount);

		for(INT i = 0; i < iPosCount; i++)
		{
			// �����������Ϣ���顣
			triInfoVector.clear();

			// ��ȡλ��
			//::fread(&pos, sizeof(pos), 1, pFile);
			memcpy(&pos, pPoint, sizeof(pos)); pPoint+=sizeof(pos);
			
			// ��ȡע�����λ�õ������εĸ���
			//::fread(&iTriCount, sizeof(iTriCount), 1, pFile);
			memcpy(&iTriCount, pPoint, sizeof(iTriCount)); pPoint+=sizeof(iTriCount);
			for(INT j = 0; j < iTriCount; j++)
			{
				//::fread(&triInfo, sizeof(triInfo), 1, pFile);
				memcpy(&triInfo, pPoint, sizeof(triInfo)); pPoint+=sizeof(triInfo);
				triInfoVector.push_back(triInfo);
			}

			if(triInfoVector.size())
			{
				m_triInfoMap[pos] = triInfoVector;
			}
		}

		//::fclose(pFile);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
	}

	return 0;
}



//
// ͨ������� ƽ��fx, fzλ�õõ��߶ȡ� 
// 
// ����1�� FLOAT fx    ��Ϸ�߼����ꡣ
//
// ����2�� FLOAT fz    ��Ϸ�߼����ꡣ
//
// ����3�� FLOAT& fy   ��Ϸ�߼�����  ���صĸ߶ȡ�
//
// return�� 1 �� �ɹ��� fy�����塣
//		    0 �� ʧ�ܣ� fy�����壬 �����˴��޽��������������ݡ�
BOOL CBuildingCollisionMng::GetHeight(FLOAT fx, FLOAT fz, FLOAT& fy)
{

	POINT_2D pos;

	FLOAT fu = 0;
	FLOAT fv = 0;

	FLOAT fX21 = 0;
	FLOAT fX31 = 0;
	FLOAT fY21 = 0;
	FLOAT fY31 = 0;

	FLOAT fX1 = 0;
	FLOAT fY1 = 0;

	POINT_3D v1;
	POINT_3D v2;
	POINT_3D v3;

	FLOAT fHeightValue = 0;
	FLOAT fFindValue   = -1000.0f;

	//pos.iX = (INT)(fx + 0.9);
	//pos.iY = (INT)(fz + 0.9);//

	pos.iX = (INT)(fx);
	pos.iY = (INT)(fz);//

	BOOL bFind = FALSE;

	if(m_triInfoMap.count(pos))
	{
		TRI_INFO_VECTOR& triInfo = m_triInfoMap[pos];
		INT iSize = (INT)triInfo.size();
		for(INT i = 0; i < iSize; i++)
		{
			v1.fx = triInfo[i].v1.fx;
			v1.fy = triInfo[i].v1.fy;
			v1.fz = triInfo[i].v1.fz;

			v2.fx = triInfo[i].v2.fx;
			v2.fy = triInfo[i].v2.fy;
			v2.fz = triInfo[i].v2.fz;

			v3.fx = triInfo[i].v3.fx;
			v3.fy = triInfo[i].v3.fy;
			v3.fz = triInfo[i].v3.fz;

			fX1 = fx - v1.fx;
			fY1 = fz - v1.fz;
			
			fX21 = v2.fx - v1.fx;
			fY21 = v2.fz - v1.fz;

			fX31 = v3.fx - v1.fx;
			fY31 = v3.fz - v1.fz;

			if(fabs(fX21*fY31 - fY21*fX31) < 0.0001f)
			{
				continue;
			}

			fu = (fX1*fY31 - fY1*fX31) / (fX21*fY31 - fY21*fX31);
			fv = (fY1*fX21 - fX1*fY21) / (fX21*fY31 - fY21*fX31);

			if((fv > 0)&&(fv < 1)&&(fu > 0)&&(fu < 1)&&(1-fv-fu > 0))
			{
				bFind = true;
				fHeightValue = v1.fy + fu*(v2.fy - v1.fy) + fv*(v3.fy - v1.fy);
				if(fHeightValue > fFindValue)
				{
					fFindValue = fHeightValue;
				}
				
			}
			

		}
	}

	fy = fFindValue;
	return bFind;
}


//
//  ������ݡ�
//
VOID CBuildingCollisionMng::ClearData()
{
	m_triInfoMap.clear();
}


BOOL CBuildingCollisionMng::SelBuilding(
										FLOAT fOrigx, FLOAT fOrigy, FLOAT fOrigz,	// ���ߵ�ԭ�㡣
										FLOAT fDirx,  FLOAT fDiry,  FLOAT fDirz,	// ���ߵķ���
										INT	  ix, INT iz,							// ѡ�е�ͼ��λ��
										FLOAT& fPosx, FLOAT& fPosy, FLOAT& fPosz	// ���ؽ������������λ�á�
										)
{
	FLOAT fDistance = 1000;

	INT iL = ix - BUILD_COLLISION_SEARCH_RANGE;
	INT iR = ix + BUILD_COLLISION_SEARCH_RANGE;
	INT iT = iz - BUILD_COLLISION_SEARCH_RANGE;
	INT iB = iz + BUILD_COLLISION_SEARCH_RANGE;

	POINT_2D pos;
	INT iSize = 0;
	POINT_3D Orig;	// ���ߵ�ԭ�㡣
	POINT_3D Dir;	// ���ߵķ���
	POINT_3D v1;
	POINT_3D v2;
	POINT_3D v3;

	FLOAT fd = 0;
	FLOAT fv = 0;
	FLOAT fu = 0;

	Orig.fx = fOrigx;
	Orig.fy = fOrigy;
	Orig.fz = fOrigz;

	Dir.fx = fDirx;
	Dir.fy = fDiry;
	Dir.fz = fDirz;

	BOOL bReturn = FALSE;


	for(INT i = iL; i < iR; i++)
		for(INT j = iT; j < iB; j++)
		{
			pos.iX = i;
			pos.iY = j;
			if(0 == m_triInfoMap.count(pos))
			{
				continue;
			}

			TRI_INFO_VECTOR& triInfo = m_triInfoMap[pos];
			iSize = (INT)triInfo.size();
			for(INT iRri = 0; iRri < iSize; iRri++)
			{
				v1.fx = triInfo[iRri].v1.fx;
				v1.fy = triInfo[iRri].v1.fy;
				v1.fz = triInfo[iRri].v1.fz;

				v2.fx = triInfo[iRri].v2.fx;
				v2.fy = triInfo[iRri].v2.fy;
				v2.fz = triInfo[iRri].v2.fz;

				v3.fx = triInfo[iRri].v3.fx;
				v3.fy = triInfo[iRri].v3.fy;
				v3.fz = triInfo[iRri].v3.fz;

				if(IntersectTriangle(Orig, Dir, v1, v2, v3, &fd, &fu, &fv))
				{
					if(fDistance > fd)
					{
						fDistance = fd;
						fPosx = (1- fv - fu)*v1.fx + fu*v2.fx + fv*v3.fx;
						fPosy = (1- fv - fu)*v1.fy + fu*v2.fy + fv*v3.fy;
						fPosz = (1- fv - fu)*v1.fz + fu*v2.fz + fv*v3.fz;
					}

					bReturn = true;
				}

			}// for(INT iRri = 0; iRri < iSize; i++)

		}

	return bReturn;
}


BOOL CBuildingCollisionMng::IntersectTriangle(  
												POINT_3D Orig,	// ���ߵ�ԭ�㡣
												POINT_3D Dir,	// ���ߵķ���
												POINT_3D v1,
												POINT_3D v2,
												POINT_3D v3,
												FLOAT* t, 
												FLOAT* u, 
												FLOAT* v 
											 )
{
    // Find vectors for two edges sharing vert0
    POINT_3D edge1;
	edge1.fx = v2.fx - v1.fx;
	edge1.fy = v2.fy - v1.fy;
	edge1.fz = v2.fz - v1.fz;

    POINT_3D edge2 ;
	edge2.fx = v3.fx - v1.fx;
	edge2.fy = v3.fy - v1.fy;
	edge2.fz = v3.fz - v1.fz;


    // Begin calculating determinant - also used to calculate U parameter
    POINT_3D pvec;
    //D3DXVec3Cross( &pvec, &dir, &edge2 );
	pvec.fx = Dir.fy* edge2.fz - Dir.fz*edge2.fy;
	pvec.fy = Dir.fz* edge2.fx - Dir.fx*edge2.fz;
	pvec.fz = Dir.fx* edge2.fy - Dir.fy*edge2.fx;


    // If determinant is near zero, ray lies in plane of triangle
	FLOAT  det = edge1.fx*pvec.fx + edge1.fy*pvec.fy + edge1.fz*pvec.fz;

    POINT_3D tvec;
    if( det > 0 )
    {
        tvec.fx = Orig.fx - v1.fx;
		tvec.fy = Orig.fy - v1.fy;
		tvec.fz = Orig.fz - v1.fz;
    }
    else
    {
        tvec.fx = -(Orig.fx - v1.fx);
		tvec.fy = -(Orig.fy - v1.fy);
		tvec.fz = -(Orig.fz - v1.fz);
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
   	*u  = tvec.fx*pvec.fx + tvec.fy*pvec.fy + tvec.fz*pvec.fz;

    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    POINT_3D qvec;
    //D3DXVec3Cross( &qvec, &tvec, &edge1 );
	qvec.fx = tvec.fy* edge1.fz - tvec.fz*edge1.fy;
	qvec.fy = tvec.fz* edge1.fx - tvec.fx*edge1.fz;
	qvec.fz = tvec.fx* edge1.fy - tvec.fy*edge1.fx;

    // Calculate V parameter and test bounds
    //*v = D3DXVec3Dot( &dir, &qvec );
	*v = Dir.fx*qvec.fx + Dir.fy*qvec.fy + Dir.fz*qvec.fz;
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    //*t = D3DXVec3Dot( &edge2, &qvec );
	 *t = edge2.fx*qvec.fx + edge2.fy*qvec.fy + edge2.fz*qvec.fz;
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return true;
}

