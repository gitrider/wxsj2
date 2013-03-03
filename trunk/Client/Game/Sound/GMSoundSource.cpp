#include "StdAfx.h"
#include "GMSoundSource.h"
#include "GMSoundBuffer.h"
#include "GMSoundSystem.h"
#include "GIUtil.h"
#include "fmod375/fmod.h"
#include "fmod375/fmod_errors.h"
#include <assert.h>
#include <math.h>
#include "GIException.h"

CSoundSourceFMod::CSoundSourceFMod(INT id, Type type, BOOL b3DMode) 
{
	_ID = id;
	_Sample = 0;
	_State = source_stoped;

	_Loop = TRUE;
	_b3DMode = b3DMode;
	_Alpha = 0.0;
	_Pos= fVector3(0.0f, 0.0f, 0.0f);

	_FModChannel= -1;
	_nType = type;
}

CSoundSourceFMod::~CSoundSourceFMod()
{
	//���ٶ���Դ������
	if(_Sample)
	{
		((CSoundBufferFMod*)_Sample)->DecRefrence(this);
	}

	Stop();
}

void CSoundSourceFMod::SetBuffer(tSoundBuffer *buffer)
{
	KLAssert(buffer);

	if(_Sample)
	{
		((CSoundBufferFMod*)_Sample)->DecRefrence(this);
	}

	_Sample = buffer;
	((CSoundBufferFMod*)_Sample)->AddRefrence(this);
}

tSoundBuffer *CSoundSourceFMod::GetBuffer(VOID)
{
	return _Sample;
}

void CSoundSourceFMod::SetLooping( BOOL loop )
{
	if(_Loop!=loop)
	{
		_Loop = loop;
		if(_FModChannel!=-1)
		{
			//������ͣ
			FSOUND_SetPaused(_FModChannel, true);
			FSOUND_SetLoopMode(_FModChannel, _Loop?FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF);
			FSOUND_SetPaused(_FModChannel, false);
		}
	}
}

VOID CSoundSourceFMod::Play(VOID)
{
	//ֹͣ
	Stop();

	//����
	if(_Sample && CSoundSystemFMod::GetMe()->IsTypeEnabled(GetType()))
	{
		CSoundBufferFMod* bufFMod= static_cast<CSoundBufferFMod*>(_Sample);

		_State= source_playing;
		//����ͣģʽ��ʼ��������
		if(bufFMod->m_pFModSample)
			_FModChannel= FSOUND_PlaySoundEx(FSOUND_FREE, bufFMod->m_pFModSample, NULL, true);

		//��Channel����
		if(_FModChannel!=-1)
		{
			FSOUND_SetLoopMode(_FModChannel, _Loop?FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF);
			//����
			UpdateVolume();
			//��ʼ����
			FSOUND_SetPaused(_FModChannel, false);
		}
	}
}

void CSoundSourceFMod::Stop()
{
	_State= source_stoped;

	// Stop the FMod channel
	if(_FModChannel!=-1)
	{
		FSOUND_StopSound(_FModChannel);
		_FModChannel= -1;
	}
}

BOOL CSoundSourceFMod::IsPlaying(VOID) const
{
	return _State == source_playing;
}

BOOL CSoundSourceFMod::IsStopped(VOID) const
{
	return _State == source_silencing || _State == source_stoped;
}

void CSoundSourceFMod::SetPos( const fVector3& pos )
{
	_Pos = pos;

	//3dģʽ
	if(_b3DMode)
	{
		UpdateVolume();
	}
}

void CSoundSourceFMod::UpdateVolume()
{
	// only if channel active
	if(_FModChannel==-1)
		return;

	float fGain = CSoundSystemFMod::GetMe()->GetTypeVolume(GetType());

	//3Dģʽ
	if(_b3DMode)
	{
		//�õ������߾���ƽ��
		FLOAT fDistanceSQR = KLU_GetDistSq(GetPos(), CSoundSystemFMod::GetMe()->Listener_GetPos());

		//���������ֱ�
		INT  nVolumeDB= INT(floor(2000.0 * log10(fGain))); // convert to 1/100th decibels
		const static INT s_nDBMin= -10000;
		const static INT s_nDBMax= 0;
		KLU_Clamp(nVolumeDB, s_nDBMin, s_nDBMax);

		//���ݾ����������
		nVolumeDB= _ComputeManualRollOff(nVolumeDB, s_nDBMin, s_nDBMax, _Alpha, fDistanceSQR);
		
		//��������ֵ
		DOUBLE	dAttGain= pow((DOUBLE)10.0, DOUBLE(nVolumeDB)/2000.0);
		KLU_Clamp(dAttGain, 0.0f, 1.0f);

		FSOUND_SetVolume(_FModChannel, UINT(dAttGain*255));
	}
	else
	{
		FSOUND_SetVolume(_FModChannel, UINT(fGain*255));
	}
}

VOID CSoundSourceFMod::UpdateStatus(VOID)
{
	if(_FModChannel!=-1 && IsPlaying())
	{
		//�������
		if(!FSOUND_IsPlaying(_FModChannel))
		{
			_State= source_silencing;
		}
	}
}

INT CSoundSourceFMod::_ComputeManualRollOff(INT volumeDB, INT dbMin, INT dbMax, DOUBLE alpha, FLOAT sqrdist) const
{
	FLOAT min = 0.0;
	FLOAT max = 40.0f;
	
	if (sqrdist < min * min) 
	{
		// no attenuation
		return volumeDB;
	}
	else if (sqrdist > max * max)
	{
		// full attenuation
		return dbMin;
	}
	else
	{
		DOUBLE dist = (DOUBLE) sqrt(sqrdist);
		
		// linearly descending volume on a dB scale
		DOUBLE db1 = dbMin * (dist - min) / (max - min);
		
		if (alpha == 0.0) {
			volumeDB += (INT) db1;
			
		} else if (alpha > 0.0) {
			DOUBLE amp2 = 0.0001 + 0.9999 * (max - dist) / (max - min); // linear amp between 0.00001 and 1.0
			DOUBLE db2 = 2000.0 * log10(amp2); // convert to 1/100th decibels
			volumeDB += (INT) ((1.0 - alpha) * db1 + alpha * db2);
			
		} else if (alpha < 0.0) {
			DOUBLE amp3 = min / dist; // linear amplitude is 1/distance
			DOUBLE db3 = 2000.0 * log10(amp3); // convert to 1/100th decibels
			volumeDB += (INT) ((1.0 + alpha) * db1 - alpha * db3);
		}
		
		KLU_Clamp(volumeDB, dbMin, dbMax);
		
		return volumeDB;
	}
}