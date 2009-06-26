#include "Shell.h"
#include "Mailer/Mailer.h"
#include "Messages/Hfiles/Message.h"
#include "Messages/Hfiles/RegMsg.h"
#include "Messages/Hfiles/InitMsg.h"
#include "Messages/Hfiles/SysMsg.h"
#include "Worker/Worker.h"
#define DEBUG false
#include <stdio.h>
using namespace std;
//buf=10;

int main(int argc,const char* argv[]){

    Shell shell;
    shell.start();
}//end main

void Shell::Exit(int i){
	exit(i);
}
Shell::Shell(){}
Shell::~Shell(){}

void Shell::start(){

	string arg = "";
	string fileConfig= "";
    string uniqueID="";
    string sourceID ="";
    string targetID ="";
    string textMsg="";
    //Mailer mailer=new Mailer();

    //Mailer mailer=new Mailer(shell,bufSize,workersNum);
 GET_COMMAND:
		while (true){
    	  cout << "enter command" <<endl;
    	  getline(cin,arg);
    	  //cout << "your choice : " <<arg<<endl;

    	  //------1--------
    	  if(arg=="Exit"){
    		  cout<<"your choice is : exit "<<endl;
    		 Exit(0);
    	  }

    	  //------2--------
    	  else if(arg=="CreateNet"){
    		  cout<<"your choice is : CreateNet "<<endl;
    		  cout<<"pleas enter file configuration name : "<<endl;
    		  getline(cin,fileConfig);
    		  createNet(fileConfig.c_str());
    		//cout<<"in start creat "<<endl;
    		  goto GET_COMMAND;

    	  }

    	  //------3--------
    	  else if(arg=="KillNode"){
    		  cout<<"your choice is : KillNode "<<endl;
    		  cout<<"pleas enter the node id you wan'a kill : "<<endl;
    		  getline(cin,uniqueID);
    		  if(uniqueID==""){
    		   	  goto GET_COMMAND;
    		 }
    		  if (_mailer==NULL){
    			  cout<<"you dont have any mailer "<<endl;
    			  goto GET_COMMAND;
    		  }
    		  //if(DEBUG){
    		    cout<<"goto killNode "<<endl;
    		    //}
    		    killNode(atoi(uniqueID.c_str()));

    		  	//if(DEBUG){
    		  	cout<<"return from killNode "<<endl;
    		  	//}
    			goto GET_COMMAND;

    	  	   }

    	  //------4--------
    	  else if(arg== "ReviveNode"){
    		  cout<<"your choice is : ReviveNode "<<endl;
    		  cout<<"pleas enter the node id you wan'a revive : "<<endl;
    		  getline(cin,uniqueID);
    		  if(uniqueID==""){
    		        goto GET_COMMAND;
    		   }
    		  reviveNode();
    		  goto GET_COMMAND;

    	  }

    	  //------5--------
    	  else if(arg=="Killall"){
    		  cout<<"your choice is : Killall "<<endl;
    		  killAll();
    		  goto GET_COMMAND;

    	  }

    	  //------6--------
    	  else if(arg=="PrintRT"){

    		  cout<<"your choice is : PrintRT "<<endl;
    		  cout<<"pleas enter the node id you wan'a is RT view : "<<endl;
    		  getline(cin,uniqueID);
    		  if(uniqueID==""){
    		  	  goto GET_COMMAND;
    		   }
    		  printRt();
    		  goto GET_COMMAND;


    	  }

    	  //------7--------
    	  else if(arg=="SendPacket"){
    		  cout<<"your choice is : SendPacket "<<endl;

			  cout<<"pleas enter the sourceID : "<<endl;
			  getline(cin,sourceID);
			  if(sourceID==""){
				  goto GET_COMMAND;
			  }
    		  cout<<"pleas enter the targetID : "<<endl;
    		  getline(cin,targetID);
    		  if(targetID==""){
    			  goto GET_COMMAND;
    		  }
    		  cout<<"pleas enter the textMsg : "<<endl;
    		  getline(cin,textMsg);
    		  if(textMsg==""){
    			  goto GET_COMMAND;
    		  }
    		  sendPacket(sourceID,targetID,textMsg);
    		  goto GET_COMMAND;
		  }

    	  //------8--------
    	  else if(arg=="Run"){
    		  cout<<"your choice is : Run "<<endl;
    		  Run();
    		  goto GET_COMMAND;
    	  }
      }//end while

}

void Shell::createNet(const char * file){
//	cout<<" vvv ";
	string line;
	ifstream netFile;
	vector<string> args;
	netFile.open(file);

	if (netFile.is_open()) {
		while (!netFile.eof() ) {
	      getline (netFile,line);
	      //cout<<tav;
	      //cout<<" --- ";
	      args.push_back(line);
		}//end while
	}//end if

	insertArgs(args);

	netFile.close();

	_mailer=new Mailer(*this,_numberOfNodes,_bufferSize);//mailer->...
	_mailer->start();//mailer thread start


}//end creatNet


void Shell::sendPacket(string sourceID,string targetID,string textMsg) {
	int src = atoi(sourceID.c_str());
	int dst = atoi(targetID.c_str());
	RegMsg *regMsg=new RegMsg(src,dst,textMsg,src);//TODO need new????
	//= new RegMsg();
	//RegMsg msg = new RegMsg();
	_mailer->rcvPacket(regMsg);

}

	void Shell::killNode(int id) {
		bool kill = _mailer->killNode(id);
		cout<<kill<<endl;


	}

    void Shell::reviveNode()  {
		//TODO

	}

    void Shell::killAll()  {
		//TODO

	}

	void Shell::printRt()  {
		//TODO

	}

	void Shell::Run()  {
		//TODO

	}

void Shell::insertArgs(vector <string> argToNet){

	char buf[256];
	string bufferSize= argToNet[0];
	string nodNumber = argToNet[1];

	_bufferSize= atoi(bufferSize.c_str());
	_numberOfNodes=atoi(nodNumber.c_str());

	_neighbor = makeMatrix(_numberOfNodes);
	initMatrix(_numberOfNodes,_neighbor);
	if(DEBUG){
		cout << "printing matrix before init:" << endl;
		//printMatrix(numberOfNodes_,neighbor);

		cout << "printing matrix after init:" << endl;
		initMatrix(_numberOfNodes,_neighbor);//TODO: init metrix even without DEBUG???
		//printMatrix(numberOfNodes_,neighbor);

	}
	//printf("the string is: ",argToNet[2]);
	char delims[] = " ";
	char *result = NULL;

	for ( unsigned int i=2 ; i<argToNet.size();i++ ){

		strcpy(buf,argToNet[i].c_str());//line

		if(DEBUG){
			cout<<"the char* buf : "<<buf<<endl;
		}

		int j=0;
		result = strtok( buf, delims ); // create tokens ""object""

		if(DEBUG){
			cout<<"the firstNode buf: "<<buf<<endl;
		}
		int firstNode ;
		//if(result!=0){
			 firstNode = atoi(result);
		//}
		if(DEBUG){
			//cout<<"the int first node is: "<<firstNode<<endl;
		}

		while( result != NULL ) {
			if(DEBUG){
				cout << "result is: "<<result << " "<<endl;
			}
			 result = strtok( NULL, delims ); // get next Token
			 // if result isn't null insert atoi (result) otherwise insert 0

			 _neighbor[firstNode][j]= (result!=NULL) ? atoi(result) : 0;//TODO : tell about change: in place of the first neigbors i will be is neigbors nodes

			 if(DEBUG){
			 cout<<"node :"<<firstNode<<" nigbore : "<<j<<" arry content for them : "<< _neighbor[firstNode][j]<<endl;
			 }

			 j++;
		}
		cout << endl;

	}

	if(DEBUG){
		cout<<"in insertArgs "<<endl;
		cout << "printing matrix after insert:" << endl;
		printMatrix(_numberOfNodes,_neighbor);
	}



}//end insert



int ** Shell::makeMatrix(unsigned int numberOfNodes_){
	int ** neighbor = new int* [numberOfNodes_+1];//[numberOfNodes_];
		for (unsigned int i=0; i<numberOfNodes_+1;i++){
			neighbor[i]= new int [numberOfNodes_];
	}
    return neighbor;
}
void Shell:: initMatrix(unsigned int numberOfNodes,int **matrix){

	for(unsigned int i = 0 ; i < numberOfNodes+1;i++){
		for(unsigned int j = 0 ; j < numberOfNodes;j++){
			matrix[i][j] = 0;
		}
	}
}

void Shell:: printMatrix(unsigned int numberOfNodes,int **matrix){

	for(unsigned int i = 0 ; i < numberOfNodes;i++){
		cout << "the "<<i<<" neigbors are: ";
		for(unsigned int j = 0 ; j < numberOfNodes;j++){
			 cout<<matrix[i][j]<<" "<<"----" ;
		}
		cout << endl;
	}
}

void Shell:: printMsgFromMailer(string msg){
	cout<<"start print msg frome mailer "<<endl;
	cout<<"mailer answer was: "<<msg<<endl;
}













