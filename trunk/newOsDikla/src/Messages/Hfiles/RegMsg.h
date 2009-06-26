#ifndef REGMSG_H_
#define REGMSG_H_
#include "Message.h"
#include <iostream>
using namespace std;

class RegMsg : public Message
{
public:
	RegMsg(int src,int dst,string textMsgatoi,int next);
	RegMsg();
	virtual ~RegMsg();
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

#endif /*REGMSG_H_*/
