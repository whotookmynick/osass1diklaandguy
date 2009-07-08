#include "LowLevelDisk.h"
#include  "Logger.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "BlockList.h"
using namespace std;

//#include "FileSystem/iNode.h"
//#include <stdexpt>

//---------------------------------------------------------------------------------
//							constarctors and distractor and inits
//--------------------------------- -----------------------------------------------

LowLevelDisk::LowLevelDisk()
{

}

LowLevelDisk::~LowLevelDisk()
{
}
//---------------------------------------------------------------------------/
//								Getters and setters
//---------------------------------------------------------------------------/
int LowLevelDisk::getNumOfBlocks(){

}
int LowLevelDisk::getNumOfInodes(){}
void LowLevelDisk::setNumOfBlocks(){}
void LowLevelDisk::setNumOfInodes(){}


//---------------------------------------------------------------------------/
//								Help function
//---------------------------------------------------------------------------/
int  LowLevelDisk::findFreeNode(){
	LOG_DEBUG("find free node");

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
	int node_id = LowLevelDisk::findFreeNode();
	LowLevelDisk::rmvNodeFromFreeNode();//remove the node frome free node list
	LowLevelDisk::initNode(node_id);//init node details to defult
	return node_id;
}


void LowLevelDisk::freeInode(int i_node){
	LowLevelDisk::initNode(i_node);//init node details to defult
	LowLevelDisk::addFreeNodeToFreeNodeList(i_node);
	LowLevelDisk::freeInodeBlocks(i_node);//directly and indirectly

}


int LowLevelDisk::allocateDataBlock(){
	int block_id=LowLevelDisk::getFreeBlock();
	LowLevelDisk::initBlock(block_id);
	LowLevelDisk::rmvBlockFromFreeBlock();//remove the Block frome free Block list
	return block_id;
}


void LowLevelDisk::freeDataBlock(int dblock){
	LowLevelDisk::initBlock(dblock);
	LowLevelDisk::addFreeBlockToFreeBlockList(dblock);
}


int LowLevelDisk::getInodeType(int i_node){
	return _iNodeTable[i_node]->getFileType();
}


void LowLevelDisk::setInodeType(int i_node, int filetype){
	if (filetype<0 | filetype>2 ){
	//	cerr<<"no such file type"<<endl;
	//	throw invalid_argument("no such file type");//TODO: add exception

	}
	if (i_node<0){//TODO::other illegal inodes numbers
	//	cerr<<"no such i_node"<<endl;
	//	throw invalid_argument("no such i_node");

	}

	_iNodeTable[i_node]->setFileType(filetype);

}


int LowLevelDisk::getDataBlock (int i_node, int i){

	if (i>=0 | i<=9){
		//return the i enter in the i_node
	}
	else{

	}
	return 1;
}


void LowLevelDisk::setDataBlock (int i_node, int i, int dblockNum ){}


void LowLevelDisk::readBlock(int dblockNum, char* buf){}



void LowLevelDisk::writeBlock(int dblockNum, char* newdata){}


int LowLevelDisk::getFileSize(int i_node){
	return 1;
}


void LowLevelDisk::setFileSize(int i_node, int newSize){}
