
/** ��������Դ
 */

#pragma once


#include "GISound.h"



class CSoundSourceFMod : public tSoundSource
{
	friend class CSoundSystemFMod;

public:

	//����id
	virtual INT				GetID(void) const { return _ID; }
 	//��ʹ�õ�����Դ
	/*
	|	!!!����ڲ��������ø���򲻻�������Ч��
	|��Ҫִ��Stop(); Play()��Ż���Ч
	|
	*/
	virtual VOID			SetBuffer(tSoundBuffer *buffer);
	virtual tSoundBuffer*	GetBuffer(VOID);

    //�Ƿ�ѭ������
	virtual VOID			SetLooping(BOOL loop);
	virtual BOOL			GetLooping(VOID) const { return _Loop; }

    //����
	virtual VOID			Play(VOID);
    //ֹͣ����
	virtual VOID			Stop(VOID);
    //�Ƿ��ڲ�����
	virtual BOOL			IsPlaying(VOID) const;
    //�Ƿ��Ѿ�ֹͣ
	virtual BOOL			IsStopped(VOID) const;

	//����Դλ��(default(0,0,0))
	virtual VOID			SetPos(const fVector3& pos);
	virtual const fVector3& GetPos(VOID) { return _Pos; }

	//��������
	virtual VOID			UpdateVolume(VOID);
	//����״̬
	virtual VOID			UpdateStatus(VOID);

	//��������
	virtual Type			GetType(void)const		{return _nType;}

protected:

	CSoundSourceFMod(INT id, Type type, BOOL b3DMode);
	virtual ~CSoundSourceFMod();

	//�õ�Channel
	INT		GetFModChannel(VOID) const { return _FModChannel; }

	//����Դ����([0, 1], default:1)
	VOID			_SetGain(FLOAT gain);

private:

	enum TSourceState
	{
		source_stoped,			//ֹͣ��
		source_playing,			//���ڲ���
		source_silencing,		//�������
	};
	TSourceState			_State;

	INT						_ID;
	tSoundBuffer			*_Sample;
	INT						_FModChannel;
	
    // States
	BOOL					_b3DMode;	//TRUE- 3Dģʽ,		 FALSE - 2Dģʽ
    BOOL					_Loop;
	DOUBLE					_Alpha;
	fVector3				_Pos;

	// Type
	Type					_nType;

protected:

	//���ݾ����������
	INT		_ComputeManualRollOff(INT volumeDB, INT dbMin, INT dbMax, DOUBLE alpha, FLOAT sqrdist) const;

};