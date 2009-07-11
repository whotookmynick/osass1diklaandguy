/*
 * FreeInodeList.h
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#ifndef FREEINODELIST_H_
#define FREEINODELIST_H_
#include "../BlockList.h"
#include  "../../LowLevelDisk.h"

class FreeInodeList : virtual BlockList {
public:
	FreeInodeList(int fd,int numOfBlocks,int firstBlockAdress,int lastBlockAdress,int blockSize,LowLevelDisk& disk);


//To freeLists to implement
	virtual void emptyBlock(int blockNum);
	virtual int  insertBlock();
};
#endif /* FREEINODELIST_H_ */
