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

//---------------------------------------------------------------------------------
//							constarctors and distractor and start
//--------------------------------- -----------------------------------------------
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


  //##############################################################################
  //						DONE
  //##############################################################################
  //------Exit--------
    	  if(arg=="Exit"){
    		  cout<<"your choice is : exit "<<endl;
    		 Exit(0);
    	  }
  //------CreateNet--------
		  else if(arg=="CreateNet"){
			  cout<<"your choice is : CreateNet "<<endl;
			  cout<<"pleas enter file configuration name : "<<endl;
			  getline(cin,fileConfig);
			  createNet(fileConfig.c_str());
			//cout<<"in start creat "<<endl;
			  goto GET_COMMAND;

    	   }
//------PrintRT--------
		  else if(arg=="PrintRT"){
			  cout<<"your choice is : PrintRT "<<endl;
			  cout<<"pleas enter the node id you wan'a is RT view : "<<endl;
			  getline(cin,uniqueID);
			  if(uniqueID==""){
				  goto GET_COMMAND;
			   }
			  printRt(atoi(uniqueID.c_str()));
			  goto GET_COMMAND;
		  }
//------SendPacket--------
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

//------Killall--------
		  else if(arg=="Killall"){
			  cout<<"your choice is : Killall "<<endl;
			  killAll();
			  goto GET_COMMAND;

		  }
  //##############################################################################
  //						TODO
  //##############################################################################
 //------KillNode--------
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

//------ReviveNode--------
    	  else if(arg== "ReviveNode"){
    		  cout<<"your choice is : ReviveNode "<<endl;
    		  cout<<"pleas enter the node id you wan'a revive : "<<endl;
    		  getline(cin,uniqueID);
    		  if(uniqueID==""){
    		        goto GET_COMMAND;
    		   }
    		  reviveNode(atoi(uniqueID.c_str()));
    		  goto GET_COMMAND;

    	  }



//------Run--------
    	  else if(arg=="Run"){
    		  cout<<"your choice is : Run "<<endl;
    		  Run();
    		  goto GET_COMMAND;
    	  }
      }//end while

}


//---------------------------------------------------------------------------------
//					Shell messages
//--------------------------------- ----------------------------------------------
void Shell::createNet(const char * file){
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
	_mailer=new Mailer(*this,_numberOfNodes,_bufferSize,*_neighbor,_numOfNighbors);//mailer->...
	_mailer->start();//mailer thread start

}//end creatNet
void Shell::Exit(int i){
	exit(i);
}

//---------------------------------------------------------------------------------
//					SYS Reg messages
//--------------------------------- -----------------------------------------------
void Shell::sendPacket(string sourceID,string targetID,string textMsg) {
	int src = atoi(sourceID.c_str());
	int dst = atoi(targetID.c_str());
	RegMsg *regMsg=new RegMsg(src,dst,textMsg,src);//TODO need new????
	//= new RegMsg();
	//RegMsg msg = new RegMsg();
	_mailer->rcvPacket(regMsg);
}

void Shell::killAll()  {

	vector<Worker*> workers =_mailer->getWorker();
	Worker* worker;
	RT* workerRT ;
	for(int i=1 ; i<_numberOfNodes+1;i++){
		worker = workers[i];
		if(worker->getActive()){
			//print rt msg
			workerRT = worker->getRT();
			workerRT->initRT();
			worker->setActive(false);
		}
	}
}//end kill all

void Shell::killNode(int id) {

	//worker->getRT()->initRT();
	//worker->setActive(false);
	_mailer->killNode(id);
}

void Shell::reviveNode(int id)  {
	_mailer->reviveNode(id);

}

//---------------------------------------------------------------------------------
//					TODO
//--------------------------------- -----------------------------------------------

void Shell::Run()  {
	//TODO

}


//---------------------------------------------------------------------------------
//					set _bufferSize,_numberOfNodes,_neighbor and _neighbor
//--------------------------------- -----------------------------------------------

void Shell::insertArgs(vector <string> argToNet){

	char buf[256];
	string bufferSize= argToNet[0];
	string nodNumber = argToNet[1];

	_bufferSize= atoi(bufferSize.c_str());
	_numberOfNodes=atoi(nodNumber.c_str());
	_neighbor = makeMatrix(_numberOfNodes);
	_numOfNighbors.resize(_numberOfNodes+1,0);
	//-----------------------------insert to _neighbor matrix----------------------
	initMatrix(_numberOfNodes,_neighbor);
	char delims[] = " ";
	char *result = NULL;

	for ( unsigned int i=2 ; i<argToNet.size();i++ ){
		int j=0;
		int firstNode ;
		strcpy(buf,argToNet[i].c_str());//line
		result = strtok( buf, delims ); // create tokens ""object""

		//if(result!=0){
			 firstNode = atoi(result);
		//}
		_numOfNighbors[firstNode]=0;
		while( result != NULL ) {
			_numOfNighbors[firstNode]=_numOfNighbors[firstNode]+1;

			 // if result isn't null insert atoi (result) otherwise insert 0
			 _neighbor[firstNode][j]= (result!=NULL) ? atoi(result) : 0;//TODO : tell about change: in place of the first neigbors i will be is neigbors nodes
			 j++;
			 result = strtok( NULL, delims ); // get next Token
		}//end while
		_numOfNighbors[firstNode]=_numOfNighbors[firstNode]-1;
		cout<<" numbers of nighbors for node "<<firstNode<<" == "<<_numOfNighbors[firstNode]<<endl;
		cout << endl;
		printnighbors(firstNode,_numOfNighbors[firstNode]);
	}//end for

}//end insert


//---------------------------------------------------------------------------------
//									inits
//--------------------------------- -----------------------------------------------

void Shell:: initMatrix(unsigned int numberOfNodes,int **matrix){

	for(unsigned int i = 0 ; i < numberOfNodes+1;i++){
		for(unsigned int j = 0 ; j < numberOfNodes;j++){
			matrix[i][j] = 0;
		}
	}
}

int ** Shell::makeMatrix(unsigned int numberOfNodes_){
	int ** neighbor = new int* [numberOfNodes_+1];//[numberOfNodes_];
		for (unsigned int i=0; i<numberOfNodes_+1;i++){
			neighbor[i]= new int [numberOfNodes_];
	}
    return neighbor;
}

//---------------------------------------------------------------------------------
//									prints
//--------------------------------- -----------------------------------------------

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

void Shell::printRt(int uniqueID)  {
	vector<Worker*> workers =_mailer->getWorker();
	Worker* worker = workers[uniqueID];
	//print rt msg
	RT* workerRT = worker->getRT();
	workerRT->printRT();
}

void Shell::printnighbors(int firstNode,int numOfnigb){
	int n = 0  ;
	cout<<" print the nigbors of : " <<firstNode<<endl;
	int *ng = _neighbor[firstNode];
	for(int i=1;i<=numOfnigb;i++){
		n = ng[i];
		cout<<" my nighbor "<<n<<endl;
	}

}
//---------------------------------------------------------------------------------
//									getters and setters
//--------------------------------- -----------------------------------------------

int** Shell::getNigebors(){
	return _neighbor;
}











