#pragma once

#include "WXOperator.h"
#include <OgreVector3.h>

namespace Fairy 
{
	class CNpcPatrolAction;
	class CNpcPatrolEditOperator : public Operator
	{
	public:
		CNpcPatrolEditOperator(void);
		~CNpcPatrolEditOperator(void);

	public:

		unsigned long m_ulPointId;
		unsigned long m_ulRegionId;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CNpcPatrolAction* m_pRegionAction;

		// ��������༭������ָ��.
		void SetRegionAction(CNpcPatrolAction* pRegionAction);

		// ��������ID
		void SetRegionId(unsigned long ulRegionId);

		// ���õ�ID
		void SetPointId(unsigned long ulPointId);

		// ����undo�� redo ������Ҫ��λ��.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

	};


	// �޸�һ�����λ�õ�redo�� undo����.
	class CNpcPatrolEditModifyOperator : public CNpcPatrolEditOperator
	{
	public:
		CNpcPatrolEditModifyOperator(void);
		~CNpcPatrolEditModifyOperator(void);

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
	class CNpcPatrolEditAddOperator : public CNpcPatrolEditOperator
	{
	public:
		CNpcPatrolEditAddOperator(void);
		~CNpcPatrolEditAddOperator(void);

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