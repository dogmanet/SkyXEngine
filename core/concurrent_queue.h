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
#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

template <typename T>
class ConcurrentQueue {
private:
	typedef std::queue<T> Queue;
	typedef std::mutex Mutex;
	typedef std::unique_lock<Mutex> ScopedLock;
	typedef std::condition_variable Condition;

public:
	bool empty() const;
	void push(const T& value);
	bool try_pop(T& result);
	T wait_pop();
	std::size_t size() const;

private:
	Queue mQueue;
	mutable Mutex mMutex;
	Condition mCondition;
};

/* implementation */
template <typename T>
bool ConcurrentQueue<T>::empty() const {
	ScopedLock lock(mMutex);
	return mQueue.empty();
}

template <typename T>
std::size_t ConcurrentQueue<T>::size() const {
	ScopedLock lock(mMutex);
	return mQueue.size();
}

template <typename T>
void ConcurrentQueue<T>::push(const T& value) {
  {
    ScopedLock lock(mMutex);
    
    mQueue.push(value);
  }
  
	mCondition.notify_one();
}

template <typename T>
bool ConcurrentQueue<T>::try_pop(T& result) {
	ScopedLock lock(mMutex);

	if (mQueue.empty())
		return false;

	result = mQueue.front();
	mQueue.pop();

	return true;
}

template <typename T>
T ConcurrentQueue<T>::wait_pop() {
	ScopedLock lock(mMutex);

	while (mQueue.empty())
		mCondition.wait(lock);

	T result(mQueue.front());
	mQueue.pop();

	return result;
}
