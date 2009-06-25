#include "RegMsg.h"

RegMsg::RegMsg()
{
}

RegMsg::~RegMsg()
{
}
int RegMsg::getSrc(
){
	return _sendTo;
};
string RegMsg::getType(){
	return "Reg";
}

int RegMsg::getDes(){
	return _receiveFrom;
};

string RegMsg::getContent(){
	return _msgContent;
};