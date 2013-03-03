#include ".\stallinfoeditoperator.h"
#include ".\stallinfoaction.h"

namespace Fairy 
{
	CStallInfoEditOperator::CStallInfoEditOperator(void)
	{
		m_pAction = NULL;
	}

	CStallInfoEditOperator::~CStallInfoEditOperator(void)
	{

	}

	// ��������༭������ָ��.
	void CStallInfoEditOperator::SetAction(CStallInfoAction* pAction)
	{
		m_pAction = pAction;
	}

	
	// ����λ��
	void CStallInfoEditOperator::SetPos(unsigned long dwposX, unsigned long dwposY, unsigned long dwEndPosX, unsigned long dwEndPosY)
	{
		m_dwStartPosX = dwposX;
		m_dwStartPosY = dwposY;
		m_dwEndPosX	  =	dwEndPosX;
		m_dwEndPosY	  = dwEndPosY;

	}

	// ����λ����Ϣ
	void CStallInfoEditOperator::SetStallInfo(	
												unsigned char	m_bCanStall,	//�Ƿ���԰�̯
												unsigned char	m_iTradeTax,	//����˰
												unsigned long	m_dwPosTax,		//̯λ��
												unsigned char	m_iExtraInfo	//������Ϣ
												)
	{
		m_bCanStall  = m_bCanStall;		//�Ƿ���԰�̯
		m_iTradeTax  = m_iTradeTax;		//����˰
		m_dwPosTax   = m_dwPosTax;		//̯λ��
		m_iExtraInfo = m_iExtraInfo;	//������Ϣ
	}

	const String& CStallInfoEditOperator::getGroupName(void) const
	{
		static String strGroupName = "StallInfo";
		return strGroupName;
	}

	String CStallInfoEditOperator::getDescription(void) const
	{
		return String("StallInfo");
	}

	String CStallInfoEditOperator::getHelp(void) const
	{
		return String("StallInfo");
	}

	void CStallInfoEditOperator::undo(void)
	{

	}

	void CStallInfoEditOperator::redo(void)
	{

	}





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ɾ��̯λ
	//

	CStallInfoEditDelOperator::CStallInfoEditDelOperator(void)
	{
	}
	CStallInfoEditDelOperator::~CStallInfoEditDelOperator(void)
	{

	}

	const String& CStallInfoEditDelOperator::getGroupName(void) const
	{
		static String strGroupName = "StallInfoDel";
		return strGroupName;
	}

	String CStallInfoEditDelOperator::getDescription(void) const
	{
		return String("StallInfoDel");
	}

	String CStallInfoEditDelOperator::getHelp(void) const
	{
		return String("StallInfoDel");
	}

	void CStallInfoEditDelOperator::undo(void)
	{

	}

	void CStallInfoEditDelOperator::redo(void)
	{

	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���̯λ
	//
	CStallInfoEditAddOperator::CStallInfoEditAddOperator(void)
	{

	}
	CStallInfoEditAddOperator::~CStallInfoEditAddOperator(void)
	{

	}
	const String& CStallInfoEditAddOperator::getGroupName(void) const
	{
		static String strGroupName = "StallInfoAdd";
		return strGroupName;
	}

	String CStallInfoEditAddOperator::getDescription(void) const
	{
		return String("StallInfoAdd");
	}

	String CStallInfoEditAddOperator::getHelp(void) const
	{
		return String("StallInfoAdd");
	}

	void CStallInfoEditAddOperator::undo(void)
	{

	}

	void CStallInfoEditAddOperator::redo(void)
	{

	}

}
