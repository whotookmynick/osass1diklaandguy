/*
 * iNode.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "iNode.h"

iNode::iNode() {


}
iNode::iNode(int offset,LowLevelDisk& disk){


}

iNode::~iNode() {

}

void iNode::setFileType(int _fileType)
{
    _inodeStruct->type = _fileType;
}

int iNode::getNumOfHardLinks()
{
   return _inodeStruct->numOfHardLinks;
}

void iNode::setNumOfHardLinks(int numOfHardLinks)
{
 _inodeStruct->numOfHardLinks= numOfHardLinks;
}

int iNode::getFileSize()
{
   return _inodeStruct->size;
}

int iNode::getFileType()
{
	return _inodeStruct->type;
}

void iNode::setFileSize(int _fileSize)
{
   _inodeStruct->size = _fileSize;
}

void iNode::setActive(bool a){
	_active=a;
}

bool iNode::getActive(){
	return _active;
	}
int iNode::getNumOfDataBlock(int dblock){
	return -1;//TODO
}
void iNode::setNumOfDataBlock(int dblockNum,int i){

}
