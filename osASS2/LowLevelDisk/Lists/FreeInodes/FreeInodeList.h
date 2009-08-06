/*
 * FreeInodeList.h
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#ifndef FREEINODELIST_H_
#define FREEINODELIST_H_
#include "../BlockList.h"
//#include  "../../LowLevelDisk.h"

class LowLevelDisk;

class FreeInodeList : virtual BlockList {
public:
	FreeInodeList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk);


//To freeLists to implement
	virtual void emptyBlock(int blockNum);
	virtual int  insertBlock();
	virtual void updateTail();
	virtual void updateHead();

};
#endif /* FREEINODELIST_H_ */
