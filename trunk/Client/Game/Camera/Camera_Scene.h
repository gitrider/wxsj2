
/**
	��Ϸ�����е������
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
	// �ƶ�����ע��λ�õ�...Pitch
	// �������Ĳ���Ϊ����ڵ��ε����꣬����Ϊ��λ
	// һ����˵�����λ��������Լ���ͷ��
	virtual VOID		SetLookAt(const fVector3& fvPos);
	
	//-------------------------------------------------
	// ������۾�����ע��λ�õľ���, ����Ϊ��λ
	virtual VOID		SetAddHeight(FLOAT fAddHeight);
		   FLOAT		GetAddHeight(VOID) const { return m_fAddHeight; }

	//-------------------------------------------------
	// �����������ˮƽ�ߵļнǣ�һ��Ϊ����(���¿�)
	static FLOAT		STATIC_PITCH;		// �̶��ӽ�
	static FLOAT		APPROCH_PITCH;		// �ӽ���������Զʱ���ƵĽǶ�

	virtual VOID		SetPitch(FLOAT fPitch);
	virtual VOID		SetApprochPitch( FLOAT fPitch );
	
	//-------------------------------------------------
	// ����������۾�������֮��ľ���
	static FLOAT		MIN_DISTANCE;	// ���������������ܹ���������������
	static FLOAT		MAX_DISTANCE;	// ���������������ܹ�Զ���������Զ����

	virtual VOID		SetDistance(FLOAT fDistance);
	virtual FLOAT		GetDistance(VOID) const;
	//-------------------------------------------------
	// ���û���������������x��ĽǶ� Should[0, 2��)
	virtual VOID		SetDirection(FLOAT fDirection);

	//-------------------------------------------------
	// ���ü���һ����
			VOID		AddOneVib(FLOAT fAmplitude, FLOAT fDuration, UINT fFrequency);



public:

	//��������캯��
	// pOgreCamera  - �����ӵ���ogre�����
	/*
	|
	|	!�����������ʼ��
	|
	|		o �����һ�ν��볡��ʱ���泯x����ָ��ķ���
	|		o �������������ı���ע�������ͷ��
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

	void SetOffset( FLOAT fOffset )	{ m_fOffset = fOffset * 100; }	// �׵����׵�ת��

	static EffectCameraShakeCallback s_EffectCameraShakeCallback;

protected:
	//-------------------------------------------------
	//�����״̬
	enum CAMERA_STATUS
	{
		NORMAL_STATUS = 0,	//��ͨ״̬
		VIBRATION_STATUS,	//��״̬
	};
	//-------------------------------------------------
	//��״̬�����ṹ
	struct VibParam_t
	{
		FLOAT	fAmplitude;	//���
		FLOAT	fDuration;	//����ʱ��
		FLOAT	fFrequency; //Ƶ��
		FLOAT   fAge;       //����ʱ��

		VibParam_t()
		{
			fAmplitude = 0.0;
			fDuration = 0.0;
			fFrequency = 0.0;
            fAge = 0.0;
		}
	};

protected:

	/// ���������λ�þ������ĸ߶ȣ�һ��������ĸ߶�, ����Ϊ��λ
	FLOAT				m_fAddHeight;
	
	/// �������ǰ״̬
	CAMERA_STATUS		m_status;

	/// ��ǰ�𵴲���
	VibParam_t			m_CurVibParam;

	FLOAT				m_fOffset;

protected:

	//�����۾�λ��
	static fVector3		_GetEyePos(
							const fVector3& fvLookAt,
							FLOAT fDistance,
							FLOAT fDirection,
							FLOAT fPitch);

	//-------------------------------------------------
	//������ײ���

	//��С�������
	static FLOAT MIN_ANGLE;
	//��С������Χ
	static FLOAT STEP_ANGLE;

	//������۲��Ľ�����ײ��
	struct CAMERA_NEAR_BALL
	{
		Ogre::Sphere sphere;	
	};

	//��������۾��н����ĵ�������(���4������)
	struct CAMERA_INTER_GRIDS
	{
		std::vector< Ogre::AxisAlignedBox >	m_fvGrid;
	};

	//������ײ�õ������Pitch
	static FLOAT		_GetValidPitch(
							const Fairy::TerrainData* pTerrainData,	//��������
							const fVector3& fvLookAt,				//��ע��
							FLOAT fDistance,						//����
							FLOAT fDirection,						//��ת�Ƕ�
							FLOAT fPitchNow,						//Ŀǰ��Pitch(�����ӽ���ֵ)
							FLOAT fPitchApproch,					//Ҫ������Pitch
							FLOAT fCameraNearDis);					//�۾��Ľ��ü�����

	//�۾��Ƿ����
	static BOOL		_IsValidEye(
							const Fairy::TerrainData* pTerrainData,	//��������
							const fVector3& fvEye,					//�۾���
							FLOAT fCameraNearDis);					//������

	//ȡ�þ����۾�����ĵ�������
	static BOOL		_GetInterPatch(
							const Fairy::TerrainData* pTerrainData,	//��������
							const fVector3& fvEyePos,				//�۾���ͶӰλ��
							CAMERA_INTER_GRIDS& theInterPatch);		//��������

	//���õ�ǰ�����״̬
	BOOL			_SetStatus(CAMERA_STATUS nStatus );				

	//���ݵ�ǰ״̬ˢ�������
	VOID			_UpdateCurStatus(
							fVector3& vEyePos, 
							fVector3& vLookatPos);							


};
