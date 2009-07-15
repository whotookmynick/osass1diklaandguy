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
void* BlockList::readDataFromHardDisk(int fromOffset,void* buf,int numOfBytes){
	off_t offset = fromOffset;
	size_t size = numOfBytes;
	pread(_fd,buf,size,offset);
	return buf;
}
void BlockList::updateTail(){
	int newBlock;
	if((_tail%((_disk.getSuperBlock())->blockSize)) == 0){
		newBlock = _disk.allocateDataBlock();
		writeDataToHardDisk(_tail,(void*)&newBlock,OFFSET_SIZE_IN_BYTES);
		_tail=newBlock*_disk.getSuperBlock()->blockSize;
	}else{
		_tail=_tail+OFFSET_SIZE_IN_BYTES;
	}

}

void BlockList::updateHead(){
	if((_head%((_disk.getSuperBlock())->blockSize)) == 0){
		copyNextBlockToSuperBlock();
	}else{
		_head=_head+OFFSET_SIZE_IN_BYTES;
	}

}

void BlockList::copyNextBlockToSuperBlock(){
	int nextBlock;
	int blocksize=_disk.getSuperBlock()->blockSize;
	readDataFromHardDisk(_head,(void*)&nextBlock,OFFSET_SIZE_IN_BYTES);
	int content ;
	int copyFromOffset=nextBlock*blocksize;
	_head = _head-blocksize+1;


	int writeToOffset=_head-(blocksize)+1;
	for(int i=0;i<blocksize;i=i+OFFSET_SIZE_IN_BYTES){
		readDataFromHardDisk(copyFromOffset,(void*)&content,OFFSET_SIZE_IN_BYTES);
		writeDataToHardDisk(writeToOffset,(void*)&content,OFFSET_SIZE_IN_BYTES);
		copyFromOffset++;
		writeToOffset++;
	}
	//if the tail is in the second block
	if(_tail<nextBlock*blocksize+blocksize-1){
		_tail=_tail%blocksize+_head;
	}
}
//---------------------------------------------------------------------------------
//
//--------------------------------- -----------------------------------------------

int BlockList::size(){
	return _size;
}

bool BlockList::empty(){
	return size()==0;
}

bool BlockList::contains(int v)  {
	return true;
}

int BlockList::head(){
	int element;
	readDataFromHardDisk(_head,(void*)&element,OFFSET_SIZE_IN_BYTES);
	return element;
}//reference to the  front

void BlockList::pop_front(){
	int null=0;
	writeDataToHardDisk(_head,(void*)&null,OFFSET_SIZE_IN_BYTES);
	updateHead();
	_size--;
}


void BlockList::push_back(int val){
	readDataFromHardDisk(_tail,(void*)&val,OFFSET_SIZE_IN_BYTES);
	updateTail();
	_size++;
}
