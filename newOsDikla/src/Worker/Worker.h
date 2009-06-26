#ifndef WORKER_H_
#define WORKER_H_
#include "Mailer/Mailer.h"
#include "Worker/OSThread.h"
#include "Messages/Hfiles/Message.h"
class Worker: public OSThread
{
	public:
		Worker(int id,Mailer & mailer);
		virtual ~Worker();
		int _id;
		void run();
		Mailer& _mailer;
		int getId();
		/* studentSend - an abstract method that is used to send the message you wish to send */
		bool studentSend(void *args);

		/* studentRecv - an abstract method that is used to receive a message */
		bool studentRecv(void **args);

};

#endif /*WORKER_H_*/
