/*
 * BlockList.h
 *
 *  Created on: Jul 8, 2009
 *      Author: dikla
 */

#ifndef BlockList_H_
#define BlockList_H_
#include "vector"
#include <iostream>
#include <list>
using namespace std;

class LowLevelDisk;

class BlockList{
public:
	 /**
	 * @param numOfBlocks
	 * @param firstBlockAdress -offset in file were the first block in the list is
	 * @param blockSize - the block size in bytes
	 *
	 */

	BlockList(int fd,int firstBlockOffset,int firstEmptyBlock,int lastEmptyBlock,LowLevelDisk& disk);

	virtual ~BlockList();

	virtual int size();
	bool empty();
	virtual bool contains(int v)  ;


	virtual int head();//reference to the  front
	virtual void pop_front();
	virtual void push_back(int& val);

//To freeLists to implement
	virtual void emptyBlock(int blockNum)=0;
	virtual int  insertBlock()=0;


private:
	int _fd;
	int _firstBlockPointer;
	int _lastBlockPointer;
	int _offset;
	LowLevelDisk& _disk;
};


#endif /* BlockList_H_ */

