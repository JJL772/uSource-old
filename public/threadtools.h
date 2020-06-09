/**
 * threadtools.h
 *
 * Threading utils impl
 */
#pragma once

class CThread
{
public:
	CThread();

	/* Calling the CThread baseclass with this parameter specified will execute the passed
	 * function instead of the potentially overridden ThreadFunction */
	CThread(void*(*threadfn)(void*));

	/* Runs the thread */
	virtual void Run() final;

	/* Sends SIGTERM to the thread */
	virtual void Terminate() final;

	/* Sends SIGKILL to the thread */
	virtual void Kill() final;

	/* Join the thread into the current one.
	 * Pauses until it exits */
	virtual void Join() final;

protected:
	/**
	 * The thread function.
	 * Override this in your child classes
	 */
	virtual void* ThreadFunction(void* param) { return nullptr; };

	virtual void OnTerminate() { };
};