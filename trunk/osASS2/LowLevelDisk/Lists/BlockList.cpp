/*
 * BlockList.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dikla
 */

#include "BlockList.h"

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

BlockList::BlockList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk) :
_fd(fd),_firstBlockPointer(firstEmptyBlock),_lastBlockPointer(lastEmptyBlock),_offset(firstBlockOffset),_disk(disk)
{


}

BlockList::~BlockList() {
	// TODO Auto-generated destructor stub
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

void BlockList::push_back(int& val){}
