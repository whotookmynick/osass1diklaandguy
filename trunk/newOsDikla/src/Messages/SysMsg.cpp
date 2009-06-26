#include "Hfiles/SysMsg.h"

SysMsg::SysMsg(int src,int dst,string textMsgatoi,int next):Message(src,dst,textMsgatoi,next)
{
}

SysMsg::~SysMsg()
{
}



	string SysMsg::getType(){
		return "Sys";
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
