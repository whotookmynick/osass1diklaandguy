#include "LowLevelDisk.h"
#include <sys/stat.h>
#include <sstream>
#include <math.h>
#include <fcntl.h>



//#include <stdexpt>

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




void LowLevelDisk::initFreeInodesList() {
    LOG_DEBUG("init freeInodeList\n");
    //set firstBlockOffset = NumOfFreeBlocks * blockSize;
    //set offset = OFFSET + ((_NumOfFreeInodes)- the first discreptors of disk  );
    //set freeInodesHead = OFFSET + ( firstBlockInFreeInodes ??? - the first discreptors of disk );
    //set freeInodesTail = OFFSET + ( lastBlockInFreeInodes???? -_diskDesc );
    //_freeInodesList = new FreeInodesList(_fd, sizeOffset, headOffset, tailOffset,firstBlockOffset,*this);
}

void LowLevelDisk::initInodesList() {
    LOG_DEBUG("init inodesList\n");
    //offset = NUM_OF_BLOCKS_IN_INODE_LIST * blockSize;
    //_inodes = new InodesList(_fd, offset, *this);
}

void LowLevelDisk::initFreeBlocksList() {
    LOG_DEBUG("init freeBlocksList\n");
    //set firstBlockOffset = freeBlocksBlockNum *blockSize;
    //set sizeOffset = OFFSET + freeBlocksCount - the first discreptors of disk );
	 //set headOffset = OFFSET + (freeBlocksHead - the first discreptors of disk);
	//set tailOffset = OFFSET + ( freeBlocksTail - the first discreptors of disk );
    //_freeBlocks = new FreeBlocksList(_fd, sizeOffset, headOffset, tailOffset,firstBlockOffset,*this);
}


void LowLevelDisk::initVars(){
		// DiskDescriptor();
	    //initFreeBlocksList();
	    //initFreeInodesList();
	    //initInodesList();
}


void LowLevelDisk::openFileSystem(){

	_fd = open(SYS_FILE_NAME.c_str(),O_RDWR);
	  if (_fd!=-1){
		  throw OpenFileExcemption(SYS_FILE_NAME);
	  }

	  initVars();


}

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------
/*LowLevelServices::LowLevelServices(const string& filename, Configuration& conf)
: _lock(), _freeBlocks(), _freeInodes(), _inodes()
{
	if (!fileSystemExists(filename)) {
			LOG_INFO("file system at ["<<filename<<"] does not exist. creating\n");
			createNewFileSystem(filename,conf);
	} else {
			LOG_INFO("file system at ["<<filename<<"] exists. opening\n");
			openExistingFileSystem(filename);
	}

	LOG_INFO("disk initialized.\n");
}
*/






LowLevelDisk::LowLevelDisk(int dataBlockSize,int numberOfInodes,int diskSize):_iNodeTable(),_freeInodesList(),_freeBlockesList()
{
	if (existsFileSystem()){
			openFileSystem();
	}
	else{
			createFileSystem();
	}
	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_RecMutex, &_mtxattr);


}


LowLevelDisk::~LowLevelDisk()
{
	delete _freeInodesList;
	delete _freeBlockesList;
	pthread_mutex_destroy(&_RecMutex);
	pthread_mutexattr_destroy(&_mtxattr);


}
//---------------------------------------------------------------------------/
//								Getters and setters
//---------------------------------------------------------------------------/
int LowLevelDisk::getNumOfBlocks(){
	return _numOfBlocks;
}

int LowLevelDisk::getNumOfInodes(){
	return _numOfInodes;
}


void LowLevelDisk::setNumOfFreeBlocks(int i){
	_numOfBlocks=i;
}

void LowLevelDisk::setNumOfFreeInodes(int i){
	_numOfInodes=i;
}

int LowLevelDisk::getDataBlockSize(){
	return _blockSize;
}
//---------------------------------------------------------------------------/
//								Help function
//---------------------------------------------------------------------------/

int  LowLevelDisk::findFreeNode(){

	LOG_DEBUG("find free node");
	if (_freeInodesList->empty()) {
		throw EmptyListException("FreeInode");
	}
	_freeInodesList->head();
	_freeInodesList->pop_front();
	//TODO create new inode
	//_iNodeTable->get(inodeNum).setActive(false);

	return 1;
}

void LowLevelDisk::rmvNodeFromFreeNode(){
	// LOG_DEBUG("remove node from freeNode table");
}

void LowLevelDisk::initNode(int node_id){
	//LOG_DEBUG("init node " << node_id);

}

void LowLevelDisk::addFreeNodeToFreeNodeList(int i_node){
	//LOG_DEBUG("add  Node "<<i_node<<" To Free Node List ");
}

void LowLevelDisk::freeInodeBlocks(int i_node){
	//LOG_DEBUG("free i Node "<<i_node<<" blocks ");
}

int  LowLevelDisk::getFreeBlock(){
	//LOG_DEBUG("get next free Node ");
	return 1;
}

void LowLevelDisk::initBlock(int block_id){
	//LOG_DEBUG("init block "<<block_id);
}

void LowLevelDisk::rmvBlockFromFreeBlock(){
	//LOG_DEBUG("remove Block from free Block table");
}

void LowLevelDisk::addFreeBlockToFreeBlockList(int dblock){
	//LOG_DEBUG("add  Block "<<dblock<<" To Free Block List ");
}

//---------------------------------------------------------------------------/
//								Low Level functions
//---------------------------------------------------------------------------/


//TODO: inform the super block
//TODO: Log msg and exception
int  LowLevelDisk::allocateInode(){
	pthread_mutex_lock(&_RecMutex);

	int node_id = LowLevelDisk::findFreeNode();

	LowLevelDisk::rmvNodeFromFreeNode();//remove the node frome free node list
	LowLevelDisk::initNode(node_id);//init node details to defult




    int inodeNum;
    pthread_mutex_unlock(&_RecMutex);
    return node_id;
}


void LowLevelDisk::freeInode(int i_node){
	pthread_mutex_lock(&_RecMutex);


	LowLevelDisk::initNode(i_node);//init node details to defult
	LowLevelDisk::addFreeNodeToFreeNodeList(i_node);
	LowLevelDisk::freeInodeBlocks(i_node);//directly and indirectly
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::allocateDataBlock(){
	pthread_mutex_lock(&_RecMutex);


	int block_id=LowLevelDisk::getFreeBlock();
	LowLevelDisk::initBlock(block_id);
	LowLevelDisk::rmvBlockFromFreeBlock();//remove the Block frome free Block list
	pthread_mutex_unlock(&_RecMutex);
	return block_id;
}


void LowLevelDisk::freeDataBlock(int dblock){
	pthread_mutex_lock(&_RecMutex);


	LowLevelDisk::initBlock(dblock);
	LowLevelDisk::addFreeBlockToFreeBlockList(dblock);
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getInodeType(int i_node){
	pthread_mutex_lock(&_RecMutex);

	pthread_mutex_unlock(&_RecMutex);
	iNode inodeN = _iNodeTable->get(i_node);

	return inodeN.getFileType();
}


void LowLevelDisk::setInodeType(int i_node, int filetype){
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
	pthread_mutex_lock(&_RecMutex);


	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::readBlock(int dblockNum, char* buf){
	pthread_mutex_lock(&_RecMutex);
	//init empty buffer if block does not exist

	pthread_mutex_unlock(&_RecMutex);
}



void LowLevelDisk::writeBlock(int dblockNum, char* newdata){
	pthread_mutex_lock(&_RecMutex);

	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getFileSize(int i_node){
	pthread_mutex_lock(&_RecMutex);


	pthread_mutex_unlock(&_RecMutex);
	return 1;
}


void LowLevelDisk::setFileSize(int i_node, int newSize){
	pthread_mutex_lock(&_RecMutex);


	pthread_mutex_unlock(&_RecMutex);
}
