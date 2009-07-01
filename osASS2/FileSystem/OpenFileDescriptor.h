/*
 * OpenFileDescriptor.h
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#ifndef OPENFILEDESCRIPTOR_H_
#define OPENFILEDESCRIPTOR_H_

#include "iNode.h"

class OpenFileDescriptor {
private:
	int _currOffset;
	iNode* _inode;

public:
	OpenFileDescriptor();
	virtual ~OpenFileDescriptor();
};

#endif /* OPENFILEDESCRIPTOR_H_ */
