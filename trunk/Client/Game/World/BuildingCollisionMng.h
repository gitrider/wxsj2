
/** BuildingCollisionMng.h
 *	����˵����	����ͻ��˵Ľ�����������.
 */

#ifndef __BUILDINGCOLLISIONMNG_H__
#define __BUILDINGCOLLISIONMNG_H__


#include <map>
#include <string>
#include <vector>



typedef struct _POINT_2D
{
	INT iX;
	INT iY;
}POINT_2D, *PPOINT_2D;

class Point_2D_less 
{
public:

	bool operator() ( const POINT_2D& point1, const POINT_2D& point2 ) const
	{
		if(point1.iX < point2.iX)
		{
			return true;
		}
		else if(point1.iX == point2.iX)
		{
			if(point1.iY < point2.iY)
			{
				return true;	
			}
		}

		return false;
	}
};

typedef struct _POINT_3D
{
	_POINT_3D()
	{
	
		fx = 0;
		fy = 0;
		fz = 0;
	}

	FLOAT fx;
	FLOAT fy;
	FLOAT fz;

}POINT_3D, *PPOINT_3D;


typedef struct _TRI_INFO
{
	POINT_3D v1;
	POINT_3D v2;
	POINT_3D v3;

}TRI_INFO, *PTRI_INFO;


// �������б���Ϣ��
typedef std::vector<TRI_INFO> TRI_INFO_VECTOR;

// ��ͼ��ע����������б���λ��ӳ���
typedef std::map<POINT_2D, TRI_INFO_VECTOR, Point_2D_less> TRI_INFO_MAP;



class CBuildingCollisionMng
{
public:

	CBuildingCollisionMng(VOID);
	~CBuildingCollisionMng(VOID);

public:

	// �ڵ�ͼ��ע���������ӳ���
	TRI_INFO_MAP m_triInfoMap;


	/** �������������ļ��ж�ȡ���ݡ�
	 *	�����������������ļ��� ����·����
	 */
	UINT LoadCollisionTriInfoFromFile(const char* szFileName);

	/** ͨ������� ƽ��fx, fzλ�õõ��߶ȡ� 
	 * 
	 *	����1�� FLOAT fx    ��Ϸ�߼����ꡣ
	 *
	 *	����2�� FLOAT fz    ��Ϸ�߼����ꡣ
	 *
	 *	����3�� FLOAT& fy   ��Ϸ�߼�����  ���صĸ߶ȡ�
	 *
	 *	return�� 1 �� �ɹ��� fy�����塣
	 *			 0 �� ʧ�ܣ� fy�����壬 �����˴��޽��������������ݡ�
	 */
	BOOL GetHeight(FLOAT fx, FLOAT fz, FLOAT& fy);


	/** ͨ������� ƽ��fx, fzλ�õõ��߶ȡ� 
	 * 
	 *	����1�� FLOAT fx    ��Ϸ�߼����ꡣ
	 *
	 *	����2�� FLOAT fz    ��Ϸ�߼����ꡣ
	 *
	 *	����3�� FLOAT& fy   ��Ϸ�߼�����  ���صĸ߶ȡ�
	 *
	 *	return�� 1 �� �ɹ��� fy�����塣
	 *		     0 �� ʧ�ܣ� fy�����壬 �����˴��޽��������������ݡ�
	 */
	BOOL SelBuilding(	FLOAT fOrigx, FLOAT fOrigy, FLOAT fOrigz,	// ���ߵ�ԭ�㡣
						FLOAT fDirx,  FLOAT fDiry,  FLOAT fDirz,	// ���ߵķ���
						INT	  ix, INT iz,							// ѡ�е�ͼ��λ��
						FLOAT& fPosx, FLOAT& fPosy, FLOAT& fPosz	// ���ؽ������������λ�á�
					);


	BOOL IntersectTriangle( POINT_3D Orig,	// ���ߵ�ԭ�㡣
							POINT_3D Dir,	// ���ߵķ���
							POINT_3D v1,
							POINT_3D v2,
							POINT_3D v3,
							FLOAT* t, 
							FLOAT* u, 
							FLOAT* v 
						  );


	/**  ������� */
	VOID ClearData();

};


#endif //__BUILDINGCOLLISIONMNG_H__