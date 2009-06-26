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


};

#endif /*WORKER_H_*/
