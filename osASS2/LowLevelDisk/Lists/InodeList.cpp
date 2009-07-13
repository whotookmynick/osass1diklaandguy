
/*
 * InodeList.cpp
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */

#include "InodeList.h"
#include "LowLevelDisk.h"
#include "iNode.h"



InodeList::InodeList(int fd,int offset, LowLevelDisk& disk):_iNodesTable(),_offset(offset),
_inodeSizeInBytes(sizeof(InodeStruct)),_disk(disk){

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
