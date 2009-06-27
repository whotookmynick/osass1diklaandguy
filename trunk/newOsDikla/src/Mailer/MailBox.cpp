#include "MailBox.h"

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------
// Reads messages from the mailbox to destinyMsgQ.
Mailbox::Mailbox(){
	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_QMutex, &_mtxattr);
}

Mailbox::~Mailbox(){
	pthread_mutex_destroy(&_QMutex);
	pthread_mutexattr_destroy(&_mtxattr);
}

//---------------------------------------------------------------------------------
//							deal with Mailes
//--------------------------------- -----------------------------------------------

/*bool Mailbox::readMsg(std::queue<Message*>& destinyMsgQ){
	return false ;
}*/

Message* Mailbox::readMsg(){
	pthread_mutex_lock(&_QMutex);
	Message * m =NULL;
	if(!_IntQueue.empty()){
		m =_IntQueue.front();
		_IntQueue.pop_front();
	}
	else if(!_sysQueue.empty()){
		m = _sysQueue.front();
		_sysQueue.pop_front();
	}
	else if(!_RegQueue.empty()){
		m = _RegQueue.front();
		_RegQueue.pop_front();

	}
	pthread_mutex_unlock(&_QMutex);
	return m;
}

bool Mailbox::insertMsg(Message* m){
	pthread_mutex_lock(&_QMutex);
	if(m==NULL){
			cout<<" ERORR: msg is Empty "<<endl;
			pthread_mutex_unlock(&_QMutex);
			return false;
	}
	string type = m->getType();
	cout<<" (insertMsg): ";
	cout<<" to Q type: "<<type;
	m->printMsg();
	cout<<endl;


	if(type == "Reg" ){
		_RegQueue.push_back(m);
		pthread_mutex_unlock(&_QMutex);
		return true;
	}
	if(type == "Init" ){
		_IntQueue.push_back(m);
		pthread_mutex_unlock(&_QMutex);
		return true;
	}
	if(type == "Sys" ){
		_sysQueue.push_back(m);
		pthread_mutex_unlock(&_QMutex);
		return true;
	}
	cout<<" no such type  "<< type << endl;
	pthread_mutex_unlock(&_QMutex);
	return false;
}//insert message in the mailbox


//---------------------------------------------------------------------------------
//							print
//--------------------------------- -----------------------------------------------
void Mailbox::printMailes(){
	pthread_mutex_lock(&_QMutex);
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
	pthread_mutex_unlock(&_QMutex);
}


void Mailbox::printQ(list<Message*> l){
	pthread_mutex_lock(&_QMutex);
	for(unsigned  int i=0; i<l.size(); ++i){
		string s ;
		Message* m = l.front();
		cout<<" from: "<< m->getSrc()<<" to "<<m->getDes()<<" - "<<m->getContent()<<endl;
		cout << endl;
		cout << "----------------"<<endl;
	}//end if*/
	pthread_mutex_unlock(&_QMutex);
}

//---------------------------------------------------------------------------------
//									getters and setters
//--------------------------------- -----------------------------------------------

