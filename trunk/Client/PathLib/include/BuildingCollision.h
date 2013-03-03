#ifndef __BuildingCollsion_H__
#define __BuildingCollsion_H__

#include "PathLibInterface.h"
#include <Opcode.h>

namespace PathLib
{

class BuildingCollsionImpl : public BuildingCollision
{
public:
	typedef std::vector< IceMaths::Point > PointBuf;

	void clear();

	virtual bool openSceneFile(const char* pSource, unsigned int fileSize);
	virtual bool getHeight(float x, float& y, float z);
	virtual bool collsionRay(const ResultPoint3& org, const ResultPoint3& dir, ResultPoint3& hit);

private:
	static void hitCallback(const Opcode::CollisionFace& hit, void* user_data);
	void _onHitCallback(const Opcode::CollisionFace& hit);

private:
	Opcode::Model*			m_modelScene;
	PointBuf				m_pointBuf;
	unsigned int*			m_indexBuf;
	Opcode::MeshInterface	m_meshInterface;

    /// 查询过程的中间变量
	float					m_hitDistance;  //  用于保存碰撞到的点与射线原点的最短距离
	IceMaths::Point			m_hitPoint; //  用于保存最终找到的离射线原点距离最短的点

	const static float MAX_Y_VALUE;

public:
	BuildingCollsionImpl();
	virtual ~BuildingCollsionImpl();
};

}

#endif