#pragma once
#include "WXOperator.h"
#include <OgreVector3.h>
#include "GrowPointEditAction.h"
namespace Fairy 
{

	class CGrowPointOperator: public Operator
	{
	public:
		CGrowPointOperator(void);
		~CGrowPointOperator(void);

	public:

	    virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
        virtual String getHelp(void) const;
	    virtual void undo(void);
        virtual void redo(void);



		// ����ʵ��id
		unsigned long m_ulInstanceId;

		// �������ε�
        unsigned long m_ulTypeId;

		// ��������
		String m_strName;

		Ogre::Vector3 m_vectorUnDoPos;
		Ogre::Vector3 m_vectorReDoPos;

		CGrowPointEditAction* m_pAction;

		// ��������༭������ָ��.
		void SetAction(CGrowPointEditAction* pAction);

		// ����ʵ��id
		void SetInstanceId(unsigned long ulInstanceId);

		// ���ù�������id
		void SetTypeId(unsigned long ulTypeId);

		// ����undo�� redo ������Ҫ��λ��.
		void SetPosition(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos);

		// ��������.
		void SetName(String& strName);


	};




	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// ���һ���¹�
	//
	class CGrowPointAddOperator : public CGrowPointOperator
	{
	public:
		CGrowPointAddOperator(void);
		~CGrowPointAddOperator(void);

	public:
		

		
		virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
	    virtual String getHelp(void) const;


		virtual void undo(void);
        virtual void redo(void);

	};



	/////////////////////////////////////////////////////////////////////////////////
	//
	// ɾ��һ������Ĳ���.
	//
	//
	class CGrowPointDelOperator : public CGrowPointOperator
	{
	public:
		CGrowPointDelOperator(void);
		~CGrowPointDelOperator(void);

	public:
		

		
		virtual const String& getGroupName(void) const;
        virtual String getDescription(void) const;
	    virtual String getHelp(void) const;

      	virtual void undo(void);
		virtual void redo(void);

	};




















}
















