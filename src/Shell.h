/*
 * Shel.h
 *
 *  Created on: Jun 10, 2009
 *      Author: dikla
 */

#ifndef SHELL_H_
#define SHELL_H_
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

class Shell {
public:
	int bufferSize_;
	int numberOfNodes_;
	vector<int> nodesVector_;
	void creatNet( char file [] );
	void insertArgs(vector <string> argToNet);
//	*vector<string> parse(char *str, char *delimiters);

};


#endif /*SHELL_H_*/
