#include "Mailer.h"
#define DEBUG false
Mailer* mailer;

Mailer::Mailer(Shell& shell,int numOfWorkers,int bufSize)
:_shell(shell),_numOfWorkers(numOfWorkers),
_inboxSize(bufSize){

}

bool Mailer:: addMsgToInbox(const string& msg){
	_inbox.push_back(msg);
	return true;
}

void Mailer::printInbox(const vector<void*>& v){
	if (!v.empty()){
		for(int i=0; i<v.size(); ++i){
			cout << v[i] << " ";
			cout << endl;
			cout << "----------------"<<endl;
		}//end for
	}//end if
}

bool Mailer::killNode(int nodeId){

	string msg = "kill node "+ nodeId;
	//if(DEBUG){
		cout<<"kill node : " <<nodeId<<endl;
	//}


	_shell.printMsgFromMailer(msg);
	return false;
}

Mailer::~Mailer()
{
}
