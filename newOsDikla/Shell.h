#ifndef SHELL_H_
#define SHELL_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
using namespace std;

class Mailer;

class Shell {
public:
	Shell();
	~Shell();
	Mailer* _mailer;
	int _bufferSize;
	int _numberOfNodes;
	vector<int> _nodesVector;
	//void creatNet( const char file [] );
	void insertArgs(vector <string> argToNet);
//	*vector<string> parse(char *str, char *delimiters);
	int ** makeMatrix(unsigned int numberOfNodes_);
	void  initMatrix(unsigned int numberOfNodes,int **matrix);
	void printMatrix(unsigned int numberOfNodes,int **matrix);
	void Exit(int i);
	void createNet(const char file []) ;
	void killNode(int id) ;
    void reviveNode() ;
    void killAll() ;
	void printRt() ;
	void sendPacket() ;
	void Run() ;
	void start();
	void printMsgFromMailer(string msg);

};
#endif /*SHELL_H_*/
