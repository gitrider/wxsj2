
/** ������Դ
 */

#pragma once


#include "GISound.h"



struct FSOUND_SAMPLE;
class CSoundSourceFMod;

class CSoundBufferFMod : public tSoundBuffer
{
	friend class CSoundSystemFMod;
public:

	//��ȡwav��Դ
	virtual BOOL		ReadWavBuffer(const STRING& name);
	//�ͷ���Դ
	virtual VOID		Release(VOID);
	//���Ӽ���
	void				AddRefrence(CSoundSourceFMod* pSource) {m_nRefCount++;}
	//���ټ���
	void				DecRefrence(CSoundSourceFMod* pSource) {m_nRefCount--;}
	//ȡ�ü���
	INT					GetRefrence(void) const { return m_nRefCount; }

protected:

	CSoundBufferFMod();
	virtual ~CSoundBufferFMod();

protected:

	friend class CSoundSourceFMod;

	/// FMODʵ��ָ��
	FSOUND_SAMPLE*		m_pFModSample;

	/// ���ü���
	INT					m_nRefCount;

};