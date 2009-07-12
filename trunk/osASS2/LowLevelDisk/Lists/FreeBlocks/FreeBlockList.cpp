/*
 * FreeBlockList.cpp
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#include "FreeBlockList.h"


FreeBlockList::FreeBlockList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk)
: BlockList(fd,firstBlockOffset,firstEmptyBlock,lastEmptyBlock,disk) {};





//To freeLists to implement
	void FreeBlockList::emptyBlock(int blockNum){}
	int  FreeBlockList::insertBlock(){
		return 1;
	 }
