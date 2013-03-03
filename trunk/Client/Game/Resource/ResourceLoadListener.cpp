#include "StdAfx.h"
#include "ResourceLoadListener.h"


VOID ScriptParsingListener::resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
{
	_groupName = groupName;
	_scriptCount = scriptCount;
	_scriptIndex = 0;
	_scriptName.clear();
//	AxTrace(0, 0, "Parsing resource group: %s", groupName.c_str());
}

VOID ScriptParsingListener::scriptParseStarted(const Ogre::String& scriptName,bool& skipThisScript)
{
	_scriptName = scriptName;
//	AxTrace(0, 0, "Parsing script: %s", scriptName.c_str());
}

VOID ScriptParsingListener::scriptParseEnded(const Ogre::String& scriptName, bool skipped)
{
	_scriptName.clear();
	++_scriptIndex;
}

VOID ScriptParsingListener::resourceGroupScriptingEnded(const Ogre::String& groupName)
{
	_groupName.clear();
	_scriptCount = 0;
}
