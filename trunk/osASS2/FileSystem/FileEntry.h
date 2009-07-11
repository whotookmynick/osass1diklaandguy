/*
 * FileEntry.h
 *
 *  Created on: Jul 11, 2009
 *      Author: aradno
 */

#ifndef FILEENTRY_H_
#define FILEENTRY_H_

class FileEntry {
private:
	int _inodeNum;
	char* _fileName;
	int _fileSize;
public:
	FileEntry(int inodeNum,char* fileName,int fileSize):
		_inodeNum(inodeNum),_fileName(fileName),_fileSize(fileSize){};

	virtual ~FileEntry(){};

    char* getFileName()
    {
        return _fileName;
    }

    int getFileSize() const
    {
        return _fileSize;
    }

    int getInodeNum() const
    {
        return _inodeNum;
    }

    void setFileName(char* fileName)
    {
        this->_fileName = fileName;
    }

    void setFileSize(int fileSize)
    {
        this->_fileSize = fileSize;
    }

    void setInodeNum(int _inodeNum)
    {
        this->_inodeNum = _inodeNum;
    }

};

#endif /* FILEENTRY_H_ */
