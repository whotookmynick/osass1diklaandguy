#ifndef REGMSG_H_
#define REGMSG_H_
#include "Message.h"
class RegMsg : public Message
{
public:
	RegMsg(int src,int dst,string textMsgatoi);
	RegMsg();
	virtual ~RegMsg();
public:
	//string toString(Message* msg);
	int getSrc();
	int getDes();
	string getContent();
	string getType();
protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;

};

#endif /*REGMSG_H_*/
