#pragma once
#include<iostream>
#include<fstream>
enum SEV {TRACE, DEBUG, INFORMATION, WARNING, ERROR, FATAL};
class Stream
{
	friend class logBuilder;
	friend class Logger;
public:
	Stream(SEV severity, const std::string& file_name)
	{
		next = nullptr;
		_severity = severity;
		if (file_name == "stdout")
		{
			write_console = true;
			return;
		}
		_stream.open(file_name);
		if (!_stream.is_open())
			throw std::exception("stream file can't be opened");
	}
	~Stream()
	{
		_stream.close();
	}
private:
	std::ofstream _stream;
	SEV _severity;
	Stream* next;
	bool write_console = false;

};

