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
	if (_lldisk->existHardDisk())
	{
		char* endOfFileEntriesString = "$";
		f_write(0,endOfFileEntriesString,0,1);
	}
}

/*
 * if flag == true then file created is a soft link
 * else regular file
 */
int FileSystem::createFile(int flag)
{
	int ans = 0;
	ans = _lldisk->allocateInode();
	if (ans == -1)
	{
		cerr<<"there are no free i-nodes"<<endl;
		return ans;
	}
//	cout<<"FS::createFile flag = "<<flag<<endl;
	if (flag == SOFT_LINK)
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
	if (newDirNum == -1)
	{
		cerr<<"there are no free i-nodes"<<endl;
		return newDirNum;
	}
	_lldisk->setInodeType(newDirNum,DIR_TYPE);
	return newDirNum;
}

int FileSystem::getFileType(int i_node)
{
//	cout<<"FS::getFileType entry i_node = "<<i_node<<endl;
	return _lldisk->getInodeType(i_node);
}

int FileSystem::f_read(int i_node,char* buffer,int offset,int nBytes)
{
	int bytesRead = 0;
	int blockNumInFile = offset/BLOCK_SIZE;
	int physicalBlockNum = _lldisk->getDataBlock(i_node,blockNumInFile);
	char currBuffer[BLOCK_SIZE];
	if (physicalBlockNum != -1)
	{
		_lldisk->readBlock(physicalBlockNum,currBuffer);
	}
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
	int offsetInFile = offset - (BLOCK_SIZE*blockNumInFile);
	int originalOffsetInFile = offsetInFile;
	while (bytesWritten < nBytes )
	{
		int physicalBlock = _lldisk->getDataBlock(i_node,blockNumInFile);
		if (physicalBlock == -1)
		{
			physicalBlock = _lldisk->allocateDataBlock();
//			cout<<"f_write physicalBlock = "<<physicalBlock<<endl;
			_lldisk->setDataBlock(i_node,blockNumInFile,physicalBlock);
		}
		_lldisk->readBlock(physicalBlock,currBuffer);
//		cout<<"f_write block Data That Was Read = ";
//		printBuffer(currBuffer,BLOCK_SIZE);
		int i,bytesWrittenInIteration = 0;
		for (i = offsetInFile; i<BLOCK_SIZE & bytesWritten + bytesWrittenInIteration < nBytes; i++)
		{
			currBuffer[i] = buffer[bytesWrittenInIteration];
			bytesWrittenInIteration++;
		}
		bytesWritten += bytesWrittenInIteration;
//		bytesWritten += i;
//		cout<<"f_write blockDataThatWas Written = "<<currBuffer<<endl;
//		cout<<"f_write blockDataThatWas Written = ";
//		printBuffer(currBuffer,BLOCK_SIZE);
		_lldisk->writeBlock(physicalBlock,currBuffer);
		blockNumInFile++;
		offsetInFile = 0;
	}
	int originalFileSize = _lldisk->getFileSize(i_node);
	int bytes_added_during_write = bytesWritten + originalOffsetInFile - originalFileSize;
	if (bytes_added_during_write > 0)
	{
		int newFileSize = bytes_added_during_write + originalFileSize;
		_lldisk->setFileSize(i_node,newFileSize);
	}
	return bytesWritten;
}

list<FileEntry>* FileSystem::d_read(int i_node)
{
	list<FileEntry> *ans = new list<FileEntry>();
	//cout<<"FileSystem::d_read before getFileSize"<<endl;
	int fileSize = _lldisk->getFileSize(i_node);
	//cout<<"FileSystem::d_read after getFileSize"<<endl;
	int currOffset = 0;
	char fileEntryBuffer[20];
	//cout<<"FileSystem::d_read before f_read"<<endl;
	int bytesRead = this->f_read(i_node,fileEntryBuffer,currOffset,16);
	//cout<<"FileSystem::d_read after f_read"<<endl;
	while (fileEntryBuffer[0] != '$' & bytesRead == 16)
	{
		//cout<<"FileSystem::d_read begin of loop first char is '"<<fileEntryBuffer[0]<<"'"<<endl;
		int entryInode = turnBytesToInt(fileEntryBuffer);
		char *entryName = (char*)malloc(sizeof(char)*12);
		strcpy(entryName,fileEntryBuffer+4);

//		cout<<"d_read entryName = "<<entryName<<endl;

		int entrySize = _lldisk->getFileSize(entryInode);
		FileEntry* entry = new FileEntry(entryInode,entryName,entrySize);
		ans->push_back(*entry);
//		cout<<"d_read ans->front().getFileName() = "<<ans->front().getFileName()<<endl;
		currOffset += 16;
		bytesRead = this->f_read(i_node,fileEntryBuffer,currOffset,16);
	}
	return ans;
}

void FileSystem::d_write(int i_node,list<FileEntry> &dlist)
{
	char currEntryBuffer[20];
	int currOffset = 0;
//	list<FileEntry> oldDir = this->d_read(i_node);
	list<FileEntry>::iterator it;
	it = dlist.begin();

	while( it != dlist.end() ) {
		FileEntry currEntry = *it;
		intToByte(currEntry.getInodeNum(),currEntryBuffer);
//		cout<<"d_write currEntry.getFileName() = "<<currEntry.getFileName()<<endl;
		strcpy(currEntryBuffer+4,currEntry.getFileName());
//		cout<<"d_write currEntryBuffer = ";
//		printBuffer(currEntryBuffer,16);
		this->f_write(i_node,currEntryBuffer,currOffset,16);
//		_lldisk->setFileSize(currEntry.getInodeNum(),currEntry.getFileSize());
		currOffset += 16;
	    ++it;
    }
	char* endOfFileEntriesString = "$";
	this->f_write(i_node,endOfFileEntriesString,currOffset,1);
//	_lldisk->setFileSize(i_node,_lldisk->getFileSize(i_node) + currOffset);

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

int FileSystem::d_delete(int i_node)
{
	list<FileEntry>* dListing = this->d_read(i_node);
	if (dListing->empty())
	{
		f_delete(i_node);
		return 1;
	}
	cerr<<"Directory is not empty"<<endl;
	return -1;
}

int FileSystem::getNumOfHardLinks(int i_node)
{
	return _lldisk->getHardLink(i_node);
}

void FileSystem::setNumOfHardLinks(int i_node,int num)
{
	_lldisk->setHardLink(i_node,num);
}

FileSystem::~FileSystem()
{

}
