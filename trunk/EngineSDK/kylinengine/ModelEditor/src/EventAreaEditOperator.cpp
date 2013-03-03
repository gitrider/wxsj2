#include "eventareaeditoperator.h"
#include "EventAreaEditAction.h"

namespace Fairy 
{
	CEventAreaEditOperator::CEventAreaEditOperator(void)
	{
		m_pAction = NULL;
	}
	
	CEventAreaEditOperator::~CEventAreaEditOperator(void)
	{

	}
	
	// ��������༭������ָ��.
	void CEventAreaEditOperator::SetAction(CEventAreaEditAction* pAction)
	{
		m_pAction = pAction;
	}

		// ��������ID
	void CEventAreaEditOperator::SetInstanceId(unsigned long ulInstanceId)
	{
		m_ulInstanceId = ulInstanceId;
	}

		// ���õ�ID
	void CEventAreaEditOperator::SetTypeId(unsigned long ulTypeId)
	{
		m_ulTypeId = ulTypeId;
	}

	// ����undo�� redo ������Ҫ��λ��.
	void CEventAreaEditOperator::SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos)
	{
		m_vectorUnDoPos = vectorUnDoPos;
		m_vectorReDoPos = vectorReDoPos;
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// ɾ��һ�����λ�õ�redo�� undo����.
	
	CEventAreaEditDelOperator::CEventAreaEditDelOperator(void)
	{

	}

	CEventAreaEditDelOperator::~CEventAreaEditDelOperator(void)
	{

	}


	const String& CEventAreaEditDelOperator::getGroupName(void) const
	{
		static String strGroupName = "EventAreaDel";
		return strGroupName;
	}

    
	String CEventAreaEditDelOperator::getDescription(void) const
	{
		return String("EventAreaDel");
	}

    
	String CEventAreaEditDelOperator::getHelp(void) const
	{
		return String("EventAreaDel");
	}

	void CEventAreaEditDelOperator::undo(void)
	{
		if(m_pAction)
		{
			m_ulInstanceId = m_pAction->AddRect(m_vectorUnDoPos, m_vectorReDoPos, m_ulInstanceId);
			
		}
	}

    
	void CEventAreaEditDelOperator::redo(void)
	{
		if(m_pAction)
		{
			m_pAction->DeleteRectNoOperator(m_ulInstanceId);
		}
	}


	/////////////////////////////////////////////////////////////////////////////////
	//
	// ���һ�����redo�� undo����.
	//
	//
	
	CEventAreaEditAddOperator::CEventAreaEditAddOperator(void)
	{

	}//
	CEventAreaEditAddOperator::~CEventAreaEditAddOperator(void)
	{

	}//

	const String& CEventAreaEditAddOperator::getGroupName(void) const
	{
		static String strGroupName = "EventAreaAdd";
		return strGroupName;
	}

     
	String CEventAreaEditAddOperator::getDescription(void) const
	{
		return String("EventAreaAdd");
	}

   
	String CEventAreaEditAddOperator::getHelp(void) const
	{
		return String("EventAreaAdd");
	}

	void CEventAreaEditAddOperator::undo(void)
	{
		if(m_pAction)
		{
			m_pAction->DeleteRectNoOperator(m_ulInstanceId);
		}
	}

       
	void CEventAreaEditAddOperator::redo(void)
	{
		if(m_pAction)
		{
			m_ulInstanceId = m_pAction->AddRect(m_vectorUnDoPos, m_vectorReDoPos, m_ulInstanceId);
			
		}
		
	}

	
}