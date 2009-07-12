/*
 * InodeList.cpp
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */

#include "InodeList.h"




InodeList::InodeList(int fd,int offset, LowLevelDisk& disk):_inodesMap(),_offset(offset),_disk(disk){

}

InodeList::~InodeList(){
	map<int,iNode*>::iterator it;
	it = _inodesMap.begin();
	while (it!=_inodesMap.end()) {
		LOG_DEBUG("free inode "<<it->first<<"\n");
		delete it->second;
		++it;
	}//end while

};

iNode& InodeList::get(int inode){
	if(_inodesMap[inode]==NULL){
		_inodesMap[inode]=createInode(inode);
	}//end if
	return *(_inodesMap[inode]);
}

iNode* InodeList::createInode(int inode){
	 int newInodeOffset = _offset + inode*sizeof(InodeStruct);
	 iNode* inodeN=new iNode(newInodeOffset,_disk);
	 return inodeN;
}


void InodeList::emptyBlock(int i_node){}
