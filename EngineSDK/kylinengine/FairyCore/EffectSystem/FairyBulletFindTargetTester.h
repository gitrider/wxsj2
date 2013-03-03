/********************************************************************
	created:	2008/03/05
	created:	5:3:2008   16:44
	file base:	FairyBulletFindTargetTester
	file ext:	h
	author:		姜威
	email:		jiang628@hotmail.com
	purpose:	
*********************************************************************/
#ifndef FairyBulletFindTargetTester_h__
#define FairyBulletFindTargetTester_h__

#include "FairyBulletTester.h"
#include "Core/FairyPrerequisites.h"
#include "FairyBulletSystem.h"
#include "OgreVector3.h"

namespace Fairy
{
	static const float BULLET_ACCELERATION = 0.3f;   //每秒加速度
	static const int   BULLET_TOTALTIME = 10000;
	static const float BULLET_ANGLE = 60;
	static const int   BULLET_NUM = 4;

	static const float BULLET_ANGLE_RATE = 90.0f; //角速度  角度/每秒
	static const float BULLET_ROTATE_RADIUS = 100.0f; //围绕人物旋转的半径


	static const int Iterative  = 150; // 曲线仿真的线数(控制在150以内， 否则算法出错) 
	static const int Iterative2 = Iterative*Iterative;	
	static const int Iterative3 = Iterative2*Iterative;

	static const float  PI = 3.1415926535f;
#define _MINFLOAT	(1.0E-9)

	float _GetYAngle(const Ogre::Vector2& fvPos1, const Ogre::Vector2& fvPos2);


	class BulletEventSystem;
	class BulletSystem;

	class BulletFindTargetTester : public BulletTester
	{
	public:
		BulletFindTargetTester(BulletEventSystem* eventSystem);
		virtual ~BulletFindTargetTester();


		class CmdPursuitType : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		class CmdPursuitSpeed : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		class CmdAcceleration : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		class CmdAngle : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
		class CmdArriveDistance : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		virtual void operateBulletEventSystem(Real timeElapsed);

		virtual void copyParameters(BulletOperator& newOperator) const;

		const String& getPursuitType() const { return m_pursuitType;}
		void setPursuitType(const String& val);

		const Fairy::Real& getPursuitSpeed() const { return m_pursuitSpeed;}
		void setPursuitSpeed(const Fairy::Real& val) { m_pursuitSpeed = val;}

		const Fairy::Real& getAcceleration() const { return m_fAcceleration;}
		void setAcceleration(const Fairy::Real& val) { m_fAcceleration = val;}

		const Fairy::Real& getArriveDistance() const { return m_arriveDistance; }
		void setArriveDistance(const Fairy::Real& val){ m_arriveDistance = val;}

		const Real& getAngleSpeed() const { return m_fAngleSpeed;}
		void setAngleSpeed(const Real& val) { m_fAngleSpeed = val; }

		static CmdPursuitType		ms_pursuitTypeCmd;
		static CmdPursuitSpeed		ms_pursuitSpeedCmd;
		static CmdArriveDistance	ms_arriveDistanceCmd;
		static CmdAcceleration		ms_AccelerationCmd;
		static CmdAngle				msAngleCmd;
	protected:
		void	pursuitTargetAsPointingType();
		void	pursuitTargetAsBeelineType();
		void	pursuitTargetAsBizzardType();

		Ogre::Vector2 HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);
		//bezier曲线算法,基于hermite算法
		inline Ogre::Vector2 BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter);
		//旋转算法
		Ogre::Vector3 Rotate(const Ogre::Vector3& pos, const Ogre::Vector3& axis, float angle );

		void AlreadyHit();
		bool IsArrive(Ogre::Vector3& vBulletPos,Ogre::Vector3 vTargetPos,Ogre::Real& fCumulateTime,
			Ogre::Real& fCumulateDistance,Ogre::Real time);

		void Curve(Ogre::Vector3& basePos, Ogre::Vector3& vBulletPos,Ogre::Vector3 vTargetPos,
			Ogre::Real& fCumulateTime,Ogre::Real time,Ogre::Real& fCumulateDistance,float fAngle );
		void Line(Ogre::Vector3& vBulletPos,Ogre::Vector3 vTargetPoint,
			Ogre::Real time,Ogre::Real& fCumulateDistance);
	protected:
		String			m_pursuitType;
		Real			m_pursuitSpeed;
		Real			m_fAcceleration;		// 子弹的加速度	
		Real			m_fAngleSpeed;
		Real			m_arriveDistance;
		Ogre::Vector3	m_beforePursuitAccelerationCache;
		bool			m_isInit;

		float				m_fCurvePoint1X;		// 曲线参考点相对比例
		float				m_fCurvePoint1Y;
		float				m_fCurvePoint2X;
		float				m_fCurvePoint2Y;
	private:
	};
}
#endif // FairyBulletFindTargetTester_h__