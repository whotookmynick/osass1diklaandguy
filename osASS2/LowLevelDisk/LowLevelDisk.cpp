#include "LowLevelDisk.h"

//#include <stdexpt>

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

LowLevelDisk::LowLevelDisk(const std::string& file)
{
	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_RecMutex, &_mtxattr);


}

LowLevelDisk::LowLevelDisk()
{
	pthread_mutexattr_init(&_mtxattr);
	pthread_mutexattr_settype(&_mtxattr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&_RecMutex, &_mtxattr);

}

LowLevelDisk::~LowLevelDisk()
{
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

      /*  synchronized(&_lock) {
                if (_freeInodes->empty()) {
                        LOG_WARNING("no more free inodes\n");
                        _lock.unlock();
                        throw OutOfInodesException();
                }

                inodeNum = _freeInodes->front();
                _freeInodes->pop_front();
        }*/

        //return inodeNum;
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
	return _iNodeTable[i_node]->getFileType();
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

	_iNodeTable[i_node]->setFileType(filetype);
	pthread_mutex_unlock(&_RecMutex);
}


int LowLevelDisk::getDataBlock (int i_node, int i){
	pthread_mutex_lock(&_RecMutex);
	if (i>=0 | i<=9){
		pthread_mutex_unlock(&_RecMutex);
		//return the i enter in the i_node
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
