#ifndef INITMSG_H_
#define INITMSG_H_
#include "Message.h"

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
protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;

};

#endif /*INITMSG_H_*/
