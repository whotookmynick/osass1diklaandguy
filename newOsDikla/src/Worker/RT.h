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

typedef struct{
				int target;
				int distance;
				int next;
}rtStruct;

class RT {
	public:
		RT(int numOfTargets,int owner);
		virtual ~RT();



		int _numberOfTargets;
		int _owner;
		vector<rtStruct> _rtTable;



		bool setRT(RT* nighborRT);
		vector<rtStruct> getRtTable();
		void initRT();
		void printRT();
		int getID();
		void setRtDis(int target,int dis,int next);
};

#endif /* RT_H_ */
