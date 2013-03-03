/****************************************\
*										*
*		   Object Manager				*
*										*
*						 by fangyuan		*
\****************************************/
#pragma once
#include "SMExportEnable.h"
#include "SMIteratorBase.h"

namespace SMITH
{
//Forward reference declarations
class Agent;
enum ObjectType;
class Object;

class ObjectManager : public LuaExport< ObjectManager >
{
public:
	/*************************************************************************
		Public Interface
	*************************************************************************/
	/*!
	\brief
		Create a new Object.

	\param type
		The type of the object.

	\param serverID
		The serverID of the object.

	\return
		Pointer to the newly created Object object
	*/
	Object* createObject(ObjectType type, int serverId=-1);

	/*!
	\brief
		Destroys the Object with the specified id.

	\param id
		The identity of the Object to be destroyed.  If no such Object exists, nothing happens.

	\return
		Nothing.
	*/
	void destroyObject(int id);

	/*!
	\brief
		Destroys the given Object object

	\param object
		Pointer to the Object to be destroyed.  If no such Object exists, nothing happens.

	\return
		Nothing.
	*/
	void destryoObject(Object* object);

	/*!
	\brief
		Destroys all OBJECT objects registered in the system

	\return
		Nothing
	*/
	void destroyAllObjects(void);

	/*!
	\brief
		Destroys all OBJECT objects registered in the system

	\return
		Nothing
	*/
	void destroyAllObjectsExceptme(void);

	/*!
	\brief
		Find the spec object with the id.

	\return
		Pointer to the Object with the id, return null if not exist.
	*/
	Object* findObject(int id);

	/*!
	\brief
		Find the spec object with this serverid.

	\return
		Client object's it, return -1 when not find;
	*/
	int findServerObject(int idServer);

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

	/*!
	\brief
		Box point a object to lua
	\return
	*/
	bool boxPointObject(int id, const char* szName);

	/*!
	\brief
		UnBox point a object from lua
	\return
	*/
	void unboxPointObject(const char* szName);

	/*!
	\brief
		Get Point to PlayerMySelf
	\return
	*/
	Object* getMySelf(void) { return m_pMySelf; }

public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/

protected:
	/*************************************************************************
		Implementation Interface
	*************************************************************************/

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Agent*					m_pAgent;			//!< Owner.

	typedef std::map< int, Object* >	ObjectRegistry;
	typedef std::map< int, int >	ServerObjectRegistry;
	ObjectRegistry	m_Objects;		//!< All object
	ServerObjectRegistry	m_ServerObjects;		//!< All object

	struct BoxPointObj
	{
		int						id;				//!< the id
		std::string				strName;		//!< name in lua
		LuaPlus::LuaObject*		pMetaTable;		//!< the metatable.
	};
	typedef std::map< std::string, BoxPointObj > BoxPointRegistry;
	BoxPointRegistry m_boxPointRegistry;	//!< All object that box point in lua

	Object*					m_pMySelf;		//!< Point to player myself.

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	IteratorBrowser<ObjectRegistry>	ObjectIterator;

	/*!
	\brief
		Return a ObjectManager::ObjectIterator object to iterate over the available agent objects.
	*/
	ObjectIterator	getIterator(void);

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Implementation method to generate a unique id to use for a Object.
    */
    int generateUniqueId(void);

	int m_uid_counter;

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	ObjectManager(Agent* pAgent);
	~ObjectManager();
};


}