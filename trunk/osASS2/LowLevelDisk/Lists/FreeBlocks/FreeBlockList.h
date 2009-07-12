/*
 * FreeBlockList.h
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#ifndef FREEBLOCKLIST_H_
#define FREEBLOCKLIST_H_
#include "../BlockList.h"
class LowLevelDisk;

class FreeBlockList : virtual BlockList {
public:
	FreeBlockList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk);


//To freeLists to implement
	virtual void emptyBlock(int blockNum);
	virtual int  insertBlock();
};

#endif /* FREEBLOCKLIST_H_ */
