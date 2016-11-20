/*
* Copyright (c) 2014 GrandMaster (gijsber@gmail)
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef _EVENTHANDLERQUEUE_H_
#define _EVENTHANDLERQUEUE_H_

#include "eventbridge.h"
#include <list>
#include <memory>

class EventChannel; //forward definition

template <typename tEvent>
class EventHandlerQueue
{
private: //the entire class is private, so no object may use it
	friend class EventChannel; //except for the EventChannel

	typedef EventHandler<tEvent> EventHandlerType;

	typedef std::shared_ptr<EventHandlerType> EventHandlerPtr; //optional, if you want to use raw pointers uncomment below

	typedef std::list<EventHandlerPtr> HandlerList;
	typedef typename HandlerList::iterator HandlerIterator;
	typedef typename HandlerList::const_iterator ConstHandlerIterator;

	HandlerList mHandlerList;

	static EventHandlerQueue& instance()
	{
		static EventHandlerQueue anInstance;
		return anInstance;
	}

	template <class tHandler>
	void add(tHandler& handler)
	{
		mHandlerList.push_back(EventHandlerPtr(new EventBridge<tEvent, tHandler>(handler)));
	}

	void broadcast(const tEvent& object)
	{
		ConstHandlerIterator next;
		if(!mHandlerList.empty())
		{
			for(ConstHandlerIterator it = mHandlerList.begin(); it != mHandlerList.end();)
			{
				//the handle function might invalidate the iterator, so make a copy and advance immediately
				next = it++;
				(*next)->handle(object);
			}
		}
	}

	//comparison predicate class (for removing handlers)
	template <class tHandler>
	class PointsToSame
	{
	public:
		typedef EventBridge<tEvent, tHandler> BridgeType;

		const tHandler& mHandler;

		PointsToSame(const tHandler& h): mHandler(h)
		{
		}

		bool operator()(EventHandlerPtr ptr)
		{
			return ((*std::static_pointer_cast<BridgeType>(ptr)) == mHandler);
		}
	};

	template <class tHandler>
	void remove(const tHandler& handler)
	{
		PointsToSame<tHandler> pts(handler);
		mHandlerList.remove_if(pts);
	}
};

#endif
