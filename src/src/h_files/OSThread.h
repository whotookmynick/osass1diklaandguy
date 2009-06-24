
#ifndef	_OSTHREAD_H_
#define	_OSTHREAD_H_

#include <pthread.h>
#include <string>

using namespace std;

/* A wrapper class for the worker threads - wraps a pthread and executes the run method */
class OSThread
{

	private:
		/* Quanta related data */
		int	m_msgQuanta; // How many messages can we send
		pthread_mutex_t	m_quantaLock;
		pthread_cond_t	m_quantaCond;

		/* Holds the ID of the created thread */
		pthread_t	m_thread;

		/* Saves the thread's phase - inner working status */
		int	m_phaseCounter;
		pthread_mutex_t	m_phaseLock;

		/* Increments the thread's phase */
		void incPhase();

		/* A flag that indicates if the thread has yielded or can continue running */
		bool m_yielded;

	public:
		/* C'tor - initialize the wrapped thread */
		OSThread();

		/* D'tor - destroy the wrapped thread */
		virtual ~OSThread();

		/* run - an abstract method that will be called when the
		 * thread starts running */
		virtual void run() = 0;

		/* OS-send - used to send a message using the scheduler */
		bool send(void* args);

		/* OS-recv - used to receive a message using the scheduler */
		bool recv(void **args);

		/* studentSend - an abstract method that is used to send the message you wish to send */
		virtual bool studentSend(void *args) = 0;

		/* studentRecv - an abstract method that is used to receive a message */
		virtual bool studentRecv(void **args) = 0;

		/* The start method - the method that starts the actual thread
		 * (executes the run method in the new thread) */
		void start();

		/* Quanta getter and setter */
		int getQuanta();
		void setQuanta(int newQuanta);

		/* Getter for the wrapped thread */
		pthread_t getThread();

		/* Returns the phase counter - the phase of the current thread */
		int getPhase();

		/* Yield a thread - the thread just stops until a new quanta is set for it */
		void yield();

};

class OSThreadException
{
	public:
		string	message;
};

#endif	// _OSTHREAD_H_
