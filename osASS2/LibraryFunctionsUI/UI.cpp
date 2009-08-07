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

	void *batch_Command_Wrapper(void *args)
	{
		OSUI *curThread = (OSUI *)args;
		string* comm = curThread->getNextBatchCommand();
		curThread->parseAndRunMethod(*comm);
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

OSUI::OSUI(SystemCalls* systemCallsCaller):
	_systemCallsCaller(systemCallsCaller),_pid(0),_fatherPid(-1)
	{
	_fdTable = new vector<int>();
	//	_systemCallsCaller = systemCallsCaller;
//	_pwd = getRealPWD();
	_pwd = "";
	_processTable = new map<int,OSUI*>();
	init();
	}

OSUI::OSUI(SystemCalls* systemCallsCaller,vector<int>* fdTable,
		int pid,int fatherPid,string pwd,map<int,OSUI*>* processTable):
			_systemCallsCaller(systemCallsCaller),_fdTable(fdTable),_pid(pid),_fatherPid(fatherPid),
			_pwd(pwd),_processTable(processTable)
		{
			init();
			pthread_mutex_lock(&_contextMutex);
		}

		void OSUI::init()
		{
			(*_processTable)[_pid] = this;
//			if (pthread_create(&ui_thread, NULL, wrapper_func, this) != 0)
//			{
//				perror("UI thread creation failed");
//				exit(1);
//			}
			pthread_mutex_init(&_contextMutex, NULL);
		}

		void OSUI::parseAndRunMethod(string & input)
		{
			vector<string> args = splitAroundWhiteSpaces(input);
			if(args[0].compare("mkdir") == 0)
			{
				this->mkdir(args[1]);
			}else if(args[0].compare("create") == 0)
			{
				int ret;
				ret = this->create(args[1], args[2]);
				cout<<"fd = "<<ret<<endl;
			}else if(args[0].compare("cd") == 0){
				this->cd(args[1]);
			}else if(args[0].compare("crprc") == 0)
			{
				int newPid;
				newPid = atoi(args[1].c_str());
				int newParentPid;
				newParentPid = atoi(args[2].c_str());
				this->crprc(newPid, _pid);
			}else if(args[0].compare("swprc") == 0)
			{
				int newPid = atoi(args[1].c_str());
				switchToProcess(newPid);
				pthread_mutex_lock(&_contextMutex);
			}else if(args[0].compare("open") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:Open <file_name> <flags>"<<endl;
					return;
				}
				int ret = open(args[1], args[2]);
			}else if(args[0].compare("close") == 0)
			{
				int fdToClose = atoi(args[1].c_str());
				close(fdToClose);
			}else if (args[0].compare("lck_rd") == 0)
			{
				int fileToLock = atoi(args[1].c_str());
				lck_rd(fileToLock);
			}else if (args[0].compare("lck_wr") == 0)
			{
				int fileToLock = atoi(args[1].c_str());
//				lck_wr(fileToLock);
			} else if (args[0].compare("ls") == 0)
			{
//				cout<<"run working.size() = "<<working_directory.size()<<" working.length() = "<<working_directory.length()<<endl;
				if (args.size() > 2)
				{
					cerr<<"Usage:ls [dir_name]"<<endl;
					return;
				}
				if (args.size() == 2)
				{
					ls(args[1],args.size());
				}
				else
				{
					ls("",args.size());
				}
			} else if (args[0].compare("read") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:read <fd> <numOfBytes>"<<endl;
					return;
				}
				int new_fd = atoi(args[1].c_str());
				int num_bytes_to_read = atoi(args[2].c_str());
				read(new_fd,num_bytes_to_read);
			} else if (args[0].compare("write") == 0)
			{
				if (args.size() < 3)
				{
					cerr<<"Usage:write <fd> <string to write>"<<endl;
					return;
				}
				int new_fd = atoi(args[1].c_str());
				string string_to_write = args[2];
				for (int k = 3; k < args.size();k++)
				{
					string_to_write.append(" ");
					string_to_write.append(args[k]);
				}
				write(new_fd,string_to_write);
			}else if (args[0].compare("rm") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:rm <file_name>"<<endl;
					return;
				}
				rm(args[1]);
			}else if (args[0].compare("rmdir") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:rmdir <dir_name>"<<endl;
					return;
				}
				rmdir(args[1]);
			}
		}

		void OSUI::run(){
			string input;
			bool stopWhile = false;
			while (!stopWhile)
			{
				cout<<_pwd<<"> ";
				getline(cin,input);
				parseAndRunMethod(input);
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
			bool correct_flage = false;
			if (flags.compare("read-and-write") == 0)
			{
				flagInt = READ_AND_WRITE;
				correct_flage = true;
			}
			if (flags.compare("read-only") == 0)
			{
				flagInt = READ_ONLY;
				correct_flage = true;
			}
			if (!correct_flage)
			{
				cerr<<"flags not in correct syntax"<<endl;
				return -1;
			}
			string temp("/");
			temp = _pwd + temp;
			file_name = temp + file_name;
			file_name = file_name.substr(1);
			char* file_name_c = (char*)file_name.c_str();
			int fd = _systemCallsCaller->MakeFile(file_name_c,0,flagInt);
			if (fd != -1)
			{
				_fdTable->push_back(fd);
			}
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
				if (dir_change.compare("..") == 0)
				{
					//		cout<<"cd goDownDir()"<<endl;
					goDownDir();
				}
				else
				{
					//		cout<<"cd goUpDir"<<endl;
					string firstDirChange;
//					new_dir = new_dir.substr(1);
					firstDirChange = new_dir.substr(0,new_dir.find('/'));
					_pwd.append("/" + firstDirChange);
				}
				int nextDirPlace = new_dir.find('/');
				if (nextDirPlace == string::npos)
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

		string OSUI::getFullPath(string file_name)
		{
//			working_directory = _pwd + working_directory;
			string working_directory = "";
			if (_pwd.size() > 0)
			{
				working_directory = _pwd + "/";

			}
			working_directory = working_directory + file_name;
//			if (working_directory.size() > 0 && working_directory.at(working_directory.size()-1) != '/')
//			{
//				working_directory.append("/");
//			}
			if (working_directory.size() > 0 && working_directory.at(0) == '/')
			{
				working_directory = working_directory.substr(1);
			}
			return working_directory;
		}

		int OSUI::crprc(int id, int parent)
		{
			if (processExists(id))
			{
				cout<<"process id already exists"<<endl;
				return -1;
			}
			new OSUI(_systemCallsCaller,_fdTable,id,parent,_pwd,_processTable);
			return 1;
		}
		int OSUI::open(string file_name,string flags)
		{
			int flagInt = READ_AND_WRITE;
			bool correct_flage = false;
			if (flags.compare("read-and-write") == 0)
			{
				flagInt = READ_AND_WRITE;
				correct_flage = true;
			}
			if (flags.compare("read-only") == 0)
			{
				flagInt = READ_ONLY;
				correct_flage = true;
			}
			if (!correct_flage)
			{
				cerr<<"flags not in correct syntax"<<endl;
				return -1;
			}
			string working_directory = getFullPath(file_name);
//			cout<<"OSUI::open working_directory = "<<working_directory<<endl;
			int new_fd = _systemCallsCaller->Open((char*)(working_directory.c_str()),flagInt);
			cout<<"fd = "<<new_fd<<endl;
			_fdTable->push_back(new_fd);
			return new_fd;
		}

		int OSUI::close(int fd)
		{
			vector<int>::iterator place = find(_fdTable->begin(),_fdTable->end(),fd);
			if (place == _fdTable->end())
			{
				cerr<<"You cannot close this file or it is not open"<<endl;
				return -1;
			}
			_fdTable->erase(place);
			place = find(_lockedReadFile.begin(),_lockedReadFile.end(),fd);
			if ( place != _lockedReadFile.end())
			{
				_lockedReadFile.erase(place);
			}
			return _systemCallsCaller->Close(fd);
		}

		int OSUI::batch(string file_name)
		{
			ifstream *myfile = new ifstream(file_name.c_str());
			while (!myfile->eof())
			{
				string *command;
				getline(*myfile,*command);
				pthread_t* batchCommandThread;
				_batchCommandsQueue.push(command);
				pthread_create(batchCommandThread, NULL, batch_Command_Wrapper, this);
			}
			return 1;
		}

		int OSUI::lck_rd(int fd)
		{
			int ans = _systemCallsCaller->lockRead(fd,_pid);
			if (ans > 0)
			{
				_lockedReadFile.push_back(fd);
			}
		}

		bool OSUI::processExists(int pid)
		{
			map<int,OSUI*>::iterator proc = _processTable->find(pid);
			if (proc == _processTable->end())
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		void OSUI::switchToProcess(int newPid)
		{
			OSUI* newProc = (*_processTable)[newPid];
			newProc->keepRunning();
		}

		void OSUI::ls(string dir_name,int numOfArgs)
		{
			string working_directory = _pwd;
			if (numOfArgs == 2)
			{
				working_directory = dir_name;
				working_directory = _pwd + working_directory;
			}

			if (working_directory.size() > 0 && working_directory.at(working_directory.size()-1) != '/')
			{
				working_directory.append("/");
				working_directory = working_directory.substr(1);
			}
			//			cout<<"OSUI::ls dir_name = "<<dir_name<<endl;
			char buff[5000];
			_systemCallsCaller->ls((char*)working_directory.c_str(),buff);
			cout<<buff<<endl;
		}

		int OSUI::read(int fd,int num_bytes)
		{
			char readBuffer[num_bytes+10];
			int ans =  _systemCallsCaller->Read(fd,num_bytes,readBuffer);
			if (ans == -1)
			{
				return ans;
			}
			readBuffer[ans] = '\0';
			ostringstream bufferString;
			bufferString<<readBuffer;
			bufferString<<"\n";
			bufferString<<ans;
			cout<<bufferString.str()<<endl;
			return ans;
		}

		int OSUI::write(int fd,string buffer)
		{
			int ans = _systemCallsCaller->Write(fd,buffer.size(),(char*)(buffer.c_str()));
			cout<<"OSUI::write ans = "<<ans<<endl;
			return ans;
		}

		int OSUI::rm(string file_name)
		{
			string working_directory = getFullPath(file_name);
//			cout<<"OSUI::rm working_directory = "<<working_directory<<endl;
			int ans = _systemCallsCaller->RmFile((char*)(working_directory.c_str()));
			return ans;

		}

		int OSUI::rmdir(string dir_name)
		{
			string working_directory = getFullPath(dir_name);
			int ans = _systemCallsCaller->RmDir((char*)(working_directory.c_str()));
			return ans;
		}

		void OSUI::keepRunning()
		{
			pthread_mutex_unlock(&_contextMutex);
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
