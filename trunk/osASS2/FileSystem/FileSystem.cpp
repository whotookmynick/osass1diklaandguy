/*
 * FileSystem.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "FileSystem.h"


FileSystem::FileSystem()
{
	_lldisk = new LowLevelDisk();
}

/*
 * if flag == true then file created is a soft link
 * else regular file
 */
int FileSystem::createFile(int flag)
{
	int ans = 0;
	if (flag)
	{
		ans = createSoftLink();
	}
	else
	{
		ans = createRegularFile();
	}
	return ans;
}

int FileSystem::createSoftLink()
{
	cout<<"FileSystem::createSoftLink not implemented yet"<<endl;
	return 0;
}

int FileSystem::createRegularFile()
{
	int newInodeNumber = _lldisk->allocateInode();
	return newInodeNumber;
}


int FileSystem::createDir()
{
	int newDirNum = this->createRegularFile();
	return newDirNum;
}

int FileSystem::getFileType(int i_node)
{
	return _lldisk->getInodeType(i_node);
}

FileSystem::~FileSystem()
{

}
