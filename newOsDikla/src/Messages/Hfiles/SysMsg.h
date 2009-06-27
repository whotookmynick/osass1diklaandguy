#ifndef SYSMSG_H_
#define SYSMSG_H_
#include "Message.h"
#include <iostream>
#include "Worker/RT.h"
using namespace std;
class SysMsg : public Message
{
public:
	SysMsg(int src,int dst,string msgContent,int next);
	virtual ~SysMsg();

public:
	//string toString(Message* msg);
	string getType();
	void printMsg();


	string _msgContent;
	RT* _rt;
	void setRT(RT* rt);
	RT* getRT();
};

#endif /*SYSMSG_H_*/
