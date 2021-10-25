#pragma once
#include "pch.h"

class Exception : public std::exception
{
public:
	explicit Exception(const std::string& message) :
		msg_(message)
	{}

	virtual ~Exception() noexcept {}
	virtual const char* what() {
		msg_ = "CLASS EXCEPTION OCCURED!\n" + msg_;
		return msg_.c_str();
	}
protected:
	std::string msg_;
};

class Logic_exc : public Exception {
public:
	explicit Logic_exc(const std::string& message) : Exception(message)
	{}
	virtual const char* what() {
		msg_ = "LOGIC ERROR:\n" + msg_;
		return msg_.c_str();
	}
};

class Bad_string : public Logic_exc {
public:
	explicit Bad_string(const std::string& message) : Logic_exc(message)
	{}
	const char* what() {
		msg_ = std::string("BAD STRING REPRESENTATION OF MATRIX:\n") + msg_;
		return msg_.c_str();
	}
};

class Bad_arith_arg : public Logic_exc {
public:
	explicit Bad_arith_arg(const std::string& message) : Logic_exc(message) 
	{}
	const char* what() { 
		msg_ = std::string("BAD ARGUMENTS FOR ARITHMETIC OPERATION:\n") + msg_;
		return msg_.c_str();
	}
};

class Elem_overflow : public Exception {
public:
	explicit Elem_overflow(const std::string& message) : Exception(message)
	{}
	const char* what() {
		msg_ = std::string("OVERFLOW EXCEPTION:\n") + msg_;
		return msg_.c_str();
	}
};

class Div_by_zero : public Exception {
public:
	explicit Div_by_zero(const std::string& message) : Exception(message)
	{}
	const char* what() {
		msg_ = std::string("ZERO DIVIDING:\n") + msg_;
		return msg_.c_str();
	}
};

class Bad_file : public Exception {
public:
	explicit Bad_file(const std::string& message = "") : Exception(message)
	{}
	const char* what() {
		msg_ = std::string("SOME PROBLEMS WITH FILE OPENNING:\n") + msg_;
		return msg_.c_str();
	}
};

