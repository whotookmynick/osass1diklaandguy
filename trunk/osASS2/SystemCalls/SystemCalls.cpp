/*
 * SystemCalls.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "SystemCalls.h"

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

SystemCalls::SystemCalls(int dataBlockSize,int numberOfInodes,int diskSize):_currFD(1)
{
	pthread_mutex_init(&_currFDMutex, NULL);
	_fileSys = new FileSystem(dataBlockSize,numberOfInodes,diskSize);
}

SystemCalls::~SystemCalls(){}



//---------------------------------------------------------------------------/
//								Help function
//---------------------------------------------------------------------------/


//---------------------------------------------------------------------------/
//								Low Level functions
//---------------------------------------------------------------------------/
int SystemCalls::MakeFile(char* file_name,int type,int flag_access_permissions){
	string file_nameString(file_name);
	string pwd = file_nameString.substr(0,file_nameString.find_last_of("/"));
	int pwdInode = -1;
	list<FileEntry> currPWD = readPWDDir(pwd,&pwdInode);
	int newFile_iNode  = _fileSys->createFile(flag_access_permissions);
	if (! newFile_iNode)
	{
		cout<<"Could not create file"<<endl;
	}
	string new_file_name = file_nameString.substr(file_nameString.find_last_of("/") + 1);
	FileEntry newDirEntry(newFile_iNode,(char*)new_file_name.c_str(),-1);
	currPWD.push_back(newDirEntry);
	_fileSys->d_write(pwdInode,currPWD);
	pthread_mutex_lock(&_currFDMutex);
	_currFD++;
	pthread_mutex_unlock(&_currFDMutex);
	return _currFD;
}


int SystemCalls::MakeHLink(char* target_file_name, char*file_name){
	return 1;
}

int SystemCalls::MakeDir(char* dir_name){
	string dir_nameString(dir_name);
	string pwd = dir_nameString.substr(0,dir_nameString.find_last_of("/"));
	int pwdInode = -1;
	list<FileEntry> currPWD = readPWDDir(pwd,&pwdInode);
	string new_dir_name = dir_nameString.substr(dir_nameString.find_last_of("/") + 1);
	int newDir_iNode = _fileSys->createDir();
	FileEntry newDirEntry(newDir_iNode,(char*)new_dir_name.c_str(),-1);
	currPWD.push_back(newDirEntry);
	_fileSys->d_write(pwdInode,currPWD);
	return 1;
}

list<FileEntry> SystemCalls::readPWDDir(string pwd,int *lastInode)
{
	int pwdInode = ROOT_PWD_INODE;
	list<FileEntry> currPWD;
	bool done = false;
	while (!done)
	{
		string currDir = pwd.substr(0, pwd.find("/"));
		currPWD = _fileSys->d_read(pwdInode);
		pwdInode = -1;
		list<FileEntry>::iterator it = currPWD.begin();
		while (it != currPWD.end() & pwdInode == -1)
		{
			FileEntry curr = *it;
			string currEntryName(curr.getFileName());
			if (currDir.compare(currEntryName) == 0)
			{
				pwdInode = curr.getInodeNum();
			}
			++it;
		}
		done = pwd.empty();
	}
	currPWD = _fileSys->d_read(pwdInode);
	*lastInode = pwdInode;
	return currPWD;
}

int SystemCalls::RmDir(char* dir_name){
	string dir_nameString(dir_name);
	string pwd = dir_nameString.substr(0,dir_nameString.find_last_of("/"));
	list<FileEntry> currPWD;
	int pwdInode;
	currPWD = readPWDDir(pwd,&pwdInode);
	list<FileEntry>::iterator it = currPWD.begin();
	bool found = false;
	while (it != currPWD.end() & !found)
	{
		FileEntry curr = *it;
		char* currFileName = curr.getFileName();
		if (strcmp(dir_name,currFileName) == 0)
		{
			found = true;
			currPWD.erase(it);
		}
		++it;
	}
	_fileSys->d_write(pwdInode,currPWD);

}

int SystemCalls::RmFile(char* file_name){
	return 1;
}

int SystemCalls::ls(char*dir_name, char * buf){
	return 1;
}


int SystemCalls::Open(char* filename, int flag_access_permissions){
	return 1;
}

int SystemCalls::Close(int fd){
	return 1;
}

int SystemCalls::Seek(int fd, int location){
	return 1;
}

int SystemCalls::Read (int fd, int nBytes, char *Buffer){
	return 1;
}


int SystemCalls::Write (int fd, int nBytes,char * Buffer){
	return 1;
}

int SystemCalls::moveFile(char* parendDir, char * new_dest){
	return 1;
}

bool SystemCalls::isDir(char * address){
	return false;
}

int SystemCalls::lockRead(int fd){
	return 1;
}

int SystemCalls::lockWrite(int fd){
	return 1;
}

int SystemCalls::releaseLockRead(int fd){
	return 1;
}

int SystemCalls::releaseLockWrite(int fd){
	return 1;
}
