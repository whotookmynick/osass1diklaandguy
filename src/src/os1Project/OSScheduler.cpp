/*
 * OSScheduler.cpp
 *
 *  Created on: Jun 24, 2009
 *      Author: dikla
 */


#include "OSScheduler.h"
#include <iostream>


OSScheduler::OSScheduler() :
	m_curThreadPhase(0)
{
}

bool OSScheduler::checkThreadBlocked()
{
	bool result = false;

	if (m_curThread != NULL)
	{
		// Check if the current thread's phase has changed from the last check
		int readThreadPhase = m_curThread->getPhase();

		// Check if the thread is still in the same phase
		if (m_curThreadPhase == readThreadPhase)
			result = true;

		m_curThreadPhase = readThreadPhase;
	}

	return result;
}

void OSScheduler::notifyThreadChange(OSThread *newThread)
{
	m_curThread = newThread;

	// Save the thread's current phase
	m_curThreadPhase = m_curThread->getPhase();
}

