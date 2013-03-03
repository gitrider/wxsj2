
#ifndef __HAIRMODELMNG_H__
#define __HAIRMODELMNG_H__

#pragma once


#include "GIDBC_Struct.h"
#include <vector>



typedef std::vector<_DBC_CHAR_HAIR_GEO> HAIR_MODLE_INFO_VECTOR;

class CHairModelMng
{
public:

	CHairModelMng(void);
	~CHairModelMng(void);

public:

	// �õ����Ƿ��͸���
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ������Ϣ����
	int GetHairModelCount(int iRace);

	// �õ������Ƿ��͸���
	int GetManHairModelCount();

	// �õ�Ů���Ƿ��͸���
	int GetWomanHairModelCount();

	// �õ����Ƿ�����Ϣ
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ������Ϣ����
	LPCSTR  GetHairModel(int iRace, int iIndex);

	// �õ������Ƿ�����Ϣ
	//
	// iIndex = ������Ϣ����
	LPCSTR  GetManHairModel(int iIndex);

	// �õ�Ů���Ƿ�����Ϣ
	//
	// iIndex = ������Ϣ����
	LPCSTR	GetWomanHairModel(int iIndex);

	// �õ���ɫ������Ϣ
	// 0 -- �õ���ɫ���ͳɹ�
	// ���� -- ʧ��.
	int GetCharacterModel();

	// �������
	void ClearData();

	// �õ�Ů���Ƿ���Id
	int GetWomanHairModelId(int iIndex);

	// �õ������Ƿ���Id
	int GetManHairModelId(int iIndex);

	// �õ�����model Id
	int GetHairModelId(int iRace, int iIndex);

	// �õ����Ƿ�����Ϣ����
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ������Ϣ����
	LPCSTR  GetHairModelShowName(int iRace, int iIndex);

	// �õ������Ƿ�����Ϣ����
	//
	// iIndex = ������Ϣ����
	LPCSTR  GetManHairModelShowName(int iIndex);

	// �õ�Ů���Ƿ�����Ϣ����
	//
	// iIndex = ������Ϣ����
	LPCSTR	GetWomanHairModelShowName(int iIndex);


public:

	/// �����Ƿ��θ���
	int						m_iManHairModelCount;
	/// Ů���Ƿ��θ���
	int						m_iWomanHairModelCount;

	/// �����Ƿ�����Ϣ����
	HAIR_MODLE_INFO_VECTOR	m_ManHairModelVector;
	/// Ů���Ƿ�����Ϣ����
	HAIR_MODLE_INFO_VECTOR	m_WomanHairModelVector;

};

#endif //__HAIRMODELMNG_H__