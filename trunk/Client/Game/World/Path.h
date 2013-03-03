// Path.h
//
// ��Ϸ�����е�·��
// 
//	o ·����һϵ��·����(PathUnit)�ļ��ϣ�ÿ��PathUnit�������յ���ɣ���ֱ��
//	o �ͻ��˰���Ѱ·�㷨����·���ֽ��һ�ζ�ֱ�ߣ��γ�һ��·������
//	o �ͻ���ֻ����ǰ��·�����͵���������ֻ�з���������·�������Żᷢ����һ��·����������
//	o ���ͻ�����ǰ���굱ǰ·���󣬲���ȴ����������꣬����ֱ������һ��·���������ڿͻ��˲������ͣ��
//	o ���ͻ����յ�����������һ��·������Ϣ�󣬻�ѵ�ǰ������Ŀ��㷢�͵���������
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
		fVector2	fvStart;		//��ʼ��
		fVector2	fvTarget;		//Ŀ���
	};	

	// ����ӿ�ʼ�㵽�������·����
	BOOL				CreateMovePath(const fVector2& fvCurrent, const fVector2& fvTarget);

	//  ֱ�Ӳ���Ŀ�ĵء�
	BOOL                GetPathDirect(const fVector2& fvCurrent, const fVector2& fvTarget, const BOOL bUseFly = FALSE);

	//ȡ��·���ε���Ŀ
	INT					GetUnitNum(VOID)const;

	const PathUnit&		GetConstPathUnit(INT nIndex)const;

	// ��ǰ���Ƿ��ڲ�����������֮��
	static BOOL IsPointInUnreachRegion(const fVector2& fvTarget);

	static FLOAT fvDistancePow(fVector2 fv1, fVector2 fv2);

private:

	//ȡ��·���е�ĳһ�Σ����nIndex�Ƿ������׳��쳣
	PathUnit&			GetPathUnit(INT nIndex);	

protected:
	std::vector< PathUnit >		m_vPosStack;		//�ؼ���ļ��ϣ�ÿ������֮����һ��·����λ
};

#endif // __PATH_H__
