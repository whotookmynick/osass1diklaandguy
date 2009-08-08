/*
 * SystemCalls.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "SystemCalls.h"
#include "../LibraryFunctionsUI/UI.h"

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

SystemCalls::SystemCalls(int dataBlockSize,int numberOfInodes,int diskSize)//:_currFD(1)
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
//	cout<<"MakeFile starting readPWDDir pwd = "<<pwd<<endl;
	list<FileEntry> *currPWD = readPWDDir(pwd,&pwdInode);
	int newFile_iNode  = _fileSys->createFile(REGULARE_FILE);
	if ( newFile_iNode == -1)
	{
		cerr<<"Could not create file"<<endl;
		return -1;
	}
//	cout<<"MakeFile finished readPWDDir successfully"<<endl;
	string new_file_name = file_nameString.substr(file_nameString.find_last_of("/") + 1);
	FileEntry *newDirEntry = new FileEntry(newFile_iNode,(char*)new_file_name.c_str(),0);
	currPWD->push_back(*newDirEntry);
	_fileSys->d_write(pwdInode,*currPWD);
	_fileSys->setNumOfHardLinks(newFile_iNode,1);
	int newFD = this->Open(file_name,flag_access_permissions);
	if (type == SOFT_LINK)
	{
		_fileSys->setFileType(newFile_iNode,SOFT_LINK);
	}
//	cout<<"SC::makefile file type = "<<_fileSys->getFileType(newFile_iNode)<< endl;
	return newFD;
}


int SystemCalls::MakeHLink(char* target_file_name, char *sourcefile_name){
	string targetFileNameString(target_file_name);
	string targetPwdString = targetFileNameString.substr(0,targetFileNameString.find_last_of("/"));
	int pwdInode = -1;
	list<FileEntry> *targetPWD = readPWDDir(targetPwdString,&pwdInode);
	string targetShortName = targetFileNameString.substr(targetFileNameString.find_last_of("/") + 1);
	list<FileEntry>::iterator targetIt = getFileEntryFromDir(*targetPWD,targetShortName.c_str());
	if (targetIt == targetPWD->end())
	{
		cerr<<"Target file does not exist"<<endl;
		return -1;
	}
	FileEntry targetFileEntry = (*targetIt);
	int targetInode = targetFileEntry.getInodeNum();
	if (_fileSys->getFileType(targetInode) == DIR_TYPE)
	{
		cerr<<"Cannot make hard link to directory"<<endl;
		return -1;
	}

	string sourceFileNameString(sourcefile_name);
	string sourcePwdString = sourceFileNameString.substr(0,sourceFileNameString.find_last_of("/"));
	list<FileEntry> *sourcePWD = readPWDDir(sourcePwdString,&pwdInode);
	string sourceShortName = sourceFileNameString.substr(sourceFileNameString.find_last_of("/") + 1);
		char* sourceShortNameChar = (char*)(sourceShortName.c_str());
	int targetFileSize = targetFileEntry.getFileSize();
	FileEntry* sourceFileEntry = new FileEntry(targetInode,sourceShortNameChar,targetFileSize);
	sourcePWD->push_back(*sourceFileEntry);
	int numOfHlinks = _fileSys->getNumOfHardLinks(targetInode);
	_fileSys->setNumOfHardLinks(targetInode,numOfHlinks+1);
	_fileSys->d_write(pwdInode,*sourcePWD);
	return 1;
}

int SystemCalls::MakeDir(char* dir_name){
	string dir_nameString(dir_name);
	string pwd = dir_nameString.substr(0,dir_nameString.find_last_of("/"));
	int pwdInode = -1;
	list<FileEntry> *currPWD = readPWDDir(pwd,&pwdInode);
	string new_dir_name = dir_nameString.substr(dir_nameString.find_last_of("/") + 1);
	int newDir_iNode = _fileSys->createDir();
	if (newDir_iNode == -1)
	{
		cerr<<"Could not create dir"<<endl;
		return newDir_iNode;
	}
	FileEntry *newDirEntry = new FileEntry(newDir_iNode,(char*)new_dir_name.c_str(),0);
	currPWD->push_back(*newDirEntry);
	_fileSys->d_write(pwdInode,*currPWD);
	_fileSys->setNumOfHardLinks(newDir_iNode,1);
	return newDir_iNode;
}

list<FileEntry>* SystemCalls::readPWDDir(string pwd,int *lastInode)
{
	int pwdInode = ROOT_PWD_INODE;
	list<FileEntry> *currPWD;
	bool done = false;
	while (!done)
	{
		string currDir = pwd.substr(0, pwd.find("/"));
		//cout<<"readPWDDir currDir = "<<currDir<<" pwdInode = "<<pwdInode<<endl;
		currPWD = _fileSys->d_read(pwdInode);
		//cout<<"readPWDDir finished d_read pwdInode = "<<pwdInode<<endl;
		pwdInode = -1;
		list<FileEntry>::iterator it = currPWD->begin();
		while (it != currPWD->end() & pwdInode == -1)
		{
			FileEntry curr = *it;
			string currEntryName(curr.getFileName());
			//cout<<"readPWDDir currEntryName = "<<currEntryName<<endl;
			//cout<<"readPWDDir curr.getFileName() = "<<curr.getFileName()<<endl;
			if (currDir.compare(currEntryName) == 0)
			{
				if (isDirFromInode(curr.getInodeNum()))
				{
					pwdInode = curr.getInodeNum();
				}
			}
			++it;
		}
		pwd = pwd.substr(pwd.find("/") + 1);
		done = pwd.find("/") == string::npos;
	}
	if (pwdInode == -1)
	{
		pwdInode = 0;
	}
	currPWD = _fileSys->d_read(pwdInode);
	*lastInode = pwdInode;
	return currPWD;
}

int SystemCalls::RmDir(char* dir_name){
	string dir_nameString(dir_name);
//	dir_nameString = "/" + dir_nameString;
	string pwd = dir_nameString.substr(0,dir_nameString.find_last_of("/"));
	if (pwd.compare(dir_nameString) == 0)
	{
		pwd = "";
	}
	string dir_nameShort = dir_nameString.substr(dir_nameString.find_last_of("/")+1);
	int pwdInode;
	list<FileEntry>* currPWD;
	currPWD = readPWDDir(pwd,&pwdInode);
	list<FileEntry>::iterator it;
	it = getFileEntryFromDir(*currPWD,dir_nameShort.c_str());
	if (it == currPWD->end())
	{
		cerr<<"Directory not found"<<endl;
		return -1;
	}
	FileEntry currEntry = (*it);
	int currInode = currEntry.getInodeNum();
	int currNumOfHardLinks = _fileSys->getNumOfHardLinks(currInode);
	_fileSys->setNumOfHardLinks(currEntry.getInodeNum(),currNumOfHardLinks - 1);
	if (currNumOfHardLinks == 1)
	{
		int ret = _fileSys->d_delete(currInode);
		if (ret == -1)
		{
			return -1;
		}
	}
	currPWD->erase(it);
	_fileSys->d_write(pwdInode,*currPWD);
}

int SystemCalls::RmDir_R(string dir_name)
{
	int pwdInode;
	list<FileEntry>* currPWD;
	string tempDirName = dir_name + "/";
	currPWD = readPWDDir(tempDirName,&pwdInode);
	while (!currPWD->empty())
	{
		list<FileEntry>::iterator it;
		it = currPWD->begin();
		FileEntry currEntry = *it;
		string newFileName = dir_name + "/";
		newFileName = newFileName + currEntry.getFileName();
		int fileType = _fileSys->getFileType(currEntry.getInodeNum());
		int ret = 0;
		if (fileType == DIR_TYPE)
		{
			ret = RmDir_R(newFileName);
		}
		if (fileType < DIR_TYPE)
		{
			ret = RmFile((char*)(newFileName.c_str()));
		}
		currPWD = readPWDDir(dir_name,&pwdInode);
	}
	RmDir((char*)(dir_name.c_str()));


}

list<FileEntry>::iterator SystemCalls::getFileEntryFromDir(list<FileEntry>& currPWD,const char* file_name)
{
	list<FileEntry>::iterator it = currPWD.begin();
	bool found = false;
	while (it != currPWD.end() & !found)
	{
		FileEntry curr = *it;
		char* currFileName = curr.getFileName();
//		cout<<"SystemCalls::getFileEntryFromDir file_name = "<<file_name<<" currFileName = "<<currFileName<<endl;
		if (strcmp(file_name,currFileName) == 0)
		{
			found = true;
			return it;
		}
		++it;
	}
	return it;
}

int SystemCalls::RmFile(char* fileName){
	string file_nameString(fileName);
	string pwd = file_nameString.substr(0,file_nameString.find_last_of("/"));
	string file_nameShort = file_nameString.substr(file_nameString.find_last_of("/")+1);
	int pwdInode = -1;
	list<FileEntry>* currPWD = readPWDDir(pwd,&pwdInode);
	list<FileEntry>::iterator currEntryIterator;
	currEntryIterator = getFileEntryFromDir(*currPWD,file_nameShort.c_str());
	if (currEntryIterator == currPWD->end())
	{
		cerr<<"File not found or is a directory"<<endl;
		return -1;
	}
	FileEntry currEntry = (*currEntryIterator);
	if (_fileSys->getFileType(currEntry.getInodeNum()) >= DIR_TYPE)
	{
		cerr<<"Name is a directory"<<endl;
		return -1;
	}
	int numOfHardLinks = _fileSys->getNumOfHardLinks(currEntry.getInodeNum());
	if (numOfHardLinks == 1)
	{
		if (isFileOpen(currEntry.getInodeNum()))
		{
			cerr<<"Could not delete file because it is being used by some program"<<endl;
			return -1;
		}
		_fileSys->f_delete(currEntry.getInodeNum());
	}
	numOfHardLinks--;
	_fileSys->setNumOfHardLinks(currEntry.getInodeNum(),numOfHardLinks);
	currPWD->erase(currEntryIterator);
	_fileSys->d_write(pwdInode,*currPWD);
	return 1;
}

int SystemCalls::ls(char *dir_name, char * buf){
	string dir_nameString(dir_name);
	string pwd = dir_nameString.substr(0,dir_nameString.find_last_of("/"));
	list<FileEntry>* currPWD;
	int pwdInode;
//	cout<<"SystemCalls:ls trying readPWDDir on pwd = "<<pwd<<endl;
	currPWD = readPWDDir(pwd,&pwdInode);
	cout<<"SystemCalls:ls finished readPWDDir"<<endl;
	ostringstream answerString;
	list<FileEntry>::iterator it = currPWD->begin();
	while (it != currPWD->end())
	{
		FileEntry currEntry = *it;
		answerString<<currEntry.getFileName();
		ostringstream tempFileName;
		tempFileName<<dir_name<<currEntry.getFileName();
		if (isDir((char*)(tempFileName.str().c_str())))
		{
			answerString<<"/";
		}
//		if (this->isDirFromInode(currEntry.getInodeNum()))
//		{
//			answerString<<"/";
//		}
//		else
//		if (_fileSys->getFileType(currEntry.getInodeNum()) == SOFT_LINK)
//		{
//
//		}
		answerString<<"\t"<<currEntry.getFileSize() <<endl;
		++it;
	}
	strcpy(buf,answerString.str().c_str());
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
//	cout<<"SC::Open starting readPWDDIR on pwd = "<<pwd<<endl;
	list<FileEntry>* currPWD = readPWDDir(pwd,&pwdInode);
	list<FileEntry>::iterator it = getFileEntryFromDir(*currPWD,file_nameShort.c_str());
	int ret = -1;
	if (it != currPWD->end())
	{
		FileEntry currEntry = *it;
		int file_inode = currEntry.getInodeNum();
		Descriptor* newDesc = new Descriptor(flag_access_permissions,file_inode);
		pthread_mutex_lock(&_currFDMutex);
		ret = _currFD.getUnusedFD();
		_openFileTable[ret] = newDesc;
//		ret = _currFD;
		if (_fileSys->getFileType(file_inode) == SOFT_LINK)
		{
			cout<<"SC::Open found SOFT_LINK file_inode = "<<file_inode<<endl;
			ostringstream fileNamePointedTo;
			char bufOfOne[2];
			Read(ret,1,bufOfOne);
			while (bufOfOne[0] != '$')
			{
//				cout<<"SC::Open reading soft link buff= "<<bufOfOne[0]<<endl;
				fileNamePointedTo<<bufOfOne[0];
				Read(ret,1,bufOfOne);
			}
			char* file_name_of_soft_link = (char*)(fileNamePointedTo.str().c_str());
			pthread_mutex_unlock(&_currFDMutex);
			return Open(file_name_of_soft_link,flag_access_permissions);
		}
		pthread_mutex_unlock(&_currFDMutex);
		return ret;
	}
	else
	{
		cerr<<"filename not found"<<endl;
	}
	pthread_mutex_unlock(&_currFDMutex);
	return ret;
}

int SystemCalls::Close(int fd){
	pthread_mutex_lock(&_currFDMutex);
	_openFileTable.erase(fd);
	_currFD.freeFD(fd);
	pthread_mutex_unlock(&_currFDMutex);
	return 1;
}

int SystemCalls::Seek(int fd, int location){
	pthread_mutex_lock(&_currFDMutex);
	map<int,Descriptor*>::iterator it = _openFileTable.find(fd);
	if (it == _openFileTable.end())
	{
		cerr<<"The file does not exist or is not open"<<endl;
		return -1;
	}
	Descriptor* desc = it->second;
	desc->_filePosition = location;
	pthread_mutex_unlock(&_currFDMutex);
	return 1;
}

int SystemCalls::Read (int fd, int nBytes, char *Buffer){
	map<int,Descriptor*>::iterator it = _openFileTable.find(fd);
	if (it == _openFileTable.end())
	{
		cerr<<"The file does not exist or is not open"<<endl;
		return -1;
	}
	Descriptor* desc = it->second;
	if (isLockedWrite(desc->_inode))
	{
		cerr<<"The file is locked for write"<<endl;
		return -1;
	}
	int bytesRead = _fileSys->f_read(desc->_inode,Buffer,desc->_filePosition,nBytes);
	desc->_filePosition += bytesRead;
	return bytesRead;
}


int SystemCalls::Write (int fd, int nBytes,char * Buffer){
	map<int,Descriptor*>::iterator it = _openFileTable.find(fd);
	if (it == _openFileTable.end())
	{
		cerr<<"The file does not exist or is not open"<<endl;
		return -1;
	}
	Descriptor* desc = it->second;
	if (isLockedRead(desc->_inode) | isLockedWrite(desc->_inode))
	{
		cerr<<"The file is locked. Please try again later"<<endl;
		return -1;
	}
	if (desc->_flag <= READ_ONLY)
	{
		cerr<<"Cannot write the file is read-only"<<endl;
		return -1;
	}
	int bytesWritten = _fileSys->f_write(desc->_inode,Buffer,desc->_filePosition,nBytes);
	desc->_filePosition += bytesWritten;
	return bytesWritten;
}

int SystemCalls::moveFile(char* parendDir, char * new_dest){

	return 1;
}

bool SystemCalls::isDir(char * fileName){
//	cout<<"SC::isDir fileName = "<<fileName<<endl;
	return myIsDir(fileName) != NULL;
//	string file_nameString(fileName);
//	string pwd = file_nameString.substr(0,file_nameString.find_last_of("/"));
//	string file_nameShort = file_nameString.substr(file_nameString.find_last_of("/")+1);
//	cout<<"SC::isDir finished cutting up string"<<endl;
//	int pwdInode = -1;
//	cout<<"SC::isDir readPWD(pwd...) pwd = "<<pwd<<endl;
//	list<FileEntry>* currPWD = readPWDDir(pwd,&pwdInode);
//	cout<<"SC::isDir pwdInode = "<<pwdInode<<endl;
//	list<FileEntry>::iterator currEntryIterator;
//	currEntryIterator = getFileEntryFromDir(*currPWD,file_nameShort.c_str());
//	bool temp = currEntryIterator == currPWD->end() ;
//	cout<<"SC::isDir currEntryIterator == currPWD->end()"<< temp <<endl;
//	FileEntry currEntry = *currEntryIterator;
//	cout<<"SC::isDir currFileName = "<<currEntry.getFileName()<<endl;
//	cout<<"SC::isDir _fileSys->getFileType(currEntry.getInodeNum())"<<_fileSys->getFileType(currEntry.getInodeNum())<<endl;
//	return _fileSys->getFileType(currEntry.getInodeNum()) == DIR_TYPE;
}

string* SystemCalls::myIsDir(char* fileName)
{
	string file_nameString(fileName);
	string pwd = file_nameString.substr(0,file_nameString.find_last_of("/"));
	string file_nameShort = file_nameString.substr(file_nameString.find_last_of("/")+1);
	int pwdInode = -1;
	string* success = readPWDDirForCD(file_nameString, &pwdInode);
	//list<FileEntry>::iterator fileEntryIt =getFileEntryFromDir(*currPWD,file_nameShort.c_str());
	return success;
}

//bool SystemCalls::readPWDDirForCD(string pwd,int *lastInode)
string* SystemCalls::readPWDDirForCD(string pwd,int *lastInode)
{
	int pwdInode = ROOT_PWD_INODE;
	list<FileEntry> *currPWD;
	bool done = false;
	bool foundFile = false;
	while (!done)
	{
		foundFile = false;
		string currDir = pwd.substr(0, pwd.find("/"));
		currPWD = _fileSys->d_read(pwdInode);
		if (currPWD->empty())
		{
			return NULL;
		}
		pwdInode = -1;
		list<FileEntry>::iterator it = currPWD->begin();
		while (it != currPWD->end() & pwdInode == -1)
		{
			FileEntry curr = *it;
			string currEntryName(curr.getFileName());
			if (currDir.compare(currEntryName) == 0)
			{
				foundFile = true;
				if (isDirFromInode(curr.getInodeNum()))
				{
					pwdInode = curr.getInodeNum();
				}
				else if (_fileSys->getFileType(curr.getInodeNum()) == SOFT_LINK)
				{
					ostringstream fileNamePointedTo;
					char bufOfOne[2];
					int offsetToReadSoftLink = 0;
					_fileSys->f_read(curr.getInodeNum(),bufOfOne,offsetToReadSoftLink,1);
					while (bufOfOne[0] != '$')
					{
//						cout<<"SC::Open reading soft link buff= "<<bufOfOne[0]<<endl;
						offsetToReadSoftLink++;
						fileNamePointedTo<<bufOfOne[0];
						_fileSys->f_read(curr.getInodeNum(),bufOfOne,offsetToReadSoftLink,1);
					}
					char* file_name_of_soft_link = (char*)(fileNamePointedTo.str().c_str());

					return myIsDir(file_name_of_soft_link);
				}
				else
				{
					return NULL;
				}
			}
			currDir.compare("..") == 0;
			if (currDir.compare("..") == 0)
			{
				foundFile = true;
			}
			++it;
		}
		done = pwd.find("/") == string::npos;
		pwd = pwd.substr(pwd.find("/") + 1);
	}
	if (!foundFile)
	{
		return NULL;
	}
	if (pwdInode == -1)
	{
		pwdInode = 0;
	}
//	currPWD = _fileSys->d_read(pwdInode);
	*lastInode = pwdInode;
	return new string(pwd);
}

bool SystemCalls::isDirFromInode(int i_node)
{
	if (_fileSys->getFileType(i_node) == DIR_TYPE)
		return true;
	return false;
}

int SystemCalls::lockRead(int fd){
	pthread_mutex_lock(&lock_mutex);
	map<int,Descriptor*>::iterator it = _openFileTable.find(fd);
	if (it == _openFileTable.end())
	{
		cerr<<"The file does not exist or is not open"<<endl;
		pthread_mutex_unlock(&lock_mutex);
		return -1;
	}
	Descriptor* desc = (*it).second;
	int fd_inode = desc->_inode;
	if (isLockedRead(fd_inode) | isLockedWrite(fd_inode))
	{
		cerr<<"File is already locked"<<endl;
		pthread_mutex_unlock(&lock_mutex);
		return -1;
	}
	int lockingUI = _currUI;
	_readLocks[fd_inode] = lockingUI;
	//_readLocks.push_back(fd_inode,lockingUI);
	pthread_mutex_unlock(&lock_mutex);
	return fd_inode;
}

bool SystemCalls::isLockedRead(int i_node_num)
{
	map<int,int>::iterator place = _readLocks.find(i_node_num);
	bool ans = false;
	if (place != _readLocks.end())
	{
		ans = true;
//		int lockingUI = (*place).second;
//		ans = lockingUI != _currUI;
	}

	return ans;
//
//	for (int i = 0; i < _readLocks.size(); i++)
//	{
//		if (_readLocks[i] == i_node_num)
//		{
//			return true;
//		}
//	}
//	return false;
}

bool SystemCalls::isLockedWrite(int i_node_num)
{
	map<int,int>::iterator place = _writeLocks.find(i_node_num);
	bool ans = false;
	if (place != _writeLocks.end())
	{
		int lockingUI = (*place).second;
		ans = lockingUI != _currUI;
	}

	return ans;
//	for (int i = 0; i < _writeLocks.size(); i++)
//	{
//		if (_writeLocks[i] == i_node_num)
//		{
//			return true;
//		}
//	}
//
//	return false;
}

int SystemCalls::lockRead(int fd,int pid)
{
	pthread_mutex_lock(&lock_mutex);
	lockingFd = pid;
	lockRead(fd);
	pthread_mutex_unlock(&lock_mutex);
}

int SystemCalls::lockWrite(int fd){
	pthread_mutex_lock(&lock_mutex);
	map<int,Descriptor*>::iterator it = _openFileTable.find(fd);
	if (it == _openFileTable.end())
	{
		cerr<<"The file does not exist or is not open"<<endl;
		pthread_mutex_unlock(&lock_mutex);
		return -1;
	}
	Descriptor* desc = (*it).second;
	int fd_inode = desc->_inode;
	if (isLockedRead(fd_inode) | isLockedWrite(fd_inode))
	{
		cerr<<"File is already locked"<<endl;
		pthread_mutex_unlock(&lock_mutex);
		return -1;
	}
	int lockingUI = _currUI;
	_writeLocks[fd_inode] = lockingUI;
	pthread_mutex_unlock(&lock_mutex);
	return fd_inode;
}

int SystemCalls::releaseLockRead(int fd){
	int lockedInode = turnFDtoInodeNum(fd);
	pthread_mutex_lock(&lock_mutex);
	map<int,int>::iterator place = _readLocks.find(lockedInode);
	if (place != _readLocks.end())
	{
		int lockerPID = (*place).second;
		if (lockerPID == _currUI)
		{
			_readLocks.erase(place);
			pthread_mutex_unlock(&lock_mutex);
			return 1;
		}
		else
		{
			cerr<<"this lock is not yours to release"<<endl;
		}
	}
	cerr<<"File is not locked"<<endl;
	pthread_mutex_unlock(&lock_mutex);
	return 1;
}

int SystemCalls::releaseLockWrite(int fd){
	int lockedInode = turnFDtoInodeNum(fd);
	pthread_mutex_lock(&lock_mutex);
	map<int,int>::iterator place = _writeLocks.find(lockedInode);
	if (place != _writeLocks.end())
	{
		int lockerPID = (*place).second;
		if (lockerPID == _currUI)
		{
			_writeLocks.erase(place);
			pthread_mutex_unlock(&lock_mutex);
			return 1;
		}
		else
		{
			cerr<<"this lock is not yours to release"<<endl;
		}
	}
	cerr<<"File is not locked"<<endl;
	pthread_mutex_unlock(&lock_mutex);
	return 1;
}

bool SystemCalls::isFileOpen(int i_node)
{
	map<int,Descriptor*>::iterator it;
	for ( it=_openFileTable.begin() ; it != _openFileTable.end(); it++ )
	{
		Descriptor* currDesc = it->second;
		if (currDesc->_inode == i_node)
		{
			return true;
		}
	}
	return false;

}

int SystemCalls::turnFDtoInodeNum(int fd)
{
	map<int,Descriptor*>::iterator it;
	it = _openFileTable.find(fd);
	if (it != _openFileTable.end())
	{
		Descriptor* desc = (*it).second;
		return desc->_inode;
	}
	return -1;
}
