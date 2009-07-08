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


class BlockList{
public:
	 /**
	 * @param numOfBlocks
	 * @param firstBlockAdress -offset in file were the first block in the list is
	 * @param blockSize - the block size in bytes
	 *
	 */

	BlockList(int numOfBlocks,int firstBlockAdress,int blockSize);
	virtual ~BlockList();


	int size();
	bool empty();

	bool contains(int& value);


	int& Head();//reference to the  front
	void pop_front();
	void push_back(int& val);

private:
	//int _numberOfBlocksInTheList;
	//list<int> _BlocksNumbersList;
	//int _offset;
	//LowLevelDisk _nextBlockAdress; //the next block adrees in the list

};

#endif /* BlockList_H_ */
