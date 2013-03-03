#include "GrowPointOperator.h"

namespace Fairy 
{

	CGrowPointOperator::CGrowPointOperator(void)
	{
	}

	CGrowPointOperator::~CGrowPointOperator(void)
	{
	}


	const String& CGrowPointOperator::getGroupName(void) const
	{
	static String strGroupName = "GrowPoint";
	return strGroupName;
	}

	String CGrowPointOperator::getDescription(void) const
	{
	return String("GrowPoint");
	}


	String CGrowPointOperator::getHelp(void) const
	{
	return String("GrowPoint");
	}


	void CGrowPointOperator::undo(void)
	{

	}


	void CGrowPointOperator::redo(void)
	{

	}




		// ��������༭������ָ��.
	void CGrowPointOperator::SetAction(CGrowPointEditAction* pAction)
	{
		
		m_pAction = pAction;

	}

		// ����ʵ��id
	void CGrowPointOperator::SetInstanceId(unsigned long ulInstanceId)
	{
		
		m_ulInstanceId = ulInstanceId;
	
	}

		// ���ù�������id
	void CGrowPointOperator::SetTypeId(unsigned long ulTypeId)
	{
		
		m_ulTypeId = ulTypeId;

	}

		// ����undo�� redo ������Ҫ��λ��.
	void CGrowPointOperator::SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos)
	{
		
		m_vectorUnDoPos = vectorUnDoPos;
		m_vectorReDoPos = vectorReDoPos;

	}

		// ��������.
	void CGrowPointOperator::SetName(String& strName)
	{
		// ��������
		 m_strName = strName;
	}





	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// ���һ���¹�
	//
	CGrowPointAddOperator::CGrowPointAddOperator(void)
	{

	}
	CGrowPointAddOperator::~CGrowPointAddOperator(void)
	{

	}


	void CGrowPointAddOperator::undo(void)
	{
		if(m_pAction)
		{
			m_pAction->DelGrowPoint(m_ulInstanceId);
		}

	}
	void CGrowPointAddOperator::redo(void)
	{
		if(m_pAction)
		{
			
			m_ulInstanceId = m_pAction->AddNewGrowPoint(m_ulTypeId, m_vectorReDoPos);
			
		}

	}

	const String& CGrowPointAddOperator::getGroupName(void) const
	{
		static String strGroupName = "GrowPointAdd";
		return strGroupName;

	}
	String CGrowPointAddOperator::getDescription(void) const
	{
		return String("GrowPointAdd");
	}
	String CGrowPointAddOperator::getHelp(void) const
	{
		return String("GrowPointAdd");
	}



	/////////////////////////////////////////////////////////////////////////////////
	//
	// ɾ��һ������Ĳ���.
	//
	//
	CGrowPointDelOperator::CGrowPointDelOperator(void)
	{

	}
	
	CGrowPointDelOperator::~CGrowPointDelOperator(void)
	{

	}

	
	void CGrowPointDelOperator::undo(void)
	{
		if(m_pAction)
		{
			m_ulInstanceId = m_pAction->AddNewGrowPoint(m_ulTypeId, m_vectorUnDoPos);
			m_vectorUnDoPos = m_vectorUnDoPos;
			m_vectorReDoPos = m_vectorUnDoPos;
		}

	}
	void CGrowPointDelOperator::redo(void)
	{
		if(m_pAction)
		{
			m_pAction->DelGrowPoint(m_ulInstanceId);
		}
	}


	const String& CGrowPointDelOperator::getGroupName(void) const
	{
		static String strGroupName = "GrowPointDel";
		return strGroupName;

	}

	String CGrowPointDelOperator::getDescription(void) const
	{
		return String("GrowPointDel");
	}

	String CGrowPointDelOperator::getHelp(void) const
	{
		return String("GrowPointDel");
	}

	

}
