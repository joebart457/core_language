#pragma once

#include <memory>

#include "scope.hpp"
#include "serialized_integral.hpp"
#include "language_callable.hpp"
#include "container.hpp"

struct activation_record {
	unsigned int id{ 0 };
	std::string szAlias;
	scope<serialized_integral> primitives{ "primitives" };
	scope<std::shared_ptr<callable>> callables{ "callables" };
	scope<std::shared_ptr<container>> containers{ "containers" };
};


std::string toString(const activation_record& ar, const std::string& exclude = "")
{
	std::ostringstream oss;
	oss << "<" << ar.szAlias << ":" << ar.id << ">";
	oss << "\r\n\t" << ar.primitives.toString(exclude);
	oss << "\r\n\t" << ar.callables.toString(exclude);
	oss << "\r\n\t" << ar.containers.toString(exclude);
	oss << "\r\n</" << (ar.szAlias.empty() ? ":"+std::to_string(ar.id) : ar.szAlias) << ">";
	return oss.str();
}