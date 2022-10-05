#pragma once
#include<vector>
#include"Cmp.h"
#include"HeapException.h"
//#include"BinaryHeap.h"
//#include"FibonacciHeap.h"
//#include"BinomialHeap.h"
template<class Tk, class Tv>
class Heap
{
public:
	virtual void insert(const Tk& key, const Tv& value) = 0;
	virtual Tv getMin() = 0;
	virtual Tv delMin() = 0;
	virtual Heap* merge(Heap* heap) = 0;
	//virtual void clean() = 0;
	virtual ~Heap(){}
};

