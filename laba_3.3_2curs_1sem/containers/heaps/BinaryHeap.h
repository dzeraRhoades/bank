#pragma once
#include"Heap.h"

template<class Tk, class Tv>
class BinaryHeap: public Heap<Tk, Tv>
{
	struct Node
	{
		Node(Tk k, Tv val) { key = k; value = val; }
		Tk key;
		Tv value;
	};
public:
	BinaryHeap(const Cmp<Tk>* cmp)
	{
		this->cmp = cmp;
	}
	~BinaryHeap()
	{
		for (int i = 0; i < arr.size(); i++)
			delete arr[i];
	}
	void insert(const Tk& key, const Tv& value) override
	{
		arr.push_back(new Node(key, value));
		siftUp(arr.size() - 1);
	}
	Tv getMin() override
	{
		if (arr.empty())
			throw HeapGetminExc;
		return arr[0]->value;
	}
	Tv delMin() override
	{
		if (arr.empty())
			throw HeapDelminExc;
		Tv tmp = arr[0]->value;
		swap(0, arr.size() - 1);
		delete arr[arr.size() - 1];
		arr.pop_back();
		return tmp;
	}
	Heap<Tk, Tv>* merge(Heap<Tk, Tv>* heap) override
	{
		BinaryHeap* tmp = dynamic_cast<BinaryHeap*>(heap);
		for (int i = 0; i < tmp->arr.size(); i++)
		{
			arr.push_back(tmp->arr[i]);
		}
		heapBuilding();
		heap = tmp;
		return heap;
	}
private: // fields
	std::vector<Node*> arr;
	const Cmp<Tk>* cmp;
private: // methods
	void siftUp(int index)
	{
		while (index != 0 && cmp->compare(arr[index]->key, arr[(index - 1) / 2]->key) < 0)
		{
			swap(index, (index - 1) / 2);
			index = (index - 1) / 2;
		}
	}
	void siftDown(int ind)
	{
		int min_ind;
		while (1)
		{
			if (ind * 2 + 1 < arr.size())
				min_ind = ind * 2 + 1;
			else
				break;
			if (ind * 2 + 2 < arr.size())
				if (cmp->compare(arr[ind * 2 + 1]->key, arr[ind * 2 + 2]->key)>0)
					min_ind = ind * 2 + 2;
			swap(ind, min_ind);
			ind = min_ind;
		}
	}
	void heapBuilding()
	{
		int ind = arr.size() / 2 - 1; // last node with childs
		while (ind != 0)
		{
			siftDown(ind);
			ind--;
		}
	}
	void swap(int ind1, int ind2)
	{
		Node tmp(arr[ind1]->key, arr[ind1]->value);
		arr[ind1]->key = arr[ind2]->key;
		arr[ind1]->value = arr[ind2]->value;
		arr[ind2]->key = tmp.key;
		arr[ind2]->value = tmp.value;
	}
};

