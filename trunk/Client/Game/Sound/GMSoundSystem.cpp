
/** ����/��Ч������
 */


//
//ref NEL(R) Copyright, 2001 Nevrax Ltd.
//
#include "StdAfx.h"
#include "GMSoundSystem.h"
#include "GMSoundBuffer.h"
#include "GMSoundSource.h"
#include "DBC/GMDataBase.h"
#include "GIException.h"
#include "Procedure/GameProcedure.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "fmod375/fmod.h"
#include "fmod375/fmod_errors.h"
#include "Procedure\GameProcedure.h"
#include "Variable\GMVariable.h"
#include "Event\GMEventSystem.h"
#include "Script\GMScriptSystem.h"


#pragma comment(lib, "../game/Sound/fmod375/fmodvc.lib")




const CHAR CSoundSystemFMod::szAllSoundDisable[] = "DisableAllSound"; //�ر�������Ч�Ŀ���
const CHAR CSoundSystemFMod::szBKSoundVar[] = "EnableBGSound";	//������Ч
const CHAR CSoundSystemFMod::sz3DSoundVar[] = "Enable3DSound";	//������Ч
const CHAR CSoundSystemFMod::szSKSoundVar[] = "EnableSKSound";	//������Ч
const CHAR CSoundSystemFMod::szUISoundVar[] = "EnableUISound";	//�¼���Ч

const CHAR CSoundSystemFMod::szBKVolumeVar[] = "VOLUME_BG";	//��������
const CHAR CSoundSystemFMod::sz3DVolumeVar[] = "VOLUME_3D";	//��������
const CHAR CSoundSystemFMod::szSKVolumeVar[] = "VOLUME_SK";	//��������
const CHAR CSoundSystemFMod::szUIVolumeVar[] = "VOLUME_UI";	//�¼�����



KL_IMPLEMENT_DYNAMIC(CSoundSystemFMod, GETCLASS(tSoundSystem));

CSoundSystemFMod* CSoundSystemFMod::s_pMe = NULL;



CSoundSystemFMod::CSoundSystemFMod()
{
	s_pMe = this;

	m_bMuteAll = FALSE;
	m_fMasterGain = 1.0f;
	m_vListenerPos = fVector3(0.0f, 0.0f, 0.0f);

	m_strSoundPath = "..\\Sound\\";

	m_pMetaTable = 0;
}

CSoundSystemFMod::~CSoundSystemFMod()
{
	if(m_pMetaTable) delete m_pMetaTable;
	m_pMetaTable = 0;
}


VOID CSoundSystemFMod::Initial(VOID*)
{
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
		KLThrow("FMOD: You are using the wrong DLL version! Should be %.2f", FMOD_VERSION);
    }

	FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
	FSOUND_SetDriver(0);
    FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
	FSOUND_3D_SetRolloffFactor(0);

	if(!FSOUND_Init(44100, 32, 0))
	{
		KLThrow("Failed to create the FMod driver object");
	}

	// ��ѯ��Դ����
	DBC_DEFINEHANDLE(s_dbcSound, DBC_SOUND_INFO);
	INT nLineNum = (INT)s_dbcSound->GetRecordsNum();
	for(INT i=0; i<nLineNum; i++)
	{
		const _DBC_SOUND_INFO* pDefine = (const _DBC_SOUND_INFO*)s_dbcSound->Search_LineNum_EQU(i);
		if(!pDefine) continue;

		SOUND_BUFFER newSound;
		newSound.pDefine = pDefine;
		newSound.pSoundBuf = NULL;

		// �����б�
		m_listSoundBuffer.push_back(newSound);

		// ��������
		m_mapSoundID[pDefine->nID] = &(m_listSoundBuffer.back());
		m_mapSoundFile[pDefine->pSoundFile] = &(m_listSoundBuffer.back());
	}

	// �ҽӱ���ϵͳ
	CEventSystem::GetMe()->RegisterEventHandle("VARIABLE_CHANGED", _OnVariableChangedEvent);

	// �������δ�����������ֵ
	CVariableSystem::GetMe()->SetVariableDefault(szBKSoundVar, "1");
	CVariableSystem::GetMe()->SetVariableDefault(sz3DSoundVar, "1");
	CVariableSystem::GetMe()->SetVariableDefault(szSKSoundVar, "1");
	CVariableSystem::GetMe()->SetVariableDefault(szUISoundVar, "1");

	//-----------------------------------------------------------
	// ��������
	m_pMetaTable = new LuaPlus::LuaObject;
	*m_pMetaTable = CScriptSystem::GetMe()->GetLuaState()->GetGlobals().CreateTable("Sound");
	m_pMetaTable->SetObject("__index", *m_pMetaTable);

	m_pMetaTable->RegisterObjectFunctor("PlaySound", &CSoundSystemFMod::Lua_PlaySound);
	m_pMetaTable->RegisterObjectFunctor("StopSound", &CSoundSystemFMod::Lua_StopSound);

	LuaObject obj_Variable = CScriptSystem::GetMe()->GetLuaState()->BoxPointer(this);
	obj_Variable.SetMetaTable(*m_pMetaTable);
	CScriptSystem::GetMe()->GetLuaState()->GetGlobals().SetObject("Sound", obj_Variable);
}

VOID CSoundSystemFMod::Release(VOID)
{
	//---------------------------------
	// �ͷ���������Դ
	std::list< SOUND_SOURCE >::iterator itSndSource;
	for(itSndSource=m_listSoundSource.begin(); itSndSource!=m_listSoundSource.end(); itSndSource++)
	{
		SOUND_SOURCE& sndSource = *itSndSource;

		if(sndSource.pSoundSource)
		{
			delete sndSource.pSoundSource;
			sndSource.pSoundSource = NULL;

			// ����ýڵ�������ⲿ���õ�ָ�룬��ͬʱ���ⲿ����ָ��ֵҲ�ÿ�		20100714 BLL
			if ( sndSource.pSoundSourceReference != NULL )
			{
				*( sndSource.pSoundSourceReference ) = NULL; 
			}			
		}
	}
	m_listSoundSource.clear();

	//---------------------------------
	// �ͷ�����������Դ
	std::list< SOUND_BUFFER >::iterator itSndBuffer;
	for(itSndBuffer = m_listSoundBuffer.begin(); itSndBuffer!=m_listSoundBuffer.end(); itSndBuffer++)
	{
		SOUND_BUFFER& sndBuffer= *itSndBuffer;

		// �ͷ�������Դ
		if(sndBuffer.pSoundBuf)
		{
			delete sndBuffer.pSoundBuf;
			sndBuffer.pSoundBuf = NULL;
		}
	}

	// �ͷ��б�
	m_listSoundBuffer.clear();
	m_mapSoundID.clear();
	m_mapSoundFile.clear();

	// �ͷ�FMod
	FSOUND_Close();
}

BOOL CSoundSystemFMod::IsTypeEnabled(tSoundSource::Type type)
{
	CVariableSystem* pVarSystem = CVariableSystem::GetMe();
	bool bRet = false;
	switch(type)
	{
	case tSoundSource::type_background:
		bRet = (pVarSystem->GetAs_Int(szBKSoundVar)==1);
		break;

	case tSoundSource::type_circumstance:
		bRet = (pVarSystem->GetAs_Int(sz3DSoundVar)==1);
		break;

	case tSoundSource::type_skill:
		bRet = (pVarSystem->GetAs_Int(szSKSoundVar)==1);
		break;

	case tSoundSource::type_UI:
		bRet = (pVarSystem->GetAs_Int(szUISoundVar)==1);
		break;

	default:
		bRet = false;
		break;
	}

	return bRet;
}

float CSoundSystemFMod::GetTypeVolume(tSoundSource::Type type)
{
	if(m_bMuteAll) 
		return 0.0f;

	CVariableSystem* pVarSystem = CVariableSystem::GetMe();
	float gain = 0.0f;
	switch(type)
	{
	case tSoundSource::type_background:
		gain = pVarSystem->GetAs_Float(szBKVolumeVar);
		break;

	case tSoundSource::type_circumstance:
		gain = pVarSystem->GetAs_Float(sz3DVolumeVar);
		break;

	case tSoundSource::type_skill:
		gain = pVarSystem->GetAs_Float(szSKVolumeVar);
		break;

	case tSoundSource::type_UI:
		gain = pVarSystem->GetAs_Float(szUIVolumeVar);
		break;

	default:
		gain = 0.0f;
		break;
	}

	return gain;
}

VOID CSoundSystemFMod::Tick(VOID)
{
	//------------------------------------
	// ˢ������λ��
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(pMySelf)
	{
		Listener_SetPos(pMySelf->GetPosition());
	}

	//------------------------------------
	// ��������Դ����
	{
		std::list< SOUND_SOURCE >::iterator it, itNext;
		for(it = m_listSoundSource.begin(); it!=m_listSoundSource.end(); it=itNext)
		{
			itNext = it; itNext++;

			SOUND_SOURCE& theSnd = *it;

			// ������Զ����ٵ�����Դ,�Զ�����
			theSnd.pSoundSource->UpdateStatus();

			if(theSnd.bAutoDestroy && theSnd.pSoundSource->IsStopped())
			{
				theSnd.pSoundSource->Stop();
				delete theSnd.pSoundSource;

				// ����ýڵ�������ⲿ���õ�ָ�룬��ͬʱ���ⲿ����ָ��ֵҲ�ÿ�				20100714 BLL
				if ( it->pSoundSourceReference != NULL )
				{
					*( it->pSoundSourceReference ) = NULL; 
				}	

				m_listSoundSource.erase(it);
				continue;
			}

			// ��������
			theSnd.pSoundSource->UpdateVolume();
		}
	}

	//------------------------------------
	// ����û�б����õ�������Դ
	{
		std::list< SOUND_BUFFER >::iterator it;

		for(it = m_listSoundBuffer.begin(); it!=m_listSoundBuffer.end(); it++)
		{
			SOUND_BUFFER& theBuf = *it;

			//������ü���Ϊ0������
			if(theBuf.pSoundBuf && theBuf.pSoundBuf->GetRefrence() == 0)
			{
				delete theBuf.pSoundBuf;
				theBuf.pSoundBuf = 0;
			}
		}
	}

	//------------------------------------
	// ������ڷǼ������
	if(!CGameProcedure::IsWindowActive() && !m_bMuteAll)
	{
		MuteAll(TRUE);
	}
	else if(CGameProcedure::IsWindowActive() && m_bMuteAll)
	{
		MuteAll(FALSE);
	}
}

// �����ڲ�����ʱ����������������
VOID CSoundSystemFMod::MuteAll(BOOL bMute)
{
	if(bMute == m_bMuteAll) return;

	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& sndSource = *it;

		if(sndSource.pSoundSource && sndSource.pSoundSource->GetFModChannel() >= 0)
			FSOUND_SetMute(sndSource.pSoundSource->GetFModChannel(), bMute);
	}
	m_bMuteAll = bMute;
}

// ����һ��������Դ
tSoundBuffer* CSoundSystemFMod::Buffer_Create(INT nID)
{
	if( nID <= 0 ) return NULL;
	std::map< INT, SOUND_BUFFER* >::iterator it;
	it = m_mapSoundID.find(nID);

	// �Ƿ�ID
	if(it == m_mapSoundID.end())
	{
		KLThrow("Invalid sound source id: %d", nID);
		return NULL;
	}

	return Source_Load(*(it->second));
}

tSoundBuffer* CSoundSystemFMod::Buffer_Create(LPCTSTR szFileName)
{
	std::map< STRING, SOUND_BUFFER* >::iterator it;
	it = m_mapSoundFile.find(szFileName);

	//�Ƿ��ļ���
	if(it == m_mapSoundFile.end()) return NULL;

	return Source_Load(*(it->second));
}

tSoundBuffer* CSoundSystemFMod::Source_Load(SOUND_BUFFER& sndBuffer)
{
	//��δ����
	if(sndBuffer.pSoundBuf == NULL)
	{
		//������Դ
		CSoundBufferFMod* pSoundBuf = new CSoundBufferFMod;

		if(!(pSoundBuf->ReadWavBuffer(sndBuffer.pDefine->pSoundFile)))
			return NULL;

		//��������map
		sndBuffer.pSoundBuf = pSoundBuf;
	}

	return sndBuffer.pSoundBuf;
}


//����һ������Դ
tSoundSource* CSoundSystemFMod::Source_Create(tSoundSource::Type type, BOOL b3DMode, 
												BOOL bAutoDestroy)
{
	static INT s_idSource = 1;

	CSoundSourceFMod* pSource = new CSoundSourceFMod(s_idSource++, type, b3DMode);

	SOUND_SOURCE newSource;
	newSource.bAutoDestroy = bAutoDestroy;
	newSource.pSoundSource = pSource;
	newSource.pSoundSourceReference = NULL; // ���ⲿ����ָ��Ϊ��	20100714 BLL

	m_listSoundSource.push_back(newSource);
	return (tSoundSource*)pSource;
}

// ������Ƶʱ�������ⲿָ��				20100714 BLL
tSoundSource* CSoundSystemFMod::Source_Create(tSoundSource::Type type, BOOL b3DMode, BOOL bAutoDestroy, tSoundSource** pSoundSource )
{
	static INT s_idSource = 1;

	CSoundSourceFMod* pSource = new CSoundSourceFMod(s_idSource++, type, b3DMode);

	SOUND_SOURCE newSource;
	newSource.bAutoDestroy = bAutoDestroy;
	newSource.pSoundSource = pSource;

	if ( pSoundSource != NULL )
	{
		newSource.pSoundSourceReference = pSoundSource;
	}

	m_listSoundSource.push_back(newSource);
	return (tSoundSource*)pSource;
}

VOID CSoundSystemFMod::Source_Destroy(tSoundSource* pSource)
{
	//��ѯID
	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& theSnd = *it;

		if(theSnd.pSoundSource == pSource)
		{
			//ɾ��ʵ��
			delete theSnd.pSoundSource;

			// ����ýڵ�������ⲿ���õ�ָ�룬��ͬʱ���ⲿ����ָ��ֵҲ�ÿ�		20100714 BLL
			if ( it->pSoundSourceReference != NULL )
			{
				*( it->pSoundSourceReference ) = NULL; 
			}	

			m_listSoundSource.erase(it);
			return;
		}
	}
}

VOID CSoundSystemFMod::Source_Destroy(INT id)
{
	//��ѯID
	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& theSnd = *it;

		if(theSnd.pSoundSource && theSnd.pSoundSource->GetID() == id)
		{
			//ɾ��ʵ��
			delete theSnd.pSoundSource;

			// ����ýڵ�������ⲿ���õ�ָ�룬��ͬʱ���ⲿ����ָ��ֵҲ�ÿ�				20100714 BLL
			if ( it->pSoundSourceReference != NULL )
			{
				*( it->pSoundSourceReference ) = NULL; 
			}	

			m_listSoundSource.erase(it);
			return;
		}
	}
}

void CSoundSystemFMod::PlayUISoundFunc( INT nID )
{
	//-----------------------------------------------------------------------
	//����3d��������Դ
	if(CVariableSystem::GetMe()->GetAs_Int(szUISoundVar) != TRUE) return;

	//������Դ
	tSoundBuffer* pSndBuffer = Buffer_Create(nID);
	if(!pSndBuffer) return;

	//����һ������Դ���Զ�ɾ��
	tSoundSource* pSndSource = Source_Create(CSoundSourceFMod::type_UI, FALSE, TRUE);
	if(!pSndSource) return;

	//���ò�����ʼ����
	pSndSource->SetBuffer(pSndBuffer);
	pSndSource->SetLooping(FALSE);

	pSndSource->Play();
}

INT CSoundSystemFMod::PlaySoundFunc(LPCTSTR szSoundFile, FLOAT* fvPos, BOOL bLoop)
{
	//-----------------------------------------------------------------------
	//����ת��
	fVector3 fvGfx(fvPos[0], fvPos[1], fvPos[2]);
	fVector3 fvGame;
	if(!CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_GFX, fvGfx, 
			tGfxSystem::AX_GAME, fvGame))
	{
		return -1;
	}

	//-----------------------------------------------------------------------
	//����3d��������Դ

	//������Դ
	tSoundBuffer* pSndBuffer = Buffer_Create(szSoundFile);
	if(!pSndBuffer) return -1;

	//����һ������Դ���Զ�ɾ��
	tSoundSource* pSndSource = Source_Create(CSoundSourceFMod::type_skill, TRUE, TRUE);
	if(!pSndSource) return -1;

	//���ò�����ʼ����
	pSndSource->SetBuffer(pSndBuffer);
	pSndSource->SetPos(fvGame);
	pSndSource->SetLooping(bLoop);

	pSndSource->Play();

	return (INT)(INT_PTR)pSndSource;
}

VOID CSoundSystemFMod::StopSoundFunc(INT nHandle)
{
	if(nHandle == -1) return;

	//ת��Ϊ����ָ��
	CSoundSourceFMod* pSnd = (CSoundSourceFMod*)(INT_PTR)nHandle;

	//�����Ƿ��и��������ڲ���
	std::list< SOUND_SOURCE >::iterator it;
	for(it=m_listSoundSource.begin(); it!=m_listSoundSource.end(); it++)
	{
		SOUND_SOURCE& theSnd = *it;
		if(theSnd.pSoundSource == pSnd)
		{
			//ɾ��ʵ��
			delete theSnd.pSoundSource;

			// ����ýڵ�������ⲿ���õ�ָ�룬��ͬʱ���ⲿ����ָ��ֵҲ�ÿ�				20100714 BLL
			if ( it->pSoundSourceReference != NULL )
			{
				*( it->pSoundSourceReference ) = NULL; 
			}	


			m_listSoundSource.erase(it);
			return;
		}
	}

}

VOID WINAPI CSoundSystemFMod::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
	{
		const STRING& strVariable = pEvent->vArg[0];
		const STRING& strValue = pEvent->vArg[1];

		BOOL bEnable = (strValue=="1");

		//ָֹͣ������ 		
		if ( bEnable == TRUE )
		{
			if( strVariable == szAllSoundDisable )	// �ر�������Ч		20100714 BLL
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_skill);
			
				GetMe()->StopSpecialSounds(tSoundSource::type_UI);
		
				GetMe()->StopSpecialSounds(tSoundSource::type_background);			
			
				GetMe()->StopSpecialSounds(tSoundSource::type_circumstance);
			}	
		}
		else
		{			
			if(strVariable == szSKSoundVar)			// ������Ч
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_skill);
			}
			else if(strVariable == szUISoundVar)	// �¼���Ч
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_UI);
			}
			else if ( strVariable == szBKSoundVar ) // ������Ч
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_background);
			}
			else if ( strVariable == sz3DSoundVar )	// ������Ч
			{
				GetMe()->StopSpecialSounds(tSoundSource::type_circumstance);
			}	

		}//if ( bEnable == TRUE )		
	}
}

VOID CSoundSystemFMod::StopSpecialSounds(tSoundSource::Type type)
{
	//��������Դ����
	std::list< SOUND_SOURCE >::iterator it, itNext;
	for(it = m_listSoundSource.begin(); it!=m_listSoundSource.end(); it=itNext)
	{
		itNext = it; itNext++;
		SOUND_SOURCE& theSnd = *it;
	
		//������Ч�Ŀ���
		if(type == theSnd.pSoundSource->GetType())
		{
			theSnd.pSoundSource->Stop();	
			delete theSnd.pSoundSource;

			// ����ýڵ�������ⲿ���õ�ָ�룬��ͬʱ���ⲿ����ָ��ֵҲ�ÿ�	20100714 BLL
			if ( it->pSoundSourceReference != NULL )
			{
				*( it->pSoundSourceReference ) = NULL; 
			}	

			m_listSoundSource.erase(it);
			continue;
		}
	}

}

INT CSoundSystemFMod::Lua_PlaySound(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if(!args[2].IsInteger()) 
	{
		KLThrow("LUA:CSoundSystemFMod::Lua_PlaySound[2] param parameter error");
		return 0;
	}
	if(!args[3].IsBoolean()) 
	{
		KLThrow("LUA:CSoundSystemFMod::Lua_PlaySound[3] param parameter error");
		return 0;
	}

	int idSource = args[2].GetInteger();
	bool bLoop =  args[3].GetBoolean();

	//����Source
	tSoundBuffer* pSndBuffer = CSoundSystemFMod::GetMe()->Buffer_Create(idSource);
	if(!pSndBuffer) 
	{
		state->PushInteger(-1);
		return 1;
	}

	//������Դ
	tSoundSource* pSound = Source_Create(CSoundSourceFMod::type_background, FALSE, !bLoop);
	if(pSndBuffer)
	{
		pSound->SetBuffer(pSndBuffer);
		pSound->SetLooping(bLoop);
		pSound->Play();
		state->PushInteger(pSound->GetID());
		return 1;
	}

	state->PushInteger(-1);
	return 1;
}

INT CSoundSystemFMod::Lua_StopSound(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if(!args[2].IsInteger()) 
	{
		KLThrow("LUA:CSoundSystemFMod::Lua_StopSound[2] param parameter error");
		return 0;
	}

	int idSound = args[2].GetInteger();

	Source_Destroy(idSound);
	return 0;
}


