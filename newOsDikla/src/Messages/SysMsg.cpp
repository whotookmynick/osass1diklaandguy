#include "Hfiles/SysMsg.h"

SysMsg::SysMsg()
{
}

SysMsg::~SysMsg()
{
}


	int SysMsg::getSrc(
	){
		return _sendTo;
	};
	string SysMsg::getType(){
		return "Sys";
	}

	int SysMsg::getDes(){
		return _receiveFrom;
	};

	string SysMsg::getContent(){
		return _msgContent;
	};
/* string  SysMsg::toString(Message* msg){
		return "TODO";
	}*/
