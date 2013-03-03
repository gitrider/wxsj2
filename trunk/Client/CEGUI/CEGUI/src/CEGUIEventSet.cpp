/************************************************************************
	filename: 	CEGUIEventSet.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the EventSet class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUIEventSet.h"
#include "CEGUIExceptions.h"
#include "CEGUIGlobalEventSet.h"
#include "CEGUIScriptModule.h"
#include "CEGUISystem.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor
*************************************************************************/
EventSet::EventSet() :
	d_muted(false)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
EventSet::~EventSet(void)
{
	removeAllEvents();
}


/*************************************************************************
	Add a new event to the EventSet
*************************************************************************/
void EventSet::addEvent(const String& name)
{
	if (isEventPresent(name))
	{
		throw AlreadyExistsException("An event named '" + name + "' already exists in the EventSet.");
	}

	d_events[name] = new Event(name);
}


/*************************************************************************
	Remove an event from the EventSet
*************************************************************************/
void EventSet::removeEvent(const String& name)
{
	EventMap::iterator pos = d_events.find(name);

	if (pos != d_events.end())
	{
		delete pos->second;
		d_events.erase(pos);
	}

}


/*************************************************************************
	Remove all events from the EventSet
*************************************************************************/
void EventSet::removeAllEvents(void)
{
	EventMap::iterator pos = d_events.begin();
	EventMap::iterator end = d_events.end()	;

	for (; pos != end; ++pos)
	{
		delete pos->second;
	}

	d_events.clear();
}


/*************************************************************************
	Check to see if an event is available
*************************************************************************/
bool EventSet::isEventPresent(const String& name)
{
	return (d_events.find(name) != d_events.end());
}

#if 0 // Not support yet!

/*************************************************************************
	Subscribe to a scripted event (no group)
*************************************************************************/
Event::Connection EventSet::subscribeScriptedEvent(const String& name, const String& subscriber_name)
{
	//return subscribeEvent(name, Event::Subscriber(ScriptFunctor(subscriber_name)));
	ScriptModule* sm = System::getSingletonPtr()->getScriptingModule();
	if (!sm)
	{
	   throw InvalidRequestException("[EventSet::subscribeScriptedEvent] No scripting module is available");
	}
	return sm->subscribeEvent(this, name, subscriber_name);
}

/*************************************************************************
	Subscribe to a scripted event
*************************************************************************/
Event::Connection EventSet::subscribeScriptedEvent(const String& name, Event::Group group, const String& subscriber_name)
{
        //return subscribeEvent(name, group, Event::Subscriber(ScriptFunctor(subscriber_name)));
        ScriptModule* sm = System::getSingletonPtr()->getScriptingModule();
	if (!sm)
	{
	   throw InvalidRequestException("[EventSet::subscribeScriptedEvent] No scripting module is available");
	}
	return sm->subscribeEvent(this, name, group, subscriber_name);
}

#endif

/*************************************************************************
	Subscribe to an event (no group)
*************************************************************************/
Event::Connection EventSet::subscribeEvent(const String& name, Event::Subscriber subscriber)
{
    // do subscription & return connection
    return getEventObject(name, true)->subscribe(subscriber);
}

bool EventSet::unsubscribeEvent(const String& name, Event::Subscriber subscriber)
{
	return getEventObject(name,true)->unsubscribe(subscriber);
}

/*************************************************************************
	Subscribe to an event group
*************************************************************************/
Event::Connection EventSet::subscribeEvent(const String& name, Event::Group group, Event::Subscriber subscriber)
{
    // do subscription with group & return connection
    return getEventObject(name, true)->subscribe(group, subscriber);
}

bool EventSet::unsubscribeEvent(const String& name, Event::Group group, Event::Subscriber subscriber)
{
	return getEventObject(name, true)->unsubscribe(subscriber,group);
}

/*************************************************************************
	Fire / Trigger an event
*************************************************************************/
void EventSet::fireEvent(const String& name, EventArgs& args, const String& eventNamespace)
{
    // handle global events
    GlobalEventSet::getSingleton().fireEvent(name, args, eventNamespace);
    // handle local event
    fireEvent_impl(name, args);
}


/*************************************************************************
	Return whether the EventSet is muted or not.	
*************************************************************************/
bool EventSet::isMuted(void) const
{
	return d_muted;
}


/*************************************************************************
	Set the mute state for this EventSet.	
*************************************************************************/
void EventSet::setMutedState(bool setting)
{
	d_muted = setting;
}


/*************************************************************************
    Return the named Event object, optionally adding it to the set
    if needed
*************************************************************************/
Event* EventSet::getEventObject(const String& name, bool autoAdd)
{
    EventMap::iterator pos = d_events.find(name);

    // if event did not exist, add it and then find it.
    if (pos == d_events.end())
    {
        if (autoAdd)
        {
            addEvent(name);
            return d_events.find(name)->second;
        }
        else
        {
            return 0;
        }
    }

    return pos->second;
}


/*************************************************************************

*************************************************************************/
void EventSet::fireEvent_impl(const String& name, EventArgs& args)
{
    // find event object
    Event* ev = getEventObject(name);

    // fire the event if present and set is not muted
    if ((ev != 0) && !d_muted)
        (*ev)(args);
}


/*************************************************************************
	Return a EventSet::EventIterator object to iterate over the available
	events.
*************************************************************************/
EventSet::EventIterator EventSet::getIterator(void) const
{
	return EventIterator(d_events.begin(), d_events.end());
}


} // End of  CEGUI namespace section
