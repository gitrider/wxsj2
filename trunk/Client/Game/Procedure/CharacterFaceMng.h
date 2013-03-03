
#ifndef __CHARACTERFACEMNG_H__
#define __CHARACTERFACEMNG_H__

#pragma once


#include "GIDBC_Struct.h"
#include <vector>



typedef std::vector<_DBC_CHAR_FACE> FACE_INFO_VECTOR;

class CCharacterFaceMng
{
public:

	CCharacterFaceMng(void);
	~CCharacterFaceMng(void);

public:

	// �õ�����ͷ�����
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ͷ����Ϣ����
	int GetManFaceCount(int iRace);

	// �õ�������ͷ�����
	int GetManFaceCount();

	// �õ�Ů����ͷ�����
	int GetWomanFaceCount();

	// �õ�����ͷ����Ϣ
	//
	// iRace = 0 - Ů
	//         1 - ��
	// iIndex = ͷ����Ϣ����
	LPCSTR  GetFaceImageInfo(int iRace, int iIndex);

	// �õ�������ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
	LPCSTR  GetManFaceImageInfo(int iIndex);

	// �õ�Ů����ͷ����Ϣ
	//
	// iIndex = ͷ����Ϣ����
	LPCSTR	GetWomanFaceImageInfo(int iIndex);

	// �õ���ɫͷ����Ϣ
	// 0 -- �õ���ɫͷ��ɹ�
	// ���� -- ʧ��.
	int GetCharacterFaceIfo();

	// �������
	void ClearData();

	// �õ�Ů����ͷ��Id
	int GetWomanFaceId(int iIndex);

	// �õ�������ͷ��Id
	int GetManFaceId(int iIndex);


public:

	/// ������ͷ�����
	int					 m_iManFaceCount;
	/// Ů����ͷ�����
	int					m_iWomanFaceCount;

	/// ������ͷ����Ϣ����
	FACE_INFO_VECTOR	m_ManFaceInfoVector;
	/// Ů����ͷ����Ϣ����
	FACE_INFO_VECTOR	m_WomanFaceInfoVector;

};

#endif //__CHARACTERFACEMNG_H__