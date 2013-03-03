
/** ����/��Ч������
 */

#pragma once


#include "GISound.h"
#include <set>
#include "GMSoundSource.h"



class CSoundBufferFMod;
struct FSOUND_STREAM;
struct _DBC_SOUND_INFO;
class CSoundSourceFMod;
struct EVENT;
namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

//����ϵͳ
class CSoundSystemFMod : public tSoundSystem
{
public:

	//---------------------------------------------------------
	//������Դ

	//����һ��������Դ
	virtual	tSoundBuffer*		Buffer_Create(INT nID);
	virtual	tSoundBuffer*		Buffer_Create(LPCTSTR szFileName);

	//---------------------------------------------------------
	//����Դ

	//����/ɾ������Դ
	// 3Dģʽ			- ����������λ�ñ仯
	// �Զ�ɾ��ģʽ		- ֻ����һ��,Ȼ���Զ��ͷ�
	virtual	tSoundSource*		Source_Create(tSoundSource::Type type, BOOL b3DMode, BOOL bAutoDestroy);
	virtual	tSoundSource*		Source_Create(tSoundSource::Type type,  BOOL b3DMode, BOOL bAutoDestroy, tSoundSource** pSoundSource );

	virtual VOID				Source_Destroy(tSoundSource* pSource);
	virtual VOID				Source_Destroy(INT id);

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
	//�����ػ��������ڿ��Ƹ�����Ч�Ŀ���
	static VOID	WINAPI			_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);

	//---------------------------------------------------------
	//��Ӳ��ż��������ӿ�
	static int WINAPI 		_PlaySoundFunc(const char* szSoundFile, float* fvPos, bool bLoop) 
	{
		return GetMe()->PlaySoundFunc(szSoundFile, fvPos, bLoop);
	}
	static void WINAPI 		_StopSoundFunc(int nHandle) 
	{ 
		GetMe()->StopSoundFunc(nHandle); 
	}

	//UI���������ӿ�
	static void _PlayUISoundFunc(INT nID) { GetMe()->PlayUISoundFunc(nID); }
	virtual void PlayUISoundDirect(INT nID) { GetMe()->PlayUISoundFunc(nID); }

	//ĳ����������Ŀǰ�Ƿ�������
	BOOL	IsTypeEnabled(tSoundSource::Type type);
	float	GetTypeVolume(tSoundSource::Type type);

	//---------------------------------------------------------
	//���ڱ���ϵͳ�п�������ϵͳ�ı�����
	static const CHAR szAllSoundDisable[]; //�ر�������Ч�Ŀ���
	static const CHAR szBKSoundVar[];	//������Ч
	static const CHAR sz3DSoundVar[];	//������Ч
	static const CHAR szSKSoundVar[];	//������Ч
	static const CHAR szUISoundVar[];	//�¼���Ч

	static const CHAR szBKVolumeVar[];	//��������
	static const CHAR sz3DVolumeVar[];	//��������
	static const CHAR szSKVolumeVar[];	//��������
	static const CHAR szUIVolumeVar[];	//�¼�����

	//---------------------------------------------------------
	//���ڽű����õĺ���
public:
	//����һ�����������ա�������Ч������
	//Param1[int]	- ����id
	//Param2[loop]	- �Ƿ�ѭ������ѭ��������������Ϻ��Զ��ͷ�
	//return[int]	- �������
	INT Lua_PlaySound(LuaPlus::LuaState* state);
	//Param1[int]	- �������
	INT Lua_StopSound(LuaPlus::LuaState* state);

protected:
	LuaPlus::LuaObject* m_pMetaTable;

	//------------------------
	//���ļ̳�
	//------------------------
public:
	virtual VOID			Initial(VOID*);
	virtual VOID			Release(VOID);
	virtual VOID			Tick(VOID);

protected:
	INT						PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop);
	VOID					StopSoundFunc(INT nHandle);
	VOID					PlayUISoundFunc(INT nID );
	VOID					MuteAll(BOOL bMute);
	//ָֹͣ����������
	VOID					StopSpecialSounds(tSoundSource::Type type);
	
	//������ԴĿ¼
	STRING					m_strSoundPath;
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
		INT					id;				//id
		BOOL				bAutoDestroy;	//�Զ�����
		CSoundSourceFMod*	pSoundSource;	//����

		tSoundSource**		pSoundSourceReference;	// �ýڵ�������ⲿ����ָ��		20100714 BLL

		SOUND_SOURCE()
		{
			pSoundSource = NULL;
			pSoundSourceReference = NULL;
		}

	};
	std::list< SOUND_SOURCE >	m_listSoundSource;

protected:
	//������Դ������
	tSoundBuffer*		Source_Load(SOUND_BUFFER& sndBuffer);

public:
	CSoundSystemFMod();
	virtual ~CSoundSystemFMod();
	static CSoundSystemFMod* GetMe() { return s_pMe; }

protected:
	static CSoundSystemFMod* s_pMe;
	KL_DECLARE_DYNAMIC(CSoundSystemFMod);
};