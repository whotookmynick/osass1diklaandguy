#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

class Message
{
public:
	//virtual string toString(Message* msg)=0;
	virtual int getSrc()=0;
	virtual int getDes()=0;
	virtual string getContent()=0;
	virtual string getType()=0;
	//void setNext(int nextId)virtual void setNext(int nextId);
protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;


};

#endif /*MESSAGE_H_*/
