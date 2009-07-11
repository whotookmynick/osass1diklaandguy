#ifndef ITERATORLIST_H_
#define ITERATORLIST_H_
#include "BlockList.h"

class IteratorList
{
public:

	BlockList& _bList;



	int _last;
	int _offset;
	int _index;


	void move();
	IteratorList(BlockList& bList);
	bool hasNext() const;
	int next();
	int nextOBlock() const;
};

#endif /*ITERATORLIST_H_*/
