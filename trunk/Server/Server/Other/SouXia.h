/********************************************************************************
 *	�ļ�����	SouXia.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2010 �� 5 �� 12 ��
 *	����˵����	�������¼��������Ϣ
*********************************************************************************/
#ifndef _SOUXIA_H_
#define _SOUXIA_H_

class SouXia
{
public:
	SouXia();
	~SouXia(void);

	VOID	CleanUp()	{ m_SouXiaData.CleanUp(); }

public:
	BYTE	AddSkillToSouXia(BYTE type, SkillID_t studyId, BYTE useTime, INT* m_nMaxPages);	// ���Ӽ���
	BOOL    HasLearnedProduct(UINT index);	//�Ƿ��Ѿ�ѧϰ���ü���
	VOID	SetSouXiaData(SOUXIA_DATA& souXiaData) {m_SouXiaData = souXiaData; }
	static  VOID MyRandNumbers(BYTE* pages, BYTE curPage);// ����ҳ��ȡ��ǰNλ
	SOUXIA_DATA&	GetSouXiaData() { return m_SouXiaData;}

private:
	SHORT	_AddSkillToSouXia(SkillID_t* skillId, BYTE count, SkillID_t studyId); // ���ӵ��Ò���¼
	SHORT	_AddSkillToSouXiaZhaoHuan(ZhaoHuan* skillId, BYTE count, SkillID_t studyId, SHORT useTime);
private:
	SOUXIA_DATA	m_SouXiaData;

};

#endif // _SOUXIA_H_
