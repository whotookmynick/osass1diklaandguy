#ifndef INITMSG_H_
#define INITMSG_H_
#include "Message.h"
#include <iostream>
using namespace std;
class InitMsg : public Message
{
public:
	InitMsg(int src,int dst,string textMsgatoi,int next);
	virtual ~InitMsg();

public:

	string getType();
	void printMsg();

	string _msgContent;
};

#endif /*INITMSG_H_*/
