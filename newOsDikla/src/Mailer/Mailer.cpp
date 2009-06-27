#include "Mailer.h"
#include "Worker/Worker.h"
#define DEBUG false
Mailer* mailer;


//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------
Mailer::Mailer(Shell& shell,int numOfWorkers,int bufSize,int* negibors,vector<int> numOfNighbors)
:_shell(shell),_numOfWorkers(numOfWorkers),
_inboxSize(bufSize),_inbox(),_mailBoxes(),_workers(),_neigbors(negibors),_waitNeeded(true),_numOfNighbors(numOfNighbors){
	//int mailBox
	cout<<" number of workers: "<<_numOfWorkers<<endl;
	cout<<" buffer size: "<<_inboxSize<<endl;

	_inbox.resize(_inboxSize);
	_mailBoxes.resize(_numOfWorkers+1);
	_workers.resize(_numOfWorkers+1);
	for( int i=1;i<numOfWorkers+1;i++ ){
		_mailBoxes[i]=new Mailbox();
	}
	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_inboxMutex, &_mtxattr);
}


Mailer::~Mailer()
{
	pthread_mutex_destroy(&_inboxMutex);
	pthread_mutexattr_destroy(&_mtxattr);
}

//---------------------------------------------------------------------------------
//									threads
//--------------------------------- -----------------------------------------------

extern "C"{
	void* mailerWrapper(void* f){
		((Mailer*)f)->run();
		return NULL;
	}
}

void Mailer::start(){

	int** nigborsMatrix = _shell.getNigebors();
	_waitNeeded=true;//no deliver till all done
	pthread_create(&_mailerThread,NULL,mailerWrapper,this);

	for(int i=1 ; i<=_numOfWorkers; i++){

		int* n = nigborsMatrix[i];
		Worker* workeri;
		workeri= new Worker(i,*this,n,_numOfWorkers,_numOfNighbors[i]);
		_workers[i]=workeri;
		workeri->start();
	}
	_waitNeeded = false;
}

void Mailer::run(){
	while(true){
		if(_waitNeeded){
			//do nothing
		}else{
			deliverMsgToMailBox();
		}
	}
}

//---------------------------------------------------------------------------------
//									inbox handle
//--------------------------------- -----------------------------------------------
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
		pthread_mutex_unlock(&_inboxMutex);
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

bool Mailer::rcvPacket(Message* msg){//string sourceID,string targetID,string textMsg
	pthread_mutex_lock(&_inboxMutex);
	_inbox.push_back(msg);
	//cout<<" printing inbox " <<endl;
	//printInbox(_inbox);
	//string type=msg->getType() ;//getType(); //TODO set the msg type in the shell
	//deliverMsgToMailBox();//TODO remove
	pthread_mutex_unlock(&_inboxMutex);
	return true;
}


//---------------------------------------------------------------------------------
//									Mail BOxes handles
//--------------------------------- -----------------------------------------------

Message* Mailer::readMails(int id){
	return _mailBoxes[id]->readMsg();

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
	cout<<" inbox not empty "<<endl;
	Message* m = _inbox.back();
	_inbox.pop_front();
	if(m==NULL){
		//cout<<" ERORR: msg is Empty "<<endl;
		pthread_mutex_unlock(&_inboxMutex);
		return false;
	}
	int sendTo = m->getNext();
	string mc = m->getContent();

	//cout<<" insert: ";
	m->printMsg();
	//cout<<" to mailBox: "<<sendTo<<endl;
	_mailBoxes[sendTo]->insertMsg(m);
	//wake worker
	_workers[sendTo]->setQuanta(1000);
	//_mailBoxes[sendTo]->printMailes();
	pthread_mutex_unlock(&_inboxMutex);
	return true;
}


//---------------------------------------------------------------------------------
//									RT
//--------------------------------- -----------------------------------------------

void Mailer::updateWorkerRT(int nodeId)//send all workers my new rt
{
	pthread_mutex_lock(&_inboxMutex);

	SysMsg* rtMsg;
	RT* deadWorkerRt = 	_workers[nodeId]->getRT();
	bool active = false;

	for (int i=1; i<=_numOfWorkers; i++){
		active =  _workers[i]->getActive();
		if(i!=nodeId && active){
			//create sysMsg with the dead worker rt
			rtMsg = new SysMsg(nodeId,i,"RTmsg",i);
			rtMsg->setRT(deadWorkerRt);
			_workers[i]->rcvSysMsg(rtMsg);
		}//end if
	}//end for
	pthread_mutex_unlock(&_inboxMutex);
}//end update


//---------------------------------------------------------------------------------
//									getters and setters
//--------------------------------- -----------------------------------------------
vector<Worker*> Mailer::getWorker(){
	return _workers;
}


//---------------------------------------------------------------------------------
//									????? TODO - needed????
//--------------------------------- -----------------------------------------------
bool Mailer::killNode(int nodeId){
	InitMsg* killme = new InitMsg(nodeId,nodeId,"lll",nodeId);
	rcvPacket(killme);
	_waitNeeded=true;
	for(int i=1; i<=_numOfWorkers; i++){
		InitMsg* initRT = new InitMsg(i,i,"initRT",i);
		rcvPacket(initRT);
	}//end for
	_waitNeeded=false;
	return false;
}

bool Mailer::reviveNode(int nodeId){
	pthread_mutex_lock(&_inboxMutex);

	string msg = "revive node "+ nodeId;
	cout<<"revive node : " <<nodeId<<endl;
	_workers[nodeId]->notify();
	InitMsg* initMsg = new InitMsg(nodeId,nodeId,"initRT",nodeId);
	_mailBoxes[nodeId]->insertMsg(initMsg);
	/*_workers[nodeId]->_myRT->initRT();
	_workers[nodeId]->setActive(true);
	updateWorkerRT(nodeId);
	pthread_mutex_unlock(&_inboxMutex);
	 */
	pthread_mutex_unlock(&_inboxMutex);
	return true;
}
