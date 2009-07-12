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
	LOG_DEBUG("find free node");

	if (((BlockList*)_freeInodesList)->empty()) {
		return -1;
	}
	int newInode=((BlockList*)_freeInodesList)->head();

	return newInode;
}

void LowLevelDisk::rmvNodeFromFreeNode(){
	LOG_DEBUG("remove node from freeNode table");
	((BlockList*)_freeInodesList)->pop_front();
}

void LowLevelDisk::initNode(int node_id){
	LOG_DEBUG("init node " << node_id);
	_iNodeTable->get(node_id).setActive(false);
}

void LowLevelDisk::addFreeNodeToFreeNodeList(int i_node){
	LOG_DEBUG("add  Node "<<i_node<<" To Free Node List ");
	((BlockList*)_freeInodesList)->push_back(i_node);
}

void LowLevelDisk::freeInodeBlocks(int i_node){
	_iNodeTable->emptyBlock(i_node);
}

int  LowLevelDisk::getFreeBlock(){
	LOG_DEBUG("get next free Node ");
	if (((BlockList*)_freeBlockesList)->empty()) {
			return -1;
	}
	int newFreeBlock=((BlockList*)_freeBlockesList)->head();

	return newFreeBlock;
}

void LowLevelDisk::initBlock(int block_id){
	LOG_DEBUG("init block "<<block_id);
	_freeBlockesList->emptyBlock(block_id);
}

void LowLevelDisk::rmvBlockFromFreeBlock(){
	LOG_DEBUG("remove Block from free Block table");
	((BlockList*)_freeBlockesList)->pop_front();
}

void LowLevelDisk::addFreeBlockToFreeBlockList(int dblock){
	LOG_DEBUG("add  Block "<<dblock<<" To Free Block List ");
	((BlockList*)_freeBlockesList)->push_back(dblock);
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

/*void LowLevelDisk::informSuperBlock(int offset,int value){
	//write the value in the offset to hard disk
	_superBlock->_numOfFreeInodes =
	writeDataToHardDisk(int fromOffset,const void* buf,int numOfBytes);
	//inform superblock struct

}*/

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

void LowLevelDisk::initFreeInodesList() {
    LOG_DEBUG("init freeInodeList\n");
    _superBlock->firstBlockOfFreeInodesOffset = FIRST_FREE_INODE_BLOCK*_superBlock->blockSize;
   _freeInodesList=new FreeInodeList(_fd,_superBlock->firstBlockOfFreeInodesOffset
    	,_superBlock->firstFreeInode, _superBlock->lastFreeInode,*this);
}

void LowLevelDisk::initFreeBlocksList() {
    LOG_DEBUG("init freeBlocksList\n");
    _superBlock->firstBlockOfFreeBlocksOffset = FIRST_FREE_BLOCK_BLOCK*_superBlock->blockSize;

    _freeBlockesList = new FreeBlockList(_fd,_superBlock->firstBlockOfFreeBlocksOffset
    		,_superBlock->firstEmptyBlock,_superBlock->lastEmptyBlock,*this);
}

void LowLevelDisk::initInodesList() {
    LOG_DEBUG("init inodesList\n");
    _numOfBlocksInInodeTable = ((_superBlock->numOfInodes) / (_superBlock->blockSize))+1;
    _superBlock->inodeTableOffset = INODE_TABLE_BLOCK_NUM*(_superBlock->blockSize);

    _iNodeTable = new InodeList(_fd, _superBlock->inodeTableOffset, *this);
}

void LowLevelDisk::initSuperBlockFromHardDisk(){

	char buf[1];//TODO: check if buf is cahnge
	initSuperBlockFromHardDisk();
	readDataFromHardDisk(NUM_OF_BLOCK_OFFSET,(void*) buf,1);
	_superBlock->numOfBlocks=buf[0];


	readDataFromHardDisk(BLOCK_SIZE_OFFSET,(void*) buf,1);
	_superBlock->blockSize=buf[0];

	readDataFromHardDisk(ROOT_INODE_OFFSET,(void*) buf,1);
	_superBlock->rootInode=buf[0];

	readDataFromHardDisk(NUM_OF_FREE_BLOCK_OFFSET,(void*) buf,1);
	_superBlock->numOfFreeBlocks=buf[0];

	readDataFromHardDisk(FIRST_EMPTY_BLOCK_POINTER_OFFSET,(void*) buf,1);
	_superBlock->firstEmptyBlock=buf[0];

	readDataFromHardDisk(LAST_EMPTY_BLOCK_POINTER_OFFSET,(void*) buf,1);
	_superBlock->lastEmptyBlock=buf[0];

	readDataFromHardDisk(INODE_TABLE_SIZE_OFFSET,(void*) buf,1);
	_superBlock->numOfInodes=buf[0];

	readDataFromHardDisk(NUM_OF_FREE_INODES_OFFSET,(void*) buf,1);
	_superBlock->numOfFreeInodes=buf[0];

	readDataFromHardDisk(FIRST_EMPTY_INODE_POINTER_OFFSET,(void*) buf,1);
	_superBlock->firstFreeInode=buf[0];

	readDataFromHardDisk(LAST_EMPTY_INODE_POINTER_OFFSET,(void*) buf,1);
	_superBlock->lastFreeInode=buf[0];

}

/*
void LowLevelDisk::initVarsFromHardDisk(){
	initSuperBlockFromHardDisk();
	initFreeBlocksList();
	initFreeInodesList();
	initInodesList();
}

void LowLevelDisk::initVarsFromConfig(){
	initFreeBlocksList();
	initFreeInodesList();
	initInodesList();
}
*/

LowLevelDisk::LowLevelDisk(int dataBlockSize,int numberOfInodes,int diskSize):_iNodeTable()
{

	if (existsFileSystem()){
			openFileSystem();
			initSuperBlockFromHardDisk();
			initFreeBlocksList();
			initFreeInodesList();
			initInodesList();
	}
	else{
			createFileSystem();
			initFreeBlocksList();
			initFreeInodesList();
			initInodesList();
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
	return _superBlock->numOfBlocks;
}

int LowLevelDisk::getNumOfInodes(){
	return _superBlock->numOfInodes;
}


void LowLevelDisk::setNumOfFreeBlocks(int i){
	_superBlock->numOfBlocks=i;
}

void LowLevelDisk::setNumOfFreeInodes(int i){
	_superBlock->numOfInodes=i;
}

int LowLevelDisk::getDataBlockSize(){
	return _superBlock->blockSize;
}

//---------------------------------------------------------------------------/
//								Low Level functions
//---------------------------------------------------------------------------/


//TODO: inform the super block
//TODO: Log msg and exception
int  LowLevelDisk::allocateInode(){
	cout<<"  allocateInode - need to finish implement lists "<<endl;

	pthread_mutex_lock(&_RecMutex);
	LOG_DEBUG("allocateInodet\n");
	int node_id = findFreeNode();
	rmvNodeFromFreeNode();//remove the node frome free node list
	initNode(node_id);//init node details to defult
    pthread_mutex_unlock(&_RecMutex);
    return node_id;
}


void LowLevelDisk::freeInode(int i_node){
	cout<<" freeInode-  need to finish implement lists "<<endl;

	LOG_DEBUG("freeInode"<<i_node<<"\n");
	pthread_mutex_lock(&_RecMutex);
	 if ((i_node<0) | (i_node>=(_superBlock->numOfInodes))){
		 //pthread_mutex_unlock(&_RecMutex);
//		 return -1;
	 }
	 else if ((_iNodeTable->get(i_node)).getActive())
	 {
		 initNode(i_node);//init node details to defult
		 addFreeNodeToFreeNodeList(i_node);
		 freeInodeBlocks(i_node);//directly and indirectly
	 }
	 else{
		 //the inode is allready free
	 }
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::allocateDataBlock(){
	cout<<"  allocateDataBlock -  need to finish implement lists "<<endl;

	pthread_mutex_lock(&_RecMutex);
	int block_id=getFreeBlock();
	//initBlock(block_id);
	rmvBlockFromFreeBlock();//remove the Block frome free Block list
	pthread_mutex_unlock(&_RecMutex);
	return block_id;
}


void LowLevelDisk::freeDataBlock(int dblock){
	cout<<"freeDataBlock -  need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if((dblock<(_superBlock->numOfBlocks)) | (dblock<0)){
		//
	}else{
		initBlock(dblock);
		addFreeBlockToFreeBlockList(dblock);
	}
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getInodeType(int i_node){
	cout<<" getInodeType -  need to finish implement lists "<<endl;
	int type=-1;
	pthread_mutex_lock(&_RecMutex);
	 if ((i_node<0) | (i_node>=(_superBlock->numOfInodes))){
		 //
	 }
	 else{
		 type =_iNodeTable->get(i_node).getFileType();;
	 }
	return type;
	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::setInodeType(int i_node, int filetype){
	cout<<"setInodeType-  need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((filetype<0) | (filetype>2 ) | (i_node<0) |(i_node>=(_superBlock->numOfInodes)) ){
	//cerr<<"no such file type"<<endl\
	//pthread_mutex_unlock(&_RecMutex);
	//	throw invalid_argument("no such file type");//TODO: add exception
	}
	else{
		_iNodeTable->get(i_node).setFileType(filetype);
	}
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getDataBlock (int i_node, int dblock){
	cout<<" getDataBlock--  need to finish implement lists  "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((dblock>=0) | (dblock<=9) |  (i_node<0) |(i_node>=(_superBlock->numOfInodes)) ){
		pthread_mutex_unlock(&_RecMutex);
		//return the i enter in the i_node
		return -1 ;
	}
	else{

        return _iNodeTable->get(i_node).getNumOfDataBlock(dblock);
        //TODO: if not allocated yet??? allocate and delete the block in the file?????
	}
	pthread_mutex_unlock(&_RecMutex);

}


void LowLevelDisk::setDataBlock (int i_node, int i, int dblockNum ){
	cout<<"setDataBlock - need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((dblockNum<0) | (i_node<0) |(i_node>=(_superBlock->numOfInodes))|(!(_iNodeTable->get(i_node).getActive()) )){
		//TODO
	}
	_iNodeTable->get(i_node).setNumOfDataBlock(dblockNum,i);
	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::readBlock(int dblockNum, char* buf){
	cout<<" readBlock  - need to finish implement lists"<<endl;
	pthread_mutex_lock(&_RecMutex);
	//init empty buffer if block does not exist
	if ((dblockNum<0) | (dblockNum<=_superBlock->numOfBlocks)) {
			//TODO
	}
	int fromOffset = dblockNum*(_superBlock->blockSize);
	readDataFromHardDisk(fromOffset,buf,_superBlock->blockSize);
	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::writeBlock(int dblockNum, char* newdata){
	cout<<"writeBlock - need to finish implement lists"<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((dblockNum<0) | (dblockNum<=_superBlock->numOfBlocks)) {
		//TODO
	}
	int fromOffset = dblockNum*(_superBlock->blockSize);
	writeDataToHardDisk(fromOffset,newdata,_superBlock->blockSize);
	pthread_mutex_unlock(&_RecMutex);
}

int LowLevelDisk::getFileSize(int i_node){
	cout<<"getFileSize - need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((i_node<0) | (i_node>=(_superBlock->numOfInodes))){
		pthread_mutex_unlock(&_RecMutex);
		return -1;
	}
	return _iNodeTable->get(i_node).getFileSize();
	pthread_mutex_unlock(&_RecMutex);
}

void LowLevelDisk::setFileSize(int i_node, int newSize){
	cout<<" setFileSize - need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((i_node<0) | (i_node>=(_superBlock->numOfInodes))|(newSize<0)){
		//TODO		pthread_mutex_unlock(&_RecMutex);
		//return -1;
	}
	_iNodeTable->get(i_node).setFileSize(newSize);
	pthread_mutex_unlock(&_RecMutex);
}
