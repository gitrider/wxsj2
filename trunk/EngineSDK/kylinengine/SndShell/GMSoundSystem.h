/****************************************\
*										*
* 			����/��Ч������				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once
#include <set>
#include "fmod374/fmod.h"
#include "fmod374/fmod_errors.h"
#include "SndShell.h"

class CSoundBufferFMod;
struct FSOUND_STREAM;
struct _DBC_SOUND_INFO;
class CSoundSourceFMod;

//����ϵͳ
class CSoundSystemFMod
{
public:
	//---------------------------------------------------------
	//������Դ

	//����һ��������Դ
	virtual	CSoundBufferFMod*		Buffer_Create(INT nID);
	virtual	CSoundBufferFMod*		Buffer_Create(LPCTSTR szFileName);

	//---------------------------------------------------------
	//����Դ

	//����/ɾ������Դ
	// 3Dģʽ			- ����������λ�ñ仯
	// �Զ�ɾ��ģʽ		- ֻ����һ��,Ȼ���Զ��ͷ�
	virtual	CSoundSourceFMod*		Source_Create(BOOL b3DMode, BOOL bAutoDestroy);
	virtual VOID				Source_Destroy(CSoundSourceFMod* pSource);

	//---------------------------------------------------------
	//������

	//��������λ��
	virtual VOID				Listener_SetPos(const fVector3& fvPos) { m_vListenerPos = fvPos; }
	virtual const fVector3&		Listener_GetPos(VOID) const { return m_vListenerPos; }

	//---------------------------------------------------------
	//�����ṩ���������Žӿ�
	virtual FUNC_PLAYSOUND		GetPlaySoundFunc(VOID) { return _PlaySoundFunc; }
	virtual FUNC_STOPSOUND		GetStopSoundFunc(VOID) { return _StopSoundFunc; }

	//---------------------------------------------------------
	//�õ����п��������ļ���
	BOOL		EnumSoundFile(INT nIndex, STRING& strFileName, int& id);

	//------------------------
	//���ļ̳�
	//------------------------
public:
	virtual VOID			Initial(LPCTSTR szSndPath, LPCTSTR szDBCFile);
	virtual VOID			Release(VOID);
	virtual VOID			Tick(VOID);
	//���������ӿ�
	static int WINAPI 		_PlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop);
	static void WINAPI 		_StopSoundFunc(int nHandle);
    static void WINAPI 		SetSoundVolume(INT nHandle, FLOAT gain);


protected:
	INT						PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop);
	VOID					StopSoundFunc(INT nHandle);
	VOID					MuteAll(BOOL bMute);
	
	//������ԴĿ¼
	STRING					m_strSoundPath;
	//���������ļ�
	STRING					m_strSndDBCFile;
	//������ [0.0, 1.0]
	FLOAT					m_fMasterGain;
	//����λ��
	fVector3				m_vListenerPos;
	//ȫ������״̬
	BOOL					m_bMuteAll;

	//------------------------------------------
	//������Դ
	struct SOUND_BUFFER
	{
		const _DBC_SOUND_INFO*		pDefine;
		//�Ѿ����ص���Դ,���Ϊ�ձ�ʾ��δ����
		CSoundBufferFMod*			pSoundBuf;
	};
    //����������Դ�б�
	std::list< SOUND_BUFFER >			m_listSoundBuffer;
	//����ID����
	std::map< INT, SOUND_BUFFER* >		m_mapSoundID;
	//�����ļ�������
	std::map< STRING, SOUND_BUFFER* >	m_mapSoundFile;

	//------------------------------------------
	//���ڲ��ŵ�����Դ
	struct SOUND_SOURCE
	{
		BOOL				bAutoDestroy;	//�Զ�����
		CSoundSourceFMod*	pSoundSource;	//����
	};
	std::list< SOUND_SOURCE >	m_listSoundSource;

protected:
	//������Դ������
	CSoundBufferFMod*		Source_Load(SOUND_BUFFER& sndBuffer);

public:
	CSoundSystemFMod();
	virtual ~CSoundSystemFMod();
	static CSoundSystemFMod* GetMe() { return s_pMe; }

protected:
	static CSoundSystemFMod* s_pMe;
};