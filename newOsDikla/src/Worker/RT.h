/*
 * RT.h
 *
 *  Created on: Jun 26, 2009
 *      Author: dikla
 */

#ifndef RT_H_
#define RT_H_
#include <vector>
#include <map>
#include <utility>
#include <iostream>
using namespace std;

class RT {
	public:
		RT();
		virtual ~RT();

		typedef struct{
					int target;
					int distance;
					int next;
		}rtStruct;

		vector<rtStruct*> _rtTable;
		int _owner;


		RT(int ownerId);
		void Reset();
		void printRT();
		bool update(RT* neighborRT);
		int getID();
};

#endif /* RT_H_ */
