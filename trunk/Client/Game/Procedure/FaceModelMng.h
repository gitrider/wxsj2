
#ifndef __FACEMODELMNG_H__
#define __FACEMODELMNG_H__

#pragma once


#include "GIDBC_Struct.h"
#include <vector>



typedef std::vector<_DBC_CHAR_HEAD_GEO> FACE_MODLE_INFO_VECTOR;

class CFaceModelMng
{
public:

	CFaceModelMng(void);
	~CFaceModelMng(void);

public:
	
	// �õ��������θ���
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ͷ����Ϣ����
	int GetFaceModelCount(int iRace);

	// �õ�������ͷ�����
	int GetManFaceModelCount();

	// �õ�Ů����ͷ�����
	int GetWomanFaceModelCount();

	// �õ�����ͷ����Ϣ
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ͷ����Ϣ����
	LPCSTR  GetFaceModel(int iRace, int iIndex);

	// �õ�������ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
	LPCSTR  GetManFaceModel(int iIndex);

	// �õ�Ů����ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
	LPCSTR	GetWomanFaceModel(int iIndex);

	// �õ���ɫͷ����Ϣ
	// 0 -- �õ���ɫͷ��ɹ�
	// ���� -- ʧ��.
	int GetCharacterModel();

	// �������
	void ClearData();

	// �õ�Ů����ͷ��Id
	int GetWomanFaceModelId(int iIndex);

	// �õ�������ͷ��Id
	int GetManFaceModelId(int iIndex);

	// �õ�����model Id
	int GetFaceModelId(int iRace, int iIndex);


	
	// �õ�����ͷ����Ϣ
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ͷ����Ϣ����
	LPCSTR  GetFaceModelShowName(int iRace, int iIndex);

	// �õ�������ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
	LPCSTR  GetManFaceModelShowName(int iIndex);

	// �õ�Ů����ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
	LPCSTR	GetWomanFaceModelShowName(int iIndex);


public:

	/// ���������θ���
	int						m_iManFaceModelCount;
	/// Ů�������θ���
	int						m_iWomanFaceModelCount;

	/// ������������Ϣ����
	FACE_MODLE_INFO_VECTOR	m_ManFaceModelVector;
	/// Ů����������Ϣ����
	FACE_MODLE_INFO_VECTOR	m_WomanFaceModelVector;

};

#endif //__FACEMODELMNG_H__