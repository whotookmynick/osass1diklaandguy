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

	string getType();
	void printMsg();



};

#endif /*REGMSG_H_*/
