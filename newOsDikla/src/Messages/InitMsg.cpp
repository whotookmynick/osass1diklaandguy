#include "Hfiles/InitMsg.h"

InitMsg::InitMsg()
{
}

InitMsg::~InitMsg()
{
}
int InitMsg::getSrc(
){
	return _sendTo;
};

string InitMsg::getType(){
	return "Init";
}

int InitMsg::getDes(){
	return _receiveFrom;
};

string InitMsg::getContent(){
	return _msgContent;
};
/*string  InitMsg::toString(Message* msg){
	return "TODO";
}*/
void InitMsg::setNext(int nextId){
	_next = nextId;
}
int InitMsg::getNext(){
	return _next;
}