/*
 * LowLevelList.h
 *
 *  Created on: Jul 8, 2009
 *      Author: dikla
 */

#ifndef LOWLEVELLIST_H_
#define LOWLEVELLIST_H_

class LowLevelList {
public:
	LowLevelList();
	virtual ~LowLevelList();


	int size();
	bool empty();

	bool contains(int& value);


	int& Head();//the front
	void pop_front();
	void push_back(int& val);
};

#endif /* LOWLEVELLIST_H_ */
