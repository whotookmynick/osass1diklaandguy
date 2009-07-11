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

SystemCalls::SystemCalls(int dataBlockSize,int numberOfInodes,int diskSize){}

SystemCalls::~SystemCalls(){}



//---------------------------------------------------------------------------/
//								Help function
//---------------------------------------------------------------------------/


//---------------------------------------------------------------------------/
//								Low Level functions
//---------------------------------------------------------------------------/
int SystemCalls::MakeFile(char* file_name,int type,int flag_access_permissions){
	return 1;
}


int SystemCalls::MakeHLink(char* target_file_name, char*file_name){
	return 1;
}


int SystemCalls::MakeDir(char* dir_name){
	return 1;
}

int SystemCalls::RmDir(char* dir_name){
	return 1;
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
