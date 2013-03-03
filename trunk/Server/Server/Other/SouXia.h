/********************************************************************************
 *	文件名：	SouXia.h
 *	创建人：	耿浩
 *	创建时间：	2010 年 5 月 12 日
 *	功能说明：	处理捜侠录的自身信息
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
	BYTE	AddSkillToSouXia(BYTE type, SkillID_t studyId, BYTE useTime, INT* m_nMaxPages);	// 增加技能
	BOOL    HasLearnedProduct(UINT index);	//是否已经学习过该技能
	VOID	SetSouXiaData(SOUXIA_DATA& souXiaData) {m_SouXiaData = souXiaData; }
	static  VOID MyRandNumbers(BYTE* pages, BYTE curPage);// 打乱页数取其前N位
	SOUXIA_DATA&	GetSouXiaData() { return m_SouXiaData;}

private:
	SHORT	_AddSkillToSouXia(SkillID_t* skillId, BYTE count, SkillID_t studyId); // 增加到该捜侠录
	SHORT	_AddSkillToSouXiaZhaoHuan(ZhaoHuan* skillId, BYTE count, SkillID_t studyId, SHORT useTime);
private:
	SOUXIA_DATA	m_SouXiaData;

};

#endif // _SOUXIA_H_
