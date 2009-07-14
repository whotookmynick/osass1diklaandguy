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
	int newFD = this->Open((char*)new_file_name.c_str(),flag_access_permissions);
	return newFD;
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
	string dir_nameShort = dir_nameString.substr(dir_nameString.find_last_of("/")+1);
	list<FileEntry> currPWD;
	int pwdInode;
	currPWD = readPWDDir(pwd,&pwdInode);
	list<FileEntry>::iterator it;// = currPWD.begin();
	it = getFileEntryFromDir(currPWD,dir_nameShort.c_str());
	_fileSys->d_write(pwdInode,currPWD);

}

list<FileEntry>::iterator SystemCalls::getFileEntryFromDir(list<FileEntry>& currPWD,const char* file_name)
{
	list<FileEntry>::iterator it = currPWD.begin();
	bool found = false;
	while (it != currPWD.end() & !found)
	{
		FileEntry curr = *it;
		char* currFileName = curr.getFileName();
		if (strcmp(file_name,currFileName) == 0)
		{
			found = true;
			return it;
		}
		++it;
	}
	return it;
}

int SystemCalls::RmFile(char* file_name){
	return 1;
}

int SystemCalls::ls(char*dir_name, char * buf){
	return 1;
}


int SystemCalls::Open(char* file_name, int flag_access_permissions){
	if (_openFileTable.size() == MAX_OPEN_FILES)
	{
		cerr<<"Too many open files"<<endl;
		throw new TooManyFilesException();
	}
	string file_nameString(file_name);
	string pwd = file_nameString.substr(0,file_nameString.find_last_of("/"));
	string file_nameShort = file_nameString.substr(file_nameString.find_last_of("/")+1);
	int pwdInode = -1;
	list<FileEntry> currPWD = readPWDDir(pwd,&pwdInode);
	list<FileEntry>::iterator it = getFileEntryFromDir(currPWD,file_nameShort.c_str());
	int ret = -1;
	if (it != currPWD.end())
	{
		int file_inode = (*it).getInodeNum();
		Descriptor* newDesc = new Descriptor(flag_access_permissions,file_inode);
		pthread_mutex_lock(&_currFDMutex);
		_currFD++;
		_openFileTable[_currFD] = newDesc;
		ret = _currFD;
		pthread_mutex_unlock(&_currFDMutex);

	}
	return ret;
}

int SystemCalls::Close(int fd){
	pthread_mutex_lock(&_currFDMutex);
	_openFileTable.erase(fd);
	pthread_mutex_unlock(&_currFDMutex);
	return 1;
}

int SystemCalls::Seek(int fd, int location){
	pthread_mutex_lock(&_currFDMutex);
	Descriptor *desc = _openFileTable[fd];
	desc->_filePosition = location;
	pthread_mutex_unlock(&_currFDMutex);
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
	int fd_inode = _openFileTable[fd]->_inode;
	if (isLockedRead(fd_inode) | isLockedWrite(fd_inode))
	{
		cerr<<"File is already locked"<<endl;
		return -1;
	}
	_readLocks.push_back(fd_inode);
	return 1;
}

bool SystemCalls::isLockedRead(int i_node_num)
{
	for (int i = 0; i < _readLocks.size(); i++)
	{
		if (_readLocks[i] == i_node_num)
		{
			return true;
		}
	}
	return false;
}

bool SystemCalls::isLockedWrite(int i_node_num)
{
	for (int i = 0; i < _writeLocks.size(); i++)
	{
		if (_writeLocks[i] == i_node_num)
		{
			return true;
		}
	}
	return false;
}

int SystemCalls::lockRead(int fd,int pid)
{
	pthread_mutex_lock(&lock_mutex);
	lockingFd = pid;
	lockRead(fd);
	pthread_mutex_unlock(&lock_mutex);
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
