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
		//		cout<<"wrapper_fucn before locking"<<endl;
		pthread_mutex_lock(&curThread->_contextMutex);
		//		cout<<"wrapper_fucn after locking"<<endl;
		curThread->run();
		return 0;
	}

	void *batch_Command_Wrapper(void *args)
	{
		OSUI *curThread = (OSUI *)args;
		string& comm = curThread->getNextBatchCommand();
		//		cout<<"batch_command_wrapper comm = "<<comm<<endl;
		curThread->parseAndRunMethod(comm);
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
	_systemCallsCaller->setCurrPID(_pid);
	_fdTable = new vector<int>();
	//	_systemCallsCaller = systemCallsCaller;
	//	_pwd = getRealPWD();
	_pwd = "";
	_processTable = new map<int,OSUI*>();
	pthread_mutex_init(&_contextMutex, NULL);
	init();
	}

OSUI::OSUI(SystemCalls* systemCallsCaller,vector<int>* fdTable,
		int pid,int fatherPid,map<int,OSUI*>* processTable):
			_systemCallsCaller(systemCallsCaller),_fdTable(fdTable),_pid(pid),_fatherPid(fatherPid),
			_processTable(processTable)
			{
			pthread_mutex_init(&_contextMutex, NULL);
			pthread_mutex_lock(&_contextMutex);
			_pwd = "";
			init();
			}

		void OSUI::init()
		{
			//			pthread_mutex_init(&_contextMutex, NULL);
			//			pthread_mutex_lock(&_contextMutex);
			(*_processTable)[_pid] = this;

/****************** I HAVE TO GET THESE LINES BACK, this is just for testing *////
//			if (pthread_create(&ui_thread, NULL, wrapper_func, this) != 0)
//			{
//				perror("UI thread creation failed");
//				exit(1);
//			}
		}

		void OSUI::parseAndRunMethod(string & input)
		{
			vector<string> args = splitAroundWhiteSpaces(input);
			if(args[0].compare("mkdir") == 0)
			{
				this->mkdir(args[1]);
			}else if(args[0].compare("create") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:create <file_name> <flags>"<<endl;
					return;
				}
				int ret;
				ret = this->create(args[1], args[2]);
				cout<<"fd = "<<ret<<endl;
			}else if(args[0].compare("cd") == 0){
				if (args.size() != 2)
				{
					cerr<<"Usage:cd <dir>"<<endl;
					return;
				}
				this->cd(args[1]);
			}else if(args[0].compare("crprc") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:crprc <new pid> <parent>"<<endl;
					return;
				}
				int newPid = atoi(args[1].c_str());
				int newParentPid = atoi(args[2].c_str());
				crprc(newPid, _pid);
			}else if(args[0].compare("swprc") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:swprc <new pid>"<<endl;
					return;
				}
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
				if (args.size() != 2)
				{
					cerr<<"Usage:lck_rd <fd>"<<endl;
					return;
				}
				int fileToLock = atoi(args[1].c_str());
				lck_rd(fileToLock);
			}else if (args[0].compare("lck_wr") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:lck_rd <fd>"<<endl;
					return;
				}
				int fileToLock = atoi(args[1].c_str());
				lck_wr(fileToLock);
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
			}else if (args[0].compare("rmdir_r") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:rmdir <dir_name>"<<endl;
					return;
				}
				rmdir_r(args[1]);
			}else if (args[0].compare("hdlink") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:hdlink <orignial_file_name> <new_file_name>"<<endl;
					return;
				}
				hdlink(args[1],args[2]);
			}else if (args[0].compare("sflink") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:sflink <orignial_file_name> <new_file_name>"<<endl;
					return;
				}
				sflink(args[1],args[2]);
			}
			else if (args[0].compare("mv") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:mv <orignial_file_name> <new_file_name>"<<endl;
					return;
				}
				mv(args[1],args[2]);
			}else if (args[0].compare("writeFile") == 0)
			{
				if (args.size() != 3)
				{
					cerr<<"Usage:writeFile <fd> <file_name>"<<endl;
					return;
				}
				int fdToWriteTo = atoi(args[1].c_str());
				writeFile(fdToWriteTo,args[2]);
			}else if (args[0].compare("rlslck_rd") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:rlslck_rd <fd>"<<endl;
					return;
				}
				int fdToRelease = atoi(args[1].c_str());
				rlslck_rd(fdToRelease);
			}else if (args[0].compare("rlslck_wr") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:rlslck_wr <fd>"<<endl;
					return;
				}
				int fdToRelease = atoi(args[1].c_str());
				rlslck_wr(fdToRelease);
			}else if (args[0].compare("batch") == 0)
			{
				if (args.size() != 2)
				{
					cerr<<"Usage:batch <file_name>"<<endl;
					return;
				}
				batch(args[1]);
			}


		}

		void OSUI::run(){
			string input;
			bool stopWhile = false;
			while (!stopWhile)
			{
				cout<<_pwd<<"> ";
				getline(cin,input);
				cout<<"OSUI::run currentpid = "<<_pid<<endl;
				//				cout<<"OSUI::run input = "<<input<<endl;
				parseAndRunMethod(input);
				stopWhile = input.compare("Exit") == 0;
			}

			cout<<"Simulation Ended"<<endl;
			pthread_exit(0);

		}

		int OSUI::mkdir (string dir_name)
		{
			string originalPWD = _pwd;
			vector<string> pathAndShortName = getFullPathAndShortName(dir_name);
			dir_name = pathAndShortName[0];
			this->cd(dir_name);
			string finalToSendToFile = _pwd + pathAndShortName.at(1);
			int i_nodeNum = _systemCallsCaller->MakeDir((char*)finalToSendToFile.c_str());
			_pwd = originalPWD;
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
			int fd = _systemCallsCaller->MakeFile(file_name_c,REGULARE_FILE,flagInt);
			if (fd >= 0)
			{
				_fdTable->push_back(fd);
			}
			return fd;
		}

		int OSUI::cd(string new_dir)
		{
			string dir_change = new_dir.substr(0,new_dir.find('/'));
			string* realFileName = _systemCallsCaller->myIsDir((char*)dir_change.c_str());
			if (realFileName != NULL)
			{
				string new_dir = *realFileName;
				dir_change = new_dir.substr(0,new_dir.find('/'));
				cout<<"OSUI::cd isADir realFileName = "<<*realFileName<<endl;
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

		int OSUI::crprc(int id, int parent)
		{
			if (processExists(id))
			{
				cerr<<"process id already exists"<<endl;
				return -1;
			}
			vector<int>* newProcessFdTable = _fdTable;
			if (parent == -1)
			{
				newProcessFdTable = new vector<int>();
			}
			new OSUI(_systemCallsCaller,newProcessFdTable,id,parent,_processTable);
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
//			string working_directory = getFullPath(file_name);
			string originalPWD = _pwd;
			vector<string> pathAndShortName = getFullPathAndShortName(file_name);
			file_name = pathAndShortName[0];
			this->cd(file_name);
			string finalToSendToFile = _pwd + pathAndShortName.at(1);
			//			cout<<"OSUI::open working_directory = "<<working_directory<<endl;
			int new_fd = _systemCallsCaller->Open((char*)(finalToSendToFile.c_str()),flagInt);
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
			rlslck_rd(fd);
			rlslck_wr(fd);
			return _systemCallsCaller->Close(fd);
		}

		int OSUI::batch(string file_name)
		{
			ifstream myfile(file_name.c_str());
			if (myfile.is_open())
			{
				while (!myfile.eof())
				{
					string command;
					getline(myfile,command);
					string *commPointer = new string(command.c_str());
					cout<<"OSUI::batch command = "<<(command)<<endl;
					if (command.size() > 0)
					{
						pthread_t batchCommandThread;
						_batchCommandsQueue.push(*commPointer);
						pthread_create(&batchCommandThread, NULL, batch_Command_Wrapper, this);
					}
				}
				return 1;
			}
			else
			{
				cerr<<"Could not open file"<<endl;
				return -1;
			}
		}

		int OSUI::lck_rd(int fd)
		{
			//			int ans = _systemCallsCaller->lockRead(fd,_pid);
			int ans = _systemCallsCaller->lockRead(fd);
			if (ans > 0)
			{
				_lockedReadFile.push_back(fd);
			}
			return ans;
		}

		int OSUI::lck_wr(int fileToLock)
		{
			int ans = _systemCallsCaller->lockWrite(fileToLock);
			if (ans > 0)
			{
				_lockedWriteFile.push_back(fileToLock);
			}
			return ans;
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
			if (processExists(newPid))
			{
				OSUI* newProc = (*_processTable)[newPid];
				_systemCallsCaller->setCurrPID(newPid);
				newProc->keepRunning();
				cout<<"Current process ID is "<<newPid<<endl;
			}
			else
			{
				cerr<<"Could not find pid"<<endl;
				return;
			}
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
			cout<<"OSUI::ls dir_name = "<<dir_name<<endl;
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

		int OSUI::rmdir_r(string dir_name)
		{
			string working_directory = getFullPath(dir_name);
			int ans = _systemCallsCaller->RmDir_R(working_directory);
			return ans;
		}

		int OSUI::hdlink(string file_to_link_to,string new_file_name)
		{
			string working_dircetory = getFullPath(file_to_link_to);
			string new_file_path = getFullPath(new_file_name);
			int ret = _systemCallsCaller->MakeHLink((char*)(working_dircetory.c_str()),(char*)(new_file_path.c_str()));
			return ret;
		}

		int OSUI::sflink(string file_to_link_to,string new_file_name)
		{
			string working_dircetory = getFullPath(file_to_link_to);
			string new_file_path = getFullPath(new_file_name);
			int fd = _systemCallsCaller->MakeFile((char*)(new_file_path.c_str()), SOFT_LINK, READ_AND_WRITE);
			if (fd != -1)
			{
				file_to_link_to.append("$");
				write(fd,file_to_link_to);
				_systemCallsCaller->Close(fd);
				return 1;
			}
			return -1;
		}

		int OSUI::mv(string old_file_name,string new_file_name)
		{
			//			string original_file_path = getFullPath(old_file_name);
			//			string new_file_path = getFullPath(new_file_name);
			int ret = hdlink(old_file_name,new_file_name);
			if (ret < 0)
			{
				cerr<<"Cannot rename"<<endl;
				return ret;
			}
			ret = rm(old_file_name);
			if (ret < 0)
			{
				cerr<<"Cannot rename"<<endl;
				return ret;
			}
		}

		int OSUI::writeFile (int fd,string fileName)
		{
			cout<<"OSUI::writeFile fd = "<<fd<<" fileName = "<<fileName<<endl;
			ifstream myfile(fileName.c_str(),ios::in);
			if (myfile.is_open())
			{
				cout<<"OSUI::writeFile file is opened"<<endl;
				string line,value;
				while (! myfile.eof() )
				{
					getline (myfile,line);
					line.append("\n");
					this->write(fd,line);
					cout << line << endl;
				}
				myfile.close();
				return 1;
			}
			else
			{
				cout << "Unable to open file"<<endl;
				return -1;
			}
		}

		int OSUI::rlslck_rd(int fd)
		{
			vector<int>::iterator place = find(_lockedReadFile.begin(),_lockedReadFile.end(),fd);
			if ( place != _lockedReadFile.end())
			{
				_lockedReadFile.erase(place);
				return _systemCallsCaller->releaseLockRead(fd);
			}
			cerr<<"File is not locked by you or is not open"<<endl;
			return -1;
		}

		int OSUI::rlslck_wr(int fd)
		{
			vector<int>::iterator place = find(_lockedWriteFile.begin(),_lockedWriteFile.end(),fd);
			if ( place != _lockedWriteFile.end())
			{
				_lockedWriteFile.erase(place);
				return _systemCallsCaller->releaseLockWrite(fd);
			}
			cerr<<"File is not locked by you or is not open"<<endl;
			return -1;
		}

		vector<string> OSUI::getFullPathAndShortName(string file_name)
		{
			vector<string> ans;
			string fullPath = getFullPath(file_name);
			string shortName = file_name;
			if (fullPath.size()>0)
			{
				shortName = file_name.substr(file_name.find_last_of('/') + 1);
			}
			ans.push_back(fullPath);
			ans.push_back(shortName);
			return ans;
		}

		string OSUI::getFullPath(string file_name)
		{
			if (file_name.find("./") == 0)
			{
				file_name = dealWithRelativePath(file_name.substr(2));
			}
			int slashPlace = file_name.find_last_of('/');
			if (slashPlace == string::npos)
			{
				return "";
			}
			return file_name.substr(0,slashPlace);

		}

		string OSUI::dealWithRelativePath(string file_name)
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
