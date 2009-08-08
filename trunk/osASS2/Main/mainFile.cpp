/*
 * mainFile.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include "../LibraryFunctionsUI/UI.h"
#include "../SystemCalls/SystemCalls.h"

using namespace std;

void parseConfigFile(ifstream *myfile,int configargs[3])
{
	string line,value;
	getline (*myfile,line);
	value = line.substr(line.find("=") + 1);
	configargs[0] = atoi(value.c_str());
	getline (*myfile,line);
	value = line.substr(line.find("=") + 1);
	configargs[1] = atoi(value.c_str());
	getline (*myfile,line);
	value = line.substr(line.find("=") + 1);
	configargs[2] = atoi(value.c_str());


}

int main(int argc,char** argv)
{
	if (argc < 2)
	{
		cout<<"Usage: sim <config_filename>"<<endl;
		return 0;
	}
	int configargs[3];
	ifstream *myfile = new ifstream(argv[1]);
	if (myfile->is_open())
	{
		parseConfigFile(myfile,configargs);
		myfile->close();
		cout <<"finished parsing" <<endl;
	}
	else
	{
		cout << "Unable to open file"<<endl;
		return 0;
	}
	cout<<"Creating system calls layer"<<endl;
	SystemCalls* systemCallsCaller = new SystemCalls(configargs[0],configargs[1],configargs[2]);
	OSUI* mainUI = new OSUI(systemCallsCaller);
//	mainUI->run();
	pthread_exit(0);
}


// how to check if a file exists
//#include <sys/stat.h>
//
//bool FileExists(string strFilename) {
//  struct stat stFileInfo;
//  bool blnReturn;
//  int intStat;
//
//  // Attempt to get the file attributes
//  intStat = stat(strFilename.c_str(),&stFileInfo);
//  if(intStat == 0) {
//    // We were able to get the file attributes
//    // so the file obviously exists.
//    blnReturn = true;
//  } else {
//    // We were not able to get the file attributes.
//    // This may mean that we don't have permission to
//    // access the folder which contains this file. If you
//    // need to do that level of checking, lookup the
//    // return values of stat which will give you
//    // more details on why stat failed.
//    blnReturn = false;
//  }
//
//  return(blnReturn);
//}
