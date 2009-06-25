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

	virtual int getSrc()=0;
	virtual int getDes()=0;
	virtual string getContent()=0;
	virtual string getType()=0;
protected:
	int _sendTo;
	int _receiveFrom;
	//string _msgType;//rt message system message
	string _msgContent;


};

#endif /*MESSAGE_H_*/