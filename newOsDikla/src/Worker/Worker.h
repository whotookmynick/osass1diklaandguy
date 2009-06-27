#ifndef WORKER_H_
#define WORKER_H_
#include "Mailer/Mailer.h"
#include "Worker/OSThread.h"
#include "Messages/Hfiles/Message.h"
#include "Messages/Hfiles/SysMsg.h"
#include "RT.h"
class Worker: public OSThread
{
	public:

		Worker(int id,Mailer & mailer,int* neigbors,int numOfWorkers,int numOfNighbors);
		virtual ~Worker();

		int* _nighbors;
		RT* _myRT;
		int _id;
		Mailer& _mailer;
		int _numOfWorkers;
		bool _active;
		pthread_cond_t _condWait;
		pthread_mutex_t _waitMutex;
		int _numOfNighbors;

		void wait();
		void broadcast();
		void setActive(bool a);
		bool getActive();
		RT* getRT();
		void run();
		int getId();
		/* studentSend - an abstract method that is used to send the message you wish to send */
		bool studentSend(void *args);
		/* studentRecv - an abstract method that is used to receive a message */
		bool studentRecv(void **args);
		void rcvSysMsg(SysMsg*msg);
		void notify();
};

#endif /*WORKER_H_*/
