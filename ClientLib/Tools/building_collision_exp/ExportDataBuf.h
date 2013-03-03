#ifndef __EXPORTDATABUF_H__
#define __EXPORTDATABUF_H__
#pragma once

#include <vector>
#include <string>

typedef struct _POS
{

	_POS()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

} POS;

typedef struct _NOR
{

	_NOR()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	float x;
	float y;
	float z;

} NOR;

typedef struct _UV
{
	_UV()
	{
		u = 0;
		v = 0;
	}
	float u;
	float v;

} UV;

//typedef struct _POINT
//{
//	_POINT()
//	{
//		iPosIndex = -1;
//		iNorIndex = -1;
//		iuvIndex  = -1;
//	}
//
//	int iPosIndex;
//	int iNorIndex;
//	int iuvIndex;
//
//} POINT;//


typedef struct _FACE
{
	_FACE()
	{
		iF1 = -1;
		iF2 = -1;
		iF3 = -1;
	}

	short iF1;
	short iF2;
	short iF3;

} FACE;


typedef struct _POS_NOR
{
	bool operator==(_POS_NOR  value)
	{
		if((iPosIndex == value.iPosIndex)&&(iuvIndex == value.iuvIndex))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int iPosIndex;
	int iuvIndex;

} POS_NOR;





typedef std::vector<POS> POS_VECTOR;
typedef std::vector<NOR> NOR_VECTOR;
typedef std::vector<UV>  UV_VECTOR;
typedef std::vector<int> INDEX_VECTOR;
//typedef std::vector<POINT> POINT_VECTOR;
typedef std::vector<FACE> FACE_VECTOR;
typedef std::vector<POS_NOR> NEW_POINT_VECTOR;

class CExportDataBuf
{
public:
	CExportDataBuf(void);
	~CExportDataBuf(void);

public:

		std::string  strName;


		int          m_iFaceCount;


		FACE_VECTOR  m_posFaceVector;
		POS_VECTOR   m_posVector;
		int			 m_posVectorCount;

		NOR_VECTOR   m_norVector;
		int			 m_norVectorCount;

		FACE_VECTOR  m_uvFaceVector;
		UV_VECTOR	 m_uvVector;
		int          m_uvVectorCount;

		       

		NEW_POINT_VECTOR	m_newPointVector;
		FACE_VECTOR			m_newFaceVector;
		//POINT_VECTOR m_point;
		INDEX_VECTOR m_index;

		void BuildData();

};

#endif //__EXPORTDATABUF_H__