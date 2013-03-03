#pragma once

#include "WXOperator.h"
#include <OgreVector3.h>

namespace Fairy 
{
	class CPathFindAction;
	class CPathFindEditOperator : public Operator
	{
	public:
		CPathFindEditOperator(void);
		~CPathFindEditOperator(void);

	public:

		unsigned long m_ulPointId;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CPathFindAction* m_pRegionAction;

		// ��������༭������ָ��.
		void SetRegionAction(CPathFindAction* pRegionAction);

		// ���õ�ID
		void SetPointId(unsigned long ulPointId);

		// ����undo�� redo ������Ҫ��λ��.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

	};


	// �޸�һ�����λ�õ�redo�� undo����.
	class CPathFindEditModifyOperator : public CPathFindEditOperator
	{
	public:
		CPathFindEditModifyOperator(void);
		~CPathFindEditModifyOperator(void);

	public:
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
			virtual const String& getGroupName(void) const;

		/** Retrieves a description of this operator
		@remarks
		The description should be display as a menu text.
		@returns
		description of this operator
		*/
		virtual String getDescription(void) const;

		/** Retrieves a help text of this operator
		@remarks
		The help text should be use as a detail description of this operator,
		when use selecting menu item of this operator, this help text will
		display in status line.
		@returns
		help text of this operator
		*/
		virtual String getHelp(void) const;

		/** Perform undo operate
		*/
		virtual void undo(void);

		/** Perform redo operate
		*/
		virtual void redo(void);

	};



	/////////////////////////////////////////////////////////////////////////////////
	//
	// ���һ�����redo�� undo����.
	//
	//
	class CPathFindEditAddOperator : public CPathFindEditOperator
	{
	public:
		CPathFindEditAddOperator(void);
		~CPathFindEditAddOperator(void);

	public:
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
			virtual const String& getGroupName(void) const;

		/** Retrieves a description of this operator
		@remarks
		The description should be display as a menu text.
		@returns
		description of this operator
		*/
		virtual String getDescription(void) const;

		/** Retrieves a help text of this operator
		@remarks
		The help text should be use as a detail description of this operator,
		when use selecting menu item of this operator, this help text will
		display in status line.
		@returns
		help text of this operator
		*/
		virtual String getHelp(void) const;

		/** Perform undo operate
		*/
		virtual void undo(void);

		/** Perform redo operate
		*/
		virtual void redo(void);

	};




}