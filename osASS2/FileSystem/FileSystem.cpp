/*
 * FileSystem.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: noam
 */

#include "FileSystem.h"


FileSystem::FileSystem(int dataBlockSize,int numberOfInodes,int diskSize)
{
//	_lldisk = new LowLevelDisk(dataBlockSize,numberOfInodes,diskSize);
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

void FileSystem::f_read(int i_node,char* buffer,int offset,int nBytes)
{
	int bytesRead = 0;
	int blockNum = offset/BLOCK_SIZE;
	char currBuffer[BLOCK_SIZE];
	_lldisk->readBlock(blockNum,currBuffer);
	int blockStartOffset = offset - (blockNum * BLOCK_SIZE);
	while (strlen(currBuffer)==0 & bytesRead < nBytes)
	{
		for (int bufferIndex = blockStartOffset; bufferIndex < BLOCK_SIZE & bytesRead < nBytes; bufferIndex++)
		{
			buffer[bytesRead] = currBuffer[bufferIndex];
		}
		blockNum++;
		_lldisk->readBlock(blockNum,currBuffer);
		blockStartOffset = 0;
	}

}

void FileSystem::f_write(int i_node,char* buffer,int offset,int nBytes )
{
	int bytesWritten = 0;
	int blockNum = offset/BLOCK_SIZE;
	char currBuffer[BLOCK_SIZE];
	while (bytesWritten < nBytes & buffer[bytesWritten] != '\0')
	{
		int physicalBlockNum = _lldisk->getDataBlock(i_node,blockNum);

		if (physicalBlockNum == -1)
		{
			physicalBlockNum = _lldisk->allocateDataBlock();
		}
		_lldisk->readBlock(physicalBlockNum,currBuffer);
		int i;
		for (i = 0; i<BLOCK_SIZE & bytesWritten < nBytes & buffer[bytesWritten+i] != '\0'; i++)
		{
			currBuffer[i] = buffer[bytesWritten+i];
		}
		bytesWritten += i;
		_lldisk->writeBlock(physicalBlockNum,currBuffer);
		offset += bytesWritten;
	}
}

list<FileEntry> FileSystem::d_read(int i_node)
{
	list<FileEntry> ans;
	int fileSize = _lldisk->getFileSize(i_node);
	int currOffset = 0;
	char fileEntryBuffer[20];
	while (currOffset < fileSize)
	{
		this->f_read(i_node,fileEntryBuffer,currOffset,16);
		int entryInode = turnBytesToInt(fileEntryBuffer);
		char entryName[12];
		strcpy(entryName,fileEntryBuffer+4);
		int entrySize = _lldisk->getFileSize(entryInode);
		FileEntry entry(entryInode,entryName,entrySize);
		ans.push_back(entry);
		currOffset += 16;
	}
	return ans;
}

int FileSystem::turnBytesToInt(char* bytes)
{
	int Int32 = 0;
	Int32 = (Int32 << 8) + bytes[3];
	Int32 = (Int32 << 8) + bytes[2];
	Int32 = (Int32 << 8) + bytes[1];
	Int32 = (Int32 << 8) + bytes[0];
	return Int32;
}


void FileSystem::intToByte(int val, char *bytes )
{
bytes[0] = (char)val;
bytes[1] = (char)(val >> 8);
bytes[2] = (char)(val >> 16);
bytes[3] = (char)(val >> 24);
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

}

void FileSystem::f_delete(int i_node)
{
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

FileSystem::~FileSystem()
{

}
