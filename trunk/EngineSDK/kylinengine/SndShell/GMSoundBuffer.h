/****************************************\
*										*
* 				������Դ				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

struct FSOUND_SAMPLE;
class CSoundBufferFMod
{
public:
	//��ȡwav��Դ
	virtual BOOL		ReadWavBuffer(const STRING& name);
	//�ͷ���Դ
	virtual VOID		Release(VOID);

public:
	CSoundBufferFMod();
	virtual ~CSoundBufferFMod();

protected:
	friend class CSoundSourceFMod;

	//FMODʵ��ָ��
	FSOUND_SAMPLE*		m_pFModSample;
};