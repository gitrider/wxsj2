#include "StdAfx.h"
#include "SMObjectManager.h"
#include "SMAgentManager.h"
#include "SMAgent.h"
#include "SMObject.h"

namespace SMITH
{

/*************************************************************************
	Constructor
*************************************************************************/
ObjectManager::ObjectManager(Agent* pAgent)
	: LuaExport< ObjectManager >("ObjectManager", pAgent->getLuaState()),
	m_pAgent(pAgent), 
	m_uid_counter(0),
	m_pMySelf(0)
{
	assert(pAgent);

}

/*************************************************************************
	Destructor
*************************************************************************/
ObjectManager::~ObjectManager()
{
}

/*************************************************************************
	Init ObjectManager Manager
*************************************************************************/
void ObjectManager::init(void)
{

}

/*************************************************************************
	Release ObjectManager manager
*************************************************************************/
void ObjectManager::release(void)
{


}

/*************************************************************************
	tickwork ObjectManager manager
*************************************************************************/
void ObjectManager::tick(float fElapsTime)
{
	ObjectRegistry::iterator it;
	for(it=m_Objects.begin(); it!=m_Objects.end(); it++)
	{
		it->second->tick(fElapsTime);
	}
}

/*************************************************************************
	Create a new Object.
*************************************************************************/
Object* ObjectManager::createObject(ObjectType type, int serverId)
{
	Object* newObject = new Object(generateUniqueId(), type, m_pAgent, serverId);

	std::map< int, Object* >::iterator it;
	it = m_Objects.find( newObject->getID() );
	if( it == m_Objects.end() )
	{
		m_Objects.insert(std::make_pair(newObject->getID(), newObject));
		m_ServerObjects.insert(std::make_pair(serverId, newObject->getID()));
	}

	// check if myslef
	if(type == OT_PLAYERMYSELF)
	{
		m_pMySelf = newObject;
		boxPointObject(newObject->getID(), "MySelf");
	}

	return newObject;
}

/*************************************************************************
	Find the spec object with this serverid.
*************************************************************************/
int ObjectManager::findServerObject(int idServer)
{
	std::map< int, int >::iterator it;
	it = m_ServerObjects.find(idServer);
	if(it == m_ServerObjects.end()) return -1;

	return it->second;
}


/*************************************************************************
	Destroys the Object with the specified id.
*************************************************************************/
void ObjectManager::destroyObject(int id)
{
	ObjectRegistry::iterator	pos = m_Objects.find(id);

	if (pos != m_Objects.end())
	{
		// check box point
		BoxPointRegistry::iterator it;
		for(it = m_boxPointRegistry.begin(); it!=m_boxPointRegistry.end(); it++)
		{
			if(it->second.id == pos->second->getID())
			{
				unboxPointObject(it->second.strName.c_str());
				break;
			}
		}

		int nServerid = pos->second->getServerID();

		delete pos->second;
		m_Objects.erase(pos);

		ServerObjectRegistry::iterator iso = m_ServerObjects.find( nServerid );;
		if( iso != m_ServerObjects.end() )
		{
			m_ServerObjects.erase( iso );
		}
	}
}

/*************************************************************************
	Destroys the given Object object
*************************************************************************/
void ObjectManager::destryoObject(Object* object)
{
	if (object != 0)
	{
		destroyObject(object->getID());
	}
}

/*************************************************************************
	Destroys the given Object object
*************************************************************************/
void ObjectManager::destroyAllObjects(void)
{
	ObjectRegistry::iterator it;
	for(it=m_Objects.begin(); it!=m_Objects.end(); it++)
	{
		destryoObject(it->second);
	}
	m_Objects.clear();
}

/*************************************************************************
	Destroys the given Object object except me
*************************************************************************/
void ObjectManager::destroyAllObjectsExceptme(void)
{
	ObjectRegistry::iterator it;
	for(it=m_Objects.begin(); it!=m_Objects.end(); it++)
	{
		if( it->second->getType() == OT_PLAYERMYSELF ) continue;

		destryoObject(it->second);
	}
	m_Objects.clear();
}

/*************************************************************************
	Destroys the given Object object
*************************************************************************/
Object* ObjectManager::findObject(int id)
{
	ObjectRegistry::iterator	pos = m_Objects.find(id);

	if (pos != m_Objects.end())
	{
		return pos->second;
	}

	return 0;
}

/*************************************************************************
	Return a ObjectManager::ObjectIterator object to iterate over 
	the available Object objects.
*************************************************************************/
ObjectManager::ObjectIterator ObjectManager::getIterator(void)
{
	return ObjectIterator(m_Objects.begin(), m_Objects.end());
}

/*************************************************************************
	Implementation method to generate a unique id to use for a Object.
*************************************************************************/
int ObjectManager::generateUniqueId(void)
{
    int ret = ++m_uid_counter;

	if(ret < 0) ret = 0;
	return ret;
}

/*************************************************************************
	UnBox point a object to lua
*************************************************************************/
void ObjectManager::unboxPointObject(const char* szName)
{
	BoxPointRegistry::iterator itFind = m_boxPointRegistry.find(szName);
	//not defined
	if(itFind == m_boxPointRegistry.end()) return;

	BoxPointObj& thisObj = itFind->second;
	delete thisObj.pMetaTable;

	m_boxPointRegistry.erase(itFind);
}

/*************************************************************************
	Box point a object to lua
*************************************************************************/
bool ObjectManager::boxPointObject(int id, const char* szName)
{
	BoxPointRegistry::iterator itFind = m_boxPointRegistry.find(szName);
	// already defined
	if(itFind != m_boxPointRegistry.end())
	{
		unboxPointObject(szName);
		m_boxPointRegistry.erase(szName);
	}
	
	//find obj
	ObjectRegistry::iterator itObj = m_Objects.find(id);
	if(itObj == m_Objects.end()) return false;

	//create metatable
	LuaPlus::LuaState* pState = m_pAgent->getLuaState();
	LuaPlus::LuaObject*	pMetaTable = new LuaPlus::LuaObject(pState);
	*pMetaTable = pState->GetGlobals().CreateTable(szName);
	pMetaTable->SetObject("__index", *pMetaTable);

	LuaObject obj = pState->BoxPointer(itObj->second);
	obj.SetMetaTable(*pMetaTable);
	pState->GetGlobals().SetObject(szName, obj);

	//register function
	pMetaTable->RegisterObjectFunctor("Goto", &Object::Lua_Goto);
	pMetaTable->RegisterObjectFunctor("GetPosition", &Object::Lua_GetPosition);
	pMetaTable->RegisterObjectFunctor("GetAIState", &Object::Lua_GetAIState);
	pMetaTable->RegisterObjectFunctor("SetAIState", &Object::Lua_SetAIState);
	pMetaTable->RegisterObjectFunctor("GetLogicMode", &Object::Lua_GetLogicMode);
	pMetaTable->RegisterObjectFunctor("Chat", &Object::Lua_Chat);

	//add to map
	BoxPointObj newObj;
	newObj.id = id;
	newObj.pMetaTable = pMetaTable;
	newObj.strName = szName;
	m_boxPointRegistry.insert(std::make_pair(szName, newObj));

	return true;
}

}