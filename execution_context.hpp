#pragma once

#include <iostream>

#include "language_callable.hpp"
#include "container.hpp"
#include "scope.hpp"
#include "statement.hpp"
#include "activation_record.hpp"
#include "exceptions.hpp"
#include "serialized_integral.hpp"

#include "list_crawler.hpp"

class execution_context {
public:
	execution_context(std::shared_ptr<activation_record> ar)
	{
		push_ar(ar);
	}
	~execution_context() {}

	void push_ar(std::string szAlias = "")
	{
		activation_record ar;
		ar.id = m_index;
		ar.szAlias = szAlias;
		push_ar(std::make_shared<activation_record>(ar));
	}

	std::shared_ptr<activation_record> pop_ar()
	{
		if (m_records.size() > 0) {
			std::shared_ptr<activation_record> ar = m_records.at(m_records.size() - 1);
			m_records.pop_back();
			return ar;
		}
		throw system_exception("unable to pop activation record; none exist");
	}

	bool exists_primitive(const std::string& szKey)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			if (crawler.next()->primitives.exists_local(szKey)) {
				return true;
			}
		}
		return false;
	}

	bool exists_callable(const std::string& szKey)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			if (crawler.next()->callables.exists_local(szKey)) {
				return true;
			}
		}
		return false;
	}

	bool exists_container(const std::string& szKey)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			if (crawler.next()->containers.exists_local(szKey)) {
				return true;
			}
		}
		return false;
	}

	serialized_integral get_primitive(const std::string& szKey, unsigned int row = 0, unsigned int column = 0)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			serialized_integral out;
			if (ar->primitives.get(szKey, out)) {
				return out;
			}
		}
		throw runtime_exception("unable to retrieve primitive with key '" + szKey + "'", row, column);
	}

	std::shared_ptr<callable> get_callable(const std::string& szKey, unsigned int row = 0, unsigned int column = 0)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			std::shared_ptr<callable> out{ nullptr };
			if (ar->callables.get(szKey, out)) {
				return out;
			}
		}
		throw runtime_exception("unable to retrieve callable with key '" + szKey + "'", row, column);
	}

	std::shared_ptr<container> get_container(const std::string& szKey, unsigned int row = 0, unsigned int column = 0)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			std::shared_ptr<container> out;
			if (ar->containers.get(szKey, out)) {
				return out;
			}
		}
		throw runtime_exception("unable to retrieve container with key '" + szKey + "'", row, column);
	}

	void assign(const std::string& szKey, serialized_integral value, bool typeRestrict, unsigned int row = 0, unsigned int column = 0)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			if (ar->primitives.assign(szKey, value, typeRestrict)) {
				return;
			}
		}
		throw runtime_exception("unable to assign value" + value.toString() + " to key '" + szKey + "'; key not found or type mismatch", row, column);
	}

	void assign(const std::string& szKey, std::shared_ptr<callable> value, bool typeRestrict, unsigned int row = 0, unsigned int column = 0)
	{
		if (value == nullptr) {
			throw system_exception("unable to assign to key '" + szKey + "'; value was nullptr", row, column);
		}
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			if (ar->callables.assign(szKey, value, typeRestrict)) {
				return;
			}
		}
		throw runtime_exception("unable to assign to key '" + szKey + "'; key not found or type mismatch", row, column);
	}

	void assign(const std::string& szKey, std::shared_ptr<container> value, bool typeRestrict, unsigned int row = 0, unsigned int column = 0)
	{
		if (value == nullptr) {
			throw system_exception("unable to assign to key '" + szKey + "'; value was nullptr", row, column);
		}
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			if (ar->containers.assign(szKey, value, typeRestrict)) {
				return;
			}
		}
		throw runtime_exception("unable to assign to key '" + szKey + "'; key not found or type mismatch", row, column);
	}

	void define(const std::string& szKey, serialized_integral value, bool overwrite, unsigned int row = 0, unsigned int column = 0)
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			if (ar->primitives.define(szKey, value, overwrite)) {
				return;
			}
		}
		throw runtime_exception("unable to define value" + value.toString() + " to key '" + szKey + "'; key already defined", row, column);
	}

	void define(const std::string& szKey, std::shared_ptr<callable> value, bool overwrite, unsigned int row = 0, unsigned int column = 0)
	{
		if (value == nullptr) {
			throw system_exception("unable to define key '" + szKey + "'; value was nullptr", row, column);
		}
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			if (ar->callables.define(szKey, value, overwrite)) {
				return;
			}
		}
		throw runtime_exception("unable to define key '" + szKey + "'; key already defined", row, column);
	}


	void define(const std::string& szKey, std::shared_ptr<container> value, bool overwrite, unsigned int row = 0, unsigned int column = 0)
	{
		if (value == nullptr) {
			throw system_exception("unable to define key '" + szKey + "'; value was nullptr", row, column);
		}
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		while (!crawler.end()) {
			std::shared_ptr<activation_record> ar = crawler.next();
			if (ar->containers.define(szKey, value, overwrite)) {
				return;
			}
		}
		throw runtime_exception("unable to define key '" + szKey + "'; key already defined", row, column);
	}

	void output(const std::string& exclude = "")
	{
		for (unsigned int i{ 0 }; i < m_records.size(); i++) {
			std::cout << ::toString(*m_records.at(i), exclude) << "\n";
		}
	}
	
	execution_context top()
	{
		rlist_crawler<std::shared_ptr<activation_record>> crawler(m_records);
		return execution_context({ crawler.back() });
	}

private:
	void push_ar(std::shared_ptr<activation_record> ar)
	{
		m_records.push_back(ar);
		m_index++;
	}

	unsigned int m_index{ 0 };
	std::vector<std::shared_ptr<activation_record>> m_records;
};

