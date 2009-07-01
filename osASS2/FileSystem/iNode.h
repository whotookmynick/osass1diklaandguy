/*
 * iNode.h
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#ifndef INODE_H_
#define INODE_H_

/*
 * Each i-node has the following structure:
File type: file, directory, soft-link. (1 byte)
Number of hard links. (1 byte)
File size in bytes. (4 bytes)
10 direct blocks. (10*4 bytes)
1 single-indirect block. (4 bytes)
 *
 */
class iNode {
private:
	int _fileType;
	int _numOfHardLinks;
	int _fileSize;
	int _directBlocks[10];
	void* _indirectBlocks;
public:
	iNode();
	virtual ~iNode();
};

#endif /* INODE_H_ */
