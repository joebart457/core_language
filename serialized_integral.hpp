#pragma once


#ifndef __INCLUDE_SERIALIZED_INTEGRAL
#define __INCLUDE_SERIALIZED_INTEGERAL

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <mutex>

#include "string_utilities.hpp"
#include "exceptions.hpp"

#define SI_PARSE_SUCCESS     0
#define SI_INCOMPLETE_PARSE -1 // Parsing stopped short of end of string
#define SI_INVALID_DATA     -2 // Unable to convert string to type
#define SI_OUT_OF_RANGE     -3 // Too big for object type


template <class Ty>
struct parse_result {
	int error = 0;
	Ty value;
};

class si_types {
public:
	static std::string NULLTYPE() { return "nulltype"; }
	static std::string UINT() { return "uint"; }
	static std::string INT() { return "int"; }
	static std::string FLOAT() { return "float"; }
	static std::string DOUBLE() { return "double"; }
	static std::string CHAR() { return "char"; }
	static std::string STRING() { return "string"; }
	static std::string ANY() { return "any"; }
};

class serialized_integral {

public:
	serialized_integral()
		:_ty{ si_types().NULLTYPE() }, _val{ "" }{}
	serialized_integral(const unsigned int& val) { fromUInt(val); }
	serialized_integral(const int& val) { fromInt(val); }
	serialized_integral(const float& val) { fromFloat(val); }
	serialized_integral(const double& val) { fromDouble(val); }
	serialized_integral(const char& val) { fromChar(val); }
	serialized_integral(const std::string& val, bool typeIncluded) {
		if (!typeIncluded) {
			fromString(val);
		}
		else {
			std::vector<std::string> result = split(val, ':');
			if (result.size() == 2) {
				_ty = result.at(0);
				_val = result.at(1);
			}
			else {
				throw std::invalid_argument("Invalid data format. Data must be formatted as type:value");
			}
		}
	}

	serialized_integral(const std::string& ty, const std::string& val)
		:_ty{ ty }, _val{ val }{};
	serialized_integral(const serialized_integral& si)
		:_val{ si._val }, _ty{ si._ty }{}
	

	~serialized_integral() {}

	void fromUInt(const unsigned int& val) // May throw std::bad_alloc from std::string constructor
	{
		std::scoped_lock lock(mux);

		_ty = si_types::UINT();
		_val = std::to_string(val);
	}

	void fromInt(const int& val) // May throw std::bad_alloc from std::string constructor
	{
		std::scoped_lock lock(mux);

		_ty = si_types::INT();
		_val = std::to_string(val);
	}

	void fromFloat(const float& val) // May throw std::bad_alloc from std::string constructor
	{
		std::scoped_lock lock(mux);
		
		_ty = si_types().FLOAT();
		_val = std::to_string(val);
	}

	void fromDouble(const double& val) // May throw std::bad_alloc from std::string constructor
	{
		std::scoped_lock lock(mux);

		_ty = si_types().DOUBLE();
		_val = std::to_string(val);
	}

	void fromChar(const char& val) // May throw std::bad_alloc from std::string constructor
	{
		std::scoped_lock lock(mux);

		_ty = si_types().CHAR();
		_val = std::string(1, val);
	}

	void fromString(const std::string& val) // May throw std::bad_alloc from std::string constructor
	{
		std::scoped_lock lock(mux);

		_ty = si_types().STRING();
		_val = val;
	}

	parse_result<unsigned int> tryParseUInt()
	{
		std::scoped_lock lock(mux);
		parse_result<unsigned int> pr;
		try {
			size_t pos;
			pr.value = std::stoul(_val, &pos);
			if (pos != _val.size()) {
				pr.error = SI_INCOMPLETE_PARSE;
			}
		}
		catch (std::invalid_argument) {
			pr.error = SI_INVALID_DATA;
		}
		catch (std::out_of_range) {
			pr.error = SI_OUT_OF_RANGE;
		}
		return pr;
	}

	parse_result<int> tryParseInt()
	{
		std::scoped_lock lock(mux);
		parse_result<int> pr;
		try {
			size_t pos;
			pr.value = std::stoi(_val, &pos);
			if (pos != _val.size()) {
				pr.error = SI_INCOMPLETE_PARSE;
			}
		}
		catch (std::invalid_argument) {
			pr.error = SI_INVALID_DATA;
		}
		catch (std::out_of_range) {
			pr.error = SI_OUT_OF_RANGE;
		}
		return pr;
	}

	parse_result<float> tryParseFloat()
	{
		std::scoped_lock lock(mux);
		parse_result<float> pr;
		try {
			size_t pos;
			pr.value = std::stof(_val, &pos);
			if (pos != _val.size()) {
				pr.error = SI_INCOMPLETE_PARSE;
			}
		}
		catch (std::invalid_argument) {
			pr.error = SI_INVALID_DATA;
		}
		catch (std::out_of_range) {
			pr.error = SI_OUT_OF_RANGE;
		}
		return pr;
	}

	parse_result<double> tryParseDouble()
	{
		std::scoped_lock lock(mux);

		parse_result<double> pr;
		try {
			size_t pos;
			pr.value = std::stof(_val, &pos);
			if (pos != _val.size()) {
				pr.error = SI_INCOMPLETE_PARSE;
			}
		}
		catch (std::invalid_argument) {
			pr.error = SI_INVALID_DATA;
		}
		catch (std::out_of_range) {
			pr.error = SI_OUT_OF_RANGE;
		}
		return pr;
	}


	parse_result<char> tryParseChar()
	{
		std::scoped_lock lock(mux);

		parse_result<char> pr;
		if (_val.size() == 0) {
			pr.error = SI_INVALID_DATA;
		}
		else {
			pr.value = _val.at(0);
			if (_val.size() > 1) {
				pr.error = SI_INCOMPLETE_PARSE;
			}
		}
		return pr;
	}

	std::string getType() const {
		return _ty;
	}

	std::string getValue() const {
		return _val;
	}


	std::string toString() {
		return "(" + _ty + ": " + _val + ")";
	}

	friend std::ostream& operator<<(std::ostream& os, const serialized_integral& si)
	{
		os << si._ty << ':' << si._val;
		return os;
	}

	serialized_integral& operator=(const serialized_integral& rhs)
	{
		this->_ty = rhs._ty;
		this->_val = rhs._val;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, serialized_integral& rhs)
	{
		out << rhs.toString();
		return out;
	}

private:
	std::string _ty, _val;
	std::mutex mux;
};

unsigned int uint_or_error(serialized_integral& si)
{
	auto parse_result = si.tryParseUInt();
	if (!parse_result.error) {
		return parse_result.value;
	}
	throw system_exception("unable to parse value to int: " + si.toString());
}


int int_or_error(serialized_integral& si)
{
	auto parse_result = si.tryParseInt();
	if (!parse_result.error) {
		return parse_result.value;
	}
	throw system_exception("unable to parse value to int: " + si.toString());
}

float float_or_error(serialized_integral& si)
{
	auto parse_result = si.tryParseFloat();
	if (!parse_result.error) {
		return parse_result.value;
	}
	throw system_exception("unable to parse value to float: " + si.toString());
}

double double_or_error(serialized_integral& si)
{
	auto parse_result = si.tryParseDouble();
	if (!parse_result.error) {
		return parse_result.value;
	}
	throw system_exception("unable to parse value to double: " + si.toString());
}

char char_or_error(serialized_integral& si)
{
	auto parse_result = si.tryParseChar();
	if (!parse_result.error) {
		return parse_result.value;
	}
	throw system_exception("unable to parse value to char: " + si.toString());
}

#endif