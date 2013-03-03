
#include "FairyBulletFindTargetTester.h"
#include "FairyBulletFlowSystemManager.h"
#include <OgreStringConverter.h>
namespace Fairy
{
	float _GetYAngle(const Ogre::Vector2& fvPos1, const Ogre::Vector2& fvPos2)
	{
		double dDistance = (double)(fvPos1-fvPos2).length();
		if(dDistance <= 0.0f) return 0.0f;

		double fACos = (fvPos2.y - fvPos1.y ) / dDistance;
		if( fACos >  1.0) fACos = 0.0;
		if( fACos < -1.0) fACos = PI;

		if( fACos > -_MINFLOAT && fACos < _MINFLOAT)
		{
			if(fvPos2.x > fvPos1.x )	return  (float)PI/2.0f;
			else						return -(float)PI/2.0f;
		}

		fACos = ::acos(fACos);

		// [0~180]
		if(fvPos2.x >= fvPos1.x)
			return (float)fACos;
		//(180, 360)
		else
			return (float)(2 * PI - fACos);
	}


	BulletFindTargetTester::CmdPursuitType			BulletFindTargetTester::ms_pursuitTypeCmd;
	BulletFindTargetTester::CmdPursuitSpeed			BulletFindTargetTester::ms_pursuitSpeedCmd;
	BulletFindTargetTester::CmdArriveDistance		BulletFindTargetTester::ms_arriveDistanceCmd;
	BulletFindTargetTester::CmdAcceleration			BulletFindTargetTester::ms_AccelerationCmd;
	BulletFindTargetTester::CmdAngle				BulletFindTargetTester::msAngleCmd;

	BulletFindTargetTester::BulletFindTargetTester(BulletEventSystem* eventSystem) : BulletTester(),
		m_isInit(false)
		,m_arriveDistance(1.0)
		,m_pursuitSpeed(0.0f)
		,m_pursuitType("beeline")
		,m_fAcceleration(0.0f)
		,m_fAngleSpeed(0.0f)
		,m_beforePursuitAccelerationCache(Ogre::Vector3::ZERO)
	{
		m_type = "findtarget";
		m_parent = eventSystem;
		if (createParamDictionary("BulletFindTargetTester"))
        {
			Ogre::ParamDictionary* dict = getParamDictionary();
			dict->addParameter(Ogre::ParameterDef("pursuittype", 
				"The pursuittype of bullet find target tester.",
				Ogre::PT_STRING),&ms_pursuitTypeCmd);
			dict->addParameter(Ogre::ParameterDef("pursuitspeed", 
				"The pursuitspeed of bullet find target tester.",
				Ogre::PT_REAL),&ms_pursuitSpeedCmd);
			dict->addParameter(Ogre::ParameterDef("acceleration", 
				"The acceleration of bullet find target tester.",
				Ogre::PT_REAL),&ms_AccelerationCmd);
			dict->addParameter(Ogre::ParameterDef("arrivedistance", 
				"The arrivedistance of bullet find target tester.",
				Ogre::PT_REAL),&ms_arriveDistanceCmd);
			dict->addParameter(Ogre::ParameterDef("anglespeed", 
				"The force  of bullet Tester.",
				Ogre::PT_REAL),&msAngleCmd);
			dict->addParameter(Ogre::ParameterDef("output", 
				"The output of bullet Tester.",
				Ogre::PT_STRING),&ms_outputCmd);	
		}
	}
	BulletFindTargetTester::~BulletFindTargetTester()
	{
	}
	void BulletFindTargetTester::copyParameters(BulletOperator& newOperator) const
    {
		assert(newOperator);
		BulletTester::copyParameters(newOperator);
	
		BulletFindTargetTester* newFindTargetTester =  dynamic_cast<BulletFindTargetTester*>(&newOperator);
		if(newFindTargetTester)
		{
			newFindTargetTester->m_pursuitType = m_pursuitType;
			newFindTargetTester->m_pursuitSpeed = m_pursuitSpeed;
			newFindTargetTester->m_arriveDistance = m_arriveDistance;
			newFindTargetTester->m_fAngleSpeed = m_fAngleSpeed;
			newFindTargetTester->m_fAcceleration = m_fAcceleration;
		}
    }
	void BulletFindTargetTester::operateBulletEventSystem(Real timeElapsed)
	{
		BulletEventSystem::BulletSystemVecotor::iterator it = 
			m_parent->getActiveBulletSystem().begin();
		while(it != m_parent->getActiveBulletSystem().end())
		{
			BulletSystem* pBulletSystem = *it;
			if(pBulletSystem && pBulletSystem->getIsCreated())
			{
				//设置曲线形式
				setPursuitType(m_pursuitType);

				Real fCumulateTime = pBulletSystem->getAge();

				Real fTime = pBulletSystem->getDelta();
				Real fCumulateDistance = pBulletSystem->getCumulateDistance();

				TransformInfo info;
				info = pBulletSystem->getTransformInfo();

				Ogre::Vector3 vBulletPosition = info.mPosition;
				Ogre::Vector3 vTargetPoint = pBulletSystem->getTargetPosition();

				Ogre::Vector3 vCurPos = pBulletSystem->getCurrentPosition();

				// 已经击中
				if(!pBulletSystem->getAlreadyHit())
				{
					//获取目标坐标
					//GetTargetPos();
					if(m_pursuitType == "parabola" || m_pursuitType == "curve")//抛物线
					{
						// 接近目标点
						if (IsArrive(vCurPos,vTargetPoint,fCumulateTime,fCumulateDistance,fTime))
						{
							vBulletPosition = vTargetPoint;	
							

							AlreadyHit();
						}
						// 还有距离
						else
						{
							Curve(vCurPos,vBulletPosition,vTargetPoint,fCumulateTime,fTime,fCumulateDistance,
								PI / 180.0f * m_fAngleSpeed);
							
							pBulletSystem->setCurrentPosition(vCurPos);

							float fDir = _GetYAngle( Ogre::Vector2(vBulletPosition.x, vBulletPosition.z ),
								Ogre::Vector2( vTargetPoint.x, vTargetPoint.z ) );					
							Ogre::Quaternion qu(Ogre::Radian(fDir), Ogre::Vector3::UNIT_Y);	

							Ogre::Vector3 basePos = vTargetPoint-vBulletPosition;

							bool up = basePos.y>0?true:false;

							basePos.normalise();
							Ogre::Vector3 formatPos = Ogre::Vector3(basePos.x, 0,basePos.z);
							formatPos.normalise();					

							Ogre::Radian fDirx = Ogre::Math::ACos(basePos.dotProduct(formatPos));
							fDirx = up?(-fDirx):fDirx;

							Ogre::Quaternion quX(fDirx, Ogre::Vector3::UNIT_X);
							info.mRotation = qu*quX;
						}						
					}
					else if(m_pursuitType == "beeline")// 直线
					{
						// 接近目标点
						if (IsArrive(vBulletPosition,vTargetPoint,fCumulateTime,fCumulateDistance,timeElapsed))
						{
							vBulletPosition  = vTargetPoint;

							AlreadyHit();
						}
						// 还有距离
						else
						{
							Line(vBulletPosition,vTargetPoint,fTime,fCumulateDistance);
						}
						pBulletSystem->setCurrentPosition(vBulletPosition);
						
						float fDir = _GetYAngle( Ogre::Vector2(vBulletPosition.x, vBulletPosition.z ),
							Ogre::Vector2( vTargetPoint.x, vTargetPoint.z ) );					
						Ogre::Quaternion qu(Ogre::Radian(fDir), Ogre::Vector3::UNIT_Y);	

						Ogre::Vector3 basePos = vTargetPoint-vBulletPosition;

						bool up = basePos.y>0?true:false;

						basePos.normalise();
						Ogre::Vector3 formatPos = Ogre::Vector3(basePos.x, 0,basePos.z);
						formatPos.normalise();					

						Ogre::Radian fDirx = Ogre::Math::ACos(basePos.dotProduct(formatPos));
						fDirx = up?(-fDirx):fDirx;

						Ogre::Quaternion quX(fDirx, Ogre::Vector3::UNIT_X);
						info.mRotation = qu*quX;
					}
					else if(m_pursuitType == "roate")//围绕人物旋转
					{
						float fCurrentAngle = fCumulateTime * BULLET_ANGLE_RATE + m_fAngleSpeed;//当前角度
						int   nCurrentAngle = (int)fCurrentAngle % 360;
						float fAngle		= PI / 180.0f * (float)nCurrentAngle;

						vBulletPosition.y = vTargetPoint.y;
						vBulletPosition.x = vTargetPoint.x + sin(fAngle) * BULLET_ROTATE_RADIUS;
						vBulletPosition.z = vTargetPoint.z + cos(fAngle) * BULLET_ROTATE_RADIUS;

						pBulletSystem->setCurrentPosition(vBulletPosition);
						
						float fDir = _GetYAngle( Ogre::Vector2(vBulletPosition.x, vBulletPosition.z ),
							Ogre::Vector2( vTargetPoint.x, vTargetPoint.z ) );					
						Ogre::Quaternion qu(Ogre::Radian(fDir), Ogre::Vector3::UNIT_Y);	

						Ogre::Vector3 basePos = vTargetPoint-vBulletPosition;

						bool up = basePos.y>0?true:false;

						basePos.normalise();
						Ogre::Vector3 formatPos = Ogre::Vector3(basePos.x, 0,basePos.z);
						formatPos.normalise();					

						Ogre::Radian fDirx = Ogre::Math::ACos(basePos.dotProduct(formatPos));
						fDirx = up?(-fDirx):fDirx;

						Ogre::Quaternion quX(fDirx, Ogre::Vector3::UNIT_X);
						info.mRotation = qu*quX;
					}
					// 无轨迹，直接爆炸
					else if(m_pursuitType == "none")
					{

					}
					/*else if(m_pursuitType ==  "exp")
					{
						// 接近目标点
						if (IsArrive(vBulletPosition,vTargetPoint,fCumulateTime,fCumulateDistance,pBulletSystem->getDelta()))
						{
							vBulletPosition = vTargetPoint;		

							AlreadyHit();
						}
						// 还有距离
						else
						{
							Curve(vBulletPosition,vTargetPoint,fCumulateTime,fTime,fCumulateDistance,
								PI / 180.0f * m_fAngleSpeed);
						}
					}*/
					else
					{
						AlreadyHit();
					}
				}				
			
				info.mPosition = vBulletPosition;
				pBulletSystem->setTransformInfo(info);				
				pBulletSystem->setPosition(info.mPosition);
				pBulletSystem->setOrientation(info.mRotation);
				pBulletSystem->setCumulateDistance(fCumulateDistance);

				Real fDis = pBulletSystem->getArriveDistance();
				if( fDis < m_arriveDistance && m_outputEventName.length()> 0)
				{
					m_parent->addTransition(m_outputEventName,pBulletSystem);
					Fairy::BulletFlowSystemManager::getSingleton().getBulletSystemHitTargetCallback()->onHitTargetCallback(
						m_parent->getParent(),pBulletSystem);
				}
			}
			it ++;

		}
	}
	String BulletFindTargetTester::CmdPursuitType::doGet(const void* source) const
	{
		const BulletFindTargetTester* object = static_cast<const BulletFindTargetTester*>(source);
		return object->getPursuitType();

	}
	void BulletFindTargetTester::CmdPursuitType::doSet(void* target, const String& val)
	{
		BulletFindTargetTester* object = static_cast<BulletFindTargetTester*>(target);
		object->setPursuitType(val);
	}

	String BulletFindTargetTester::CmdPursuitSpeed::doGet(const void* source) const
	{
		const BulletFindTargetTester* object = static_cast<const BulletFindTargetTester*>(source);
		return Ogre::StringConverter::toString(object->getPursuitSpeed());
	}
	void BulletFindTargetTester::CmdPursuitSpeed::doSet(void* target, const String& val)
	{
		BulletFindTargetTester* object = static_cast<BulletFindTargetTester*>(target);
		object->setPursuitSpeed(Ogre::StringConverter::parseReal(val));
	}

	
	String BulletFindTargetTester::CmdAcceleration::doGet(const void* source) const
	{
		const BulletFindTargetTester* object = static_cast<const BulletFindTargetTester*>(source);
		return Ogre::StringConverter::toString(object->getAcceleration());
	}
	void BulletFindTargetTester::CmdAcceleration::doSet(void* target, const String& val)
	{
		BulletFindTargetTester* object = static_cast<BulletFindTargetTester*>(target);
		object->setAcceleration(Ogre::StringConverter::parseReal(val));
	}

	String BulletFindTargetTester::CmdAngle::doGet(const void* source) const
	{
		const BulletFindTargetTester* object = static_cast<const BulletFindTargetTester*>(source);
		return Ogre::StringConverter::toString(object->getAngleSpeed());
	}
	void BulletFindTargetTester::CmdAngle::doSet(void* target, const String& val)
	{
		BulletFindTargetTester* object = static_cast<BulletFindTargetTester*>(target);
		object->setAngleSpeed(Ogre::StringConverter::parseReal(val));
	}

	String BulletFindTargetTester::CmdArriveDistance::doGet(const void* source) const
	{
		const BulletFindTargetTester* object = static_cast<const BulletFindTargetTester*>(source);
		return Ogre::StringConverter::toString(object->getArriveDistance());
	}
	void BulletFindTargetTester::CmdArriveDistance::doSet(void* target, const String& val)
	{
		BulletFindTargetTester* object = static_cast<BulletFindTargetTester*>(target);
		object->setArriveDistance(Ogre::StringConverter::parseReal(val));
	}

	void BulletFindTargetTester::setPursuitType(const String& val)
	{

		m_pursuitType = val;

		if(m_pursuitType == "parabola")//抛物线
		{
			m_fCurvePoint1X = 0.0f;		//	1						 2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 1.0f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x

		
			//m_fCurvePoint1X = 0.0f;		//	1		   2
			//m_fCurvePoint1Y = 0.5f;		//

			//m_fCurvePoint2X = 0.5f;		//
			//m_fCurvePoint2Y = 0.5f;		//  x						 x

			//m_fCurvePoint1X = 0.5f;		//	            1			 2
			//m_fCurvePoint1Y = 0.5f;		//

			//m_fCurvePoint2X = 1.0f;		//
			//m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		else if(m_pursuitType == "beeline")
		{
			m_fCurvePoint1X = 0.0f;		//	1		   2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 0.5f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		else if(m_pursuitType == "roate")
		{
			m_fCurvePoint1X = 0.5f;		//	            1			 2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 1.0f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}
		else if(m_pursuitType == "curve")//曲线-s形
		{
			m_fCurvePoint1X = 0.0f;		//	1
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 0.5f;		//
			m_fCurvePoint2Y = 0.0f;		//  x			2			 x
		}
		//else if(m_pursuitType ==  "exp")
		//{
		//	m_fCurvePoint1X = 0.0f;		//	1
		//	m_fCurvePoint1Y = 0.5f;		//

		//	m_fCurvePoint2X = 0.5f;		//
		//	m_fCurvePoint2Y = 0.0f;		//  x			2			 x		
		//}
		else
		{
			m_fCurvePoint1X = 0.0f;		//	1						 2
			m_fCurvePoint1Y = 0.5f;		//

			m_fCurvePoint2X = 1.0f;		//
			m_fCurvePoint2Y = 0.5f;		//  x						 x
		}

	}

		/*--------------------------------------------------
		x1,y1,x2,y2     =   曲线端点，最好限制在1000以内
		xr1,yr1,xr2,yr2 =   曲线两参考向量, 最好限制在1000以内	
		currIter = 位置所在段数
	--------------------------------------------------*/ 
	Ogre::Vector2 BulletFindTargetTester::HermiteCurve (int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter)   
	{
		if (currIter>Iterative)
		{
			assert(!"子弹曲线段值错误！");
			currIter = Iterative;
		}

		long	oldx	= x1,
			oldy	= y1,
			m1		= Iterative3,
			m2		= 0,
			m3		= 0,
			m4		= 0,
			k1		= 0,
			k2		= 0;
		Ogre::Vector2 point;

		for (int i=0; i<Iterative; ++i)
		{
			k1   =   (i   <<   1)   +   1;
			k2   =   (k1+i)*i   +   k1;  
			m4   +=  (k2   -=   (k1   *=   Iterative));   
			m3   +=  (k1   =   k2   -   k1)   +   Iterative2;   
			m2   -=  (k2   +=   k1);
			m1   +=   k2;

			point.x	= (int) (((long)x1*m1 + (long)x2*m2 + (long)xr1*m3 + (long)xr2*m4) / Iterative3);
			point.y = (int) (((long)y1*m1 + (long)y2*m2 + (long)yr1*m3 + (long)yr2*m4) / Iterative3);    

			if (i >= currIter)
				return point;
		}

		return point; 
	}

	//同HermiteCurve
	Ogre::Vector2 BulletFindTargetTester::BezierCurve(int x1, int y1, int x2, int y2, int xr1, int yr1, int xr2, int yr2, int currIter)
	{    
		return HermiteCurve(x1,y1,x2,y2,3*(xr1-x1),3*(yr1-y1),3*(x2-xr2),3*(y2-yr2), currIter);   
	}
	Ogre::Vector3 BulletFindTargetTester::Rotate(const Ogre::Vector3& pos, const Ogre::Vector3& axis, float angle )
	{
		const Ogre::Vector3			vector		= pos;	
		Ogre::Vector3				unitAxis	= axis;
		unitAxis.normalise();
		const float				halfAngle	= angle/float(2);
		const float				s			= sin(halfAngle);
		const float				c			= cos(halfAngle);
		const float				x			= unitAxis.x * s;
		const float				y			= unitAxis.y * s;
		const float				z			= unitAxis.z * s;
		const float				w			= c;
		const float				xx			= x*x;
		const float				xy 			= y*x;
		const float				xz			= z*x;
		const float				yy			= y*y;
		const float				yz			= z*y;
		const float				zz			= z*z;
		const float				wx			= w*x;
		const float				wy			= w*y;
		const float				wz			= w*z;
		const float				M[3][3]		=
		{
			{float(1)-float(2)*(yy+zz),		float(2)*(xy-wz),				float(2)*(xz+wy)},
			{float(2)*(xy+wz),				float(1)-float(2)*(xx+zz),      float(2)*(yz-wx)},
			{float(2)*(xz-wy),				float(2)*(yz+wx),				float(1)-float(2)*(xx+yy)},
		};

		return Ogre::Vector3( 
			vector.x*M[0][0] + vector.y*M[0][1] + vector.z*M[0][2],
			vector.x*M[1][0] + vector.y*M[1][1] + vector.z*M[1][2],
			vector.x*M[2][0] + vector.y*M[2][1] + vector.z*M[2][2] );
	}

	void BulletFindTargetTester::Line(Ogre::Vector3& vBulletPos,Ogre::Vector3 vTargetPoint,
		Ogre::Real time,Ogre::Real& fCumulateDistance)
	{
		float fDist = (vBulletPos - vTargetPoint).length();//逻辑子弹 到 目标的距离
		fDist += fCumulateDistance;	//从起始点到目标点的总飞行距离
		float fS = m_pursuitSpeed * time + m_fAcceleration * time * time / 2;	
		if (fS > fDist)
		{
			fS = fDist;
		}
		Ogre::Vector3 vDir = vTargetPoint - vBulletPos;
		vDir.normalise();//单位矢量
		Ogre::Vector3 vOffset = vDir * fS;
		vBulletPos += vOffset;

	}

		/*--------------------------------------------------
	曲线
	--------------------------------------------------*/
	void BulletFindTargetTester::Curve(Ogre::Vector3& basePos, Ogre::Vector3& vBulletPos,Ogre::Vector3 vTargetPos,
		Ogre::Real& fCumulateTime,Ogre::Real time,Ogre::Real& fCumulateDistance,float fAngle )
	{
		float fDist = basePos.distance(vTargetPos);//逻辑子弹 到 目标的距离
		fDist += fCumulateDistance;	//从起始点到目标点的总飞行距离

		//S=V0t+1/2at^2
		float t = fCumulateTime;
		float fS = m_pursuitSpeed * t + m_fAcceleration * t * t / 2;	
		if (fS > fDist)
		{
			fS = fDist;
		}
		//把移动距离换算成段数
		float iter = fS / fDist * (float)Iterative;
		
		int nDist = (int)(fDist/10.0f);//缩小距离，为了符合曲线计算范围的限制
		//得到曲线相对坐标
		float x1, x2, y1, y2;
		x1 = (float)nDist * m_fCurvePoint1X;
		y1 = (float)nDist * m_fCurvePoint1Y;
		x2 = (float)nDist * m_fCurvePoint2X;
		y2 = (float)nDist * m_fCurvePoint2Y;

		Ogre::Vector2 point = BezierCurve(  0,          0,			//1
											(int)nDist, 0,			//2		
											(int)x1,    (int)y1 ,	//3		
											(int)x2,    (int)y2,	//4		
											(int)iter );	


		Ogre::Vector3 vDir = vTargetPos - basePos;
		vDir.normalise();//单位矢量		
		

		//2d-3d变换	
		float pointX = (float)point.x*10.0f;
		float pointY = (float)point.y*10.0f;
		float pointZ = 0.0f;

		//绕Y轴转
		float angleY = _GetYAngle( Ogre::Vector2(vDir.x, vDir.z), Ogre::Vector2(0.0f, 0.0f));
		angleY = angleY + PI * 0.5f;

		float fx1 = pointZ * sin(angleY) + pointX * cos(angleY);
		float fy1 = pointY;
		float fz1 = pointZ * cos(angleY) - pointX * sin(angleY);

		//绕Z轴旋转
		float angleZ = _GetYAngle( Ogre::Vector2(vDir.y, vDir.x), Ogre::Vector2(0.0f, 0.0f));
		if (vDir.x > _MINFLOAT)
		{
			if (vDir.y > _MINFLOAT)
				angleZ = abs(PI - angleZ);
			else
				angleZ = PI + angleZ;
		}	

		float fx2 = fx1 * cos(angleZ) - fy1 * sin(angleZ);
		float fy2 = fx1 * sin(angleZ) + fy1 * cos(angleZ);
		float fz2 = fz1;

		//角度旋转变换
		Ogre::Vector3 fvPos = Rotate(Ogre::Vector3(fx2, fy2, fz2), vTargetPos - basePos, fAngle);

		//计算相对起始点	
		//Ogre::Vector3 vOffset = vDir * fS;
		//vBulletPos += (vOffset + fvPos);

		//计算相对起始点	
		Ogre::Vector3 fvStartPos;
		fvStartPos.x = vTargetPos.x - fDist * vDir.x;
		fvStartPos.y = vTargetPos.y - fDist * vDir.y;
		fvStartPos.z = vTargetPos.z - fDist * vDir.z;
		////平移到场景坐标
		Ogre::Vector3 fvCurrentPos;	
		fvCurrentPos.x = fvStartPos.x + fvPos.x;
		fvCurrentPos.y = fvStartPos.y + fvPos.y;
		fvCurrentPos.z = fvStartPos.z + fvPos.z;

		vBulletPos = fvCurrentPos;

		//float fDir = _GetYAngle( Ogre::Vector2( vBulletPos.x, vBulletPos.z ), 
		//							Ogre::Vector2( vTargetPos.x, vTargetPos.z ) );	
		//SetFaceDir( fDir );
		//m_vRotation.y = fDir;
		//Ogre::Quaternion qu(Ogre::Radian(m_vRotation.y), Ogre::Vector3::UNIT_Y);
		//m_transInfo.mRotation = qu;
			
	}
		/*--------------------------------------------------
	运算子弹位置，并根据 当前子弹位置 判断是否到达目的地
	--------------------------------------------------*/
	bool BulletFindTargetTester::IsArrive(Ogre::Vector3& vBulletPos,Ogre::Vector3 vTargetPos,Ogre::Real& fCumulateTime,
		Ogre::Real& fCumulateDistance,Ogre::Real time)
	{
		//vt=v0+at
		//前一桢结束速度
		float fSpeed = m_pursuitSpeed + m_fAcceleration * (fCumulateTime - time);

		//S=V0t+1/2at2	
		float fCurTickFlyDist = fSpeed * time + m_fAcceleration * time * time / 2;//本桢所飞行的距离

		fCumulateDistance += fCurTickFlyDist; //累计飞行距离

		Ogre::Vector3 vDir = vTargetPos - vBulletPos;
		vDir.normalise();
		Ogre::Vector3 vFlyLength;
		vFlyLength.x = vDir.x * fCurTickFlyDist;
		vFlyLength.y = vDir.y * fCurTickFlyDist;
		vFlyLength.z = vDir.z * fCurTickFlyDist;
		vBulletPos = vFlyLength + vBulletPos;	

		float fDir = _GetYAngle( Ogre::Vector2( vBulletPos.x, vBulletPos.z ), 
			Ogre::Vector2( vBulletPos.x, vBulletPos.z ) );
		//SetFaceDir( fDir );
		//m_vRotation.y = fDir;
		//Ogre::Quaternion qu(Ogre::Radian(m_vRotation.y), Ogre::Vector3::UNIT_Y);
		//m_transInfo.mRotation = qu;
		

		float fCurTickFlyDistSq = fCurTickFlyDist * fCurTickFlyDist;
		float fDistSq = vTargetPos.squaredDistance(vBulletPos);//当前位置到目标的直线距离

		// 接近目标点
		if( fDistSq <= fCurTickFlyDistSq )	
			return true;	
		else	
			return false;
		
		return false;
	}

	void BulletFindTargetTester::AlreadyHit()
	{
		//有下一个状态
		/*if (m_bActionAgain)
		{
			m_bActionAgain = false;

			if (m_pNextInitData)
			{
				//从新设置起始点

				tObject* obj = CObjectManager::GetMe()->FindServerObject(m_pNextInitData->m_nSendID);
				obj->GetRenderInterface()->Actor_GetLocator(GetCharaLocatorName(LOCATOR_CHAR_ATTACK), m_pNextInitData->m_fvPos);	// "人物身体受击点"


				//从新初始化
				Initial(m_pNextInitData);
			}
		}
		//没有下一个状态
		else
		{
			if(m_idTarget != INVALID_ID)
			{
				CObject_Character *pChar = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(m_idTarget));
				if(pChar != NULL)
				{
					pChar->ShowLogicEvent(m_idSend, m_nSendLogicCount, TRUE);
				}
			}

			m_bAlreadyHit = true;

			if( m_pBulletData != NULL && strlen( m_pBulletData->m_szHitEffect ) > 0 )
			{
				SetPosition( m_fvTargetPos );
				ChangEffect( m_pBulletData->m_szHitEffect, FALSE );		
			}
			else
			{
				if(m_pRenderInterface != NULL)
					m_pRenderInterface->Detach_Effect();
			}
		}*/
	}

		
	void BulletFindTargetTester::pursuitTargetAsPointingType()
	{
	}
	void BulletFindTargetTester::pursuitTargetAsBeelineType()
	{
	}
	void BulletFindTargetTester::pursuitTargetAsBizzardType()
	{
	}
	
} 