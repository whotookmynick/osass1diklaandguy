#ifndef MASSAGE_H_
#define MASSAGE_H_

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;
//#include "Worker.h"

class Message {
public:
	Message(string content,int rcvId, int sndId,string msgType);
	~Message();
	//Msg getMsg();
	void setContend(string content);
	string getMsgType();


private:
	int _sendTo;
	int _receiveFrom;
	string _msgType;//rt message system message
	string _msgContent;

};






#endif /*MASSAGE_H_*/
