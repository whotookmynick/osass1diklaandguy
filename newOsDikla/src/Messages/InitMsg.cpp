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
void InitMsg::printMsg(){
	if (this==NULL){
		cout<<"ERROR: i am NULL msg "<<endl;
		return ;
	}
	int src = getSrc();
	int des = getDes();
	int nextId = getNext();
	string content = getContent();
	cout<<" msg from: "<<src<<" to: "<<des<<" throw: "<<nextId<<" --- "<<content<<endl;
}
