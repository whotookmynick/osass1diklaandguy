/*
 * BlockList.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dikla
 */

#include "BlockList.h"
#include "LowLevelDisk.h"
//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

BlockList::BlockList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk):
_fd(fd),_firstBlockPointer(firstEmptyBlock),_lastBlockPointer(lastEmptyBlock),_offset(firstBlockOffset),_disk(disk)
{


}

BlockList::~BlockList() {
	// TODO Auto-generated destructor stub
}
//---------------------------------------------------------------------------------
//								Help functions
//--------------------------------- -----------------------------------------------
int BlockList::writeDataToHardDisk(int fromOffset,const void* buf,int numOfBytes){
	off_t offset = fromOffset;
	size_t size = numOfBytes;
	size_t sizeWriten = pwrite(_fd,buf,size,offset);
	int sizeW = sizeWriten;
	return sizeW;
}

void BlockList::updateTail(){
	int newBlock;
	_disk.getSuperBlock();
	if((_tail%((_disk.getSuperBlock())->blockSize)) == 0){
		newBlock = _disk.allocateDataBlock();
		writeDataToHardDisk(_tail,(void*)&newBlock,OFFSET_SIZE_IN_BYTES);
		_tail=newBlock*_disk.getSuperBlock()->blockSize;
	}else{
		_tail=_tail+OFFSET_SIZE_IN_BYTES;
	}

}

void BlockList::updateHead(){
	/*int newBlock;
	_disk.getSuperBlock();
	if((_tail%((_disk.getSuperBlock())->blockSize)) == 0){
		newBlock = _disk.allocateDataBlock();
		writeDataToHardDisk(_tail,(void*)&newBlock,OFFSET_SIZE_IN_BYTES);
		_tail=newBlock*_disk.getSuperBlock()->blockSize;
	}else{
		_tail=_tail+OFFSET_SIZE_IN_BYTES;
	}*/

}
//---------------------------------------------------------------------------------
//
//--------------------------------- -----------------------------------------------

int BlockList::size(){
	return 1;
}

bool BlockList::empty(){
	return size()==0;
}

bool BlockList::contains(int v)  {
	return true;
}

int BlockList::head(){
	return 1;
}//reference to the  front

void BlockList::pop_front(){


}


void BlockList::push_back(int val){}
