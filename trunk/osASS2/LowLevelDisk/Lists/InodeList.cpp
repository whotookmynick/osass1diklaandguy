/*
 * InodeList.cpp
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */

#include "InodeList.h"
#include "LowLevelDisk.h"



InodeList::InodeList(int fd,int offset, LowLevelDisk& disk):_iNodesTable(),_offset(offset),_disk(disk){

	int tableSize=_disk.getNumOfInodes();
	_iNodesTable.resize(tableSize);
}

InodeList::~InodeList(){
	for( int i=0; i<_iNodesTable.size();i++){
		delete _iNodesTable[i];
	}//end while

};

iNode& InodeList::get(int inode){
	if(_iNodesTable[inode]==NULL){
		_iNodesTable[inode]=createInode(inode);
	}//end if
	return *(_iNodesTable[inode]);
}

iNode* InodeList::createInode(int inode){
	 int newInodeOffset = _offset + inode*sizeof(InodeStruct);
	 iNode* inodeN=new iNode(newInodeOffset,_disk);
	 return inodeN;
}


void InodeList::emptyBlock(int i_node){


}
