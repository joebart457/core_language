#pragma once

#include "exceptions.hpp"


template <class _Ty>
class list_crawler
{
public:
	list_crawler(std::vector<_Ty>& ls)
		:m_list{ ls }
	{
		begin();
	}

	~list_crawler() {}


	_Ty current()
	{
		check_index();
		return m_list.at(m_index);
	}

	_Ty back()
	{
		if (m_list.size() > 0) {
			return m_list.at(m_list.size() - 1);
		}
		else {
			throw system_exception("current index out of range [" + std::to_string(m_list.size() - 1) + "] when size was ["
				+ std::to_string(m_list.size()) + "] at check_index of rlist_crawler<_Ty>");
		}
	}

	void begin()
	{
		m_index = 0;
	}

	_Ty next()
	{
		m_index++;
		return current();
	}

	bool end()
	{
		return m_index == m_list.size();
	}


private:

	void check_index()
	{
		if (!(m_index < m_list.size())) {
			throw system_exception("current index out of range ["
				+ std::to_string(m_index) + "] when size was [" + std::to_string(m_list.size()) + "] at check_index of list_crawler<_Ty>");
		}
	}

	unsigned int m_index{ 0 };
	std::vector<_Ty> m_list;
};


template <class _Ty>
class rlist_crawler
{
public:
	rlist_crawler(std::vector<_Ty>& ls)
		:m_list{ ls }
	{
		begin();
	}
	~rlist_crawler() {}


	_Ty current()
	{
		check_index();
		return m_list.at(m_index);
	}

	_Ty back()
	{
		if (0 < m_list.size()) {
			return m_list.at(0);
		}
		else {
			throw system_exception("current index out of range [0] when size was [" + std::to_string(m_list.size()) + "] at check_index of rlist_crawler<_Ty>");
		}
	}

	void begin()
	{
		m_index = m_list.size();
	}

	_Ty next()
	{
		m_index--;
		return current();
	}


	bool end()
	{
		return m_index == 0;
	}


private:

	void check_index()
	{
		if (!(m_index < m_list.size())) {
			throw system_exception("current index out of range ["
				+ std::to_string(m_index) + "] when size was [" + std::to_string(m_list.size()) + "] at check_index of rlist_crawler<_Ty>");
		}
	}

	unsigned int m_index{ 0 };
	std::vector<_Ty> m_list;
};