#pragma once
#include"BinaryHeap.h"
#include"BinomialHeap.h"
#include"FibonacciHeap.h"

template<class Tk, class Tv>
class HeapCreator
{
public:
	virtual ~HeapCreator(){}
	virtual Heap<Tk, Tv>* createHeap(const Cmp<Tk>* cmp) = 0;
	void heapTesting()
	{
		Heap<Tk, Tv>* heap = createHeap();
		// testing
		delete heap;
	}
};

template<class Tk, class Tv>
class BinaryHeapCreator : public HeapCreator<Tk, Tv>
{
public:
	Heap<Tk, Tv>* createHeap(const Cmp<Tk>* cmp) override
	{
		return new BinaryHeap<Tk, Tv>(cmp);
	}
	
};

template<class Tk, class Tv>
class FibonacciHeapCreator : public HeapCreator<Tk, Tv>
{
public:
	Heap<Tk, Tv>* createHeap(const Cmp<Tk>* cmp) override
	{
		return new FibonacciHeap<Tk, Tv>(cmp);
	}

};

template<class Tk, class Tv>
class BinomialHeapCreator : public HeapCreator<Tk, Tv>
{
public:
	Heap<Tk, Tv>* createHeap(const Cmp<Tk>* cmp) override
	{
		return new BinomialHeap<Tk, Tv>(cmp);
	}

};

