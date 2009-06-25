#ifndef MAILER_H_
#define MAILER_H_
#include <stdio.h>
#include "Shell.h"
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
		vector<string> _inbox;

	public:
		Mailer(Shell& shell,int numOfWorkers,int bufSize);
		virtual ~Mailer();
		bool addMsgToInbox(const string& msg);
		void printInbox(const vector<void*>& v);
		bool killNode(int nodeId);
		bool rcvPacket(string sourceID,string targetID,string textMsg);
};

#endif /*MAILER_H_*/
