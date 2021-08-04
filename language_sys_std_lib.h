#pragma once


#include "serialized_integral.hpp"
#include "exceptions.hpp"
#include "language_callable.hpp"
#include "interpreter.hpp"


serialized_integral sys_pop_scope(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 0) {
		throw runtime_exception("expected 0 arguments but got " + std::to_string(args.size()));
	}
	if (i == nullptr) {
		throw system_exception("interp was null");
	}
	auto context = i->context();
	if (context == nullptr) {
		throw system_exception("application context in error state");
	}
	context->pop_ar();
	return serialized_integral();
}

serialized_integral sys_push_scope(interpreter* i, std::vector<serialized_integral> args)
{
	if (args.size() != 0) {
		throw runtime_exception("expected 0 arguments but got " + std::to_string(args.size()));
	}
	if (i == nullptr) {
		throw system_exception("interp was null");
	}
	auto context = i->context();
	if (context == nullptr) {
		throw system_exception("application context in error state");
	}
	context->push_ar();
	return serialized_integral();
}


serialized_integral debug_show_context(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() > 1) {
		throw runtime_exception("expected 0 or 1 argument but got " + std::to_string(args.size()));
	}
	if (args.size() == 1) {
		i->output_context(args.at(0).getValue());
	}else{
		i->output_context();
	}
	return serialized_integral();
}


serialized_integral debug_show_dcontext(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() > 1) {
		throw runtime_exception("expected 0 or 1 argument but got " + std::to_string(args.size()));
	}
	if (args.size() == 1) {
		i->output_dcontext(args.at(0).getValue());
	}
	else {
		i->output_dcontext();
	}
	return serialized_integral();
}

serialized_integral reset_callstack_index(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() != 0) {
		throw runtime_exception("expected 0 arguments but got " + std::to_string(args.size()));
	}
	i->set_callstack_index(1); // needs to be 1 because this itself is a call on the stack
	return serialized_integral();

}

serialized_integral set_callstack_index(interpreter* i, std::vector<serialized_integral> args)
{
	if (i == nullptr) {
		throw system_exception("interp was null.");
	}
	if (args.size() != 1) {
		throw runtime_exception("expected 1 argument but got " + std::to_string(args.size()));
	}
	if (args.at(0).getType() != si_types().INT() && args.at(0).getType() != si_types().UINT()) {
		throw runtime_exception("expected int or uint type but got " + args.at(0).toString());
	}
	unsigned int value = uint_or_error(args.at(0));
	i->set_callstack_index(value);
	return serialized_integral();

}


std::map<std::string, std::shared_ptr<callable>> get_system_macros()
{
	std::map<std::string, std::shared_ptr<callable>> system_macros;
	system_macros["pop_scope"] = std::make_shared<native_fn>(sys_pop_scope);
	system_macros["push_scope"] = std::make_shared<native_fn>(sys_push_scope);
	system_macros["show_context"] = std::make_shared<native_fn>(debug_show_context);
	system_macros["show_dcontext"] = std::make_shared<native_fn>(debug_show_dcontext);
	system_macros["reset_callstack_index"] = std::make_shared<native_fn>(reset_callstack_index);
	system_macros["set_callstack_index"] = std::make_shared<native_fn>(set_callstack_index);

	return system_macros;
}