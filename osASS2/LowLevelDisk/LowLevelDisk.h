#ifndef LOWLEVELDISK_H_
#define LOWLEVELDISK_H_

#include <string>
#include <string.h>
#include <string>
using namespace std;

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

class LowLevelDisk
{

public:
	LowLevelDisk();
	virtual ~LowLevelDisk();

	/*The low Level Disk offers the following services
	 *allocates a new i-node and returns its number
	 */
	void allocateInode();

	/*
	 * frees the specified i-node
	 * and also frees all the data blocks pointed to by it
	 * (directly or indirectly).
	 */
	void freeInode(int i_node);

	/*
	 * allocates a data block and returns its number
	 */
	void allocateDataBlock();

	/*
	* frees the specified block.
	*Use (and update) the maps in the super
	*block in these allocate/free methods.
	*/
	void freeDataBlock(int dblock);

	/*
	 *return the type of the file (normal/directory/soft-link)
	 */
	string getInodeType(int i_node);

	/*
	 * set the type of the file (normal/directory/soft-link).
	 */
	void setInodeType(int i_node, string filetype);

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

	/*The low Level Disk offers the following services:*/


};



#endif /*LOWLEVELDISK_H_*/
