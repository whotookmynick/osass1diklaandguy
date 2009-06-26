#include "Mailer.h"

#define DEBUG false
Mailer* mailer;

Mailer::Mailer(Shell& shell,int numOfWorkers,int bufSize)
:_shell(shell),_numOfWorkers(numOfWorkers),
_inboxSize(bufSize){

}

bool Mailer:: addMsgToInbox(Message* msg){
	_inbox.push_back(msg);
	return true;
}

void Mailer::printInbox(vector<Message*> v){
	if (!v.empty()){
		for(int i=0; i<v.size(); ++i){
			string s ;
			Message* m = v[i];
			cout<<" from: "<< m->getSrc()<<" to "<<m->getDes()<<" - "<<m->getContent()<<endl ;
			cout << endl;
			cout << "----------------"<<endl;
		}//end for
	}//end if
}

bool Mailer::killNode(int nodeId){

	string msg = "kill node "+ nodeId;
	//if(DEBUG){
		cout<<"kill node : " <<nodeId<<endl;
	//}


	_shell.printMsgFromMailer(msg);
	return false;
}
bool Mailer::rcvPacket(Message* msg){//string sourceID,string targetID,string textMsg
	_inbox.push_back(msg);
	vector<Message*> v=_inbox;
	printInbox(v);
	//string type=msg->getType() ;//getType(); //TODO set the msg type in the shell

	return true;
}
Mailer::~Mailer()
{
}
