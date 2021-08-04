#pragma once


#include <string>
#include <map>


template<typename T>
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<T>& ptr) {
	if (ptr == nullptr) {
		out << "null";
		return out;
	}
	else {
		out << *ptr;
		return out;
	}
}


template <class _Ty>
class scope {
public:
	scope(): m_id{ 0 } {}
	scope(unsigned int id) :m_id{ id } {}
	scope(const std::string& szAlias): m_id{ 0 }, m_szAlias{ szAlias } {}

	scope(std::map<std::string, _Ty> lookup, const std::string& szAlias)
		:m_id{ 0 }, m_szAlias{ szAlias }
	{
		m_lookup = lookup;
	}

	~scope() {}

	bool exists_local(const std::string& szKey)
	{
		return m_lookup.count(szKey);
	}

	bool define(const std::string& szKey, _Ty val, bool overwrite = false)
	{
		if (overwrite) {
			m_lookup[szKey] = val;
			return true;
		}
		else {
			if (!exists_local(szKey)) {
				m_lookup[szKey] = val;
				return true;
			}
			return false;
		}
	}

	bool assign(const std::string& szKey, _Ty val, bool typeRestrict = true)
	{
		if (exists_local(szKey)) {
			if (typeRestrict) {
				/*
				if (m_lookup[szKey].getType() == val.getType()) {
					m_lookup[szKey] = val;
					return true;
				}
				else {
					return false;
				}
				*/
				m_lookup[szKey] = val;
				return true;
			}
			else {
				m_lookup[szKey] = val;
				return true;
			}
		}
		return false;
	}

	bool get(const std::string& szKey, _Ty& out)
	{
		if (exists_local(szKey)) {
			out = m_lookup[szKey];
			return true;
		}
		return false;
	}


	unsigned int id() const
	{
		return m_id;
	}

	void set_id(unsigned int id)
	{
		m_id = id;
	}

	std::string toString(const std::string& exclude = "") const
	{
		std::ostringstream oss;
		oss << "<" << (m_szAlias.empty() ? "scopeId" : m_szAlias) << ": " << m_id << "> {";
		for (auto it = m_lookup.begin(); it != m_lookup.end(); ++it) {
			if (exclude.empty()) {
				oss << "\n\t\t" << it->first << " := " << it->second << ",";
			}
			else {
				if (!contains(it->first, exclude)) {
					oss << "\n\t\t" << it->first << " := " << it->second << ",";
				}
			}
		}
		oss << "}</" << m_szAlias << ">";
		return oss.str();
	}

	std::string toStringIndented(const std::string& exclude) const
	{
		std::ostringstream oss;
		oss << "<" << (m_szAlias.empty() ? "scopeId" : m_szAlias) << ": " << m_id << "> {";
		for (auto it = m_lookup.begin(); it != m_lookup.end(); ++it) {
			if (exclude.empty()) {
				oss << "\n\t\t\t" << it->first << " := " << it->second << ",";
			}
			else {
				if (!contains(it->first, exclude)) {
					oss << "\n\t\t\t" << it->first << " := " << it->second << ",";
				}
			}
		}
		oss << "\n\t\t}</" << m_szAlias << ">";
		return oss.str();
	}



private:
	std::string m_szAlias{ "" };
	unsigned int m_id{ 0 };
	std::map<std::string, _Ty> m_lookup;
};