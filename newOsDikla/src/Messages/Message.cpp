/*
 * Message.cpp
 *
 *  Created on: Jun 26, 2009
 *      Author: dikla
 */

#include "Hfiles/Message.h"



Message::Message(int src,int dst,string textMsgatoi,int next):
	_receiveFrom(src),_sendTo(dst),
	_msgContent(textMsgatoi),_next(next){}


Message::~Message()
{
}
int Message::getSrc(){
	return _receiveFrom;
};

int Message::getDes(){
	return _sendTo;
};

string Message::getContent(){
	return _msgContent;
};
/*string  Message::toString(Message* msg){
	return "TODO";
}*/
void Message::setNext(int nextId){
	_next = nextId;
}
int Message::getNext(){
	return _next;
}
