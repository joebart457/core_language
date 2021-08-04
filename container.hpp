#pragma once

#include <vector>
#include "scope.hpp"
#include "exceptions.hpp"

class container {
public:
	container() {}
	container(scope<serialized_integral> s) :
		m_scope{ s } {}
	container(scope<serialized_integral> s, unsigned int scopeId)
	{
		m_scope = s;
		m_scope.set_id(scopeId);
	}

	container(std::vector<serialized_integral> elements)
	{
		for (unsigned int i{ 0 }; i < elements.size(); i++) {
			m_scope.define(std::to_string(i), elements.at(i), true);
		}
		m_scope.define("length", serialized_integral(static_cast<int>(elements.size())), true);
	}

	~container() {}

	serialized_integral get(const std::string& szKey, unsigned int row = 0, unsigned int column = 0)
	{
		serialized_integral val;
		if (m_scope.get(szKey, val)) {
			return val;
		}
		throw runtime_exception("key '" + szKey + "' could not be found", row, column);
	}

	bool exists(const std::string& szKey)
	{
		return m_scope.exists_local(szKey);
	}

	void define(const std::string& szKey, serialized_integral val, bool overwrite = false)
	{
		if (!m_scope.define(szKey, val, overwrite)) {
			throw runtime_exception("unable to define object " + val.toString() + " with key '" + szKey + "' ");
		}
	}

	void assign(const std::string& szKey, serialized_integral val, bool strictMode = true)
	{
		if (m_scope.assign(szKey, val, strictMode)) {
			return;
		}
		throw runtime_exception("unable to assign object " + val.toString() + " with key '" + szKey + "'; key not found or type mismatch");

	}

	scope<serialized_integral> get_scope()
	{
		return m_scope;
	}

	unsigned int scope_id()
	{
		return m_scope.id();
	}

	std::string toString()
	{
		std::ostringstream oss;
		oss << m_scope.toStringIndented("");
		return oss.str();
	}

	friend std::ostream& operator<<(std::ostream& out, container& rhs)
	{
		out << rhs.toString();
		return out;
	}

private:
	scope<serialized_integral> m_scope;
};

