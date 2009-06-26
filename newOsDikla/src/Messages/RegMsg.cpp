#include "Hfiles/RegMsg.h"

RegMsg::RegMsg(int src,int dst,string textMsgatoi,int next):Message(src,dst,textMsgatoi,next){

}
RegMsg::~RegMsg()
{
}

string RegMsg::getType(){
	return "Reg";
}


void RegMsg::printMsg(){
	if (this==NULL){
		cout<<"ERROR: i am NULL msg "<<endl;
		return ;
	}
	int src = getSrc();
	int des = getDes();
	int nextId = getNext();
	string content = getContent();
	cout<<" msg from: "<<src<<" to: "<<des<<" throw: "<<nextId<<" --- "<<content<<endl;
}
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
