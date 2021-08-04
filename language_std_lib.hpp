#pragma once

#include <map>
#include <conio.h>
#include <iostream>

#include "serialized_integral.hpp"
#include "exceptions.hpp"
#include "language_callable.hpp"
#include "interpreter.hpp"
#include "ufhndl.hpp"


serialized_integral typeof_any(serialized_integral& rhs)
{
	return serialized_integral(rhs.getType(), false);
}

serialized_integral add_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) + int_or_error(rhs));
}

serialized_integral add_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) + float_or_error(rhs));
}

serialized_integral add_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) + double_or_error(rhs));
}

serialized_integral add_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) + int_or_error(rhs));
}

serialized_integral add_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) + float_or_error(rhs));
}

serialized_integral add_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) + double_or_error(rhs));
}

serialized_integral add_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) + int_or_error(rhs));
}

serialized_integral add_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) + float_or_error(rhs));
}

serialized_integral add_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) + double_or_error(rhs));
}




serialized_integral sub_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) - int_or_error(rhs));
}

serialized_integral sub_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) - float_or_error(rhs));
}

serialized_integral sub_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) - double_or_error(rhs));
}

serialized_integral sub_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) - int_or_error(rhs));
}

serialized_integral sub_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) - float_or_error(rhs));
}

serialized_integral sub_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) - double_or_error(rhs));
}

serialized_integral sub_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) - int_or_error(rhs));
}

serialized_integral sub_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) - float_or_error(rhs));
}

serialized_integral sub_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) - double_or_error(rhs));
}




serialized_integral multiply_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) * int_or_error(rhs));
}

serialized_integral multiply_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) * float_or_error(rhs));
}

serialized_integral multiply_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) * double_or_error(rhs));
}

serialized_integral multiply_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) * int_or_error(rhs));
}

serialized_integral multiply_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) * float_or_error(rhs));
}

serialized_integral multiply_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) * double_or_error(rhs));
}

serialized_integral multiply_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) * int_or_error(rhs));
}

serialized_integral multiply_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) * float_or_error(rhs));
}

serialized_integral multiply_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) * double_or_error(rhs));
}


serialized_integral divide_int_int(serialized_integral& lhs, serialized_integral& rhs)
{

	int rhs_val = int_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(int_or_error(lhs) / rhs_val);
}

serialized_integral divide_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	float rhs_val = float_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(int_or_error(lhs) / rhs_val);
}

serialized_integral divide_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	double rhs_val = double_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(int_or_error(lhs) / rhs_val);
}

serialized_integral divide_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	int rhs_val = int_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(float_or_error(lhs) / rhs_val);
}

serialized_integral divide_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	float rhs_val = float_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(float_or_error(lhs) / rhs_val);
}

serialized_integral divide_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	double rhs_val = double_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(float_or_error(lhs) / rhs_val);
}

serialized_integral divide_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	int rhs_val = int_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(double_or_error(lhs) / rhs_val);
}

serialized_integral divide_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	float rhs_val = float_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(double_or_error(lhs) / rhs_val);
}

serialized_integral divide_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	double rhs_val = double_or_error(rhs);
	if (rhs_val == 0) {
		throw runtime_exception("division by zero not allowed");
	}
	return serialized_integral(double_or_error(lhs) / rhs_val);
}

serialized_integral add_string_string(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() + rhs.getValue(), false);
}

serialized_integral add_string_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() + rhs.getValue(), false);
}

serialized_integral add_string_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() + rhs.getValue(), false);
}

serialized_integral add_string_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() + rhs.getValue(), false);
}

serialized_integral add_string_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() + rhs.getValue(), false);
}

serialized_integral add_string_fn_ptr(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() + rhs.getValue(), false);
}

serialized_integral negate_int(serialized_integral& rhs)
{
	return serialized_integral(-int_or_error(rhs));
}

serialized_integral negate_float(serialized_integral& rhs)
{
	return serialized_integral(-float_or_error(rhs));
}

serialized_integral negate_double(serialized_integral& rhs)
{
	return serialized_integral(-double_or_error(rhs));
}

serialized_integral not_int(serialized_integral& rhs)
{
	return serialized_integral(!(bool)int_or_error(rhs));
}

serialized_integral not_float(serialized_integral& rhs)
{
	return serialized_integral(!(bool)float_or_error(rhs));
}

serialized_integral not_double(serialized_integral& rhs)
{
	return serialized_integral(!(bool)double_or_error(rhs));
}

serialized_integral not_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue() == "" ? true : false);
}

serialized_integral not_char(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue() == std::string(1, static_cast<char>(0)) ? true : false);
}

serialized_integral not_fn_ptr(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue() == "" ? true : false);
}



serialized_integral equalequal_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) == int_or_error(rhs));
}

serialized_integral equalequal_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) == float_or_error(rhs));
}

serialized_integral equalequal_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) == double_or_error(rhs));
}

serialized_integral equalequal_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) == int_or_error(rhs));
}

serialized_integral equalequal_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) == float_or_error(rhs));
}

serialized_integral equalequal_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) == double_or_error(rhs));
}

serialized_integral equalequal_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) == int_or_error(rhs));
}

serialized_integral equalequal_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) == float_or_error(rhs));
}

serialized_integral equalequal_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) == double_or_error(rhs));
}



serialized_integral notequal_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) != int_or_error(rhs));
}

serialized_integral notequal_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) != float_or_error(rhs));
}

serialized_integral notequal_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) != double_or_error(rhs));
}

serialized_integral notequal_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) != int_or_error(rhs));
}

serialized_integral notequal_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) != float_or_error(rhs));
}

serialized_integral notequal_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) != double_or_error(rhs));
}

serialized_integral notequal_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) != int_or_error(rhs));
}

serialized_integral notequal_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) != float_or_error(rhs));
}

serialized_integral notequal_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) != double_or_error(rhs));
}



serialized_integral less_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) < int_or_error(rhs));
}

serialized_integral less_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) < float_or_error(rhs));
}

serialized_integral less_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) < double_or_error(rhs));
}

serialized_integral less_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) < int_or_error(rhs));
}

serialized_integral less_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) < float_or_error(rhs));
}

serialized_integral less_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) < double_or_error(rhs));
}

serialized_integral less_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) < int_or_error(rhs));
}

serialized_integral less_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) < float_or_error(rhs));
}

serialized_integral less_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) < double_or_error(rhs));
}




serialized_integral lessequal_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) <= int_or_error(rhs));
}

serialized_integral lessequal_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) <= float_or_error(rhs));
}

serialized_integral lessequal_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) <= double_or_error(rhs));
}

serialized_integral lessequal_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) <= int_or_error(rhs));
}

serialized_integral lessequal_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) <= float_or_error(rhs));
}

serialized_integral lessequal_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) <= double_or_error(rhs));
}

serialized_integral lessequal_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) <= int_or_error(rhs));
}

serialized_integral lessequal_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) <= float_or_error(rhs));
}

serialized_integral lessequal_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) <= double_or_error(rhs));
}



serialized_integral greater_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) > int_or_error(rhs));
}

serialized_integral greater_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) > float_or_error(rhs));
}

serialized_integral greater_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) > double_or_error(rhs));
}

serialized_integral greater_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) > int_or_error(rhs));
}

serialized_integral greater_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) > float_or_error(rhs));
}

serialized_integral greater_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) > double_or_error(rhs));
}

serialized_integral greater_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) > int_or_error(rhs));
}

serialized_integral greater_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) > float_or_error(rhs));
}

serialized_integral greater_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) > double_or_error(rhs));
}




serialized_integral greaterequal_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) >= int_or_error(rhs));
}

serialized_integral greaterequal_int_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) >= float_or_error(rhs));
}

serialized_integral greaterequal_int_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) >= double_or_error(rhs));
}

serialized_integral greaterequal_float_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) >= int_or_error(rhs));
}

serialized_integral greaterequal_float_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) >= float_or_error(rhs));
}

serialized_integral greaterequal_float_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(float_or_error(lhs) >= double_or_error(rhs));
}

serialized_integral greaterequal_double_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) >= int_or_error(rhs));
}

serialized_integral greaterequal_double_float(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) >= float_or_error(rhs));
}

serialized_integral greaterequal_double_double(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(double_or_error(lhs) >= double_or_error(rhs));
}


serialized_integral equalequal_string_string(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() == rhs.getValue());
}

serialized_integral equalequal_string_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() == rhs.getValue());
}

serialized_integral equalequal_char_string(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() == rhs.getValue());
}

serialized_integral equalequal_char_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() == rhs.getValue());
}

serialized_integral notequal_string_string(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() != rhs.getValue());
}

serialized_integral notequal_string_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() != rhs.getValue());
}

serialized_integral notequal_char_string(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() != rhs.getValue());
}

serialized_integral notequal_char_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(lhs.getValue() != rhs.getValue());
}

serialized_integral convert_uint_int(serialized_integral& rhs)
{
	return serialized_integral(int_or_error(rhs));
}

serialized_integral convert_uint_float(serialized_integral& rhs)
{
	return serialized_integral(float_or_error(rhs));
}

serialized_integral convert_uint_double(serialized_integral& rhs)
{
	return serialized_integral(double_or_error(rhs));
}


serialized_integral convert_uint_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue(), false);
}

serialized_integral convert_int_float(serialized_integral& rhs)
{
	return serialized_integral(float_or_error(rhs));
}

serialized_integral convert_int_double(serialized_integral& rhs)
{
	return serialized_integral(double_or_error(rhs));
}

serialized_integral convert_int_uint(serialized_integral& rhs)
{
	return serialized_integral(uint_or_error(rhs));
}

serialized_integral convert_float_int(serialized_integral& rhs)
{
	return serialized_integral(static_cast<int>(float_or_error(rhs)));
}

serialized_integral convert_float_double(serialized_integral& rhs)
{
	return serialized_integral(double_or_error(rhs));
}

serialized_integral convert_double_int(serialized_integral& rhs)
{
	return serialized_integral(static_cast<int>(double_or_error(rhs)));
}

serialized_integral convert_double_float(serialized_integral& rhs)
{
	return serialized_integral(float_or_error(rhs));
}

serialized_integral convert_int_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue(), false);
}

serialized_integral convert_float_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue(), false);
}

serialized_integral convert_double_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue(), false);
}

serialized_integral convert_char_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue(), false);
}

serialized_integral convert_int_char(serialized_integral& rhs)
{
	return serialized_integral((char)int_or_error(rhs));
}

serialized_integral convert_string_char(serialized_integral& rhs)
{
	if (rhs.getValue().size() > 0) {
		return serialized_integral(rhs.getValue().at(0));
	}
	throw runtime_exception("unable to cast value " + rhs.toString() + " to type 'char'");
}

serialized_integral convert_string_int(serialized_integral& rhs)
{
	return serialized_integral(int_or_error(rhs));
}

serialized_integral convert_string_float(serialized_integral& rhs)
{
	return serialized_integral(float_or_error(rhs));
}

serialized_integral convert_string_double(serialized_integral& rhs)
{
	return serialized_integral(double_or_error(rhs));
}


serialized_integral covert_int_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_float_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_double_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_char_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_string_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_fn_ptr_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_group_ptr_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().ANY(), rhs.getValue());
}

serialized_integral covert_uint_any(serialized_integral& rhs)
{
	return serialized_integral(si_types().UINT(), rhs.getValue());
}

serialized_integral covert_any_int(serialized_integral& rhs)
{
	return serialized_integral(int_or_error(rhs));
}

serialized_integral covert_any_float(serialized_integral& rhs)
{
	return serialized_integral(float_or_error(rhs));
}

serialized_integral covert_any_double(serialized_integral& rhs)
{
	return serialized_integral(double_or_error(rhs));
}

serialized_integral covert_any_char(serialized_integral& rhs)
{
	if (rhs.getValue().empty()) {
		return serialized_integral(rhs.getValue().at(0));
	}
	throw runtime_exception("unable to cast value " + rhs.toString() + " to type 'char'");
}

serialized_integral covert_any_string(serialized_integral& rhs)
{
	return serialized_integral(rhs.getValue(), false);
}

serialized_integral covert_any_fn_ptr(serialized_integral& rhs)
{
	return serialized_integral("fn_ptr", rhs.getValue());
}

serialized_integral covert_any_group_ptr(serialized_integral& rhs)
{
	return serialized_integral("group_ptr", rhs.getValue());
}

serialized_integral covert_any_uint(serialized_integral& rhs)
{
	return serialized_integral(uint_or_error(rhs));
}

serialized_integral covert_any_system_ptr(serialized_integral& rhs)
{
	return serialized_integral("system_ptr", rhs.getValue());
}

serialized_integral index_string_int(serialized_integral& lhs, serialized_integral& rhs)
{
	unsigned int index = uint_or_error(rhs);
	if (lhs.getValue().size() >= index) {
		return serialized_integral();
	}
	return lhs.getValue().at(index);
}

serialized_integral index_string_string(serialized_integral& lhs, serialized_integral& rhs)
{
	unsigned int found = lhs.getValue().find(rhs.getValue());
	if (found != std::string::npos) {
		return serialized_integral(found);
	}
	return serialized_integral(-1);
}


serialized_integral index_group_ptr_string(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 2) {
		throw runtime_exception("invalid number of aruguments; expected 2 but got " + std::to_string(args.size()));
	}
	return i->get_from_container(args.at(0), args.at(1).getValue());
}

serialized_integral index_group_ptr_int(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 2) {
		throw runtime_exception("invalid number of aruguments; expected 2 but got " + std::to_string(args.size()));
	}
	return i->get_from_container(args.at(0), args.at(1).getValue());
}


serialized_integral std_print(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() > 0) {
		std::cout << args.at(0).getValue();
		for (unsigned int i{ 1 }; i < args.size(); i++) {
			std::cout << ", " << args.at(i).getValue();
		}
	}
	return serialized_integral();
}

serialized_integral std_get_timestamp(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 0) {
		throw runtime_exception("expected 0 arguments but got " + std::to_string(args.size()));
	}
	const auto p1 = std::chrono::system_clock::now();
	return serialized_integral((int)std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count());
}

serialized_integral std_relpath(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 1) {
		throw runtime_exception("expected 1 arguments but got " + std::to_string(args.size()));
	}
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.at(0).getType() != si_types().STRING()) {
		throw runtime_exception("expected argument of type string but got " + args.at(0).toString());
	}
	std::string wd = i->dcontext()->as_string_or_default("wd", "");
	return serialized_integral(wd.empty() ? args.at(0).getValue() : wd + "\\" + args.at(0).getValue(), false);
}


serialized_integral std_includepath(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 1) {
		throw runtime_exception("expected 1 arguments but got " + std::to_string(args.size()));
	}
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.at(0).getType() != si_types().STRING()) {
		throw runtime_exception("expected argument of type string but got " + args.at(0).toString());
	}
	std::string includeDir = i->dcontext()->as_string_or_default("include_dir", "");
	return serialized_integral(includeDir.empty() ? args.at(0).getValue() : includeDir + "\\" + args.at(0).getValue(), false);
}



serialized_integral std_getch(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 0) {
		throw runtime_exception("expected 0 arguments but got " + std::to_string(args.size()));
	}
	return serialized_integral(static_cast<char>(_getch()));
}

serialized_integral std_current_path(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 0) {
		throw runtime_exception("expected 0 arguments but got " + std::to_string(args.size()));
	}
	return serialized_integral(FileHandle().current_path(), false);
}

serialized_integral std_parent_path(serialized_integral& arg)
{
	if (arg.getType() != si_types().STRING()) {
		throw runtime_exception("expect argument of type '" + si_types().STRING() + "' but got " + arg.toString());
	}
	return serialized_integral(FileHandle().parent_path(arg.getValue()), false);
}

serialized_integral std_file_exists(serialized_integral& rhs)
{
	return serialized_integral(FileHandle().exists(rhs.getValue()));
}

serialized_integral std_file_size(serialized_integral& rhs)
{
	/* TODO: need to change this to support long long type*/
	try {
		return serialized_integral(static_cast<unsigned int>(FileHandle().file_size(rhs.getValue())));
	}
	catch (std::filesystem::filesystem_error fe) {
		return serialized_integral(fe.what(), false);
	}
}

serialized_integral std_file_rename(serialized_integral& lhs, serialized_integral& rhs)
{
	try {
		FileHandle().rename(lhs.getValue(), rhs.getValue());
	}
	catch (std::filesystem::filesystem_error fe) {
		return serialized_integral(fe.what(), false);
	}
	return serialized_integral(false);
}

serialized_integral std_file_copy(serialized_integral& lhs, serialized_integral& rhs)
{
	try {
		FileHandle().copyFile(lhs.getValue(), rhs.getValue());
	}
	catch (std::filesystem::filesystem_error fe) {
		return serialized_integral(fe.what(), false);
	}
	return serialized_integral(false);
}

serialized_integral std_file_remove(serialized_integral& lhs)
{
	try {
		bool ret = FileHandle().removeAll(lhs.getValue());
		return serialized_integral(ret);
	}
	catch (std::filesystem::filesystem_error fe) {
		return serialized_integral(fe.what(), false);
	}
	return serialized_integral(false);
}

serialized_integral std_file_readline(serialized_integral& lhs, serialized_integral& rhs) {
	if (rhs.getType() != si_types().INT()) {
		throw runtime_exception("expected argument 2 to be of type INT or UINT but got " + rhs.getType());
	}
	std::string data = FileHandle().readLine(lhs.getValue(), uint_or_error(rhs));
	return serialized_integral(data, false);
}

serialized_integral std_file_countlines(serialized_integral& lhs) {
	unsigned int lineCount = FileHandle().countLines(lhs.getValue());
	return serialized_integral(lineCount);
}

serialized_integral std_run_query(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() == 1) {
		if (args.at(0).getType() != si_types().STRING()) {
			throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
		}
		return i->run_query(args.at(0).getValue());
	}
	if (args.size() == 2) {
		if (args.at(0).getType() != si_types().STRING()) {
			throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
		}
		if (args.at(1).getType() != "group_ptr") {
			throw runtime_exception("expected argument 2 of type group_ptr but got " + args.at(0).toString());
		}
		return i->run_map_query(args.at(0).getValue(), args.at(1));
	}
	else {
		throw runtime_exception("expected 2 arguments but got " + std::to_string(args.size()));
	}
}

serialized_integral std_run_prepared_query(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() == 0) {
		throw runtime_exception("expected more than 0 arguments");
	}

	if (args.at(0).getType() != si_types().STRING()) {
		throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
	}

	if (args.size() > 1) {
		if (args.at(0).getType() != si_types().STRING()) {
			throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
		}
		std::vector<serialized_integral> parameters;

		if (args.at(1).getType() != "group_ptr") {
			for (unsigned int i{ 1 }; i < args.size(); i++) {
				parameters.push_back(args.at(i));
			}
			return i->run_prepared_query(args.at(0).getValue(), parameters);
		}
		else {
			for (unsigned int i{ 2 }; i < args.size(); i++) {
				parameters.push_back(args.at(i));
			}
			return i->run_map_prepared_query(args.at(0).getValue(), args.at(1), parameters, false);
		}
	}
	else {
		throw runtime_exception("expected at least 2 arguments but got " + std::to_string(args.size()));
	}
}

serialized_integral std_run_prepared_query_include_unexpected(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() == 0) {
		throw runtime_exception("expected more than 0 arguments");
	}

	if (args.at(0).getType() != si_types().STRING()) {
		throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
	}

	if (args.size() >= 1) {
		if (args.at(0).getType() != si_types().STRING()) {
			throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
		}
		// use a fake serialized_integral of type group_ptr since a mapper is not needed for include_unexpected
		serialized_integral fake_mapper;

		std::vector<serialized_integral> parameters;
		for (unsigned int i{ 1 }; i < args.size(); i++) {
			parameters.push_back(args.at(i));
		}
		return i->run_map_prepared_query(args.at(0).getValue(), fake_mapper, parameters, true);
	}
	else {
		throw runtime_exception("expected at least 1 argument but got " + std::to_string(args.size()));
	}
}


serialized_integral xor_int_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) ^ int_or_error(rhs));
}

serialized_integral xor_int_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(int_or_error(lhs) ^ char_or_error(rhs));
}

serialized_integral xor_char_int(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(char_or_error(lhs) ^ int_or_error(rhs));
}

serialized_integral xor_char_char(serialized_integral& lhs, serialized_integral& rhs)
{
	return serialized_integral(static_cast<char>(char_or_error(lhs) ^ char_or_error(rhs)));
}

serialized_integral xor_string_string(serialized_integral& lhs, serialized_integral& rhs)
{
	std::string szLhs = lhs.getValue();
	std::string szRhs = rhs.getValue();
	if (szRhs.size() < szLhs.size()) {
		throw runtime_exception("rhs length must be greater than lhs length to perform operation 'xor'");
	}
	for (unsigned int i{ 0 }; i < szLhs.size(); i++) {
		szLhs[i] ^= szRhs.at(i);
	}
	return serialized_integral(szLhs, false);
}


serialized_integral std_string_length(serialized_integral& rhs) {
	return serialized_integral(rhs.getValue().length());
}

serialized_integral std_string_substring(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() != 3) {
		throw runtime_exception("expected 3 arguments but got "+std::to_string(args.size()));
	}

	if (args.at(0).getType() != si_types().STRING()) {
		throw runtime_exception("expected argument 1 of type string but got " + args.at(0).toString());
	}

	if (args.at(1).getType() != si_types().UINT() && args.at(1).getType() != si_types().INT()) {
		throw runtime_exception("expected argument 2 of type int or uint but got " + args.at(1).toString());
	}

	if (args.at(2).getType() != si_types().UINT() && args.at(2).getType() != si_types().INT()) {
		throw runtime_exception("expected argument 3 of type int or uint but got " + args.at(2).toString());
	}

	return serialized_integral(args.at(0).getValue().substr(uint_or_error(args.at(1)), uint_or_error(args.at(2))), false);
}


std::map<std::string, std::shared_ptr<callable>> get_std_operators()
{
	std::map<std::string, std::shared_ptr<callable>> operators;
	operators["operator|(any)"] = std::make_shared<unary_operator>(typeof_any);
	operators["operator+(int,int)"] = std::make_shared<binary_operator>(add_int_int);
	operators["operator+(int,float)"] = std::make_shared<binary_operator>(add_int_float);
	operators["operator+(int,double)"] = std::make_shared<binary_operator>(add_int_double);
	operators["operator+(float,int)"] = std::make_shared<binary_operator>(add_float_int);
	operators["operator+(float,float)"] = std::make_shared<binary_operator>(add_float_float);
	operators["operator+(float,double)"] = std::make_shared<binary_operator>(add_float_double);
	operators["operator+(double,int)"] = std::make_shared<binary_operator>(add_double_int);
	operators["operator+(double,float)"] = std::make_shared<binary_operator>(add_double_float);
	operators["operator+(double,double)"] = std::make_shared<binary_operator>(add_double_double);

	operators["operator-(int,int)"] = std::make_shared<binary_operator>(sub_int_int);
	operators["operator-(int,float)"] = std::make_shared<binary_operator>(sub_int_float);
	operators["operator-(int,double)"] = std::make_shared<binary_operator>(sub_int_double);
	operators["operator-(float,int)"] = std::make_shared<binary_operator>(sub_float_int);
	operators["operator-(float,float)"] = std::make_shared<binary_operator>(sub_float_float);
	operators["operator-(float,double)"] = std::make_shared<binary_operator>(sub_float_double);
	operators["operator-(double,int)"] = std::make_shared<binary_operator>(sub_double_int);
	operators["operator-(double,float)"] = std::make_shared<binary_operator>(sub_double_float);
	operators["operator-(double,double)"] = std::make_shared<binary_operator>(sub_double_double);

	operators["operator*(int,int)"] = std::make_shared<binary_operator>(multiply_int_int);
	operators["operator*(int,float)"] = std::make_shared<binary_operator>(multiply_int_float);
	operators["operator*(int,double)"] = std::make_shared<binary_operator>(multiply_int_double);
	operators["operator*(float,int)"] = std::make_shared<binary_operator>(multiply_float_int);
	operators["operator*(float,float)"] = std::make_shared<binary_operator>(multiply_float_float);
	operators["operator*(float,double)"] = std::make_shared<binary_operator>(multiply_float_double);
	operators["operator*(double,int)"] = std::make_shared<binary_operator>(multiply_double_int);
	operators["operator*(double,float)"] = std::make_shared<binary_operator>(multiply_double_float);
	operators["operator*(double,double)"] = std::make_shared<binary_operator>(multiply_double_double);

	operators["operator/(int,int)"] = std::make_shared<binary_operator>(divide_int_int);
	operators["operator/(int,float)"] = std::make_shared<binary_operator>(divide_int_float);
	operators["operator/(int,double)"] = std::make_shared<binary_operator>(divide_int_double);
	operators["operator/(float,int)"] = std::make_shared<binary_operator>(divide_float_int);
	operators["operator/(float,float)"] = std::make_shared<binary_operator>(divide_float_float);
	operators["operator/(float,double)"] = std::make_shared<binary_operator>(divide_float_double);
	operators["operator/(double,int)"] = std::make_shared<binary_operator>(divide_double_int);
	operators["operator/(double,float)"] = std::make_shared<binary_operator>(divide_double_float);
	operators["operator/(double,double)"] = std::make_shared<binary_operator>(divide_double_double);

	operators["operator+(string,string)"] = std::make_shared<binary_operator>(add_string_string);
	operators["operator+(string,int)"] = std::make_shared<binary_operator>(add_string_int);
	operators["operator+(string,float)"] = std::make_shared<binary_operator>(add_string_float);
	operators["operator+(string,double)"] = std::make_shared<binary_operator>(add_string_double);
	operators["operator+(string,char)"] = std::make_shared<binary_operator>(add_string_char);
	operators["operator+(string,fn_ptr)"] = std::make_shared<binary_operator>(add_string_fn_ptr);

	operators["operator-(int)"] = std::make_shared<unary_operator>(negate_int);
	operators["operator-(float)"] = std::make_shared<unary_operator>(negate_float);
	operators["operator-(double)"] = std::make_shared<unary_operator>(negate_double);

	operators["operator!(int)"] = std::make_shared<unary_operator>(not_int);
	operators["operator!(float)"] = std::make_shared<unary_operator>(not_float);
	operators["operator!(double)"] = std::make_shared<unary_operator>(not_double);
	operators["operator!(string)"] = std::make_shared<unary_operator>(not_string);
	operators["operator!(char)"] = std::make_shared<unary_operator>(not_char);
	operators["operator!(fn_ptr)"] = std::make_shared<unary_operator>(not_fn_ptr);

	operators["operator==(int,int)"] = std::make_shared<binary_operator>(equalequal_int_int);
	operators["operator==(int,float)"] = std::make_shared<binary_operator>(equalequal_int_float);
	operators["operator==(int,double)"] = std::make_shared<binary_operator>(equalequal_int_double);
	operators["operator==(float,int)"] = std::make_shared<binary_operator>(equalequal_float_int);
	operators["operator==(float,float)"] = std::make_shared<binary_operator>(equalequal_float_float);
	operators["operator==(float,double)"] = std::make_shared<binary_operator>(equalequal_float_double);
	operators["operator==(double,int)"] = std::make_shared<binary_operator>(equalequal_double_int);
	operators["operator==(double,float)"] = std::make_shared<binary_operator>(equalequal_double_float);
	operators["operator==(double,double)"] = std::make_shared<binary_operator>(equalequal_double_double);

	operators["operator!=(int,int)"] = std::make_shared<binary_operator>(notequal_int_int);
	operators["operator!=(int,float)"] = std::make_shared<binary_operator>(notequal_int_float);
	operators["operator!=(int,double)"] = std::make_shared<binary_operator>(notequal_int_double);
	operators["operator!=(float,int)"] = std::make_shared<binary_operator>(notequal_float_int);
	operators["operator!=(float,float)"] = std::make_shared<binary_operator>(notequal_float_float);
	operators["operator!=(float,double)"] = std::make_shared<binary_operator>(notequal_float_double);
	operators["operator!=(double,int)"] = std::make_shared<binary_operator>(notequal_double_int);
	operators["operator!=(double,float)"] = std::make_shared<binary_operator>(notequal_double_float);
	operators["operator!=(double,double)"] = std::make_shared<binary_operator>(notequal_double_double);

	operators["operator<(int,int)"] = std::make_shared<binary_operator>(less_int_int);
	operators["operator<(int,float)"] = std::make_shared<binary_operator>(less_int_float);
	operators["operator<(int,double)"] = std::make_shared<binary_operator>(less_int_double);
	operators["operator<(float,int)"] = std::make_shared<binary_operator>(less_float_int);
	operators["operator<(float,float)"] = std::make_shared<binary_operator>(less_float_float);
	operators["operator<(float,double)"] = std::make_shared<binary_operator>(less_float_double);
	operators["operator<(double,int)"] = std::make_shared<binary_operator>(less_double_int);
	operators["operator<(double,float)"] = std::make_shared<binary_operator>(less_double_float);
	operators["operator<(double,double)"] = std::make_shared<binary_operator>(less_double_double);

	operators["operator<=(int,int)"] = std::make_shared<binary_operator>(lessequal_int_int);
	operators["operator<=(int,float)"] = std::make_shared<binary_operator>(lessequal_int_float);
	operators["operator<=(int,double)"] = std::make_shared<binary_operator>(lessequal_int_double);
	operators["operator<=(float,int)"] = std::make_shared<binary_operator>(lessequal_float_int);
	operators["operator<=(float,float)"] = std::make_shared<binary_operator>(lessequal_float_float);
	operators["operator<=(float,double)"] = std::make_shared<binary_operator>(lessequal_float_double);
	operators["operator<=(double,int)"] = std::make_shared<binary_operator>(lessequal_double_int);
	operators["operator<=(double,float)"] = std::make_shared<binary_operator>(lessequal_double_float);
	operators["operator<=(double,double)"] = std::make_shared<binary_operator>(lessequal_double_double);

	operators["operator>(int,int)"] = std::make_shared<binary_operator>(greater_int_int);
	operators["operator>(int,float)"] = std::make_shared<binary_operator>(greater_int_float);
	operators["operator>(int,double)"] = std::make_shared<binary_operator>(greater_int_double);
	operators["operator>(float,int)"] = std::make_shared<binary_operator>(greater_float_int);
	operators["operator>(float,float)"] = std::make_shared<binary_operator>(greater_float_float);
	operators["operator>(float,double)"] = std::make_shared<binary_operator>(greater_float_double);
	operators["operator>(double,int)"] = std::make_shared<binary_operator>(greater_double_int);
	operators["operator>(double,float)"] = std::make_shared<binary_operator>(greater_double_float);
	operators["operator>(double,double)"] = std::make_shared<binary_operator>(greater_double_double);

	operators["operator>=(int,int)"] = std::make_shared<binary_operator>(greaterequal_int_int);
	operators["operator>=(int,float)"] = std::make_shared<binary_operator>(greaterequal_int_float);
	operators["operator>=(int,double)"] = std::make_shared<binary_operator>(greaterequal_int_double);
	operators["operator>=(float,int)"] = std::make_shared<binary_operator>(greaterequal_float_int);
	operators["operator>=(float,float)"] = std::make_shared<binary_operator>(greaterequal_float_float);
	operators["operator>=(float,double)"] = std::make_shared<binary_operator>(greaterequal_float_double);
	operators["operator>=(double,int)"] = std::make_shared<binary_operator>(greaterequal_double_int);
	operators["operator>=(double,float)"] = std::make_shared<binary_operator>(greaterequal_double_float);
	operators["operator>=(double,double)"] = std::make_shared<binary_operator>(greaterequal_double_double);

	operators["operator==(string,string)"] = std::make_shared<binary_operator>(equalequal_string_string);
	operators["operator==(string,char)"] = std::make_shared<binary_operator>(equalequal_string_char);
	operators["operator==(char,string)"] = std::make_shared<binary_operator>(equalequal_char_string);
	operators["operator==(char,char)"] = std::make_shared<binary_operator>(equalequal_char_char);

	operators["operator->(int,float)"] = std::make_shared<unary_operator>(convert_int_float);
	operators["operator->(int,double)"] = std::make_shared<unary_operator>(convert_int_double);
	operators["operator->(int,uint)"] = std::make_shared<unary_operator>(convert_int_uint);
	operators["operator->(float,int)"] = std::make_shared<unary_operator>(convert_float_int);
	operators["operator->(float,double)"] = std::make_shared<unary_operator>(convert_float_double);
	operators["operator->(double,int)"] = std::make_shared<unary_operator>(convert_double_int);
	operators["operator->(double,float)"] = std::make_shared<unary_operator>(convert_double_float);

	operators["operator->(int,string)"] = std::make_shared<unary_operator>(convert_int_string);
	operators["operator->(float,string)"] = std::make_shared<unary_operator>(convert_float_string);
	operators["operator->(double,string)"] = std::make_shared<unary_operator>(convert_double_string);
	operators["operator->(char,string)"] = std::make_shared<unary_operator>(convert_char_string);
	operators["operator->(int,char)"] = std::make_shared<unary_operator>(convert_int_char);

	operators["operator->(string,char)"] = std::make_shared<unary_operator>(convert_string_char);
	operators["operator->(string,int)"] = std::make_shared<unary_operator>(convert_string_int);
	operators["operator->(string,float)"] = std::make_shared<unary_operator>(convert_string_float);
	operators["operator->(string,double)"] = std::make_shared<unary_operator>(convert_string_double);

	operators["operator->(any,int)"] = std::make_shared<unary_operator>(covert_any_int);
	operators["operator->(any,float)"] = std::make_shared<unary_operator>(covert_any_float);
	operators["operator->(any,double)"] = std::make_shared<unary_operator>(covert_any_double);
	operators["operator->(any,char)"] = std::make_shared<unary_operator>(covert_any_char);
	operators["operator->(any,string)"] = std::make_shared<unary_operator>(covert_any_string);
	operators["operator->(any,fn_ptr)"] = std::make_shared<unary_operator>(covert_any_fn_ptr);
	operators["operator->(any,group_ptr)"] = std::make_shared<unary_operator>(covert_any_group_ptr);
	operators["operator->(any,uint)"] = std::make_shared<unary_operator>(covert_any_uint);
	operators["operator->(any,system_ptr)"] = std::make_shared<unary_operator>(covert_any_system_ptr);

	operators["operator->(int,any)"] = std::make_shared<unary_operator>(covert_int_any);
	operators["operator->(float,any)"] = std::make_shared<unary_operator>(covert_float_any);
	operators["operator->(double,any)"] = std::make_shared<unary_operator>(covert_double_any);
	operators["operator->(char,any)"] = std::make_shared<unary_operator>(covert_char_any);
	operators["operator->(string,any)"] = std::make_shared<unary_operator>(covert_string_any);
	operators["operator->(fn_ptr,any)"] = std::make_shared<unary_operator>(covert_fn_ptr_any);
	operators["operator->(group_ptr,any)"] = std::make_shared<unary_operator>(covert_group_ptr_any);
	operators["operator->(uint,any)"] = std::make_shared<unary_operator>(covert_uint_any);

	operators["operator->(uint,int)"] = std::make_shared<unary_operator>(convert_uint_int);
	operators["operator->(uint,float)"] = std::make_shared<unary_operator>(convert_uint_float);
	operators["operator->(uint,double)"] = std::make_shared<unary_operator>(convert_uint_double);
	operators["operator->(uint,string)"] = std::make_shared<unary_operator>(convert_uint_string);

	operators["operator[(group_ptr,string)"] = std::make_shared<native_fn>(index_group_ptr_string);
	operators["operator[(group_ptr,int)"] = std::make_shared<native_fn>(index_group_ptr_int);
	operators["operator[(string,int)"] = std::make_shared<binary_operator>(index_string_int);
	operators["operator[(string,string)"] = std::make_shared<binary_operator>(index_string_string);


	operators["operator^(int,int)"] = std::make_shared<binary_operator>(xor_int_int);
	operators["operator^(int,char)"] = std::make_shared<binary_operator>(xor_int_char);
	operators["operator^(char,int)"] = std::make_shared<binary_operator>(xor_char_int);
	operators["operator^(char,char)"] = std::make_shared<binary_operator>(xor_char_char);
	operators["operator^(string,string)"] = std::make_shared<binary_operator>(xor_string_string);


	operators["output"] = std::make_shared<native_fn>(std_print);
	operators["timestamp"] = std::make_shared<native_fn>(std_get_timestamp);
	operators["relpath"] = std::make_shared<native_fn>(std_relpath);
	operators["include"] = std::make_shared<native_fn>(std_includepath);
	operators["getch"] = std::make_shared<native_fn>(std_getch);
	operators["file_current_path"] = std::make_shared<native_fn>(std_current_path);
	operators["file_parent_path"] = std::make_shared<unary_operator>(std_parent_path);
	operators["file_size"] = std::make_shared<unary_operator>(std_file_size);
	operators["file_exists"] = std::make_shared<unary_operator>(std_file_exists);
	operators["file_rename"] = std::make_shared<binary_operator>(std_file_rename);
	operators["file_copy"] = std::make_shared<binary_operator>(std_file_copy);
	operators["file_remove"] = std::make_shared<unary_operator>(std_file_remove);
	operators["file_readline"] = std::make_shared<binary_operator>(std_file_readline);
	operators["file_countlines"] = std::make_shared<unary_operator>(std_file_countlines);
	operators["run_query"] = std::make_shared<native_fn>(std_run_query);
	operators["run_prepared_query"] = std::make_shared<native_fn>(std_run_prepared_query);
	operators["run_prepared_query_gather_all"] = std::make_shared<native_fn>(std_run_prepared_query_include_unexpected);
	operators["strlen"] = std::make_shared<unary_operator>(std_string_length);
	operators["substr"] = std::make_shared<native_fn>(std_string_substring);

	return operators;
}