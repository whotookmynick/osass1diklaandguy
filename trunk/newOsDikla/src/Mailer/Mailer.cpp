#include "Mailer.h"
#include "Worker/Worker.h"
#define DEBUG false
Mailer* mailer;

Mailer::Mailer(Shell& shell,int numOfWorkers,int bufSize,int* negibors)
:_shell(shell),_numOfWorkers(numOfWorkers),
_inboxSize(bufSize),_inbox(),_mailBoxes(),_workers(),_neigbors(negibors){
	//int mailBox
	cout<<" number of workers: "<<_numOfWorkers<<endl;
	cout<<" buffer size: "<<_inboxSize<<endl;

	_inbox.resize(_inboxSize);
	_mailBoxes.resize(_numOfWorkers+1);
	_workers.resize(_numOfWorkers+1);
	for( int i=0;i<numOfWorkers+1;i++ ){
		_mailBoxes[i]=new Mailbox();
	}
	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_inboxMutex, &_mtxattr);
}


extern "C"{
	void* mailerWrapper(void* f){
		((Mailer*)f)->run();
		return NULL;
	}
}

void Mailer::start(){
	pthread_create(&_mailerThread,NULL,mailerWrapper,this);

	for(int i=1 ; i<=_numOfWorkers; i++){
		int** nigborsMatrix = _shell.getNigebors();
		int* n = nigborsMatrix[i];
		Worker* workeri;
		workeri= new Worker(i,*this,n,_numOfWorkers);
		_workers[i]=workeri;
		workeri->start();
	}

}
void Mailer::run(){
	while(true){
		deliverMsgToMailBox();
	}
}
bool Mailer:: addMsgToInbox(Message* msg){
	pthread_mutex_lock(&_inboxMutex);
	_inbox.push_back(msg);
	pthread_mutex_unlock(&_inboxMutex);
	return true;
}

void Mailer::printInbox(list<Message*> v){
	pthread_mutex_lock(&_inboxMutex);
	if(v.empty()){
		cout<<" ERROR : trying to print null vector of messages"<<endl;
		return;
	}
	else if (!v.empty()){
		for(list<Message*>::iterator itr=v.begin(); itr!=v.end(); ++itr){
			string s ;
			Message* m = *itr;
			if(m==NULL){
				cout<<" No Msg in inbox "<<endl;
			}
			else{
				m->printMsg();
				cout << endl;
			}
		}//end for
	}//end if
	pthread_mutex_unlock(&_inboxMutex);
}

bool Mailer::killNode(int nodeId){
	pthread_mutex_lock(&_inboxMutex);
	string msg = "kill node "+ nodeId;
	//if(DEBUG){
		cout<<"kill node : " <<nodeId<<endl;
	//}


	_shell.printMsgFromMailer(msg);
	pthread_mutex_unlock(&_inboxMutex);
	return false;
}

bool Mailer::rcvPacket(Message* msg){//string sourceID,string targetID,string textMsg
	pthread_mutex_lock(&_inboxMutex);
	_inbox.push_back(msg);
	cout<<" printing inbox " <<endl;
	printInbox(_inbox);
	//string type=msg->getType() ;//getType(); //TODO set the msg type in the shell
	//deliverMsgToMailBox();//TODO remove
	pthread_mutex_unlock(&_inboxMutex);
	return true;
}

//deliver ong msg to the next des
bool Mailer::deliverMsgToMailBox(){
	pthread_mutex_lock(&_inboxMutex);
	//	cout<<" start to deliver "<<_inbox.size()<<endl;
	if(_inbox.empty()){
		//cout<<" ERORR: inbox is Empty "<<endl;
		pthread_mutex_unlock(&_inboxMutex);
		return false;
	}
	Message* m = _inbox.back();
	_inbox.pop_front();
	if(m==NULL){
		//cout<<" ERORR: msg is Empty "<<endl;
		pthread_mutex_unlock(&_inboxMutex);
		return false;
	}
	int sendTo = m->getNext();
	string mc = m->getContent();

	cout<<" insert: ";
	m->printMsg();
	cout<<" to mailBox: "<<sendTo<<endl;
	_mailBoxes[sendTo]->insertMsg(m);
	//wake worker
	_workers[sendTo]->setQuanta(1);
	_mailBoxes[sendTo]->printMailes();
	pthread_mutex_unlock(&_inboxMutex);
	return true;
}

Mailer::~Mailer()
{
	pthread_mutex_destroy(&_inboxMutex);
	pthread_mutexattr_destroy(&_mtxattr);
}
Message* Mailer::readMails(int id){
	return _mailBoxes[id]->readMsg();

}

vector<Worker*> Mailer::getWorker(){
	return _workers;
}
