/*
 * Util.cpp
 *
 *  Created on: Jul 15, 2009
 *      Author: aradno
 */
#ifndef UTIL_CPP_
#define UTIL_CPP_

	static int turnBytesToInt(char* bytes)
	{
		int Int32 = 0;
		Int32 = (Int32 << 8) + bytes[3];
		Int32 = (Int32 << 8) + bytes[2];
		Int32 = (Int32 << 8) + bytes[1];
		Int32 = (Int32 << 8) + bytes[0];
		return Int32;
	}


	static void intToByte(int val, char *bytes )
	{
	bytes[0] = (char)val;
	bytes[1] = (char)(val >> 8);
	bytes[2] = (char)(val >> 16);
	bytes[3] = (char)(val >> 24);
	}



#endif /*LOWLEVELDISK_H_*/
