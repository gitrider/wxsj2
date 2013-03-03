#include "BuildingCollision.h"

#include <set>
#include <limits>

#include "ieee.h"

namespace PathLib
{
struct fPoint3
{
	float x,y,z;
	fPoint3()
	{
		x=y=z=0.f;
	}
	fPoint3(const fPoint3& other)
	{
		x=other.x;		
		y=other.y;		
		z=other.z;		
	}
	fPoint3(const IceMaths::Point& other)
	{
		x=other.x;		
		y=other.y;		
		z=other.z;		
	}
};

struct fTriangle
{
	fPoint3 v1, v2, v3;
};


typedef std::pair< int, std::pair< int, int > > iPoint3;
__forceinline void toPoint(iPoint3& pt, float x, float y, float z)
{
	pt.first			= PathLib::fast_int(x*10000.f);
	pt.second.first		= PathLib::fast_int(y*10000.f);
	pt.second.second	= PathLib::fast_int(z*10000.f);
}

__forceinline void toPoint(iPoint3& pt, unsigned int x, unsigned int y, unsigned int z)
{
	pt.first			= x;
	pt.second.first		= y;
	pt.second.second	= z;
}

typedef std::map< iPoint3, int > iPointMap;
typedef std::set< iPoint3 > iTriangleSet;

unsigned int _insertPoint(iPointMap& iAllPointMap, BuildingCollsionImpl::PointBuf& fAllPointBuf, const fPoint3& point)
{
	//if(fAllPointBuf.size() == 5)
	//{
	//	int a=0;
	//}

	//if(point.x==10.333056 && point.y==10.979897 && point.z==185.997559)
	//{
	//	int size = fAllPointBuf.size();
	//	int a=0;
	//}

	iPoint3 ip;
	toPoint(ip, point.x, point.y, point.z);
	
	iPointMap::iterator itFind = iAllPointMap.find(ip);

	if(itFind==iAllPointMap.end())
	{
		fAllPointBuf.push_back(IceMaths::Point(point.x, point.y, point.z));
		unsigned int index = (unsigned int)fAllPointBuf.size()-1;
		iAllPointMap.insert(std::make_pair(ip, index));
		return index;
	}
	else
	{
		return itFind->second;
	}
}


const float BuildingCollsionImpl::MAX_Y_VALUE = 10000.f;
BuildingCollsionImpl::BuildingCollsionImpl()
	: m_modelScene(0)
	, m_pointBuf(0)
	, m_indexBuf(0)
{
}

BuildingCollsionImpl::~BuildingCollsionImpl()
{
	clear();
}

void BuildingCollsionImpl::clear()
{
	if(m_modelScene)
	{
		delete m_modelScene;
		m_modelScene = 0;
	}

	if(m_indexBuf)
	{
		delete[] m_indexBuf;
		m_indexBuf = 0;
	}
	m_pointBuf.clear();
}

bool BuildingCollsionImpl::openSceneFile(const char* pSource, unsigned int fileSize)
{
	//清除旧数据
	clear();

	const char* point = pSource;

	unsigned int dwVersion = 0;
	memcpy(&dwVersion, point, sizeof(unsigned int)); point += sizeof(unsigned int);

	int nPointsCounts = 0;
	memcpy(&nPointsCounts, point, sizeof(int));	point += sizeof(int);
	
	iTriangleSet iAllTriangle;
	iPointMap iAllPointMap;
	m_pointBuf.reserve(5000);

	//int totalCounts = 0;
	if(nPointsCounts <= 0) return true;		//skip
	for(int i=0; i<nPointsCounts; i++)
	{
		point += sizeof(int)*2;		//skip x,y

		int triCounts;
		memcpy(&triCounts, point, sizeof(int));	point += sizeof(int);
		for(int j=0; j<triCounts; j++)
		{
			fTriangle f_triangle;
			memcpy(&f_triangle, point, sizeof(fTriangle));	point += sizeof(fTriangle);

			iPoint3 i_triangle;
			toPoint(i_triangle,
				_insertPoint(iAllPointMap, m_pointBuf, f_triangle.v1),
				_insertPoint(iAllPointMap, m_pointBuf, f_triangle.v2),
				_insertPoint(iAllPointMap, m_pointBuf, f_triangle.v3));

			std::pair< iTriangleSet::iterator, bool > ret = 
				iAllTriangle.insert(i_triangle);
		}
	}
#if 0
	FILE* fpDebug = fopen("d:\\out.txt", "w");
	for(int i=0; i<(int)m_pointBuf.size(); i++)
	{
		fprintf(fpDebug, "%d\t%f\t%f\t%f\n", i, m_pointBuf[i].x, m_pointBuf[i].y, m_pointBuf[i].z);
	}
	fclose(fpDebug); fpDebug=0;
#endif
	//create index buf
	m_indexBuf = new unsigned int[iAllTriangle.size()*3];
	iTriangleSet::iterator it, end=iAllTriangle.end();
	int i=0;
	for(it=iAllTriangle.begin(); it!=end; it++)
	{
		m_indexBuf[i++]=it->first;
		m_indexBuf[i++]=it->second.first;
		m_indexBuf[i++]=it->second.second;
	}

	m_meshInterface.SetNbTriangles((uword)iAllTriangle.size());
	m_meshInterface.SetNbVertices((uword)m_pointBuf.size());
	m_meshInterface.SetPointers((const IndexedTriangle*)m_indexBuf, &(m_pointBuf[0]));

	Opcode::OPCODECREATE Create;
	Create.mIMesh			= &m_meshInterface;
	Create.mSettings.mLimit	= 1;
	Create.mSettings.mRules	= Opcode::SPLIT_SPLATTER_POINTS|Opcode::SPLIT_GEOM_CENTER;
	Create.mNoLeaf			= true;
	Create.mQuantized		= true;
	Create.mKeepOriginal	= false;
	Create.mCanRemap		= false;


	m_modelScene = new Opcode::Model;
	bool ret = m_modelScene->Build(Create);

	return true;
}


void BuildingCollsionImpl::hitCallback(const Opcode::CollisionFace& hit, void* user_data)
{
	((BuildingCollsionImpl*)user_data)->_onHitCallback(hit);
}

void BuildingCollsionImpl::_onHitCallback(const Opcode::CollisionFace& hit)
{
	iPoint3 triangle;
	toPoint(triangle, 
		m_indexBuf[hit.mFaceID*3+0],
		m_indexBuf[hit.mFaceID*3+1],
		m_indexBuf[hit.mFaceID*3+2]);

	fPoint3 p1 = m_pointBuf[triangle.first];
	fPoint3 p2 = m_pointBuf[triangle.second.first];
	fPoint3 p3 = m_pointBuf[triangle.second.second];

    // 因为这个函数会被回调多次，所以只需保存离射线原点距离最短的点
	if(hit.mDistance > m_hitDistance)
	{
		return;
	}

	m_hitDistance = hit.mDistance;
	m_hitPoint.x = p2.x*hit.mU + p3.x*hit.mV + p1.x*(1-hit.mU-hit.mV);
	m_hitPoint.y = p2.y*hit.mU + p3.y*hit.mV + p1.y*(1-hit.mU-hit.mV);
	m_hitPoint.z = p2.z*hit.mU + p3.z*hit.mV + p1.z*(1-hit.mU-hit.mV);

#if 0
	printf(
		"Hit[%d]<%d,%d,%d>\n" 
		"v1=%.5f, %.5f, %.5f\n"
		"v2=%.5f, %.5f, %.5f\n"
		"v3=%.5f, %.5f, %.5f\n"
		"distance=%.2f(%.2f), U=%.3f, V=%.3f!\n"
		"hit=%.5f, %.5f, %.5f\n"
		"\n", 
		hit.mFaceID, 
		triangle.first, triangle.second.first, triangle.second.second,
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z,
		hit.mDistance,  MAX_Y_VALUE-hit.mDistance,
		hit.mU, hit.mV,
		m_hitPoint.x, m_hitPoint.y, m_hitPoint.z
		);
#endif
}

bool BuildingCollsionImpl::getHeight(float x, float& y, float z)
{
	ResultPoint3 hit(x,y,z);
	if(collsionRay(ResultPoint3(x, MAX_Y_VALUE, z), ResultPoint3(0, -1, 0), hit))
	{
		y = hit.y;
		return true;
	}

	return false;
}

bool BuildingCollsionImpl::collsionRay(const ResultPoint3& org, const ResultPoint3& dir, ResultPoint3& hit)
{
	IceMaths::Ray ray(Point(org.x, org.y, org.z), Point(dir.x, dir.y, dir.z));

    // 每次查询前初始化射线于交点的距离为最大值
    m_hitDistance = std::numeric_limits<float>::infinity();

	Opcode::RayCollider rayCollider;
	Opcode::CollisionFaces result;
	rayCollider.SetUserData(this);
	rayCollider.SetHitCallback(hitCallback);
    // flag为false时，执行完整的查询，hitCallback会被回调多次（次数等同于射线在这次查询中碰到的点的个数）
	rayCollider.SetFirstContact(false);
	rayCollider.SetCulling(false);
//	rayCollider.SetClosestHit(true);
//	rayCollider.SetMaxDist();
	rayCollider.Collide(ray, *m_modelScene);

	if(rayCollider.GetContactStatus())
	{
		hit.x = m_hitPoint.x;
		hit.y = m_hitPoint.y;
		hit.z = m_hitPoint.z;
		return true;
	}

	return false;
}


}