#ifndef MAILER_H_
#define MAILER_H_
#include <stdio.h>
#include "Shell/Shell.h"
#include "Messages/Hfiles/Message.h"
#include "Mailer/MailBox.h"
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <list>
using namespace std;
class Worker;
class Mailer
{
	private:
		Shell& _shell;
		int _numOfWorkers;
		int _inboxSize;
		list<Message* > _inbox;
		vector<Mailbox* > _mailBoxes;
		vector<Worker*> _workers;
		pthread_t _mailerThread;
		pthread_mutex_t _inboxMutex;
		pthread_mutexattr_t _mtxattr;
		//void* mailerWrapper(void* f);
	public:
		Mailer(Shell& shell,int numOfWorkers,int bufSize,int* neigbors);
		virtual ~Mailer();
		bool addMsgToInbox(Message* msg);
		void printInbox(list<Message*> v);
		bool killNode(int nodeId);
		bool rcvPacket(Message* msg);
		bool deliverMsgToMailBox();//deliver ong msg to the next des
		void run();
		void start();
		Message* readMails(int id);
		int* _neigbors;
};

#endif /*MAILER_H_*/
