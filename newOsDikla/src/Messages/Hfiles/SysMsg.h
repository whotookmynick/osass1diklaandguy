#ifndef SYSMSG_H_
#define SYSMSG_H_
#include "Message.h"
#include <iostream>
using namespace std;
class SysMsg : public Message
{
public:
	SysMsg(int src,int dst,string textMsgatoi,int next);
	virtual ~SysMsg();

public:
	//string toString(Message* msg);
	string getType();
	void printMsg();



};

#endif /*SYSMSG_H_*/
