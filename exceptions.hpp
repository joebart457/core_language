#pragma once

#include <string>

struct err_trace {
	std::string szMsg{ "" };
	unsigned int row{ 0 };
	unsigned int column{ 0 };
};

class system_exception {
public:
	system_exception(const std::string& msg, unsigned int row = 0, unsigned int column = 0)
		:m_szMsg{ msg }, m_row{ row }, m_column{ column }{}
	system_exception(const std::string& ty, std::string msg, unsigned int row = 0, unsigned int column = 0)
		:m_szTy{ ty }, m_szMsg{ msg }, m_row{ row }, m_column{ column }{}
	~system_exception() {}

	virtual std::string what()
	{
		std::ostringstream oss;
		oss << "[" << m_row << "][" << m_column << "] " << m_szTy << ": " << m_szMsg;
		return oss.str();
	}

	virtual std::string fullTrace()
	{
		std::ostringstream oss;
		oss << "[" << m_row << "][" << m_column << "] " << m_szTy << ": " << m_szMsg << "\n";
		for (auto trace : m_trace) {
			oss << "\tat [" << trace.row << "][" << trace.column << "] " << trace.szMsg << "\n";
		}
		return oss.str();
	}

	virtual void addStackTrace(const std::string& szMsg, unsigned int row, unsigned int column)
	{
		err_trace err;
		err.szMsg = szMsg;
		err.row = row;
		err.column = column;
		m_trace.push_back(err);
	}

private:
	std::string m_szTy{ "system_exception" };
	std::string m_szMsg;

	std::vector<err_trace> m_trace;

	unsigned int m_row;
	unsigned int m_column;

};

class parsing_exception:
	public system_exception {
public:
	parsing_exception(std::string msg, unsigned int row = 0, unsigned int column = 0)
		:system_exception("parsing_exception", msg, row, column){}
	~parsing_exception() {}

};

class runtime_exception:
	public system_exception {
public:
	runtime_exception(std::string msg, unsigned int row = 0, unsigned int column = 0)
		:system_exception("runtime_exception", msg, row, column) {}
	runtime_exception(std::string ty, std::string msg, unsigned int row = 0, unsigned int column = 0)
		:system_exception(ty, msg, row, column) {}
	~runtime_exception() {}
};


class break_exception :
	public runtime_exception {
public:
	break_exception(unsigned int row = 0, unsigned int column = 0)
		:runtime_exception("break_exception", "uncaught break", row, column){}

	~break_exception(){}
};

class io_exception :
	public runtime_exception {
public:
	io_exception(const std::string& msg, unsigned int row = 0, unsigned int column = 0)
		:runtime_exception("io_exception", msg, row, column) {}

	~io_exception() {}

};

template <class _Ty>
class return_exception:
	public runtime_exception {
public:
	return_exception(_Ty val, unsigned int row = 0, unsigned int column = 0)
		:m_val{ val }, m_row{ row }, m_column{ column }, runtime_exception("uncaught return", row, column) {}
	~return_exception() {}


	virtual _Ty value() { return m_val; }

protected:
	_Ty m_val;
	std::string m_szTy{ "return_exception" };

	unsigned int m_row;
	unsigned int m_column;
};