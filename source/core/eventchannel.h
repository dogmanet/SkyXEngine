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

#ifndef _EVENTCHANNEL_H_
#define _EVENTCHANNEL_H_

#include "eventhandlerqueue.h"

class EventChannel
{
public:
	template <typename tEvent, class tHandler>
	void add(tHandler& handler)
	{
		EventHandlerQueue<tEvent>::instance().add(handler);
	}

	template <typename tEvent, class tHandler>
	void remove(const tHandler& handler)
	{
		EventHandlerQueue<tEvent>::instance().remove(handler);
	}

	template <typename tEvent>
	void broadcast(const tEvent& object)
	{
		EventHandlerQueue<tEvent>::instance().broadcast(object);
	}
};
#endif
