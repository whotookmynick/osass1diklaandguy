/*
 * FreeInodeList.cpp
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#include "FreeInodeList.h"

  FreeInodeList::FreeInodeList(int fd,int numOfBlocks,int firstBlockAdress,int lastBlockAdress,int blockSize,LowLevelDisk& disk)
    : BlockList(fd,numOfBlocks,firstBlockAdress,lastBlockAdress,blockSize,disk) {};




//To freeLists to implement
	void FreeInodeList::FreeInodeList::emptyBlock(int blockNum){}
	int  FreeInodeList::insertBlock(){
		return 1;
	 }

