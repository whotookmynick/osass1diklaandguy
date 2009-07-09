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

OSUI::OSUI()
{

	if (pthread_create(&ui_thread, NULL, wrapper_func, this) != 0)
	{
		perror("UI thread creation failed");
		exit(1);
	}
	_systemCallsCaller = new SystemCalls();
}

void OSUI::run(){
		string input;
		bool stopWhile = false;
		while (!stopWhile)
		{
			cout<<">"<<endl;
			getline(cin,input);
			vector<string> args = splitAroundWhiteSpaces(input);
			if (args[0].compare("mkdir") == 0)
			{
				this->mkdir(args[1]);
			}
			else if (args[0].compare("create"))
			{
				this->create(args[1]);
			}
			stopWhile = input.compare("Exit") == 0;
		}

		cout<<"Simulation Ended"<<endl;
		pthread_exit(0);

	}

int OSUI::mkdir (string dir_name)
{
	char* dir_name_c = (char*)dir_name.c_str();
	_systemCallsCaller->MakeDir(dir_name_c);
}

int OSUI::create(string file_name)
{
	char* file_name_c = (char*)file_name.c_str();
	_systemCallsCaller->MakeFile(file_name_c,0,0);
}
OSUI::~OSUI()
{

}
