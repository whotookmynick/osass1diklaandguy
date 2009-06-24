#include "Shell.h"
#define DEBUG true
using namespace std;
//buf=10;

int main(int argc,const char* argv[]){
    string arg = "";
    string fileConfig= "";
    string uniqueID="";
    string sourceID ="";
    string targetID ="";
    string textMsg="";
    Shell shell;
 GET_COMMAND:
		while (true){
    	  cout << "enter command" <<endl;
    	  getline(cin,arg);
    	  //cout << "your choice : " <<arg<<endl;

    	  //------1--------
    	  if(arg=="Exit"){
    		  cout<<"your choice is : exit "<<endl;
    		  shell.Exit(0);
    	  }

    	  //------2--------
    	  else if(arg=="CreateNet"){
    		  cout<<"your choice is : CreateNet "<<endl;
    		  cout<<"pleas enter file configuration name : "<<endl;
    		  getline(cin,fileConfig);
    		  shell.createNet(fileConfig.c_str());
    		  goto GET_COMMAND;

    	  }

    	  //------3--------
    	  else if(arg== "KillNode"){
    		  cout<<"your choice is : KillNode "<<endl;
    		  cout<<"pleas enter the node id you wan'a kill : "<<endl;
    		  getline(cin,uniqueID);
    		  if(uniqueID==""){
    		   	  goto GET_COMMAND;
    		 }
    		 shell.killNode();
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
    		  shell.reviveNode();
    		  goto GET_COMMAND;

    	  }

    	  //------5--------
    	  else if(arg=="Killall"){
    		  cout<<"your choice is : Killall "<<endl;
    		  shell.killAll();
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
    		  shell.printRt();
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
    		  shell.sendPacket();
    		  goto GET_COMMAND;
		  }

    	  //------8--------
    	  else if(arg=="Run"){
    		  cout<<"your choice is : Run "<<endl;
    		  shell.Run();
    		  goto GET_COMMAND;
    	  }
      }//end while
}//end main
void Shell::Exit(int i){
	exit(i);
}
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

}//end creatNet

	void Shell::killNode() {
		//TODO

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

	void Shell::sendPacket() {
		//TODO

	}

	void Shell::Run()  {
		//TODO

	}

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















