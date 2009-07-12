/*
 * FreeInodeList.cpp
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#include "FreeInodeList.h"

  FreeInodeList::FreeInodeList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk)
    : BlockList(fd,firstBlockOffset,firstEmptyBlock,lastEmptyBlock,disk) {};




//To freeLists to implement
	void FreeInodeList::FreeInodeList::emptyBlock(int blockNum){}
	int  FreeInodeList::insertBlock(){
		return 1;
	 }

