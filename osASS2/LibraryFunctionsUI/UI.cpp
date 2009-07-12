/*
 * UI.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "UI.h"
#include <pthread.h>

extern "C"
{
	void *wrapper_func(void *args)
	{
		OSUI *curThread = (OSUI *)args;
		curThread->run();
		return 0;
	}
}

static vector<string> splitAroundWhiteSpaces(string line)
{
	string buf; // Have a buffer string
	stringstream ss(line); // Insert the string into a stream
	vector<string> tokens; // Create vector to hold our words
	while (ss >> buf)
		tokens.push_back(buf);
	return tokens;

}

OSUI::OSUI(SystemCalls* systemCallsCaller):_systemCallsCaller(systemCallsCaller)
{
	_fdTable = new vector<int>();
//	_systemCallsCaller = systemCallsCaller;
	_pwd = getRealPWD();
	init();
}

OSUI::OSUI(SystemCalls* systemCallsCaller,vector<int>* fdTable,int pid,string pwd,vector<OSUI*>* processTable):
_systemCallsCaller(systemCallsCaller),_fdTable(fdTable),_pid(pid),_pwd(pwd),_processTable(processTable)
{
	init();
}

void OSUI::init()
{
	_processTable->push_back(this);
	if (pthread_create(&ui_thread, NULL, wrapper_func, this) != 0)
	{
		perror("UI thread creation failed");
		exit(1);
	}
}

void OSUI::run(){
	string input;
	bool stopWhile = false;
	while (!stopWhile)
	{
		cout<<_pwd<<"> ";
		getline(cin,input);
		vector<string> args = splitAroundWhiteSpaces(input);
		if (args[0].compare("mkdir") == 0)
		{
			this->mkdir(args[1]);
		}
		else if (args[0].compare("create") == 0)
		{
			this->create(args[1],args[2]);
		}
		else if (args[0].compare("cd") == 0)
		{
			cout<<"run found cd and args[1] = "<<args[1]<<endl;
			this->cd(args[1]);
		}
		stopWhile = input.compare("Exit") == 0;
	}

	cout<<"Simulation Ended"<<endl;
	pthread_exit(0);

}

int OSUI::mkdir (string dir_name)
{
	string temp;
	temp = _pwd;
	temp.append("/" + dir_name);
//	char* dir_name_c = (char*)dir_name.c_str();
	int i_nodeNum = _systemCallsCaller->MakeDir((char*)temp.c_str());
	return i_nodeNum;
}

int OSUI::create(string file_name,string flags)
{
	int flagInt = READ_AND_WRITE;
	if (flags.compare("read-only") == 0)
	{
		flagInt = READ_ONLY;
	}
	char* file_name_c = (char*)file_name.c_str();
	int fd = _systemCallsCaller->MakeFile(file_name_c,0,flagInt);
	_fdTable->push_back(fd);
	return fd;
}

int OSUI::cd(string new_dir)
{
	string dir_change = new_dir.substr(0,new_dir.find('/'));
	if (_systemCallsCaller->isDir((char*)dir_change.c_str()))
	{
		string next_dir;
		if (new_dir.empty() | new_dir.compare("/") == 0)
			return 1;
		if (new_dir.at(0) == '.')
		{
			//		cout<<"cd goDownDir()"<<endl;
			goDownDir();
		}
		else
		{
			//		cout<<"cd goUpDir"<<endl;
			string firstDirChange;
			new_dir = new_dir.substr(1);
			firstDirChange = new_dir.substr(0,new_dir.find('/'));
			_pwd.append("/" + firstDirChange);
		}
		int nextDirPlace = new_dir.find('/');
		if (nextDirPlace == -1)
			return 1;
		next_dir = new_dir.substr(nextDirPlace);
		return this->cd(next_dir);
	}
	cerr<<"Could not find dir"<<endl;
	return 1;
}

void OSUI::goDownDir()
{
	int lastDirPlace = _pwd.find_last_of("/");
	_pwd = _pwd.substr(0,lastDirPlace);
	//	cout<<"goDownDir _pwd = "<<_pwd<<endl;
}

int OSUI::crprc(int id, int parent)
{
	if (processExists(id))
	{
		cout<<"process id already exists"<<endl;
		return -1;
	}
	new OSUI(_systemCallsCaller,_fdTable,id,_pwd,_processTable);
}

bool OSUI::processExists(int pid)
{

	for(int i = 0; i < _processTable->size();i++)
	{
		OSUI* curr = _processTable->at(i);
		if (curr->getPid() == pid)
		{
			return true;
		}
	}
	return false;
}

string OSUI::getRealPWD()
{
	char *path = NULL;
	path = getcwd(NULL, 0); // or _getcwd
	return string(path);
}

OSUI::~OSUI()
{

}
