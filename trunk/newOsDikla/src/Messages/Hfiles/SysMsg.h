#ifndef SYSMSG_H_
#define SYSMSG_H_
#include "Message.h"

class SysMsg : public Message
{
public:
	SysMsg();
	virtual ~SysMsg();

public:
	//string toString(Message* msg);
	int getSrc();
	int getDes();
	string getContent();
	string getType();
	void setNext(int nextId);
	int getNext();
protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;
	int _next;

};

#endif /*SYSMSG_H_*/