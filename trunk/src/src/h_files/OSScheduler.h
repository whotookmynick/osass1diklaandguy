
#ifndef	_OSSCHEDULER_H_
#define	_OSSCHEDULER_H_

/* A scheduler wrapper that can check if a thread is blocked or not */

#include "OSThread.h"

using namespace std;

class OSScheduler
{
	private:
		/* Will be used to verify if the thread is blocked or not */
		int m_curThreadPhase;

		OSThread*	m_curThread;

	protected:
		/* Checks if the current thread is blocked or not.
		 * Returns true if it is blocked, and false otherwise */
		bool checkThreadBlocked();

		/* A notification that the current running thread has changed
		 * to the given thread. */
		void notifyThreadChange(OSThread *newThread);

	public:
		OSScheduler();

		virtual ~OSScheduler() { }

};


#endif	//_OSSCHEDULER_H_

