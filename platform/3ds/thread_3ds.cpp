/*************************************************************************/
/*  thread_3ds.cpp                                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifdef __3DS__
#include "thread_3ds.h"
#include "platform/3ds/os_3ds.h"
#include "memory.h"


Thread* Thread3ds::create_func_3ds(ThreadCreateCallback p_callback,void * p_user,const Thread::Settings& p_settings) {
	// Get base thread priority for relative priority setting
	int32_t priority;
	svcGetThreadPriority(&priority, (Thread::_main_thread_id == 0) ? CUR_THREAD_HANDLE : Thread::_main_thread_id);
	
	if (p_settings.priority == PRIORITY_LOW)
		priority++;
	else if (p_settings.priority == PRIORITY_HIGH)
		priority--;
	
	ThreadCtrWrapper* thread_wrapper = memnew(ThreadCtrWrapper(p_callback, p_user, priority));
	return memnew(Thread3ds(thread_wrapper));
}

void Thread3ds::make_default() {
	create_func = create_func_3ds;
	get_thread_ID_func = ThreadCtrWrapper::get_thread_ID_func_3ds;
	wait_to_finish_func = wait_to_finish_func_3ds;
}

Thread::ID Thread3ds::get_ID() const {
	return id;
}

void Thread3ds::wait_to_finish_func_3ds(Thread* p_thread) {
	Thread3ds *t = static_cast<Thread3ds*>(p_thread);
	t->thread->wait();
}

Thread3ds::Thread3ds(ThreadCtrWrapper* p_thread) {
	thread = p_thread;
}

Thread3ds::~Thread3ds() {
	memdelete(thread);
}
#endif
