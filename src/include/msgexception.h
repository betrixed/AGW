#ifndef _H_MSG_EXCEPTION
#define _H_MSG_EXCEPTION

#include <exception>
#include <string>

class MsgException : public std::exception {
public:
	MsgException(const std::string& msg) : msg_(msg)
	{
	}

	MsgException()
	{
	}

	const char* msg() const noexcept { return msg_.c_str(); }
	const char* what() const noexcept {return msg_.c_str(); }
protected:
	std::string msg_;
};

#endif
