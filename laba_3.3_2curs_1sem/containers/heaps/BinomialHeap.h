#pragma once
#include"Heap.h"
#include"HeapException.h"
template<class Tk, class Tv>
class BinomialHeap : public Heap<Tk, Tv>
{
	struct Node
	{
		Node(const Tk& k, const Tv& v) { key = k; value = v; }
		Node* parent = nullptr;
		Tk key;
		Tv value;
		Node* child = nullptr;
		Node* sibling = nullptr;
		int degree = 0;
	};

private:
	Node* root = nullptr;
	const Cmp<Tk>* cmp;
public:
	BinomialHeap(const Cmp<Tk>* cmp)
	{
		this->cmp = cmp;
	}
	~BinomialHeap()
	{
		clean(root);
	}

	Heap<Tk, Tv>* merge(Heap<Tk, Tv>* heap) override
	{
		// сливаем списки корней
		BinomialHeap* _heap = dynamic_cast<BinomialHeap*>(heap);
		if (_heap == this)
			return this;
		Node* tmp_1, * tmp_2;
		tmp_1 = this->root;
		tmp_2 = _heap->root;
		std::vector<Node*> new_root;

		if (this->root == nullptr || _heap->root == nullptr)
		{
			if (this->root == nullptr)
				this->root = _heap->root;
			return this;
		}

		while (tmp_1 != nullptr && tmp_2 != nullptr)
		{
			if (cmp->compare(tmp_1->degree, tmp_2->degree) < 0)
			{
				new_root.push_back(tmp_1);
				tmp_1 = tmp_1->sibling;
			}
			else
			{
				new_root.push_back(tmp_2);
				tmp_2 = tmp_2->sibling;
			}
		}
		while (tmp_1 != nullptr)
		{
			new_root.push_back(tmp_1);
			tmp_1 = tmp_1->sibling;
		}
		while (tmp_2 != nullptr)
		{
			new_root.push_back(tmp_2);
			tmp_2 = tmp_2->sibling;
		}
		this->root = new_root[0];
		for (int i = 0; i < new_root.size(); i++)
		{
			if (i == new_root.size() - 1)
				new_root[i]->sibling = nullptr;
			else
				new_root[i]->sibling = new_root[i + 1];
		}
		// теперь нужно проеврить на наличие корней с одинаковыми degree
		Node* prev, * cur, * next;
		prev = nullptr;
		cur = this->root;
		next = this->root->sibling;

		while (next != nullptr)
		{
			if (cmp->compare(cur->degree, next->degree) == 0 && !(next->sibling != nullptr && cmp->compare(next->sibling->degree, cur->degree) == 0))
			{

				if (cmp->compare(cur->key, next->key) < 0)
				{
					cur->sibling = next->sibling;
					next->sibling = cur->child;
					cur->child = next;
					next->parent = cur;
					cur->degree++;
					next = cur->sibling;
				}
				else
				{
					if (prev != nullptr)
						prev->sibling = next;
					else
						this->root = next;
					cur->sibling = next->child;
					next->child = cur;
					cur->parent = next;
					next->degree++;
					cur = next;
					next = next->sibling;
				}
			}
			else
			{
				prev = cur;
				cur = next;
				next = next->sibling;
			}
		}
		_heap->root = nullptr;
		return this;
	}

	void insert(const Tk& key, const Tv& value) override
	{
		if (root == nullptr)
		{
			root = new Node(key, value);
			return;
		}
		BinomialHeap heap(cmp);
		heap.root = new Node(key, value);
		merge(&heap);
		heap.root = nullptr; // чтобы деструктор не очищал узлы
	}

	Tv delMin()override // doesn't work? should be done correctly
	{
		Tv del_val;
		BinomialHeap heap(cmp);
		Node* tmp = this->root;
		Node* tmp_2, * tmp_3 = nullptr;
		Node* min_node = tmp;
		Node* pre_min = nullptr;
		if (this->root == nullptr)
			throw HeapDelminExc();
		while (tmp->sibling != nullptr)
		{
			if (cmp->compare(tmp->sibling->key, min_node->key) < 0)
			{
				pre_min = tmp;
				min_node = tmp->sibling;
			}
			tmp = tmp->sibling;
		}
		del_val = min_node->value;
		if (pre_min != nullptr)
		{
			pre_min->sibling = min_node->sibling;
		}
		else
		{
			this->root = min_node->sibling;
		}
		tmp = min_node->child;
		while (tmp != nullptr)
		{
			// разворачиваем список деревьев
			tmp->parent = nullptr;
			tmp_2 = tmp->sibling;
			tmp->sibling = tmp_3;
			tmp_3 = tmp;
			tmp = tmp_2;
		}

		heap.root = tmp_3;
		delete min_node;
		merge(&heap);
		heap.root = nullptr;
		return del_val;
	}
	Tv getMin()override
	{
		if (this->root == nullptr)
			throw HeapGetminExc();
		Node* tmp = this->root;
		Node* min_node = tmp;
		while (tmp != nullptr)
		{
			if (cmp->compare(tmp->key, min_node->key) < 0)
				min_node = tmp;
			tmp = tmp->sibling;
		}
		return min_node->value;
	}
	bool is_empty()
	{
		return root == nullptr;
	}
private:
	void clean(Node* nd)
	{
		Node* tmp;
		while (nd != nullptr)
		{
			tmp = nd;
			clean(nd->child);
			nd = nd->sibling;
			delete tmp;
		}
	}


};

