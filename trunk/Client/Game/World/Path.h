// Path.h
//
// 游戏世界中的路径
// 
//	o 路径是一系列路径段(PathUnit)的集合，每个PathUnit由起点和终点组成，是直线
//	o 客户端按照寻路算法，将路径分解成一段段直线，形成一个路径队列
//	o 客户端只将当前的路径发送到服务器，只有服务器这条路径走完后才会发送下一个路径到服务器
//	o 当客户端提前走完当前路径后，不会等待服务器走完，而会直接走下一条路径，这样在客户端不会出现停顿
//	o 当客户端收到服务器走完一条路径的消息后，会把当前作标点和目标点发送到服务器上
// 
/////////////////////////////////////////////////////

#ifndef __PATH_H__
#define __PATH_H__

#pragma once

#include <vector>
#include <map>

class CScene;
class CPath
{
public:
	CPath();
	virtual ~CPath();

	void Clean(void);

	struct PathUnit
	{
		fVector2	fvStart;		//起始点
		fVector2	fvTarget;		//目标点
	};	

	// 计算从开始点到结束点的路径。
	BOOL				CreateMovePath(const fVector2& fvCurrent, const fVector2& fvTarget);

	//  直接查找目的地。
	BOOL                GetPathDirect(const fVector2& fvCurrent, const fVector2& fvTarget, const BOOL bUseFly = FALSE);

	//取得路径段的数目
	INT					GetUnitNum(VOID)const;

	const PathUnit&		GetConstPathUnit(INT nIndex)const;

	// 当前点是否在不可行走区域之内
	static BOOL IsPointInUnreachRegion(const fVector2& fvTarget);

	static FLOAT fvDistancePow(fVector2 fv1, fVector2 fv2);

private:

	//取得路径中的某一段，如果nIndex非法，则抛出异常
	PathUnit&			GetPathUnit(INT nIndex);	

protected:
	std::vector< PathUnit >		m_vPosStack;		//关键点的集合，每两个点之间是一条路径单位
};

#endif // __PATH_H__
