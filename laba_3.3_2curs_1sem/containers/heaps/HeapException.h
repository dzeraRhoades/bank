#pragma once
#include<iostream>
class HeapException : public std::exception
{
protected:
	std::string error;
public:
	virtual std::string info() = 0;
};

class HeapInsertExc : public HeapException
{
public:
	HeapInsertExc()
	{
		this->error = "Insert exception";
	}
	std::string info()override
	{
		return error;
	}
};

class HeapGetminExc : public HeapException
{
public:
	HeapGetminExc()
	{
		this->error = "Get minimum exception: the heap is empty";
	}
	std::string info()override
	{
		return error;
	}
};

class HeapDelminExc : public HeapException
{
public:
	HeapDelminExc()
	{
		this->error = "Delete minimum exception: the heap is empty";
	}
	std::string info()override
	{
		return error;
	}
};

