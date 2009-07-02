#include "LowLevelDisk.h"


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
//								Help function
//---------------------------------------------------------------------------/


//---------------------------------------------------------------------------/
//								Low Level functions
//---------------------------------------------------------------------------/



int  LowLevelDisk::allocateInode(){
	return 1;
}



void LowLevelDisk::freeInode(int i_node){

}


void LowLevelDisk::allocateDataBlock(){}


void LowLevelDisk::freeDataBlock(int dblock){}


int LowLevelDisk::getInodeType(int i_node){
	return NULL;
}


void LowLevelDisk::setInodeType(int i_node, int filetype){}


int LowLevelDisk::getDataBlock (int i_node, int i){
	return 1;
}


void LowLevelDisk::setDataBlock (int i_node, int i, int dblockNum ){}


void LowLevelDisk::readBlock(int dblockNum, char* buf){}



void LowLevelDisk::writeBlock(int dblockNum, char* newdata){}


int LowLevelDisk::getFileSize(int i_node){
	return 1;
}


void LowLevelDisk::setFileSize(int i_node, int newSize){}
