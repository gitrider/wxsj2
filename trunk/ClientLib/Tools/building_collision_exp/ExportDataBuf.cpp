#include ".\exportdatabuf.h"

CExportDataBuf::CExportDataBuf(void)
{
}

CExportDataBuf::~CExportDataBuf(void)
{
}



void CExportDataBuf::BuildData()
{
	POS_NOR	pointTemp;
	FACE	faceNew;
	for(int i = 0; i < m_iFaceCount; i++)
	{
		pointTemp.iPosIndex = m_posFaceVector[i].iF1;
		pointTemp.iuvIndex  = m_uvFaceVector[i].iF1;
		
		int j=0;

		for(j = 0; j < (int)m_newPointVector.size(); j++)
		{
			if(pointTemp == m_newPointVector[j])
			{
				faceNew.iF1 = j;
				break;
			}
		}

		if(j == m_newPointVector.size())
		{
			faceNew.iF1 = j;
			m_newPointVector.push_back(pointTemp);
		}


		pointTemp.iPosIndex = m_posFaceVector[i].iF2;
		pointTemp.iuvIndex  = m_uvFaceVector[i].iF2;

		for(j = 0; j < (int)m_newPointVector.size(); j++)
		{
			if(pointTemp == m_newPointVector[j])
			{
				faceNew.iF2 = j;
				break;
			}
		}

		if(j == m_newPointVector.size())
		{
			faceNew.iF2 = j;
			m_newPointVector.push_back(pointTemp);
		}


		pointTemp.iPosIndex = m_posFaceVector[i].iF3;
		pointTemp.iuvIndex  = m_uvFaceVector[i].iF3;

		for(j = 0; j < (int)m_newPointVector.size(); j++)
		{
			if(pointTemp == m_newPointVector[j])
			{
				faceNew.iF3 = j;
				break;
			}
		}

		if(j == m_newPointVector.size())
		{
			faceNew.iF3 = j;
			m_newPointVector.push_back(pointTemp);
		}

		m_newFaceVector.push_back(faceNew);

	}
}
