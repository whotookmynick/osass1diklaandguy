
#include "OSThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/* Declaration of the wrapper function */
extern "C"
{
	void *wrapper_func(void *args)
	{
		OSThread *curThread = (OSThread *)args;
		curThread->run();
	}
}

OSThread::OSThread() :
	m_msgQuanta(0),
	m_phaseCounter(0),
	m_yielded(false)
{
	// Initialize quanta related data
	if (pthread_mutex_init(&m_quantaLock, NULL) != 0)
	{
		perror("Quanta lock initialization");
		exit(1);
	}
	if (pthread_cond_init(&m_quantaCond, NULL) != 0)
	{
		perror("Quanta cond var initialization");
		exit(1);
	}
	if (pthread_mutex_init(&m_phaseLock, NULL) != 0)
	{
		perror("Phase lock initialization");
		exit(1);
	}
}

void OSThread::start(){

	// Initialize the thread itself
	if (pthread_create(&m_thread, NULL, wrapper_func, this) != 0)
	{
		perror("OSThread thread creation failed");
		exit(1);
	}
}

bool OSThread::send(void *args)
{
	// OS-Send - check the quanta and block if no more quanta left.
	// Otherwise, decrement the quanta and send a message(using the studentSend method)

	// Increment phase
	incPhase();

	// add message to mailer's buffer
	pthread_mutex_lock(&m_quantaLock);
	while (m_msgQuanta <= 0)
		pthread_cond_wait(&m_quantaCond, &m_quantaLock);
	// After waiting (if needed), the process can resume work
	m_msgQuanta--;
	pthread_mutex_unlock(&m_quantaLock);

	// Call student's send method
	bool result = studentSend(args);

	// Increment phase
	incPhase();

	return result;
}

bool OSThread::recv(void **args)
{
	// OS-Recv - check the quanta and block if no more quanta left.
	// Otherwise, decrement the quanta and receive a message(using the studentRecv method)

	// Increment phase
	incPhase();

	pthread_mutex_lock(&m_quantaLock);
	while (m_msgQuanta <= 0)
		pthread_cond_wait(&m_quantaCond, &m_quantaLock);
	// After waiting (if needed), the process can resume work
	m_msgQuanta--;
	pthread_mutex_unlock(&m_quantaLock);

	// Call student's recv method
	bool result = studentRecv(args);

	// Increment phase
	incPhase();

	return result;
}

OSThread::~OSThread()
{
	pthread_mutex_destroy(&m_phaseLock);
	// Destroy the quanta related data
	pthread_cond_destroy(&m_quantaCond);
	pthread_mutex_destroy(&m_quantaLock);
}


int OSThread::getQuanta()
{
	return m_msgQuanta;
}

void OSThread::setQuanta(int newQuanta)
{
	pthread_mutex_lock(&m_quantaLock);
	m_msgQuanta = newQuanta;
	m_yielded = false;
	// signal if there is any one waiting
	pthread_cond_signal(&m_quantaCond);
	pthread_mutex_unlock(&m_quantaLock);
}

pthread_t OSThread::getThread()
{
	return m_thread;
}

int OSThread::getPhase()
{
	int result = 0;

	// Locked in order to avoid running concurrently with phase increment
	pthread_mutex_lock(&m_phaseLock);
	result = m_phaseCounter;
	pthread_mutex_unlock(&m_phaseLock);
	return result;
}

void OSThread::incPhase()
{
	pthread_mutex_lock(&m_phaseLock);
	m_phaseCounter++;
	pthread_mutex_unlock(&m_phaseLock);
}

void OSThread::yield()
{
	pthread_mutex_lock(&m_quantaLock);
	m_yielded = true;
	while (m_yielded)
		pthread_cond_wait(&m_quantaCond, &m_quantaLock);
	// When the setQuanta will be called, the m_yielded value will be false
	pthread_mutex_unlock(&m_quantaLock);
}
