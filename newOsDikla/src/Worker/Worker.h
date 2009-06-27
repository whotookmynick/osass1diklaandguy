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

		Worker(int id,Mailer & mailer,int* neigbors,int numOfWorkers);
		virtual ~Worker();

		int* _nighbors;
		RT* _myRT;
		void broadcast();
		int _id;
		Mailer& _mailer;
		int _numOfWorkers;


		void run();
		int getId();
		/* studentSend - an abstract method that is used to send the message you wish to send */
		bool studentSend(void *args);
		/* studentRecv - an abstract method that is used to receive a message */
		bool studentRecv(void **args);
		void rcvSysMsg(SysMsg*msg);
};

#endif /*WORKER_H_*/
