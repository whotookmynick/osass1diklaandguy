/*
 * iNode.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "iNode.h"

iNode::iNode() {
	// TODO Auto-generated constructor stub

}

iNode::~iNode() {
	// TODO Auto-generated destructor stub
}

void iNode::setFileType(int _fileType)
{
    this->_fileType = _fileType;
}

int iNode::getNumOfHardLinks()
{
   return _numOfHardLinks;
}

void iNode::setNumOfHardLinks(int _numOfHardLinks)
{
   this->_numOfHardLinks = _numOfHardLinks;
}

int iNode::getFileSize()
{
   return _fileSize;
}

int iNode::getFileType()
{
   return _fileType;
}

void iNode::setFileSize(int _fileSize)
{
   this->_fileSize = _fileSize;
}


