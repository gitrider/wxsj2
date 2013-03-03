/****************************************\
*										*
* 			��������Դ					*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

class CSoundBufferFMod;
class CSoundSourceFMod
{
public:
 	//��ʹ�õ�����Դ
	/*
	|	!!!����ڲ��������ø���򲻻�������Ч��
	|��Ҫִ��Stop(); Play()��Ż���Ч
	|
	*/
	virtual VOID				SetBuffer(CSoundBufferFMod *buffer);
	virtual CSoundBufferFMod*	GetBuffer(VOID);

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

	//����Դ����([0, 1], default:1)
	virtual VOID			SetGain(FLOAT gain);
	virtual FLOAT			GetGain(VOID) const { return _Gain; }

	//��������
	virtual VOID			UpdateVolume(VOID);
	//����״̬
	virtual VOID			UpdateStatus(VOID);

public:
	CSoundSourceFMod(BOOL b3DMode);
	virtual ~CSoundSourceFMod();

	//�õ�Channel
	INT		GetFModChannel(VOID) const { return _FModChannel; }

private:
	enum TSourceState
	{
		source_stoped,			//ֹͣ��
		source_playing,			//���ڲ���
		source_silencing,		//�������
	};
	TSourceState			_State;

	CSoundBufferFMod			*_Sample;
	INT						_FModChannel;
	
    // States
	BOOL					_b3DMode;	//TRUE- 3Dģʽ,		 FALSE - 2Dģʽ
    BOOL					_Loop;
	FLOAT					_Gain;
	double					_Alpha;
	fVector3				_Pos;

protected:
	//���ݾ����������
	INT		_ComputeManualRollOff(INT volumeDB, INT dbMin, INT dbMax, double alpha, FLOAT sqrdist) const;

};