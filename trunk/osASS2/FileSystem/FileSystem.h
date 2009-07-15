#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <list>
#include <vector>
#include <iostream>
#include <string>
#include "FileEntry.h"
#include "LowLevelDisk.h"
#include "../Main/Util.cpp"
//#include "OpenFileDescriptor.h"

//const int BLOCK_SIZE = 64;


using namespace std;
class FileSystem
{
private:
	LowLevelDisk* _lldisk;
	int BLOCK_SIZE;
//	vector<OpenFileDescriptor*> _openFilesTable;

	/* ------------------------- HELP FUNCTIONS ---------------------------------*/


	/* END HELP FUNCTIONS */

public:

	FileSystem(int dataBlockSize,int numberOfInodes,int diskSize);

	virtual ~FileSystem();
	/**
	 ** creates a file returning its i_node number. Flag indicates whether the file to create is a regular file or a soft link.
	 **/
	virtual int createFile(int flag);
	/**
	 ** creates a directory, returning its i_node number.
	 **/
	virtual int createDir();

	/**
	 ** Returns the type of the file this i_node represents.
	 **/
	virtual int getFileType(int i_node);
	/**
	 * reads nBytes (at most) from the given file (represented by this i_node) at the given offset inside the file
	 * into the buffer.
	 * returns the number of bytes read
	 **/
	virtual int f_read(int i_node,char* buffer,int offset,int nBytes);

	/**
	 ** writes nBytes (at most) to the given file (represented by this i_node)
	 **  at the given offset inside that file, from the buffer.
	 **  If the amount to be written exceeds the current allocated block,
	 **  a new block should be allocated to accommodate the rest of the data.
	 **/
	virtual int f_write(int i_node,char* buffer,int offset,int nBytes );

	/**
	 ** Notice that both f_read and f_write only work on normal files (not directories).
	 **/

	/**
	 ** reads the entire content of the directory file specified by the given i_node.
	 ** This information is then entered into a data structure (a vector or list)
	 ** that lists the files in this directory.
	 ** Each element in this list therefore contains a filename and i_node number.
	 ** returns the number of bytes written.
	 **/
	virtual list<FileEntry> d_read(int i_node);

	/**
	 ** replaces the content of the given directory file (specified by i_node)
	 ** with the information stored in the list.
	 ** The list has the same format as that returned by d_read().
	 **/
	virtual void d_write(int i_node,list<FileEntry> dlist);

	/**
	 ** delete the file.
	 **/
	virtual void f_delete(int i_node);
	/**
	 ** delete the directory if it is empty.
	 **/
	virtual void d_delete(int i_node);

};

#endif /**FILESYSTEM_H_**/
