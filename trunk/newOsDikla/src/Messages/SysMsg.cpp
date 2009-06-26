#include "Hfiles/SysMsg.h"

SysMsg::SysMsg()
{
}

SysMsg::~SysMsg()
{
}


	int SysMsg::getSrc(
	){
		return _sendTo;
	};
	string SysMsg::getType(){
		return "Sys";
	}

	int SysMsg::getDes(){
		return _receiveFrom;
	};

	string SysMsg::getContent(){
		return _msgContent;
	};
	void SysMsg::setNext(int nextId){
		_next = nextId;
	}
	int SysMsg::getNext(){
		return _next;
	}
	void SysMsg::printMsg(){
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
/* string  SysMsg::toString(Message* msg){
		return "TODO";
	}*/
