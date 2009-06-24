#include "Message.h"



	Message::Message(string content,int rcvId, int sndId,string msgType){
		_msgContent=content;
		_msgType=msgType;
		_receiveFrom=rcvId;
		_sendTo=sndId;
	};

	virtual::~Message()
	{

	}

	string Message::getMsgType(){
		return _msgType;
	}
	void Message::setContend(string content){
		_msgContent=content;
	}

