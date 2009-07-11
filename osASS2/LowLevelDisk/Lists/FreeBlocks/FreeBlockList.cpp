/*
 * FreeBlockList.cpp
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#include "FreeBlockList.h"


FreeBlockList::FreeBlockList(int fd,int numOfBlocks,int firstBlockAdress,int lastBlockAdress,int blockSize,LowLevelDisk& disk)
    : BlockList(fd,numOfBlocks,firstBlockAdress,lastBlockAdress,blockSize,disk) {};




//To freeLists to implement
	void FreeBlockList::emptyBlock(int blockNum){}
	int  FreeBlockList::insertBlock(){
		return 1;
	 }
