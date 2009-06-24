/*
 * Message.h
 *
 *  Created on: Jun 24, 2009
 *      Author: dikla
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

//#include <iostream>
//#include <vector>
//
//#include <string>
//#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

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


#endif /* MESSAGE_H_ */
