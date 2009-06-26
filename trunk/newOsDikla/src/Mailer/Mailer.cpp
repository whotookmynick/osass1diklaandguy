#include "Mailer.h"

#define DEBUG false
Mailer* mailer;

Mailer::Mailer(Shell& shell,int numOfWorkers,int bufSize)
:_shell(shell),_numOfWorkers(numOfWorkers),
_inboxSize(bufSize),_inbox(),_mailBoxes(){
	//int mailBox
	cout<<" number of workers: "<<_numOfWorkers<<endl;
	cout<<" buffer size: "<<_inboxSize<<endl;

	_inbox.resize(_inboxSize);
	_mailBoxes.resize(_numOfWorkers);

	for( int i=0;i<numOfWorkers;i++ ){
		_mailBoxes[i]=new Mailbox();
	}

}

bool Mailer:: addMsgToInbox(Message* msg){
	_inbox.push_back(msg);
	return true;
}

void Mailer::printInbox(vector<Message*> v){
	if(v.empty()){
		cout<<" ERROR : trying to print null vector of messages"<<endl;
		return;
	}
	else if (!v.empty()){
		for(unsigned int i=0; i<v.size(); ++i){
			string s ;
			Message* m = v[i];
			if(m==NULL){
				cout<<" No Msg in "<<i<<" position "<<endl;
			}
			else{
				m->printMsg();
				cout << endl;
			}
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
	cout<<" printing inbox " <<endl;
	printInbox(_inbox);
	//string type=msg->getType() ;//getType(); //TODO set the msg type in the shell
	deliverMsgToMailBox();//TODO remove
	return true;
}

//deliver ong msg to the next des
bool Mailer::deliverMsgToMailBox(){
	cout<<" start to deliver "<<endl;
	if(_inbox.empty()){
		cout<<" ERORR: inbox is Empty "<<endl;
		return false;
	}
	Message* m = _inbox.back();
	_inbox.pop_back();
	if(m==NULL){
		cout<<" ERORR: msg is Empty "<<endl;
		return false;
	}
	int sendTo = m->getNext();
	string mc = m->getContent();

	cout<<" insert: ";
	m->printMsg();
	cout<<" to mailBox: "<<sendTo<<endl;
	_mailBoxes[sendTo]->insertMsg(m);
	_mailBoxes[sendTo]->printMailes();
	return true;
}

Mailer::~Mailer()
{
}
