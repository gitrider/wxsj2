/****************************************\
*										*
*		   Logic Data Pool				*
*										*
*						 by fangyuan		*
\****************************************/
#pragma once


namespace SMITH
{

//Forward reference declarations
class Agent;
class LoginCharacterList;

class DataPool
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/
	/*!
	\brief
		Init manager
	\return
	*/
	void init(void);
	/*!
	\brief
		Release manager
	\return
	*/
	void release(void);
	/*!
	\brief
		Tickwork
	\return
	*/
	void tick(float fElapsTime);

public:
	/*************************************************************************
		Public Data
	*************************************************************************/
	LoginCharacterList*		m_pLoginCharacterList;		//!< character list data


protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;					//!< Owner.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	DataPool(Agent* pAgent);
	~DataPool();
};


}