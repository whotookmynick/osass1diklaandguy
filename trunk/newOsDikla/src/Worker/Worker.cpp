#include "Worker.h"

Worker::Worker(int id,Mailer& mailer):_id(id),_mailer(mailer)
{
}

Worker::~Worker()
{
}
int Worker::getId(){
	return _id;
}

 void Worker::run(){
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

 /* studentSend - an abstract method that is used to send the message you wish to send */
 		bool Worker::studentSend(void *args) {
 			return false;
 		}

 		/* studentRecv - an abstract method that is used to receive a message */
 		bool Worker::studentRecv(void **args) {
 			return false;
 		}
