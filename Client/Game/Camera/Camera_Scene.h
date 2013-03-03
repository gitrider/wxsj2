
/**
	游戏流程中的摄像机
 */

#pragma once


#include "Camera.h"
#include <OgreSphere.h>
#include <OgreAxisAlignedBox.h>
#include "EffectSystem/FairyBulletCallbacks.h"


namespace Fairy
{
	class TerrainData;
}

struct EffectCameraShakeCallback : public Fairy::CameraShakeCallback
{
	virtual VOID onShakeCallBack(float amplitude,float duration,float requency);
};
class CCamera_Scene : public CCamera
{
public:

	//-------------------------------------------------
	// 移动所关注的位置到...Pitch
	// 所传进的参数为相对于地形的坐标，以米为单位
	// 一般来说，这个位置是玩家自己的头顶
	virtual VOID		SetLookAt(const fVector3& fvPos);
	
	//-------------------------------------------------
	// 摄像机眼睛距离注视位置的距离, 以米为单位
	virtual VOID		SetAddHeight(FLOAT fAddHeight);
		   FLOAT		GetAddHeight(VOID) const { return m_fAddHeight; }

	//-------------------------------------------------
	// 摄像机视线与水平线的夹角，一般为负数(向下看)
	static FLOAT		STATIC_PITCH;		// 固定视角
	static FLOAT		APPROCH_PITCH;		// 视角拉进或拉远时控制的角度

	virtual VOID		SetPitch(FLOAT fPitch);
	virtual VOID		SetApprochPitch( FLOAT fPitch );
	
	//-------------------------------------------------
	// 设置摄像机眼睛与人物之间的距离
	static FLOAT		MIN_DISTANCE;	// 极限情况，摄像机能够离人物的最近距离
	static FLOAT		MAX_DISTANCE;	// 极限情况，摄像机能够远离人物的最远距离

	virtual VOID		SetDistance(FLOAT fDistance);
	virtual FLOAT		GetDistance(VOID) const;
	//-------------------------------------------------
	// 设置机视线与世界坐标x轴的角度 Should[0, 2л)
	virtual VOID		SetDirection(FLOAT fDirection);

	//-------------------------------------------------
	// 设置加入一次震荡
			VOID		AddOneVib(FLOAT fAmplitude, FLOAT fDuration, UINT fFrequency);



public:

	//摄像机构造函数
	// pOgreCamera  - 所附加到的ogre摄像机
	/*
	|
	|	!关于摄像机初始化
	|
	|		o 人物第一次进入场景时，面朝x轴所指向的方向
	|		o 摄像机则在人物的背后，注视人物的头部
	|
	*/
	CCamera_Scene(Ogre::Camera* pOgreCamera, 
						const fVector3& fvPos	= fVector3(0.0f, 0.0f, 0.0f),
						FLOAT fAddHeight		= 0.7f,
						FLOAT fPitch			= STATIC_PITCH, 
						FLOAT fDistance			= MAX_DISTANCE, 
						FLOAT fDirection		= 3.1416);
	virtual ~CCamera_Scene(VOID);

public:
	virtual VOID		Update(VOID);

	void SetOffset( FLOAT fOffset )	{ m_fOffset = fOffset * 100; }	// 米到厘米的转换

	static EffectCameraShakeCallback s_EffectCameraShakeCallback;

protected:
	//-------------------------------------------------
	//摄像机状态
	enum CAMERA_STATUS
	{
		NORMAL_STATUS = 0,	//普通状态
		VIBRATION_STATUS,	//震荡状态
	};
	//-------------------------------------------------
	//震荡状态参数结构
	struct VibParam_t
	{
		FLOAT	fAmplitude;	//振幅
		FLOAT	fDuration;	//持续时间
		FLOAT	fFrequency; //频率
		FLOAT   fAge;       //已震荡时间

		VibParam_t()
		{
			fAmplitude = 0.0;
			fDuration = 0.0;
			fFrequency = 0.0;
            fAge = 0.0;
		}
	};

protected:

	/// 相机所看的位置距离地面的高度，一般是人物的高度, 以米为单位
	FLOAT				m_fAddHeight;
	
	/// 摄像机当前状态
	CAMERA_STATUS		m_status;

	/// 当前震荡参数
	VibParam_t			m_CurVibParam;

	FLOAT				m_fOffset;

protected:

	//计算眼睛位置
	static fVector3		_GetEyePos(
							const fVector3& fvLookAt,
							FLOAT fDistance,
							FLOAT fDirection,
							FLOAT fPitch);

	//-------------------------------------------------
	//物理碰撞检测

	//最小计算误差
	static FLOAT MIN_ANGLE;
	//最小调动范围
	static FLOAT STEP_ANGLE;

	//摄像机眼部的近距碰撞球
	struct CAMERA_NEAR_BALL
	{
		Ogre::Sphere sphere;	
	};

	//和摄像机眼睛有交互的地形网格(最多4个网格)
	struct CAMERA_INTER_GRIDS
	{
		std::vector< Ogre::AxisAlignedBox >	m_fvGrid;
	};

	//根据碰撞得到合理的Pitch
	static FLOAT		_GetValidPitch(
							const Fairy::TerrainData* pTerrainData,	//地形数据
							const fVector3& fvLookAt,				//关注点
							FLOAT fDistance,						//距离
							FLOAT fDirection,						//旋转角度
							FLOAT fPitchNow,						//目前的Pitch(尽量接近此值)
							FLOAT fPitchApproch,					//要趋近的Pitch
							FLOAT fCameraNearDis);					//眼睛的近裁减距离

	//眼睛是否合理
	static BOOL		_IsValidEye(
							const Fairy::TerrainData* pTerrainData,	//地形数据
							const fVector3& fvEye,					//眼睛点
							FLOAT fCameraNearDis);					//近距离

	//取得距离眼睛最近的地形数据
	static BOOL		_GetInterPatch(
							const Fairy::TerrainData* pTerrainData,	//地形数据
							const fVector3& fvEyePos,				//眼睛的投影位置
							CAMERA_INTER_GRIDS& theInterPatch);		//地形数据

	//设置当前摄像机状态
	BOOL			_SetStatus(CAMERA_STATUS nStatus );				

	//根据当前状态刷新摄像机
	VOID			_UpdateCurStatus(
							fVector3& vEyePos, 
							fVector3& vLookatPos);							


};
