/*
 * Mailer.h
 *
 *  Created on: Jun 24, 2009
 *      Author: dikla
 */

#ifndef MAILER_H_
#define MAILER_H_




#include "Message.h"
#include "Scheduler.h"
#include "Shell.h"
#include "Worker.h"
#include "MailBox.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;


 class Mailer{
	 private:

	    	Scheduler* _scheduler;
			pthread_t _schedulerThread;
			Shell& _shell;
		// ****************workers*******************
			int _numOfWorkers;
			std::vector<Worker*> _workersVector;
			std::vector<Mailbox> _mailboxesVector;

			// ****************messages & inbox*******************
			std::queue<Message*> _inboxQ;
		    unsigned int _inboxSize;
			bool _msgInProgress;
			bool _noMsgInProgress;
			pthread_mutex_t _msgInProgressMutex;
			pthread_mutex_t _inboxMutex;

	 public:

		 Mailer(Shell& shell, int numOfWorkers, int bfrSize);
		 virtual ~Mailer();

		 // ***********************************************
		 void run();
		 void start();
		 void setScheduler(Scheduler* s);

		 // ****************handle messages***********************
		 bool setMsgInInbox(Message* m);//if inbox full return false
		// Reads the messages from the mailbox to msgQ
		bool receive(int mailboxID, std::queue<Message*>& msgQ);
		Message* readMsgFromInbox();
		void setMsgInProgress(bool en);//_noMsgInProgress==true or false

		// ****************handle workers***********************
		Worker& getWorker(int wID);
		void setWorkerOff(int wID);
		void setWorkerOn(int wID);
		void addWorker(Worker* worker);
		Mailbox& getMailbox(int mailboxID);
		void reInitWorkers(int workerID);
	};




#endif /* MAILER_H_ */
