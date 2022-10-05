#pragma once
#include"Heap.h"
#include"HeapException.h"

template<class Tk, class Tv>
class FibonacciHeap : public Heap<Tk, Tv>
{
	struct Node
	{
		Node(const Tk& k, const Tv& v) { key = k; value = v; }
		Tv value;
		Tk key;
		Node* child = nullptr;
		Node* parent = nullptr;
		Node* right = this;
		Node* left = this;
		int degree = 0;
	};
	FibonacciHeap(const Cmp<Tk>* cmp)
	{
		if (cmp == nullptr)
			throw std::exception("Wrong Cmp address");
		this->cmp = cmp;
	}
private:
	Node* min = nullptr;
	int size = 0;
	const Cmp<Tk>* cmp;
public:
	
	~FibonacciHeap()
	{
		clean(min);
	}
	Heap<Tk, Tv>* merge(Heap<Tk, Tv>* heap)override
	{
		FibonacciHeap* _heap = dynamic_cast<FibonacciHeap*>(heap);
		this->min = rootsMerge(this->min, _heap->min);
		this->size += _heap->size;
		return this;
	}
	void insert(const Tk& key, const Tv& value)override
	{
		Node* new_node = new Node(key, value);
		this->min = rootsMerge(this->min, new_node);
		size++;
	}
	Tv delMin()override
	{
		if (size == 0)
		{
			throw HeapDelminExc;
		}
		Tv ret_val = min->value;
		Node* L, * R;
		Node* tmp;

		if (size == 1)
		{
			delete min;
			min = nullptr;
			size = 0;
			return ret_val;
		}
		
		L = min->left;
		R = min->right;
		L->right = R;
		R->left = L;
		if (min == R)
			R = nullptr;

		tmp = min->child;
		while (tmp != nullptr)
		{
			tmp->parent = nullptr;
			tmp = tmp->right;
			if (tmp == min->child)
				break;
		}
		R = rootsMerge(R, tmp);
		delete min;
		min = R;
		consolidate();
		size--;
		return ret_val;
	}
	Tv getMin()
	{
		if (size == 0)
			throw HeapGetminExc;
		return min->value;
	}
	void clean()override
	{
		clean(min);
		min = nullptr;
	}
private:
	Node* rootsMerge(Node* first, Node* second)
	{
		if (second == nullptr)
			return first;
		else if (first == nullptr)
			return second;
		Node* new_min = (cmp->compare(first->key, second->key) < 0 ? first : second);
		Node* R = first->right;
		Node* L = second->left;
		first->right = second;
		second->left = first;
		R->left = L;
		L->right = R;
		return new_min;
	}
	Node* treesMerge(Node* first, Node* second)
	{
		Node* L, * R;

		L = second->left;
		R = second->right;
		L->right = R;
		R->left = L;
		
		if (cmp->compare(first->key, second->key) < 0)
		{
			
			second->left = second;
			second->right = second;
			first->child = rootsMerge(first->child, second);
			first->degree++;
			return first;
		}
		else
		{
			if (L == R && L == first)
			{
				second->left = second;
				second->right = second;
				second->child = rootsMerge(second->child, first);
				second->degree++;
				return second;
			}
			L = first->left;
			R = first->right;
			R->left = second;
			L->right = second;
			second->right = R;
			second->left = L;
			first->left = first;
			first->right = first;
			second->child = rootsMerge(second->child, first);
			second->degree++;
			return second;
		}
	}
	void consolidate()
	{
		Node** A = new Node * [size];
		for (int i = 0; i < size; i++)
			A[i] = nullptr;
		Node* cur = min;
		int tmp;
		while (cur != A[cur->degree])
		{
			
			if (cmp->compare(cur->key, min->key) < 0)
				min = cur;
			if (A[cur->degree] == nullptr)
			{
				A[cur->degree] = cur;
			}
			else
			{
				tmp = cur->degree;
				cur = treesMerge(cur, A[cur->degree]);
				A[tmp] = nullptr;
				continue;
			}
			
			cur = cur->right;
		}
		
		delete A;
	}
	void clean(Node* nd)
	{
		Node* tmp;
		while (nd != nullptr)
		{
			tmp = nd;
			clean(nd->child);
			nd = nd->right;
			delete tmp;
		}
	}

};

