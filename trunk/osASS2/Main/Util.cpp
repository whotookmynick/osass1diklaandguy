/*
 * Util.cpp
 *
 *  Created on: Jul 15, 2009
 *      Author: aradno
 */
#ifndef UTIL_CPP_
#define UTIL_CPP_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <pthread.h>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <queue>

using namespace std;

const int REGULARE_FILE = 0;
const int SOFT_LINK = 1;
const int DIR_TYPE = 3;

const int READ_ONLY = 1;
const int READ_AND_WRITE = 2;

	static int turnBytesToInt(char* bytes)
	{
//		int Int32 = 0;
//		Int32 = (Int32 << 8) + bytes[3];
//		Int32 = (Int32 << 8) + bytes[2];
//		Int32 = (Int32 << 8) + bytes[1];
//		Int32 = (Int32 << 8) + bytes[0];
		int Int32 = 0;
		Int32 = (Int32 << 8) + bytes[0];
		Int32 = (Int32 << 8) + bytes[1];
		Int32 = (Int32 << 8) + bytes[2];
		Int32 = (Int32 << 8) + bytes[3];
		return Int32;
	}


	static void intToByte(int val, char *bytes )
	{
	bytes[3] = (char)val;
	bytes[2] = (char)(val >> 8);
	bytes[1] = (char)(val >> 16);
	bytes[0] = (char)(val >> 24);
	}

	static void printBuffer(char* buff,int size)
	{
		for (int i = 0; i < size;i++)
			{
	//			cout<<buff[i];
				char temp = buff[i];
				printf("%c",buff[i]);
			}
		cout<<endl;
	}

	static void myStrCopy(char* target,char* source)
	{
		int i = 0;
		while (source[i] != '\0')
		{
			target[i] = source[i];
			i++;
		}
	}

	static int roundUpNumber(float num)
	{
		int ans = num;
		if (num == ans)
			return ans;
		num = num + 1;
		ans = num;
		return ans;
	}

#endif /*LOWLEVELDISK_H_*/
