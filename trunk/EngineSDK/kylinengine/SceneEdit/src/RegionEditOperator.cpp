#include "regioneditoperator.h"
#include "RegionEditAction.h"

namespace Fairy 
{
	
	////////////////////////////////////////////////////////////////////////////////
	//
	// ���� CRegionEditOperator ��ʵ�ֺ���
	//
	CRegionEditOperator::CRegionEditOperator(void)
	{
		m_pRegionAction = NULL;
		m_bFireUI = false;
	}

	CRegionEditOperator::~CRegionEditOperator(void)
	{
	}

	void CRegionEditOperator::SetRegionAction(CRegionEditAction* pRegionAction)
	{
		m_pRegionAction = pRegionAction;
	}

	// ��������ID
	void CRegionEditOperator::SetRegionId(unsigned long ulRegionId)
	{
		m_ulRegionId = ulRegionId;
	}

	// ���õ�ID
	void CRegionEditOperator::SetPointId(unsigned long ulPointId)
	{
		m_ulPointId = ulPointId;
	}

	// ����undo�� redo ������Ҫ��λ��.
	void CRegionEditOperator::SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos)
	{
		m_vectorUnDoPos = vectorUnDoPos;
		m_vectorReDoPos = vectorReDoPos;
	}

	// ���ñ任������ID
	void CRegionEditOperator::SetChangeRegionId(unsigned long ulUnDoRegionId,unsigned long ulReDoRegionId)
	{
		m_ulUnDoRegionId = ulUnDoRegionId;
		m_ulReDoRegionId = ulReDoRegionId;

	}
	// �����Ƿ�֪ͨUI
	void CRegionEditOperator::SetFireUI(bool fire)
	{
		m_bFireUI = fire;
	}



	/////////////////////////////////////////////////////////////////////////////////
	//
	// �޸�һ�����redo�� undo����.
	//
	//
	CRegionEditModifyOperator::CRegionEditModifyOperator(void)
	{

	}

	CRegionEditModifyOperator::~CRegionEditModifyOperator(void)
	{

	}


	/** Retrieves a group name of this operator
    @remarks
        The OperatorManager will manager grouping operators by group name,
        so undo/redo can be perform independent with each operator group.
    @par
        The operators must use same group name when their may be influence
        by each other.
    @returns
        group name of this operator
    */
	const String& CRegionEditModifyOperator::getGroupName(void) const
	{
		static String strGroupName = "RegionModify";
		return strGroupName;
	}

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
     String CRegionEditModifyOperator::getDescription(void) const
	 {
		return String("RegionModify");
	 }

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
      String CRegionEditModifyOperator::getHelp(void) const
	  {
		return String("RegionModify");
	  }

        /** Perform undo operate
        */
      void CRegionEditModifyOperator::undo(void)
	  {
		 if(m_pRegionAction)
		 {
			 m_pRegionAction->ChangeCurEditPointPos(m_ulRegionId, m_ulPointId, m_vectorUnDoPos);
		 }
	  }

        /** Perform redo operate
        */
      void CRegionEditModifyOperator::redo(void)
	  {
		 if(m_pRegionAction)
		 {
			 m_pRegionAction->ChangeCurEditPointPos(m_ulRegionId, m_ulPointId, m_vectorReDoPos);
		 }
	  }


	  /////////////////////////////////////////////////////////////////////////////////
	  //
	  // ѡ������redo�� undo����.
	  //
	  //
	  CRegionChangeSelectOperator::CRegionChangeSelectOperator(void)
	  {

	  }

	  CRegionChangeSelectOperator::~CRegionChangeSelectOperator(void)
	  {

	  }


	  /** Retrieves a group name of this operator
	  @remarks
	  The OperatorManager will manager grouping operators by group name,
	  so undo/redo can be perform independent with each operator group.
	  @par
	  The operators must use same group name when their may be influence
	  by each other.
	  @returns
	  group name of this operator
	  */
	  const String& CRegionChangeSelectOperator::getGroupName(void) const
	  {
		  static String strGroupName = "RegionChange";
		  return strGroupName;
	  }

	  /** Retrieves a description of this operator
	  @remarks
	  The description should be display as a menu text.
	  @returns
	  description of this operator
	  */
	  String CRegionChangeSelectOperator::getDescription(void) const
	  {
		  return String("RegionChange");
	  }

	  /** Retrieves a help text of this operator
	  @remarks
	  The help text should be use as a detail description of this operator,
	  when use selecting menu item of this operator, this help text will
	  display in status line.
	  @returns
	  help text of this operator
	  */
	  String CRegionChangeSelectOperator::getHelp(void) const
	  {
		  return String("RegionChange");
	  }

	  /** Perform undo operate
	  */
	  void CRegionChangeSelectOperator::undo(void)
	  {
		  if(m_pRegionAction)
		  {
			  m_pRegionAction->ChangeCurEditRegion(m_ulUnDoRegionId,m_bFireUI);
		  }
	  }

	  /** Perform redo operate
	  */
	  void CRegionChangeSelectOperator::redo(void)
	  {
		  if(m_pRegionAction)
		  {
			  m_pRegionAction->ChangeCurEditRegion(m_ulReDoRegionId,m_bFireUI);
		  }
	  }







	/////////////////////////////////////////////////////////////////////////////////
	//
	// ���һ�����redo�� undo����.
	//
	//


	  CRegionEditAddPointOperator::CRegionEditAddPointOperator(void)
	  {

	  }
		
	  CRegionEditAddPointOperator::~CRegionEditAddPointOperator(void)
	  {

	  }

	
		/** Retrieves a group name of this operator
        @remarks
            The OperatorManager will manager grouping operators by group name,
            so undo/redo can be perform independent with each operator group.
        @par
            The operators must use same group name when their may be influence
            by each other.
        @returns
            group name of this operator
        */
	  const String& CRegionEditAddPointOperator::getGroupName(void) const
	  {
		  static String strGroupName = "AddNewPoint";

		  return strGroupName;
	  }

        /** Retrieves a description of this operator
        @remarks
            The description should be display as a menu text.
        @returns
            description of this operator
        */
	  String CRegionEditAddPointOperator::getDescription(void) const
	  {
		  return "AddNewPoint";
	  }

        /** Retrieves a help text of this operator
        @remarks
            The help text should be use as a detail description of this operator,
            when use selecting menu item of this operator, this help text will
            display in status line.
        @returns
            help text of this operator
        */
	  String CRegionEditAddPointOperator::getHelp(void) const
	  {
		 return "AddNewPoint";
	  }

        /** Perform undo operate
        */
		void CRegionEditAddPointOperator::undo(void)
		{
			if(m_pRegionAction)
			{
				m_pRegionAction->DelPoint(m_ulRegionId, m_ulPointId);
				m_pRegionAction->ChangeCurEditRegion(m_ulRegionId,true);
				
			}
		}

        /** Perform redo operate
        */
		void CRegionEditAddPointOperator::redo(void)
		{
			if(m_pRegionAction)
			{
				m_pRegionAction->AddNewPoint(m_ulRegionId, m_ulPointId, m_vectorReDoPos);
				m_pRegionAction->ChangeCurEditRegion(m_ulRegionId,true);
				
			}
		
		}


		/////////////////////////////////////////////////////////////////////////////////
		//
		// ɾ��һ�����redo�� undo����.
		//
		//


		CRegionEditDeletePointOperator::CRegionEditDeletePointOperator(void)
		{

		}

		CRegionEditDeletePointOperator::~CRegionEditDeletePointOperator(void)
		{

		}


		/** Retrieves a group name of this operator
		@remarks
		The OperatorManager will manager grouping operators by group name,
		so undo/redo can be perform independent with each operator group.
		@par
		The operators must use same group name when their may be influence
		by each other.
		@returns
		group name of this operator
		*/
		const String& CRegionEditDeletePointOperator::getGroupName(void) const
		{
			static String strGroupName = "DeletePoint";

			return strGroupName;
		}

		/** Retrieves a description of this operator
		@remarks
		The description should be display as a menu text.
		@returns
		description of this operator
		*/
		String CRegionEditDeletePointOperator::getDescription(void) const
		{
			return "DeletePoint";
		}

		/** Retrieves a help text of this operator
		@remarks
		The help text should be use as a detail description of this operator,
		when use selecting menu item of this operator, this help text will
		display in status line.
		@returns
		help text of this operator
		*/
		String CRegionEditDeletePointOperator::getHelp(void) const
		{
			return "DeletePoint";
		}

		/** Perform undo operate
		*/
		void CRegionEditDeletePointOperator::undo(void)
		{
			if(m_pRegionAction)
			{
				m_pRegionAction->AddNewPoint(m_ulRegionId, m_ulPointId, m_vectorUnDoPos);
				m_pRegionAction->ChangeCurEditRegion(m_ulRegionId,true);

			}
		}

		/** Perform redo operate
		*/
		void CRegionEditDeletePointOperator::redo(void)
		{
			if(m_pRegionAction)
			{
				m_pRegionAction->DelPoint(m_ulRegionId, m_ulPointId);
				m_pRegionAction->ChangeCurEditRegion(m_ulRegionId,true);

			}
			
		}




}