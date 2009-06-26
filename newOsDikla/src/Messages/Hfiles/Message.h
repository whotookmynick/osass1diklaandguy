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
	Message(int src,int dst,string textMsgatoi,int next);
	virtual ~Message();
	virtual int getSrc();
	virtual int getDes();
	virtual string getContent();
	virtual string getType()=0;
	virtual void setNext(int nextId);
	virtual int getNext();
	virtual void printMsg()=0;
protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;
	int _next;


};

#endif /*MESSAGE_H_*/

