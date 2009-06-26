#include "Hfiles/RegMsg.h"

RegMsg::RegMsg(int src,int dst,string textMsgatoi):_receiveFrom(src),_sendTo(dst),_msgContent(textMsgatoi){}

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
/* string RegMsg::toString(Message* msg){
	string startM =" msg from : ";
	string to =" to " ;
	int src = getSrc();
	int des = getDes();
	char result[100];
	char result2[100];

	sprintf( result, "%d", src );//src as ~string in result

	string srcS = startM+result;

	sprintf( result2, "%d", des );//src as ~string in result

	string srcTo = to+result2;
	string srcToContent = srcTo+" - "+getContent();

	//string b = a + getSrc();
	//char* sms =strcat(,getSrc() ); //" msg from : " + getSrc() +" to: "+ getDes() + " - " + getContent();
	return srcToContent;
}
*/
