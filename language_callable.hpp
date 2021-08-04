#pragma once

#include <memory>
#include "exceptions.hpp"
#include "serialized_integral.hpp"

class interpreter;

class callable :
	public std::enable_shared_from_this<callable> {
public:
	callable() {}
	~callable() {}

	virtual serialized_integral call(interpreter* i, std::vector<serialized_integral> args) = 0;

	friend std::ostream& operator<<(std::ostream& out, const callable& rhs)
	{
		out << "callable";
		return out;
	}
};

class native_fn :
	public callable {
public:
	native_fn(serialized_integral(*fn_ptr)(interpreter*, std::vector<serialized_integral>))
		:fn_ptr{ fn_ptr } {}
	~native_fn() {}

	virtual serialized_integral call(interpreter* i, std::vector<serialized_integral> args)
	{
		if (fn_ptr != nullptr) {
			return fn_ptr(i, args);
		}
		throw system_exception("function pointer was null");
	}

protected:
	serialized_integral(*fn_ptr)(interpreter*, std::vector<serialized_integral>);
};

class binary_operator:
	 public callable
 {
 public:
	 binary_operator(serialized_integral(*bin_op)(serialized_integral&, serialized_integral&))
		 :bin_op{ bin_op }{}
	 ~binary_operator(){}


	 virtual serialized_integral call(interpreter* i, std::vector<serialized_integral> args)
	 {
		 if (args.size() != 2) {
			 throw runtime_exception("invalid number of arguments; expected 2 but got " + args.size());
		 }
		 if (bin_op != nullptr) {
			 return bin_op(args.at(0), args.at(1));
		 }
		 throw system_exception("function pointer was null");
	 }

 protected:
	 serialized_integral(*bin_op)(serialized_integral&, serialized_integral&);
 };


class unary_operator :
	public callable
{
public:
	unary_operator(serialized_integral(*un_op)(serialized_integral&))
		:un_op{ un_op }{}
	~unary_operator() {}


	virtual serialized_integral call(interpreter* i, std::vector<serialized_integral> args)
	{
		if (args.size() != 1) {
			throw runtime_exception("invalid number of arguments; expected 1 but got " + args.size());
		}
		if (un_op != nullptr) {
			return un_op(args.at(0));
		}
		throw system_exception("function pointer was null");
	}

protected:
	serialized_integral(*un_op)(serialized_integral&);
};

