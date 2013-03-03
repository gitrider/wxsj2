
/** ����/��Ч������
 */

#pragma once


#include "GIBasicType.h"
#include "GINode.h"
#include "GIMath.h"




/** ������Դ�ӿ�
 */
class KYLIN_API tSoundBuffer
{
public:

	//�����ļ�
	virtual BOOL ReadWavBuffer(const STRING& name) = 0;
	//�ͷ���Դ
	virtual VOID Release(VOID) = 0;

};	// class


// �����ṩ���������Žӿ�,��������ԴHANDLE
typedef int (__stdcall* FUNC_PLAYSOUND)(const char* szSoundFile, float* fvPos, bool bLoop);
// �����ṩ������ֹͣ�ӿ�
typedef VOID (__stdcall* FUNC_STOPSOUND)(int);


/** �����������߽ӿ�
 */
class KYLIN_API tSoundSource
{
public:

	//�����߼�����
	enum Type
	{
		type_background,		//������
		type_skill,				//������
		type_circumstance,		//������
		type_UI,				//UI��
	};

public:

	//����id
	virtual INT				GetID(void) const = 0;

	//��ʹ�õ�����Դ
	/*
	|	!!!����ڲ��������ø���򲻻�������Ч��
	|��Ҫִ��Stop(); Play()��Ż���Ч
	|
	*/
	virtual VOID			SetBuffer(tSoundBuffer *buffer) = 0;
	virtual tSoundBuffer*	GetBuffer(VOID) = 0;

    //�Ƿ�ѭ������
	virtual VOID			SetLooping(BOOL loop) = 0;
	virtual BOOL			GetLooping(VOID) const = 0;

    //����
	virtual VOID			Play(VOID) = 0;
    //ֹͣ����
	virtual VOID			Stop(VOID) = 0;
    //�Ƿ��ڲ�����
	virtual BOOL			IsPlaying(VOID) const = 0;
    //�Ƿ��Ѿ�ֹͣ
	virtual BOOL			IsStopped(VOID) const = 0;

	//����Դλ��(default(0,0,0))
	virtual VOID			SetPos(const fVector3& pos) = 0;
	virtual const fVector3& GetPos(VOID) = 0;
	//��������
	virtual VOID			UpdateVolume(VOID) = 0;

};	// class


/** ��������ϵͳ
 */
class KYLIN_API tSoundSystem : public tNode
{
public:

	//---------------------------------------------------------
	//������Դ

	//����һ��������Դ
	virtual	tSoundBuffer* Buffer_Create(INT nID) = 0;

	//---------------------------------------------------------
	//����Դ

	//����/ɾ������Դ
	// 3Dģʽ - ����������λ�ñ仯
	// �Զ�ɾ��ģʽ		- ֻ����һ��,Ȼ���Զ��ͷ�
	virtual	tSoundSource* Source_Create(tSoundSource::Type nType, BOOL b3DMode, BOOL bAutoDestroy) = 0;
	virtual	tSoundSource* Source_Create(tSoundSource::Type nType, BOOL b3DMode, BOOL bAutoDestroy, tSoundSource** pSoundSourceReference ) = 0;

	virtual VOID Source_Destroy(tSoundSource* pSource) = 0;

	virtual void PlayUISoundDirect(INT id) =0;

	//---------------------------------------------------------
	//������

	//��������λ��
	virtual VOID Listener_SetPos(const fVector3& fvPos)	= 0;
	virtual const fVector3& Listener_GetPos(VOID) const = 0;

	//---------------------------------------------------------
	//�����ṩ�������ӿ�
	virtual FUNC_PLAYSOUND GetPlaySoundFunc(VOID) = 0;
	virtual FUNC_STOPSOUND GetStopSoundFunc(VOID) = 0;


	KL_DECLARE_DYNAMIC(tSoundSystem);

};	// class
