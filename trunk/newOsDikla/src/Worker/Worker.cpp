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
			 msg->printMsg();}
	 }
 }

