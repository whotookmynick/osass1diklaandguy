/*
 * iNode.h
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#ifndef INODE_H_
#define INODE_H_
#define Num_DirectBlocks 10

class LowLevelDisk;
/*
 * Each i-node has the following structure:
File type: file, directory, soft-link. (1 byte)
Number of hard links. (1 byte)
File size in bytes. (4 bytes)
10 direct blocks. (10*4 bytes)
1 single-indirect block. (4 bytes)
 *
 */

typedef struct {
	int  type;
	int  numOfHardLinks;
	int  size;
	int  directBlock[Num_DirectBlocks];
	int  indirectBlockAdress;
} InodeStruct;


class iNode {
private:
	InodeStruct* _inodeStruct;
	bool _active;
public:


	iNode(int offset,LowLevelDisk& disk);
	iNode();
	virtual ~iNode();
	void setFileType(int _fileType);
	int getNumOfHardLinks();
	void setNumOfHardLinks(int _numOfHardLinks);
	int getFileSize();

	void setFileSize(int _fileSize);
	int getFileType();

	void setActive(bool a);
	bool getActive();

	int getNumOfDataBlock(int dblock);//return the number of data block contain this file
	void setNumOfDataBlock(int dblockNum,int i);
};

#endif /* INODE_H_ */
//0==normal 1==directory 2==soft link
