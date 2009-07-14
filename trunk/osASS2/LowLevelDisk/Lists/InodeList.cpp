
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
	InodeStruct* is;
	pread(_fd,&is,sizeof(is),offset);
	return is;
}

//TODO:what if inode is not active?
//TODO:need to clear inode first???
void InodeList::writeInodeToHardDisk(InodeStruct* is,int offset){
	pwrite(_fd,&is,sizeof(is),offset);
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

	if (blockIndex<9)
		return getInodeOffset(i_node)+OFFSET_SIZE_IN_BYTES*blockIndex;

	else{
		int indirectBlock= is->indirectBlockAdress;
		int blockOffset = indirectBlock*_disk.getDataBlockSize();
		return blockOffset+(blockIndex-NUM_OF_DIRECT_BLOCKS)*OFFSET_SIZE_IN_BYTES;
	}

}

int InodeList::getInodeOffset(int i_node){
	if((i_node<_disk.getNumOfInodes())|(i_node<0)){
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

int InodeList::getDataBlockNum(int i_node,int dblock){
	return -1;
}

void InodeList::setNumOfDataBlock(int i_node, int i, int dblockNum ){
	//TODO

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



//find the inode offset
	//int inodeOfsset = _offset+(_inodeSizeInBytes*(inode-1));
	//i_node._inodeStruct=
	//iNode i_node = new iNode(inodeOfsset,_disk);
	//InodeStruct inodeStruct= pread(_fd,)
	//read the inode from the SYS_FILE to new inodeStruct


/*
int LowLevelDisk::getInodeType(int i_node){
	cout<<" getInodeType -  need to finish implement lists "<<endl;
	int type=-1;
	pthread_mutex_lock(&_RecMutex);
	 if ((i_node<0) | (i_node>=(_superBlock->numOfInodes))){
		 //
	 }
	 else{
		 type =_iNodeTable->get(i_node).getFileType();;
	 }
	return type;
	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::setInodeType(int i_node, int filetype){
	cout<<"setInodeType-  need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);

	if ((filetype<0) | (filetype>2 ) | (i_node<0) |(i_node>=(_superBlock->numOfInodes)) ){
	//cerr<<"no such file type"<<endl\
	//pthread_mutex_unlock(&_RecMutex);
	//	throw invalid_argument("no such file type");//TODO: add exception
	}
	else{
		_iNodeTable->get(i_node).setFileType(filetype);
	}
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getDataBlock (int i_node, int dblock){
	cout<<" getDataBlock--  need to finish implement lists  "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((dblock>=0) | (dblock<=9) |  (i_node<0) |(i_node>=(_superBlock->numOfInodes)) ){
		pthread_mutex_unlock(&_RecMutex);
		//return the i enter in the i_node
		return -1 ;
	}
	else{

        return _iNodeTable->get(i_node).getNumOfDataBlock(dblock);
        //TODO: if not allocated yet??? allocate and delete the block in the file?????
	}
	pthread_mutex_unlock(&_RecMutex);

}

http://www.cplusplus.com/reference/iostream/ofstream/ofstream/

http://www.java2s.com/Tutorial/Cpp/0240__File-Stream/Saveandreadstructure.htm


void LowLevelDisk::setDataBlock (int i_node, int i, int dblockNum ){
	cout<<"setDataBlock - need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((dblockNum<0) | (i_node<0) |(i_node>=(_superBlock->numOfInodes))|(!(_iNodeTable->get(i_node).getActive()) )){
		//TODO
	}
	_iNodeTable->get(i_node).setNumOfDataBlock(dblockNum,i);
	pthread_mutex_unlock(&_RecMutex);
}
*/
