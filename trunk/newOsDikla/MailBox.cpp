#include "MailBox.h"

// Reads messages from the mailbox to destinyMsgQ.

bool Mailbox::readMsg(std::queue<Message*>& destinyMsgQ){
	return true;
}
bool Mailbox::insertMsg(Message* m){
	string type = m->getType();
	if(type == "Reg" ){
		_RegQueue.push_back((RegMsg*)m);
		return true;
	}
	if(type == "Init" ){
		_IntQueue.push_back((InitMsg*)m);
		return true;
	}
	if(type == "Sys" ){
		_sysQueue.push_back((SysMsg*)m);
		return true;
	}
	return false;
}//insert message in the mailbox

