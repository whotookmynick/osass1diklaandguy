#include "LowLevelDisk.h"
#include <sys/stat.h>
#include <sstream>
#include <math.h>
#include <fcntl.h>
//---------------------------------------------------------------------------/
//								TODO
//---------------------------------------------------------------------------/

//---------------------------------------------------------------------------/
//								Help function
//---------------------------------------------------------------------------/
int LowLevelDisk::getNumOfBlocksInFreeInodeList(){
	int numOfInodes=_superBlock->numOfFreeInodes;
	int blockSize=_superBlock->blockSize;
	int numOfBlockInFreeInodeList;
	if(numOfInodes%blockSize>0){
		numOfBlockInFreeInodeList =numOfInodes/blockSize+1;
   }else{
	numOfBlockInFreeInodeList = numOfInodes/blockSize;
   }//end else
	return numOfBlockInFreeInodeList;
 }

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
	_iNodeTable->setActive(node_id,false);
	//get(node_id).setActive(false);
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

void LowLevelDisk::createFileSystem(){
	_fd = open(SYS_FILE_NAME.c_str(), O_RDWR | O_CREAT, (mode_t)0600);
	//cannot create
	//initSuperBlock();

}


void LowLevelDisk::openFileSystem(){
	_fd = open(SYS_FILE_NAME.c_str(),O_RDWR);
	  if (_fd==-1){
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
void LowLevelDisk::initSuperBlock(int dataBlockSize,int numberOfInodes,int diskSize){

		_superBlock->numOfBlocks=diskSize/dataBlockSize;//TODO=round up? down??
		writeDataToHardDisk(NUM_OF_BLOCK_OFFSET,(void*)&_superBlock->numOfBlocks,OFFSET_SIZE_IN_BYTES);


		_superBlock->blockSize=dataBlockSize;//offset 1 in file
		writeDataToHardDisk(BLOCK_SIZE_OFFSET,(void*)&_superBlock->blockSize,OFFSET_SIZE_IN_BYTES);

		_superBlock->rootInode=ROOT_INODE;//offset 2 in file
			writeDataToHardDisk(INODE_TABLE_BLOCK_NUM*_superBlock->blockSize,(void*)&_superBlock->rootInode,OFFSET_SIZE_IN_BYTES);


		//	int numOfFreeBlocks;//offset 3 in file

		//in offset 4 in file - in freeblock init
		_superBlock->firstEmptyBlock=FIRST_FREE_BLOCK_BLOCK*_superBlock->blockSize;
		writeDataToHardDisk(FIRST_EMPTY_BLOCK_POINTER_OFFSET,(void*)&_superBlock->firstEmptyBlock,OFFSET_SIZE_IN_BYTES);

		// lastEmptyBlock;//in offset 5  in freeblock init

		_superBlock->numOfInodes=numberOfInodes;//in offset 6
		writeDataToHardDisk(INODE_TABLE_SIZE_OFFSET,(void*)&_superBlock->numOfInodes,OFFSET_SIZE_IN_BYTES);

		_superBlock->numOfFreeInodes=numberOfInodes;//in offset 7
		writeDataToHardDisk(NUM_OF_FREE_INODES_OFFSET,(void*)&_superBlock->numOfFreeInodes,OFFSET_SIZE_IN_BYTES);


		//offset 8
		_superBlock->firstFreeInode=FIRST_FREE_INODE_BLOCK*_superBlock->blockSize;
		writeDataToHardDisk(FIRST_EMPTY_INODE_POINTER_OFFSET,(void*)&_superBlock->firstFreeInode,OFFSET_SIZE_IN_BYTES);

		_superBlock->firstFreeBlockNumber = getNumOfBlocksInInodeTable()+INODE_TABLE_BLOCK_NUM-1;
		//int lastFreeInode;//offset 9  - in freeinode init

		//	int firstBlockOfFreeInodesOffset;-in freeinode init

		//	int firstBlockOfFreeBlocksOffset;in freeblock init

		// TODO	int inodeTableOffset;
		// TODO	int firstFreeBlockNumber;
}

void LowLevelDisk::initFreeInodesList() {
    LOG_DEBUG("init freeInodeList\n");
    _superBlock->firstBlockOfFreeInodesOffset = FIRST_FREE_INODE_BLOCK*_superBlock->blockSize;

    int numOfFreeBlocks = (_superBlock->numOfBlocks) - SIZE_OF_SUPER_BLOCK
           		-getNumOfBlocksInInodeTable();

    int offset=_superBlock->firstBlockOfFreeInodesOffset;
    int nextBlockToWriteFreeInodes= _superBlock->numOfBlocks-numOfFreeBlocks;//the end of InodeTable

    int inodeIndex=0;
    int numOfinodeTowrite = _superBlock->numOfInodes;

    //int nextOffset=	firstAvailbleBlockOffset
    for(int j=0;j<_superBlock->numOfFreeInodes;j++){

       	if((j==(_superBlock->blockSize-1))and(numOfinodeTowrite>1)){//if we get the one befor last offset in block and there is more then one freeBlock to add
       		//insert the next blockNumber of free inodes to the end of this block
   			writeDataToHardDisk(offset,(void*)&nextBlockToWriteFreeInodes,OFFSET_SIZE_IN_BYTES);
   			//jump to next block and fill it
   		//	offset=nextFreeBlock*blockSize;
   			offset=nextBlockToWriteFreeInodes*_superBlock->blockSize;//set next place to write list
   			writeDataToHardDisk(offset,(void*)&inodeIndex,OFFSET_SIZE_IN_BYTES);
   			numOfinodeTowrite--;
   		}//end if
       	else{
       		writeDataToHardDisk(offset,(void*)&inodeIndex,OFFSET_SIZE_IN_BYTES);
       	}
       	inodeIndex++;
       	offset=offset+OFFSET_SIZE_IN_BYTES;

       }//end for
    _superBlock->lastFreeInode=offset;
    writeDataToHardDisk(LAST_EMPTY_INODE_POINTER_OFFSET,(void*)&offset,OFFSET_SIZE_IN_BYTES);
   _freeInodesList=new FreeInodeList(_fd,_superBlock->firstBlockOfFreeInodesOffset
    	,_superBlock->firstFreeInode, _superBlock->lastFreeInode,*this);
}

void LowLevelDisk::initFreeBlocksList() {
	LOG_DEBUG("init freeBlocksList\n");
    _superBlock->firstBlockOfFreeBlocksOffset = FIRST_FREE_BLOCK_BLOCK*_superBlock->blockSize;
    //TODO init super block var in offset 4 in file - in freeblock init
    int blockSize = _superBlock->blockSize;
    //the first free block is after the inode table


    //start fill the first Block in list
    int offset = (_superBlock->firstFreeBlockNumber)*(_superBlock->blockSize);
    writeDataToHardDisk(offset, (void*)&_superBlock->firstFreeBlockNumber,OFFSET_SIZE_IN_BYTES);

    int numOfFreeBlocks = (_superBlock->numOfBlocks) - SIZE_OF_SUPER_BLOCK
    		-getNumOfBlocksInInodeTable()-getNumOfBlocksInFreeInodeList();

    _superBlock->firstBlockOfFreeBlocksOffset = ((_superBlock->numOfBlocks) - numOfFreeBlocks)*blockSize;

    int firstEmptyBlockNumber =(_superBlock->numOfBlocks) - numOfFreeBlocks;

    //start fill the first block in the freeBlockList

    int nextFreeBlock = firstEmptyBlockNumber;
    int freeOffset = FIRST_FREE_BLOCK_BLOCK*blockSize;
    writeDataToHardDisk(freeOffset,(void*)&nextFreeBlock,OFFSET_SIZE_IN_BYTES);
    int i=0;
    int numOfBlockInFreeBlockList=numOfFreeBlocks;
    int numOfFreeBlocksToWrite=numOfFreeBlocks;
    //calac how much blocks

    while(numOfFreeBlocksToWrite>0){//(blockSize-i-1)//while there are blocks to store in freeBlockList
    	if((i==blockSize-1)and(numOfBlockInFreeBlockList>1)){//if we get the one befor last offset in block and there is more then one freeBlock to add
			//insert the next block of free blocks
			writeDataToHardDisk(freeOffset,(void*)&nextFreeBlock,OFFSET_SIZE_IN_BYTES);
			//jump to next block and fill it
			freeOffset=nextFreeBlock*blockSize;
			numOfFreeBlocks= numOfFreeBlocks-1;
			i=-1;//go to begining of the new block in list
		}//end if
    	i++;
    	numOfFreeBlocksToWrite--;
		freeOffset=freeOffset+OFFSET_SIZE_IN_BYTES;
		nextFreeBlock=nextFreeBlock+1;
		writeDataToHardDisk(freeOffset,(void*)&nextFreeBlock,OFFSET_SIZE_IN_BYTES);
    }//end while

    _superBlock->numOfFreeBlocks=numOfFreeBlocks;
    writeDataToHardDisk(NUM_OF_FREE_BLOCK_OFFSET,(void*)&(_superBlock->numOfFreeBlocks),OFFSET_SIZE_IN_BYTES);
    _superBlock->lastEmptyBlock=numOfFreeBlocks-1;
    writeDataToHardDisk(LAST_EMPTY_BLOCK_POINTER_OFFSET,(void*)&(_superBlock->lastEmptyBlock),OFFSET_SIZE_IN_BYTES);

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

//	char buf[1];//TODO: check if buf is cahnge
	int buf;
	readDataFromHardDisk(NUM_OF_BLOCK_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->numOfBlocks=buf;


	readDataFromHardDisk(BLOCK_SIZE_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->blockSize=buf;

	readDataFromHardDisk(ROOT_INODE_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->rootInode=buf;

	readDataFromHardDisk(NUM_OF_FREE_BLOCK_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->numOfFreeBlocks=buf;

	readDataFromHardDisk(FIRST_EMPTY_BLOCK_POINTER_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->firstEmptyBlock=buf;

	readDataFromHardDisk(LAST_EMPTY_BLOCK_POINTER_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->lastEmptyBlock=buf;

	readDataFromHardDisk(INODE_TABLE_SIZE_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->numOfInodes=buf;

	readDataFromHardDisk(NUM_OF_FREE_INODES_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->numOfFreeInodes=buf;

	readDataFromHardDisk(FIRST_EMPTY_INODE_POINTER_OFFSET,(void*) buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->firstFreeInode=buf;

	readDataFromHardDisk(LAST_EMPTY_INODE_POINTER_OFFSET,(void*)& buf,OFFSET_SIZE_IN_BYTES);
	_superBlock->lastFreeInode=buf;

	_superBlock->firstFreeBlockNumber = getNumOfBlocksInInodeTable()+INODE_TABLE_BLOCK_NUM-1;

}

int LowLevelDisk::getNumOfBlocksInInodeTable(){
	//calac number of block in inodetable:
	int tableSizeInBytes = sizeof(InodeStruct)*(_superBlock->numOfInodes);
	int numOfBlocks;
	if((tableSizeInBytes % _superBlock->blockSize) !=0 ){
		numOfBlocks = (tableSizeInBytes/_superBlock->blockSize)+1;
	}else{
		numOfBlocks = tableSizeInBytes/_superBlock->blockSize;
	}
	return numOfBlocks;
}

LowLevelDisk::LowLevelDisk(int dataBlockSize,int numberOfInodes,int diskSize):_iNodeTable()
{
	_superBlock=(superBlock*)malloc(sizeof(superBlock));
	_iNodeTable=(InodeList*)malloc(sizeof(InodeList));
	if (existsFileSystem()){
			openFileSystem();
			initSuperBlockFromHardDisk();
			initFreeBlocksList();
			initFreeInodesList();
			initInodesList();
	}
	else{
			createFileSystem();
			initSuperBlock(dataBlockSize,numberOfInodes,diskSize);
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
int LowLevelDisk::getHardLink(int i_node){
	int offset = (INODE_TABLE_BLOCK_NUM*_superBlock->blockSize)+(SIZE_OF_INODE*i_node)+HARD_LINK_OFFSET;
	int ans;
	readDataFromHardDisk(offset,(void*)&ans,OFFSET_SIZE_IN_BYTES);
	return ans;
}

void LowLevelDisk::setHardLink(int i_node,int val){
	int offset = (INODE_TABLE_BLOCK_NUM*_superBlock->blockSize)+(SIZE_OF_INODE*i_node)+HARD_LINK_OFFSET;
	writeDataToHardDisk(offset,(void*)&val,OFFSET_SIZE_IN_BYTES);
}


superBlock* LowLevelDisk::getSuperBlock(){
	return _superBlock;
}


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
	 else if (_iNodeTable->getActive(i_node))
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
		 type =_iNodeTable->getFileType(i_node);;
	 }
	return type;
	pthread_mutex_unlock(&_RecMutex);
}


void LowLevelDisk::setInodeType(int i_node, int filetype){
	cout<<"setInodeType-  need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((filetype<0) | (filetype>2 ) | (i_node<0) |(i_node>=(_superBlock->numOfInodes)) ){
	//pthread_mutex_unlock(&_RecMutex);
	//	throw invalid_argument("no such file type");//TODO: add exception
	}
	else{
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

        return _iNodeTable->getDataBlockNum(i_node,dblock);
        //getNumOfDataBlock(i_node,dblock);
        //TODO: if not allocated yet??? allocate and delete the block in the file?????
	}
	pthread_mutex_unlock(&_RecMutex);

}


void LowLevelDisk::setDataBlock (int i_node, int i, int dblockNum ){
	cout<<"setDataBlock - need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((dblockNum<0) | (i_node<0) |(i_node>=(_superBlock->numOfInodes))|(!(_iNodeTable->getActive(i_node)) )){
		//TODO
	}
	_iNodeTable->setNumOfDataBlock(i_node,dblockNum,i);
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
	return _iNodeTable->getFileSize(i_node);
	pthread_mutex_unlock(&_RecMutex);
}

void LowLevelDisk::setFileSize(int i_node, int newSize){
	cout<<" setFileSize - need to finish implement lists "<<endl;
	pthread_mutex_lock(&_RecMutex);
	if ((i_node<0) | (i_node>=(_superBlock->numOfInodes))|(newSize<0)){
		//TODO		pthread_mutex_unlock(&_RecMutex);
		//return -1;
	}
	_iNodeTable->setFileSize(i_node,newSize);
	pthread_mutex_unlock(&_RecMutex);
}

