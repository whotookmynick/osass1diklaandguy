#include "MailBox.h"

// Reads messages from the mailbox to destinyMsgQ.
Mailbox::Mailbox(){

}
Mailbox::~Mailbox(){}
bool Mailbox::readMsg(std::queue<Message*>& destinyMsgQ){
	return true;
}
bool Mailbox::insertMsg(Message* m){
	if(m==NULL){
			cout<<" ERORR: msg is Empty "<<endl;
			return false;
	}
	string type = m->getType();
	cout<<" (insertMsg): ";
	cout<<" to Q type: "<<type;
	m->printMsg();
	cout<<endl;


	if(type == "Reg" ){
		_RegQueue.push_back(m);
		return true;
	}
	if(type == "Init" ){
		_IntQueue.push_back(m);
		return true;
	}
	if(type == "Sys" ){
		_sysQueue.push_back(m);
		return true;
	}
	cout<<" no such type  "<< type << endl;
	return false;
}//insert message in the mailbox
void Mailbox::printMailes(){

	if(!_sysQueue.empty()){
		cout<<" print sys q from mailebox "<<endl;
		printQ(_sysQueue);
	}
	cout<<" return from  print sys q "<<endl;
	cout<<endl;

	if(!_RegQueue.empty()){
		cout<<" print Reg q from mailebox "<<endl;
		printQ(_RegQueue);
	}
	cout<<" return from  print Reg q "<<endl;
	cout<<endl;

	if(!_IntQueue.empty()){
		cout<<" print Init q from mailebox "<<endl;
		printQ(_IntQueue);
	}
	cout<<" return from  print Init q "<<endl;
	cout<<endl;

	if(_RegQueue.empty() && _RegQueue.empty() && _IntQueue.empty()){
		cout<<" nothing in mail boxes "<<endl;
	}

}
void Mailbox::printQ(list<Message*> l){
	for(unsigned  int i=0; i<l.size(); ++i){
		string s ;
		Message* m = l.front();
		cout<<" from: "<< m->getSrc()<<" to "<<m->getDes()<<" - "<<m->getContent()<<endl;
		cout << endl;
		cout << "----------------"<<endl;
	}//end if*/
}



