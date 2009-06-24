/*
 * SampleThread.cpp
 *
 *  Created on: Jun 24, 2009
 *      Author: dikla
 */

#ifndef SAMPLETHREAD_CPP_
#define SAMPLETHREAD_CPP_

#include "SampleThread.h"
#include <iostream>
#include <sstream>

SampleThread::SampleThread()
{
	cout << "SampleThread initialized" << endl;
}

SampleThread::~SampleThread()
{
}

void SampleThread::run()
{
	// This simple thread just "sends" two messages,
	// and "receives" two messages.

	// Create and send message 1
	Message *sendMsg1 = new Message();
	sendMsg1->content = "Message sending 1";
	send(sendMsg1);
	delete sendMsg1;

	// Create and send message 2
	Message *sendMsg2 = new Message();
	sendMsg2->content = "Message sending 2";
	send(sendMsg2);
	delete sendMsg2;

	// Receive message 1
	Message *recvMsg1 = NULL;
	recv((void **)&recvMsg1);
	cout << "Received message: " << recvMsg1->content << endl;
	delete recvMsg1;

	// Receive message 2
	Message *recvMsg2 = NULL;
	recv((void **)&recvMsg2);
	cout << "Received message: " << recvMsg2->content << endl;
	delete recvMsg2;

}

bool SampleThread::studentSend(void *args)
{
	Message* msg = (Message *)args;
	// Print the message on the screen
	cout << "studentSend is sending the message: " << msg->content << endl;

	return true;
}


bool SampleThread::studentRecv(void **args)
{
	static int count = 1;
	// Create a sample message that will be returned
	Message *msg = new Message();
	ostringstream msgStream;
	msgStream << "Message no. " << count;
	msg->content = msgStream.str();

	// Return the message in args
	*args = msg;

	count++;
	return true;
}

//TODO
/* The main function - initialize the sample thread and execute it
int main(void)
{
	OSThread *th = new SampleThread();

	// Start the thread (will create the thread and execute its run method)
	th->start();

	// Give the thread some quanta
	th->setQuanta(4);

	// Wait a few seconds before exiting, let the thread do what it needs
	sleep(10);

	delete th;
}
*/

#endif /* SAMPLETHREAD_CPP_ */
