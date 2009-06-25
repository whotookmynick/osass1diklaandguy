#ifndef REGMSG_H_
#define REGMSG_H_
#include "Message.h"
class RegMsg : public Message
{
public:
	RegMsg();
	virtual ~RegMsg();
public:

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
