/*
 * Descriptor.h
 *
 *  Created on: Jul 13, 2009
 *      Author: noam
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

class Descriptor {
public:
	int _filePosition;
	int _flag;
	int _inode;

	Descriptor(int flag,int inode):_filePosition(0),_flag(flag),_inode(inode){};
	virtual ~Descriptor(){};
};

#endif /* DESCRIPTOR_H_ */
