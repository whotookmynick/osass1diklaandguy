#ifndef SHELL_H_
#define SHELL_H_
/*
 * Shel.h
 *
 *  Created on: Jun 10, 2009
 *      Author: dikla
 */
//TEST


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>


using namespace std;

class Shell {
public:
	int bufferSize_;
	int numberOfNodes_;
	vector<int> nodesVector_;
	//void creatNet( const char file [] );
	void insertArgs(vector <string> argToNet);
//	*vector<string> parse(char *str, char *delimiters);
	int ** makeMatrix(unsigned int numberOfNodes_);
	void  initMatrix(unsigned int numberOfNodes,int **matrix);
	void printMatrix(unsigned int numberOfNodes,int **matrix);
	void Exit(int i);
	void createNet(const char file []) ;
	void killNode() ;
    void reviveNode() ;
    void killAll() ;
	void printRt() ;
	void sendPacket() ;
	void Run() ;

};


#endif /*SHELL_H_*/
