
/*
 * InodeList.cpp
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */

#include "InodeList.h"
#include "LowLevelDisk.h"
//#include "iNode.h"



InodeList::InodeList(int fd,int offset, LowLevelDisk& disk):_offset(offset),
_inodeSizeInBytes(sizeof(InodeStruct)),_disk(disk),
_fd(fd),_numOfTotalBlocks((disk.getSuperBlock()->blockSize)+NUM_OF_DIRECT_BLOCKS){

	int tableSize=_disk.getNumOfInodes();

}

InodeList::~InodeList(){
	//	delete all;
}//end while

//---------------------------------------------------------------------------/
//							SYS_FILE Action
//---------------------------------------------------------------------------/
InodeStruct* InodeList::readInodeToStruct(int i_node){
	int offset = getInodeOffset(i_node);
	InodeStruct* is = (InodeStruct*)malloc(sizeof(InodeStruct));
	pread(_fd,is,sizeof(InodeStruct),offset);
	return is;
}

//TODO:what if inode is not active?
//TODO:need to clear inode first???
void InodeList::writeInodeToHardDisk(InodeStruct* is,int offset){
	pwrite(_fd,&is,sizeof(is),offset);
}


void* InodeList::readDataFromHardDisk(int fromOffset,void* buf,int numOfBytes){
	off_t offset = fromOffset;
	size_t size = numOfBytes;
	pread(_fd,buf,size,offset);
	return buf;
}

int InodeList::writeDataToHardDisk(int fromOffset,const void* buf,int numOfBytes){
	off_t offset = fromOffset;
	size_t size = numOfBytes;
	size_t sizeWriten = pwrite(_fd,buf,size,offset);
	int sizeW = sizeWriten;
	return sizeW;
}

//---------------------------------------------------------------------------/
//								OFFSETS CALC
//---------------------------------------------------------------------------/
int InodeList::getBlockOffsetInInode(int blockIndex,int i_node){
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
				//TODO
		return -1;
	}
	if((blockIndex<0) | (blockIndex>(_numOfTotalBlocks) )){
		return -1;
	}
	InodeStruct* is=readInodeToStruct(i_node);

	//iNode& i_node = getInode()

	if (blockIndex<(Num_DirectBlocks-1))
		return getInodeOffset(i_node)+OFFSET_SIZE_IN_BYTES*blockIndex;

	else{
		int indirectBlock= is->indirectBlockAdress;
		int blockOffset = indirectBlock*_disk.getDataBlockSize();
		return blockOffset+(blockIndex-NUM_OF_DIRECT_BLOCKS)*OFFSET_SIZE_IN_BYTES;
	}

}

int InodeList::getInodeOffset(int i_node){
	if((i_node > _disk.getNumOfInodes())|(i_node < 0)){
				//TODO
		return -1;
	}
	return _offset+(_inodeSizeInBytes*(i_node-1));
}


//---------------------------------------------------------------------------/
//								Action per i_Node
//---------------------------------------------------------------------------/
void InodeList::emptyBlock(int i_node){
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
				//TODO
	}

}

int InodeList::getDataBlockNum(int i_node,int blockIndex){
	int blockoffset;
	int blockNum ;
	void* buf [1];
	InodeStruct* is=readInodeToStruct(i_node);

	if((i_node>_disk.getNumOfInodes())|(i_node<0)){
			//TODO
			return -1;
	}
	if((blockIndex<0) | (blockIndex>(_numOfTotalBlocks) )){
		return -1;
	}
	if(is->numOfFullBlocks<=blockIndex){
		return -1;
	}

	if (blockIndex<(Num_DirectBlocks-1)){
		blockoffset = getInodeOffset(i_node)+OFFSET_SIZE_IN_BYTES*blockIndex;
		readDataFromHardDisk(blockoffset,buf,OFFSET_SIZE_IN_BYTES);
	}
	else{
		int indirectBlock= is->indirectBlockAdress;
		int inDirectBlockOffset = indirectBlock*_disk.getDataBlockSize();
		blockoffset = inDirectBlockOffset+(blockIndex-NUM_OF_DIRECT_BLOCKS)*OFFSET_SIZE_IN_BYTES;
		readDataFromHardDisk(blockoffset,buf,OFFSET_SIZE_IN_BYTES);
	}
	blockNum = (int)buf[0];
	return blockNum;
}

void InodeList::setNumOfDataBlock(int i_node, int blockIndex, int dblockNum ){
	int blockoffset;
	InodeStruct* is=readInodeToStruct(i_node);

	if((i_node>_disk.getNumOfInodes())|(i_node<0)){
			//TODO
	}
	if((blockIndex<0) | (blockIndex>(_numOfTotalBlocks) )){
		//TODO
	}

	if (blockIndex<(Num_DirectBlocks-1)){
		blockoffset = getInodeOffset(i_node)+OFFSET_SIZE_IN_BYTES*blockIndex;
		writeDataToHardDisk(blockoffset,(void*)dblockNum,OFFSET_SIZE_IN_BYTES);
	}
	else{
		int indirectBlock= is->indirectBlockAdress;
		int inDirectBlockOffset = indirectBlock*_disk.getDataBlockSize();
		blockoffset = inDirectBlockOffset+(blockIndex-NUM_OF_DIRECT_BLOCKS)*OFFSET_SIZE_IN_BYTES;
		writeDataToHardDisk(blockoffset,(void*)dblockNum,OFFSET_SIZE_IN_BYTES);
	}
}
//---------------------------------------------------------------------------/
//								Getters and setters For i_Node
//---------------------------------------------------------------------------/

int InodeList::getNumOfTotalBlocks(){
	return _numOfTotalBlocks;
}

//--------------------------------------------------------------------------//
void InodeList::setFileType(int i_node,int _fileType)
{
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
				//TODO
	}
	InodeStruct* is=readInodeToStruct(i_node);
	is->type=_fileType;
	int inodeOffset=getInodeOffset(i_node);
	writeInodeToHardDisk(is,inodeOffset);
}

int InodeList::getFileType(int i_node)
{
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
			//TODO
		return -1;
	}
	InodeStruct* is= readInodeToStruct(i_node);
	return is->type;
}

//--------------------------------------------------------------------------//
void InodeList::setFileSize(int i_node,int _fileSize)
{
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
			//TODO
	}
	InodeStruct* is=readInodeToStruct(i_node);
	is->numOfFullBlocks=_fileSize;
	int inodeOffset=getInodeOffset(i_node);
	writeInodeToHardDisk(is,inodeOffset);

}
int InodeList::getFileSize(int i_node)
{
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
		//TODO
		return -1;
	}
	InodeStruct* is= readInodeToStruct(i_node);
	return is->numOfFullBlocks;
}

//--------------------------------------------------------------------------//
void InodeList::setActive(int i_node,bool a){
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
				//TODO
	}
	InodeStruct* is=readInodeToStruct(i_node);
	is->active=a;
	int inodeOffset=getInodeOffset(i_node);
	writeInodeToHardDisk(is,inodeOffset);

}
bool InodeList::getActive(int i_node){
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
				//TODO
	}
	InodeStruct* is= readInodeToStruct(i_node);
	return is->active;
}


