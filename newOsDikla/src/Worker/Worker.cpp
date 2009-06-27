#include "Worker.h"


//---------------------------------------------------------------------------------
//									Contractor and detractor
//--------------------------------- -----------------------------------------------

Worker::Worker(int id,Mailer& mailer,int* neigbors,int numOfWorkers):_id(id),
_mailer(mailer),_numOfWorkers(numOfWorkers),_active(true)
{
	_myRT =new RT(numOfWorkers,_id);
	pthread_mutex_init(&_waitMutex,NULL);
	pthread_cond_init(&_condWait,NULL);
}

Worker::~Worker()
{
	pthread_mutex_destroy(&_waitMutex);
	pthread_cond_destroy(&_condWait);
}


//---------------------------------------------------------------------------------
//									getters and setters
//--------------------------------- -----------------------------------------------
int Worker::getId(){
	return _id;
}

RT* Worker::getRT(){
	return _myRT;
}

bool Worker::getActive(){
	return _active;
}


void Worker::setActive(bool a){
	_active=a;
}


//---------------------------------------------------------------------------------
//									OSthreads
//--------------------------------- -----------------------------------------------

 /* studentSend - an abstract method that is used to send the message you wish to send */
bool Worker::studentSend(void *args) {
	return false;
}

/* studentRecv - an abstract method that is used to receive a message */
bool Worker::studentRecv(void **args) {
	return false;
}


//---------------------------------------------------------------------------------
//									Run and Broadcast
//--------------------------------- -----------------------------------------------
 void Worker::run(){
	 broadcast();
	 while(true){
		 Message* msg = _mailer.readMails(getId());
		 if(msg==NULL){
			 //give up time
			 yield();
		 }
		 else {
			 string type = msg->getType();
			 if(type=="Init"){
				 string initM = msg->getContent();
				 if(initM=="killMe"){
					 wait();
				 }else if(initM=="initRT"){
					 _myRT->initRT();
					 broadcast();
				 }

			 }
			 if(type=="Sys"){
				 rcvSysMsg((SysMsg*) msg);
			 }
			 cout<<" i am your worker my id "<<_id<<endl;
			 msg->printMsg();}

	 }
 }
void Worker::wait(){
	pthread_mutex_lock(&_waitMutex);
	pthread_cond_wait(&_condWait,&_waitMutex);
	pthread_mutex_unlock(&_waitMutex);

}

 //---------------------------------------------------------------------------------
 //									RT
 //--------------------------------- -----------------------------------------------

 void Worker::broadcast(){//send my rt
	 //move on all neighbors  and send them messgage with my RT
	 for(int i = 1 ;i<=_numOfWorkers;i++){
		 if(i==_id){
		 }
		 else{
			 SysMsg* m = new SysMsg(_id,i,"RT msg",i);
			 m->setRT(_myRT);
			 _mailer.rcvPacket(m);
		 }//end else

	 }//end for
 }//end broadcast

 void Worker::rcvSysMsg(SysMsg* msg){
	RT* nighborRT = msg->getRT();
	_myRT->setRT(nighborRT);
 }
