#pragma once
#include<iostream>
template <typename T>
class Cmp
{
public:
	virtual ~Cmp()
	{

	}
	virtual int compare(const T& left, const T& right) const = 0;
};

class CmpForString : public Cmp<std::string>
{
public:
	int compare(const std::string& left, const std::string& right)const override
	{
		for (int i = 0; i < std::max(left.size(), right.size()); i++)
		{
			if (i == left.size())
				return -1;
			else if (i == right.size())
				return 1;
			if (left[i] > right[i])
				return 1;
			else if (left[i] < right[i])
				return -1;
		}
		return 0;
	}

};

class CmpForInt :
	public Cmp<int>
{
public:
	int compare(const int& left, const int& right) const override
	{
		if (left > right)
			return 1;
		else if (left < right)
			return -1;
		else if (left == right)
			return 0;
	}
};
//int CmpForInt::compare(const int& left, const int& right) const
//{
//	if (left > right)
//		return 1;
//	else if (left < right)
//		return -1;
//	else if (left == right)
//		return 0;
//}
//CmpForInt::~CmpForInt()
//{
//
//}

