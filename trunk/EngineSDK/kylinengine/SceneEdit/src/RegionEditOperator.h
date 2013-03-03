#ifndef __REGIONEDITOPERATOR_H__
#define __REGIONEDITOPERATOR_H__
#pragma once
#include "WXOperator.h"
#include <OgreVector3.h>

namespace Fairy 
{
	class CRegionEditAction;
	class CRegionEditOperator : public Operator
	{
	public:
		CRegionEditOperator(void);
		~CRegionEditOperator(void);

	public:

		unsigned long m_ulPointId;
		unsigned long m_ulRegionId;
		unsigned long m_ulUnDoRegionId;
		unsigned long m_ulReDoRegionId;

		bool m_bFireUI;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CRegionEditAction* m_pRegionAction;

		// ��������༭������ָ��.
		void SetRegionAction(CRegionEditAction* pRegionAction);

		// ��������ID
		void SetRegionId(unsigned long ulRegionId);

		// ���õ�ID
		void SetPointId(unsigned long ulPointId);

		// ����undo�� redo ������Ҫ��λ��.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

		// ���ñ任������ID
		void SetChangeRegionId(unsigned long ulUnDoRegionId,unsigned long ulReDoRegionId);

		// �����Ƿ�֪ͨUI
		void SetFireUI(bool fire);

	};


	// �޸�һ�����λ�õ�redo�� undo����.
	class CRegionEditModifyOperator : public CRegionEditOperator
	{
	public:
		CRegionEditModifyOperator(void);
		~CRegionEditModifyOperator(void);

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

	// ѡ������redo�� undo����.
	class CRegionChangeSelectOperator : public CRegionEditOperator
	{
	public:
		CRegionChangeSelectOperator(void);
		~CRegionChangeSelectOperator(void);

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
	class CRegionEditAddPointOperator : public CRegionEditOperator
	{
	public:
		CRegionEditAddPointOperator(void);
		~CRegionEditAddPointOperator(void);

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
	// ɾ��һ�����redo�� undo����.
	//
	//
	class CRegionEditDeletePointOperator : public CRegionEditOperator
	{
	public:
		CRegionEditDeletePointOperator(void);
		~CRegionEditDeletePointOperator(void);

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

#endif //__REGIONEDITOPERATOR_H__