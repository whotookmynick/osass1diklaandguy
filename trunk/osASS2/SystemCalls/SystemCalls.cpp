/*
 * SystemCalls.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "SystemCalls.h"
	SystemCalls::SystemCalls(){}
	SystemCalls::~SystemCalls(){}

	int SystemCallsMakeFile(char* file_name,int type,int flag_access_permissions){
		return 1;
	}


	int SystemCallsMakeHLink(char* target_file_name, char*file_name){
		return 1;
	}


	int MakeDir(char* dir_name){
		return 1;
	}

	int RmDir(char* dir_name){
		return 1;
	}

	int RmFile(char* file_name){
		return 1;
	}

	int ls(char*dir_name, char * buf){
		return 1;
	}


	int Open(char* filename, int flag_access_permissions){
		return 1;
	}

	int Close(int fd){
		return 1;
	}

	int Seek(int fd, int location){
		return 1;
	}

	int Read (int fd, int nBytes, char *Buffer){
		return 1;
	}


	int Write (int fd, int nBytes,char * Buffer){
		return 1;
	}

	int moveFile(char* parendDir, char * new_dest){
		return 1;
	}

	bool isDir(char * address){
		return false;
	}

	int lockRead(int fd){
		return 1;
	}

	int lockWrite(int fd){
		return 1;
	}

	int releaseLockRead(int fd){
		return 1;
	}

	int releaseLockWrite(int fd){
		return 1;
	}
