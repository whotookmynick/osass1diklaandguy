#include "Worker.h"


//---------------------------------------------------------------------------------
//									Contractor and detractor
//--------------------------------- -----------------------------------------------
Worker::Worker(int id,Mailer& mailer,int* neigbors,int numOfWorkers):_id(id),
_mailer(mailer),_numOfWorkers(numOfWorkers)
{
	_myRT =new RT();
}
Worker::~Worker()
{
}



//---------------------------------------------------------------------------------
//									getters and setters
//--------------------------------- -----------------------------------------------
int Worker::getId(){
	return _id;
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
	 //broadcast();
	 while(true){
		 Message* msg = _mailer.readMails(getId());
		 if(msg==NULL){
			 //give up time
			 yield();
		 }
		 else {
			 cout<<" i am your worker my id "<<_id<<endl;
			 msg->printMsg();}
	 }
 }

 void Worker::broadcast(){
	 //move on all neighbors  and send them messgage with my RT
	 for(int i = 1 ;i<=_numOfWorkers;i++){
		 if(i==_id){
		 }
		 else{
			 SysMsg* m = new SysMsg(_id,i,"RT msg",i);
			 m->setRT(_myRT);
		 }//end else

	 }//end for
 }//end broadcast


