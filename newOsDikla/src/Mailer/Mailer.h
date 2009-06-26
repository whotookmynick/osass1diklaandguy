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
using namespace std;

class Mailer
{
	private:
		Shell& _shell;
		int _numOfWorkers;
		int _inboxSize;
		vector<Message* > _inbox;
		vector<Mailbox* > _mailBoxes;

	public:
		Mailer(Shell& shell,int numOfWorkers,int bufSize);
		virtual ~Mailer();
		bool addMsgToInbox(Message* msg);
		void printInbox(vector<Message*> v);
		bool killNode(int nodeId);
		bool rcvPacket(Message* msg);
		bool deliverMsgToMailBox();//deliver ong msg to the next des

};

#endif /*MAILER_H_*/
