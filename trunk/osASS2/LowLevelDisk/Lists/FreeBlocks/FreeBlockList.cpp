/*
 * FreeBlockList.cpp
 *
 *  Created on: Jul 11, 2009
 *      Author: dikla
 */

#include "FreeBlockList.h"
#include "LowLevelDisk.h"

FreeBlockList::FreeBlockList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk)
: BlockList(fd,firstBlockOffset,firstEmptyBlock,lastEmptyBlock,disk) {

	superBlock *super = _disk.getSuperBlock();
	_size=super->numOfFreeBlocks;
	_head=(super->firstEmptyBlock);// * super->blockSize;
	_tail= super->lastEmptyBlock;

};





//To freeLists to implement
	void FreeBlockList::emptyBlock(int blockNum){}
	int  FreeBlockList::insertBlock(){
		return 1;
	 }

	void FreeBlockList::updateHead(){
	//	if((_head%((_disk.getSuperBlock())->blockSize)) == 0){
		if(((_head%((_disk.getSuperBlock())->blockSize))-1) == 0){
			copyNextBlockToSuperBlock();
		}else{
			_head=_head+OFFSET_SIZE_IN_BYTES;

		}
		writeDataToHardDisk(FIRST_EMPTY_BLOCK_POINTER_OFFSET,&_head,OFFSET_SIZE_IN_BYTES);

	}

	void FreeBlockList::updateTail(){
		int newBlock;
		if(((_tail%((_disk.getSuperBlock())->blockSize))-1) == 0){
			newBlock = _disk.allocateDataBlock();
			writeDataToHardDisk(_tail,(void*)&newBlock,OFFSET_SIZE_IN_BYTES);
			_tail=newBlock*_disk.getSuperBlock()->blockSize;
		}else{
			_tail=_tail+OFFSET_SIZE_IN_BYTES;

		}
		writeDataToHardDisk(LAST_EMPTY_BLOCK_POINTER_OFFSET,&_tail,OFFSET_SIZE_IN_BYTES);

	}
