#include "Messages/Hfiles/SysMsg.h"
#include "Messages/Hfiles/RegMsg.h"
#include "Messages/Hfiles/InitMsg.h"
#include <pthread.h>
#include <queue>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

class Mailbox {
	private:

		pthread_mutex_t _QMutex;//define rec mutex
		list<Message*> _sysQueue;//TODO remove
		list<Message*> _RegQueue;
		list<Message*> _IntQueue;


	public://pthread_mutex_lock(_QMutex) and unlock
		Mailbox();
		virtual ~Mailbox();

		bool readMsg(std::queue<Message*>& destinyMsgQ);// Reads messages from the mailbox to destinyMsgQ.
														//System messages are read first.true on success
		bool insertMsg(Message* m);//insert message in the mailbox
		void printMailes();
		void printQ(list<Message*> l);
		Message* readMsg();
};
