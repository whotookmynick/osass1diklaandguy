#include "Shell.h"
#define DEBUG true
using namespace std;
//buf=10;

int main(int argc,const char* argv[]){
    string arg = "";

 GET_COMMAND:
		while (true){
    	  cout << "enter command" <<endl;
    	  getline(cin,arg);
    	  cout << "your choice : " <<arg<<endl;

    	  if(arg=="Exit"){
    		  cout<<"your choice is : exit "<<endl;
    		  exit(0);
    	  }else if(arg=="CreateNet"){
    		  //TODO: get file name
    		  cout<<"your choice is : CreateNet "<<endl;
    		  cout<<"pleas enter file configuration name : "<<endl;
    		  goto GET_COMMAND;

    	  }else if(arg== "KillNode"){
    		  //TODO: get uniqueID)
    		  cout<<"your choice is : KillNode "<<endl;
    		  cout<<"pleas enter the node id you wan'a kill : "<<endl;
    		  goto GET_COMMAND;

    	  }else if(arg== "ReviveNode"){
    		  //TODO: get uniqueID
    		  cout<<"your choice is : ReviveNode "<<endl;
    		  cout<<"pleas enter the node id you wan'a revive : "<<endl;
    		  goto GET_COMMAND;

    	  }else if(arg=="Killall"){
    		  cout<<"your choice is : Killall "<<endl;
    		  goto GET_COMMAND;

    	  }else if(arg=="PrintRT"){
    		  //TODO: get uniqueID
    		  cout<<"your choice is : PrintRT "<<endl;
    		  cout<<"pleas enter the node id you wan'a is RT view : "<<endl;
    		  goto GET_COMMAND;

    	  }else if(arg=="SendPacket"){
    		  //TODO: get sourceID targetID textMsg
    		  cout<<"your choice is : SendPacket "<<endl;

			  cout<<"pleas enter the sourceID : "<<endl;

    		  cout<<"pleas enter the targetID : "<<endl;

    		  cout<<"pleas enter the textMsg : "<<endl;
    		  goto GET_COMMAND;
		  }
    	  else if(arg=="Run"){
    		  cout<<"your choice is : Run "<<endl;
    		  goto GET_COMMAND;
    	  }
      }//end while
}//end main

void Shell::creatNet(char * file){
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
}//end creatNet


void Shell::insertArgs(vector <string> argToNet){

	char buf[256];
	string bufferSize= argToNet[0];
	string nodNumber = argToNet[1];

	bufferSize_= atoi(bufferSize.c_str());
	numberOfNodes_=atoi(nodNumber.c_str());

	int ** neighbor = makeMatrix(numberOfNodes_);

	if(DEBUG){
		//cout << "printing matrix before init:" << endl;
	//	printMatrix(numberOfNodes_,neighbor);

		//cout << "printing matrix after init:" << endl;
		initMatrix(numberOfNodes_,neighbor);//TODO: init metrix even without DEBUG???
		//printMatrix(numberOfNodes_,neighbor);

	}
	//printf("the string is: ",argToNet[2]);
	char delims[] = " ";
	char *result = NULL;

	for ( unsigned int i=2 ; i<argToNet.size();i++ ){
		strcpy(buf,argToNet[i].c_str());//line
		cout<<"the char* buf : "<<buf<<endl;
		int j=0;
		result = strtok( buf, delims ); // create tokens ""object""
		cout<<"the firstNode buf: "<<buf<<endl;
		int firstNode = atoi(result);
		cout<<"the int first node is: "<<firstNode<<endl;
		while( result != NULL ) {
			 cout << "result is: "<<result << " "<<endl;
			 result = strtok( NULL, delims ); // get next Token
			 // if result isn't null insert atoi (result) otherwise insert 0

			 neighbor[firstNode][j]= (result!=NULL) ? atoi(result) : 0;//TODO : tell about change: in place of the first neigbors i will be is neigbors nodes
			 j++;
		}
		cout << endl;
	}
	if(DEBUG){
		cout << "printing matrix after insert:" << endl;
		printMatrix(numberOfNodes_,neighbor);
	}



}//end insert



int ** Shell::makeMatrix(unsigned int numberOfNodes_){
	int ** neighbor = new int* [numberOfNodes_];//[numberOfNodes_];
		for (unsigned int i=0; i<numberOfNodes_;i++){
			neighbor[i]= new int [numberOfNodes_];
	}
    return neighbor;
}
void Shell:: initMatrix(unsigned int numberOfNodes,int **matrix){

	for(unsigned int i = 0 ; i < numberOfNodes;i++){
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















