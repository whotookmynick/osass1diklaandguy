#include "LowLevelDisk.h"
#include <sys/stat.h>
#include <sstream>
#include <math.h>
#include <fcntl.h>


//---------------------------------------------------------------------------/
//								Help function
//---------------------------------------------------------------------------/

int getInodeFromString(const string& elementName){

	cout<<" unimplemented function getInodeFromString "<<endl;
	return 1;
}

int  LowLevelDisk::findFreeNode(){
	cout<<" unimplemented function findFreeNode "<<endl;
	LOG_DEBUG("find free node");
	/*if (((BlockList*)_freeInodesList)->empty()) {
		throw EmptyListException("FreeInode");
	}
	((BlockList*)_freeInodesList)->head();
	((BlockList*)_freeInodesList)->pop_front();
	//TODO create new inode
	//_iNodeTable->get(inodeNum).setActive(false);
*/
	return 1;
}

void LowLevelDisk::rmvNodeFromFreeNode(){
	cout<<" unimplemented function  "<<endl;

	// LOG_DEBUG("remove node from freeNode table");
}

void LowLevelDisk::initNode(int node_id){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("init node " << node_id);

}

void LowLevelDisk::addFreeNodeToFreeNodeList(int i_node){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("add  Node "<<i_node<<" To Free Node List ");
}

void LowLevelDisk::freeInodeBlocks(int i_node){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("free i Node "<<i_node<<" blocks ");
}

int  LowLevelDisk::getFreeBlock(){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("get next free Node ");
	return 1;
}

void LowLevelDisk::initBlock(int block_id){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("init block "<<block_id);
}

void LowLevelDisk::rmvBlockFromFreeBlock(){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("remove Block from free Block table");
}

void LowLevelDisk::addFreeBlockToFreeBlockList(int dblock){
	cout<<" unimplemented function  "<<endl;
	//LOG_DEBUG("add  Block "<<dblock<<" To Free Block List ");
}


bool LowLevelDisk::existsFileSystem() {
  struct stat stFileInfo;
  bool blnReturn;
  int intStat;

  // Attempt to get the file attributes
  intStat = stat(SYS_FILE_NAME.c_str(),&stFileInfo);
  if(intStat == 0) {
    // We were able to get the file attributes
    // so the file obviously exists.
    blnReturn = true;
  } else {
    // We were not able to get the file attributes.
    // This may mean that we don't have permission to
    // access the folder which contains this file. If you
    // need to do that level of checking, lookup the
    // return values of stat which will give you
    // more details on why stat failed.
    blnReturn = false;
  }

  return(blnReturn);
}

void* LowLevelDisk::createFileSystem(){

	return NULL;
}

void LowLevelDisk::openFileSystem(){

	_fd = open(SYS_FILE_NAME.c_str(),O_RDWR);
	  if (_fd!=-1){
		  throw OpenFileExcemption(SYS_FILE_NAME);
	  }




}

void* LowLevelDisk::readDataFromHardDisk(int fromOffset,void* buf,int numOfBytes){
	off_t offset = fromOffset;
	size_t size = numOfBytes;
	pread(_fd,buf,size,offset);
	return buf;
}

int LowLevelDisk::writeDataToHardDisk(int fromOffset,const void* buf,int numOfBytes){
	off_t offset = fromOffset;
	size_t size = numOfBytes;
	size_t sizeWriten = pwrite(_fd,buf,size,offset);
	int sizeW = sizeWriten;
	return sizeW;
}
//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

void LowLevelDisk::initFreeInodesList() {
    LOG_DEBUG("init freeInodeList\n");
    _superBlock->_firstBlockOfFreeInodesOffset = FIRST_FREE_INODE_BLOCK*_superBlock->_blockSize;
   // _freeInodesList=new FreeInodeList(_fd,_superBlock->_firstBlockOfFreeInodesOffset
    //		,_superBlock->_firstFreeInode, _superBlock->_lastFreeInode,*this);
}

void LowLevelDisk::initFreeBlocksList() {
    LOG_DEBUG("init freeBlocksList\n");
    _superBlock->_firstBlockOfFreeBlocksOffset = FIRST_FREE_BLOCK_BLOCK*_superBlock->_blockSize;

    _freeBlockesList = new FreeBlockList(_fd,_superBlock->_firstBlockOfFreeBlocksOffset
    		,_superBlock->_firstEmptyBlock,_superBlock->_lastEmptyBlock,*this);
}

void LowLevelDisk::initInodesList() {
    LOG_DEBUG("init inodesList\n");
    _numOfBlocksInInodeTable = ((_superBlock->_numOfInodes) / (_superBlock->_blockSize))+1;
    _superBlock->_inodeTableOffset = INODE_TABLE_BLOCK_NUM*(_superBlock->_blockSize);

    _iNodeTable = new InodeList(_fd, _superBlock->_inodeTableOffset, *this);
}

void LowLevelDisk::initVarsFromHardDisk(){
	////////////////////////////////////////////////////////////////////
	//						init super block						  //
	///////////////////////////////init super block/////////////////////
	char buf[1];//TODO: check if buf is cahnge

	readDataFromHardDisk(NUM_OF_BLOCK_OFFSET,(void*) buf,1);
	_superBlock->_numOfBlocks=buf[0];


	readDataFromHardDisk(BLOCK_SIZE_OFFSET,(void*) buf,1);
	_superBlock->_blockSize=buf[0];

	readDataFromHardDisk(ROOT_INODE_OFFSET,(void*) buf,1);
	_superBlock->_rootInode=buf[0];

	readDataFromHardDisk(NUM_OF_FREE_BLOCK_OFFSET,(void*) buf,1);
	_superBlock->_numOfFreeBlocks=buf[0];

	readDataFromHardDisk(FIRST_EMPTY_BLOCK_POINTER_OFFSET,(void*) buf,1);
	_superBlock->_firstEmptyBlock=buf[0];

	readDataFromHardDisk(LAST_EMPTY_BLOCK_POINTER_OFFSET,(void*) buf,1);
	_superBlock->_lastEmptyBlock=buf[0];

	readDataFromHardDisk(INODE_TABLE_SIZE_OFFSET,(void*) buf,1);
	_superBlock->_numOfInodes=buf[0];

	readDataFromHardDisk(NUM_OF_FREE_INODES_OFFSET,(void*) buf,1);
	_superBlock->_numOfFreeInodes=buf[0];

	readDataFromHardDisk(FIRST_EMPTY_INODE_POINTER_OFFSET,(void*) buf,1);
	_superBlock->_firstFreeInode=buf[0];

	readDataFromHardDisk(LAST_EMPTY_INODE_POINTER_OFFSET,(void*) buf,1);
	_superBlock->_lastFreeInode=buf[0];

	initFreeBlocksList();
	initFreeInodesList();
	initInodesList();

	/*	_numOfBlocks;//offset 0 in file
	_blockSize;//offset 1 in file
	_rootInode;//offset 2 in file
	_numOfFreeBlocks;//offset 3 in file
	_firstEmptyBlock;//in offset 4 in file
	_lastEmptyBlock;//in offset 5
	_numOfInodes;//in offset 6
	_numOfFreeInodes;//in offset 7


	_firstFreeInode;//offset 8
	_lastFreeInode;//offset 9

	    BlockList* _freeInodesList;//block 3 in super block
	    BlockList* _freeBlockesList;//block 4 in super block
	    InodeList* _iNodeTable;//block 5




	    // DiskDescriptor();
	    //initFreeBlocksList();

	    //initFreeInodesList();
	    //initInodesList();
*/
}

void LowLevelDisk::initVarsFromConfig(){

	/*

	BlockList* _freeInodesList;//block 3 in super block
	    BlockList* _freeBlockesList;//block 4 in super block
	    InodeList* _iNodeTable;//block 5
	    // DiskDescriptor();
	    //initFreeBlocksList();
	    //initFreeInodesList();
	    //initInodesList();	     */
}


LowLevelDisk::LowLevelDisk(int dataBlockSize,int numberOfInodes,int diskSize):_iNodeTable()
{

	if (existsFileSystem()){
			openFileSystem();
			initVarsFromHardDisk();
	}
	else{
			createFileSystem();
			initVarsFromConfig();
	}

	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_RecMutex, &_mtxattr);
}


LowLevelDisk::~LowLevelDisk()
{
	//delete _freeInodesList;
	delete _freeBlockesList;
	pthread_mutex_destroy(&_RecMutex);
	pthread_mutexattr_destroy(&_mtxattr);


}


//---------------------------------------------------------------------------/
//								Getters and setters
//---------------------------------------------------------------------------/
int LowLevelDisk::getNumOfBlocks(){
	return _superBlock->_numOfBlocks;
}

int LowLevelDisk::getNumOfInodes(){
	return _superBlock->_numOfInodes;
}


void LowLevelDisk::setNumOfFreeBlocks(int i){
	_superBlock->_numOfBlocks=i;
}

void LowLevelDisk::setNumOfFreeInodes(int i){
	_superBlock->_numOfInodes=i;
}

int LowLevelDisk::getDataBlockSize(){
	return _superBlock->_blockSize;
}

//---------------------------------------------------------------------------/
//								Low Level functions
//---------------------------------------------------------------------------/


//TODO: inform the super block
//TODO: Log msg and exception
int  LowLevelDisk::allocateInode(){
	cout<<" unimplemented function allocateInode "<<endl;
	pthread_mutex_lock(&_RecMutex);

	int node_id = LowLevelDisk::findFreeNode();

	LowLevelDisk::rmvNodeFromFreeNode();//remove the node frome free node list
	LowLevelDisk::initNode(node_id);//init node details to defult




    int inodeNum;
    pthread_mutex_unlock(&_RecMutex);
    return node_id;
}


void LowLevelDisk::freeInode(int i_node){
	cout<<" unimplemented function freeInode "<<endl;
	pthread_mutex_lock(&_RecMutex);


	LowLevelDisk::initNode(i_node);//init node details to defult
	LowLevelDisk::addFreeNodeToFreeNodeList(i_node);
	LowLevelDisk::freeInodeBlocks(i_node);//directly and indirectly
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::allocateDataBlock(){
	cout<<" unimplemented function  allocateDataBlock "<<endl;
	pthread_mutex_lock(&_RecMutex);


	int block_id=LowLevelDisk::getFreeBlock();
	LowLevelDisk::initBlock(block_id);
	LowLevelDisk::rmvBlockFromFreeBlock();//remove the Block frome free Block list
	pthread_mutex_unlock(&_RecMutex);
	return block_id;
}


void LowLevelDisk::freeDataBlock(int dblock){
	cout<<" unimplemented function freeDataBlock "<<endl;
	pthread_mutex_lock(&_RecMutex);


	LowLevelDisk::initBlock(dblock);
	LowLevelDisk::addFreeBlockToFreeBlockList(dblock);
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getInodeType(int i_node){
	cout<<" unimplemented function  getInodeType "<<endl;
	pthread_mutex_lock(&_RecMutex);

	pthread_mutex_unlock(&_RecMutex);
	iNode inodeN = _iNodeTable->get(i_node);

	return inodeN.getFileType();
}


void LowLevelDisk::setInodeType(int i_node, int filetype){
	cout<<" unimplemented function setInodeType "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if (filetype<0 | filetype>2 ){
	//	cerr<<"no such file type"<<endl
		pthread_mutex_unlock(&_RecMutex);
	//	throw invalid_argument("no such file type");//TODO: add exception

	}
	if (i_node<0){//TODO::other illegal inodes numbers
		pthread_mutex_unlock(&_RecMutex);
	//	cerr<<"no such i_node"<<endl;
	//	throw invalid_argument("no such i_node");

	}

	_iNodeTable->get(i_node).setFileType(filetype);
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getDataBlock (int i_node, int i){
	cout<<" unimplemented function getDataBlock  "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if (i>=0 | i<=9){
		pthread_mutex_unlock(&_RecMutex);
		//return the i enter in the i_node
		//return -1 if block does not exist

	}
	else{

	}
	pthread_mutex_unlock(&_RecMutex);
	return 1;
}


void LowLevelDisk::setDataBlock (int i_node, int i, int dblockNum ){
	cout<<" unimplemented function setDataBlock "<<endl;
	pthread_mutex_lock(&_RecMutex);


	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::readBlock(int dblockNum, char* buf){
	cout<<" unimplemented function  readBlock"<<endl;
	pthread_mutex_lock(&_RecMutex);
	//init empty buffer if block does not exist

	pthread_mutex_unlock(&_RecMutex);
}



void LowLevelDisk::writeBlock(int dblockNum, char* newdata){
	cout<<" unimplemented function  writeBlock"<<endl;
	pthread_mutex_lock(&_RecMutex);

	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getFileSize(int i_node){
	cout<<" unimplemented function getFileSize "<<endl;
	pthread_mutex_lock(&_RecMutex);


	pthread_mutex_unlock(&_RecMutex);
	return 1;
}


void LowLevelDisk::setFileSize(int i_node, int newSize){
	cout<<" unimplemented function setFileSize "<<endl;
	pthread_mutex_lock(&_RecMutex);


	pthread_mutex_unlock(&_RecMutex);
}
