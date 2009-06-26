#ifndef INITMSG_H_
#define INITMSG_H_
#include "Message.h"
#include <iostream>
using namespace std;
class InitMsg : public Message
{
public:
	InitMsg();
	virtual ~InitMsg();

public:
	//string toString(Message* msg);
	int getSrc();
	int getDes();
	string getContent();
	string getType();
	void setNext(int nextId);
	int getNext();
	void printMsg();

protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;
	int _next;

};

#endif /*INITMSG_H_*/
