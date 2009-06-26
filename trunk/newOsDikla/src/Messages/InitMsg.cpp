#include "Hfiles/InitMsg.h"

InitMsg::InitMsg(int src,int dst,string textMsgatoi,int next):Message(src,dst,textMsgatoi,next)
{
}

InitMsg::~InitMsg()
{
}
string InitMsg::getType(){
	return "Init";
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
