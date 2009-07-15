/*
 * FileSystem.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "FileSystem.h"


FileSystem::FileSystem(int dataBlockSize,int numberOfInodes,int diskSize):BLOCK_SIZE(dataBlockSize)
{
	_lldisk = new LowLevelDisk(dataBlockSize,numberOfInodes,diskSize);
}

/*
 * if flag == true then file created is a soft link
 * else regular file
 */
int FileSystem::createFile(int flag)
{
	int ans = 0;
	ans = _lldisk->allocateInode();
	if (flag)
	{
		_lldisk->setInodeType(ans,SOFT_LINK);
	}
	else
	{
		_lldisk->setInodeType(ans,REGULARE_FILE);
	}
	return ans;
}


int FileSystem::createDir()
{
	int newDirNum = _lldisk->allocateInode();
	_lldisk->setInodeType(newDirNum,DIR_TYPE);
	return newDirNum;
}

int FileSystem::getFileType(int i_node)
{
	return _lldisk->getInodeType(i_node);
}

int FileSystem::f_read(int i_node,char* buffer,int offset,int nBytes)
{
	int bytesRead = 0;
	int blockNumInFile = offset/BLOCK_SIZE;
	int physicalBlockNum = _lldisk->getDataBlock(i_node,blockNumInFile);
	char currBuffer[BLOCK_SIZE];
	_lldisk->readBlock(physicalBlockNum,currBuffer);
	int blockStartOffset = offset - (blockNumInFile * BLOCK_SIZE);
	while (physicalBlockNum!=-1 & bytesRead < nBytes)
	{
		for (int bufferIndex = blockStartOffset; bufferIndex < BLOCK_SIZE & bytesRead < nBytes; bufferIndex++)
		{
			buffer[bytesRead] = currBuffer[bufferIndex];
			bytesRead++;
		}
		blockNumInFile++;
		physicalBlockNum = _lldisk->getDataBlock(i_node,blockNumInFile);
		_lldisk->readBlock(physicalBlockNum,currBuffer);
		blockStartOffset = 0;
	}
	return bytesRead;

}

int FileSystem::f_write(int i_node,char* buffer,int offset,int nBytes )
{
	int bytesWritten = 0;
	int blockNumInFile = offset/BLOCK_SIZE;
	char currBuffer[BLOCK_SIZE];
	while (bytesWritten < nBytes )
	{
		int physicalBlock = _lldisk->getDataBlock(i_node,blockNumInFile);
		if (physicalBlock == -1)
		{
			physicalBlock = _lldisk->allocateDataBlock();
			_lldisk->setDataBlock(i_node,blockNumInFile,physicalBlock);
		}
		_lldisk->readBlock(physicalBlock,currBuffer);
		int i;
		for (i = 0; i<BLOCK_SIZE & bytesWritten < nBytes & buffer[bytesWritten+i] != '\0'; i++)
		{
			currBuffer[i] = buffer[bytesWritten+i];
			bytesWritten++;
		}
//		bytesWritten += i;
		_lldisk->writeBlock(physicalBlock,currBuffer);
		blockNumInFile++;
	}
	return bytesWritten;
}

list<FileEntry> FileSystem::d_read(int i_node)
{
	list<FileEntry> ans;
	int fileSize = _lldisk->getFileSize(i_node);
	int currOffset = 0;
	char fileEntryBuffer[20];
	this->f_read(i_node,fileEntryBuffer,currOffset,16);
	while (fileEntryBuffer[0] != '\0')
	{
		int entryInode = turnBytesToInt(fileEntryBuffer);
		char entryName[12];
		strcpy(entryName,fileEntryBuffer+4);
		int entrySize = _lldisk->getFileSize(entryInode);
		FileEntry entry(entryInode,entryName,entrySize);
		ans.push_back(entry);
		currOffset += 16;
		this->f_read(i_node,fileEntryBuffer,currOffset,16);
	}
	return ans;
}

void FileSystem::d_write(int i_node,list<FileEntry> dlist)
{
	char currEntryBuffer[20];
	int currOffset = 0;
	list<FileEntry> oldDir = this->d_read(i_node);
	list<FileEntry>::iterator it;
	it = dlist.begin();

	while( it != dlist.end() ) {
		FileEntry currEntry = *it;
		intToByte(currEntry.getInodeNum(),currEntryBuffer);
		strcpy(currEntryBuffer+4,currEntry.getFileName());
		this->f_write(i_node,currEntryBuffer,currOffset,16);
		currOffset += 16;
	    ++it;
    }
	this->f_write(i_node,"\0",currOffset+1,1);
	/*
	if (currOffset < _lldisk->getFileSize(i_node))
	{
		int currBlockInFile = currOffset / BLOCK_SIZE;
		int currBlock = _lldisk->getDataBlock(i_node, currBlockInFile);
		char currBlockBuffer[BLOCK_SIZE];
		_lldisk->readBlock(currBlock,currBlockBuffer);
		for(int i = currOffset - (currBlockInFile * BLOCK_SIZE); i < BLOCK_SIZE; i++)
		{
			currBlockBuffer[i] = 0;
		}
		_lldisk->writeBlock(currBlock,currBlockBuffer);
		currBlockInFile++;
		currBlock = _lldisk->getDataBlock(i_node,currBlockInFile);
		while (currBlock != -1)
		{
			_lldisk->freeDataBlock(currBlock);
			currBlockInFile++;
			currBlock = _lldisk->getDataBlock(i_node,currBlockInFile);
		}
	}
// Now to check if no old data is left.
*/
}

void FileSystem::f_delete(int i_node)
{
	int blockNumInFile = 0;
	int physicalBlockNum = _lldisk->getDataBlock(i_node,blockNumInFile);
	while(physicalBlockNum != -1)
	{
		_lldisk->freeDataBlock(physicalBlockNum);
		blockNumInFile++;
		physicalBlockNum = _lldisk->getDataBlock(i_node,blockNumInFile);
	}
	_lldisk->freeInode(i_node);
}

void FileSystem::d_delete(int i_node)
{
	list<FileEntry> dListing = this->d_read(i_node);
	if (dListing.empty())
	{
		f_delete(i_node);
	}
}

int FileSystem::getNumOfHardLinks(int i_node)
{
	//TODO: IMPLEMEMNT HERE
	return 1;
}

void FileSystem::setNumOfHardLinks(int i_node,int num)
{

}

FileSystem::~FileSystem()
{

}
