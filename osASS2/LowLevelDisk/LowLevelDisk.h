#ifndef LOWLEVELDISK_H_
#define LOWLEVELDISK_H_

#include "Lists/BlockList.h"
//#include "../FileSystem/iNode.h"
#include  "../LogMessages/Logger.h"
#include "../Exception/Exception.cpp"
#include "Lists/FreeBlocks/FreeBlockList.h"
#include "Lists/FreeInodes/FreeInodeList.h"
#include "../Main/Util.cpp"
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include  "Lists/InodeList.h"

const int Num_DirectBlocksHelp = 10;
typedef struct {
	int  directBlock[Num_DirectBlocksHelp];//40 ~56
	int  indirectBlockAdress;//4
	int  type;//4
	int  numOfHardLinks;
	int  numOfFullBlocks;
	bool active;

} InodeStructHelp;

//class InodeList;
using namespace std;

//const int OFFSET = 0;
const int SIZE_OF_SUPER_BLOCK = 5;
const int SIZE_OF_INODE = sizeof(InodeStructHelp);
const int ROOT_INODE=0;
//SUPER BLOCK CONTROL
const int NUM_OF_BLOCK_OFFSET=0;
const int BLOCK_SIZE_OFFSET=1*4;
const int ROOT_INODE_OFFSET=2*4;

const int NUM_OF_FREE_BLOCK_OFFSET=3*4;
const int FIRST_EMPTY_BLOCK_POINTER_OFFSET=4*4;
const int LAST_EMPTY_BLOCK_POINTER_OFFSET=5*4;
const int INODE_TABLE_SIZE_OFFSET=6*4;
const int NUM_OF_FREE_INODES_OFFSET=7*4;
const int FIRST_EMPTY_INODE_POINTER_OFFSET=8*4;
const int LAST_EMPTY_INODE_POINTER_OFFSET=9*4;


const int FIRST_FREE_BLOCK_BLOCK =  3;//block 3
const int FIRST_FREE_INODE_BLOCK =  4;//block 4
const int INODE_TABLE_BLOCK_NUM = 5;

const string SYS_FILE_NAME = "FILE_SYS";

const int HARD_LINK_OFFSET = 48;

/*
* Regular files simply store the data written to them by user programs.
*A directory file stores a sequence of directory entries, each of which having the following structure:
*i-node number [4 bytes].
*file name [12 bytes].
Note – file names in dir-entries are kept without their path (so the file name of /ab/cd.out is just 'cd.out' ).
Note – all services mentioned above should return appropriate error codes to stderr, when things go bad
(for example: requesting to free an i-node that was not allocated,
 requesting to allocate an i-node when there are none free,
*asking for the file name of a free i-node, asking the 34th data block of a file that contains only 30 data blocks, etc.)
*/

typedef struct{
	int numOfBlocks;//offset 0 in file
	int blockSize;//offset 1 in file
	int rootInode;//offset 2 in file
	int numOfFreeBlocks;//offset 3 in file
	int firstEmptyBlock;//in offset 4 in file
	int lastEmptyBlock;//in offset 5
	int numOfInodes;//in offset 6
	int numOfFreeInodes;//in offset 7
	int firstFreeInode;//offset 8
	int lastFreeInode;//offset 9
	int firstBlockOfFreeInodesOffset;
	int firstBlockOfFreeBlocksOffset;
	int inodeTableOffset;
	int firstFreeBlockNumber;

}superBlock;

class LowLevelDisk
{

public:
	LowLevelDisk(int dataBlockSize,int numberOfInodes,int diskSize);
	virtual ~LowLevelDisk();



	//---------------------------------------------------------------------------/
	//								Low Level functions
	//---------------------------------------------------------------------------/


	//int allocateInode();

	/*The low Level Disk offers the following services
	 *allocates a new i-node and returns its number
	 */
	int allocateInode();

	/*
	 * frees the specified i-node
	 * and also frees all the data blocks pointed to by it
	 * (directly or indirectly).
	 */
	void freeInode(int i_node);

	/*
	 * allocates a data block and returns its number
	 */
	int allocateDataBlock();

	/*
	* frees the specified block.
	*Use (and update) the maps in the super
	*block in these allocate/free methods.
	*/
	void freeDataBlock(int dblock);

	/*
	 *return the type of the file (normal/directory/soft-link)
	 */
	int getInodeType(int i_node);

	/*
	 * set the type of the file (normal/directory/soft-link).
	 */
	void setInodeType(int i_node, int filetype);

	/*
	 *return the number of the i-th data block containing the data of this file
	 *(for 0<=i<=9 this is just stored in the 10 pointers in this i_node, for i>9,
	 * you must retrieve it from the indirect data block..).
	 */
	int getDataBlock (int i_node, int i);

	/*
	 * sets the i-th data block of this file to be dblockNum.
	 */
	void setDataBlock (int i_node, int i, int dblockNum );

	/*
	 *read the entire content of the dblockNum's and copy it into buf.
	 */
	void readBlock(int dblockNum, char* buf);


	/*
	 *overwrite the content of block with number dblockNum with the newdata block.
	 * Assume newdata's length is equal to dataBlockSize.
	 */
	void writeBlock(int dblockNum, char* newdata);

	/*
	 *return the file size saved in the given i_node.
	 *This is the exact size of the file in bytes
	 *not the number of blocks used by this file.
	 */
	int getFileSize(int i_node);

	/*
	 * set the file size.
	 */
	void setFileSize(int i_node, int newSize);

	//------------------------------------------------------------//
		int getHardLink(int i_node);
		void setHardLink(int i_node,int val);
		void initSuperBlock(int dataBlockSize,int numberOfInodes,int diskSize);


		int getNumOfBlocks();
		int getNumOfInodes();
	//    void setNumOfBlocks();
	//    void setNumOfInodes();

		/////////////////////////////////////////////
		int getNumOfFreeBlocks();
		int getNumOfFreeInodes();
		void setNumOfFreeBlocks(int i);
		void setNumOfFreeInodes(int i);

		int getDataBlockSize();
		int getInodeFromString(const string& elementName);//find the inode concted to the file name or directory name
		superBlock* getSuperBlock();
		bool existHardDisk();
private:

		bool _existHardDisk;
		int getNumOfBlocksInInodeTable();
		void createFileSystem();
		int getNumOfBlocksInFreeInodeList();

		void openFileSystem();

		/////////////////////////////////////////////


	//---------------------------------------------------------------------------/
	//								Help function
	//---------------------------------------------------------------------------/
	int  findFreeNode();
	void rmvNodeFromFreeNode();
	void initNode(int node_id);
	void addFreeNodeToFreeNodeList(int i_node);
	void freeInodeBlocks(int i_node);
	int  getFreeBlock();
	void initBlock(int block_id);
	void rmvBlockFromFreeBlock();
	void addFreeBlockToFreeBlockList(int dblock);
	bool existsFileSystem();
	void createFyleSystem(const string& filename);

	void initFreeBlocksList();
    void initFreeInodesList();
    void initInodesList();
    void* readDataFromHardDisk(int fromOffset,void* buf,int numOfBytes);//read data from hard disk
    int writeDataToHardDisk(int fromOffset,const void* buf,int numOfBytes);

   // void informSuperBlock(int offset,int value);
   // void  initVarsFromConfig();
    //void initVarsFromHardDisk();

    void initSuperBlockFromHardDisk();
	//define recursive mutex
	pthread_t _mainThread;
	pthread_mutex_t _RecMutex;
	pthread_mutexattr_t _mtxattr;




	int _fd;
	superBlock* _superBlock;
	InodeList* _iNodeTable;//block 5



	FreeBlockList* _freeBlockesList;//block 4 in super block
	FreeInodeList* _freeInodesList; //_freeInodesList;

	//block 3 in super block
	int _numOfBlocksInInodeTable;
    //rest of the block in block 5 + _numOfInodes/blocksize round up


};



#endif /*LOWLEVELDISK_H_*/
