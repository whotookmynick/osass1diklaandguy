/*
 * InodeList.h
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */

#ifndef INODELIST_H_
#define INODELIST_H_

#include "../../LogMessages/Logger.h"


#include <sstream>
#include <math.h>
#include <sys/mman.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include <stdlib.h>
#include <unistd.h>

//#include "LowLevelDisk.h"
class LowLevelDisk;

const int OFFSET_SIZE_IN_BYTES = 4;
const int NUM_OF_DIRECT_BLOCKS = 10;
const int Num_DirectBlocks=10;

typedef struct {
	int  directBlock[Num_DirectBlocks];
	int  indirectBlockAdress;
	int  type;
	int  numOfHardLinks;
	int  numOfFullBlocks;
	bool active;

} InodeStruct;



//class LowLevelDisk;
using namespace std;

class InodeList{

public:

	InodeList(int fd,int offset, LowLevelDisk& disk);
	virtual ~InodeList();

	//---------------------------------------------------//
	void emptyBlock(int i_node);
	InodeStruct* readInodeToStruct(int i_node);
	int getNumOfTotalBlocks();
	void writeInodeToHardDisk(InodeStruct* is,int offset);
	int getDataBlockNum(int i_node,int dblock);
	void setNumOfDataBlock(int i_node, int i, int dblockNum );
	void* readDataFromHardDisk(int fromOffset,void* buf,int numOfBytes);//read data from hard disk
	int writeDataToHardDisk(int fromOffset,const void* buf,int numOfBytes);

	//---------------------------------------------------//

	int getInodeOffset(int i_node);
	int getBlockOffsetInInode(int blockIndex,int i_node);

	//---------------------------------------------------//
	void setFileType(int i_node,int _fileType);
	int getFileType(int i_node);

	int getFileSize(int i_node);
	void setFileSize(int i_node,int _fileSize);

	void setActive(int inode,bool a);
	bool getActive(int i_node);
	//---------------------------------------------------//


private:
	int _offset;
	int _inodeSizeInBytes;
	LowLevelDisk& _disk;
	int _fd;
	int _numOfTotalBlocks ;


};

#endif /* INODELIST_H_ */

