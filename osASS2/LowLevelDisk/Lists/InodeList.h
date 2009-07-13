/*
 * InodeList.h
 *
 *  Created on: Jul 10, 2009
 *      Author: dikla
 */

#ifndef INODELIST_H_
#define INODELIST_H_

#include "../../LogMessages/Logger.h"
//#include "LowLevelDisk.h"
#include "../../FileSystem/iNode.h"
#include <sstream>
#include <math.h>
#include <sys/mman.h>
#include <map>
#include <vector>

//class LowLevelDisk;
using namespace std;

class InodeList{
public:

	InodeList(int fd,int offset, LowLevelDisk& disk);

	virtual ~InodeList();

	iNode& get(int inode);

	vector<iNode*> _iNodesTable;

	iNode* createInode(int inode);
	void emptyBlock(int i_node);

private:
    int _offset;
    int _inodeSizeInBytes;
    LowLevelDisk& _disk;
    int _fd;
};

#endif /* INODELIST_H_ */

