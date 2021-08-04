#pragma once

#include "execution_context.hpp"

class developer_context :
	public execution_context 
{
public:
	developer_context(std::shared_ptr<activation_record> ar)
		:execution_context(ar){}
	~developer_context(){}


	unsigned int as_uint_or_default(std::string szKey, unsigned int _default)
	{
		try {
			serialized_integral value = get_primitive(szKey);
			return uint_or_error(value);
		}
		catch (system_exception) {
			return _default;
		}
	}

	int as_int_or_default(std::string szKey, int _default)
	{
		try {
			serialized_integral value = get_primitive(szKey);
			return int_or_error(value);
		}
		catch (system_exception) {
			return _default;
		}
	}

	char as_char_or_default(std::string szKey, char _default)
	{
		try {
			serialized_integral value = get_primitive(szKey);
			return char_or_error(value);
		}
		catch (system_exception) {
			return _default;
		}
	}


	double as_double_or_default(std::string szKey, double _default)
	{
		try {
			serialized_integral value = get_primitive(szKey);
			return double_or_error(value);
		}
		catch (system_exception) {
			return _default;
		}
	}

	std::string as_string_or_default(std::string szKey, std::string _default)
	{
		try {
			serialized_integral value = get_primitive(szKey);
			return value.getValue();
		}
		catch (system_exception) {
			return _default;
		}
	}

};
