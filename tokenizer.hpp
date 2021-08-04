#pragma once


#pragma once
#pragma once

#ifndef __INCLUDE_UTOKENIZER_H
#define __INCLUDE_UTOKENIZER_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

#define TOKEN_TYPE_STRING_ENCLOSING				"tt_str_enclosing"
#define TOKEN_TYPE_EOL_COMMENT					"tt_eol_comment"
#define TOKEN_TYPE_EOF							"tt_eof"
#define TOKEN_TYPE_WORD							"tt_word"
#define TOKEN_TYPE_STRING						"tt_string"
#define TOKEN_TYPE_DOUBLE						"tt_double"
#define TOKEN_TYPE_FLOAT						"tt_float"
#define TOKEN_TYPE_INTEGER						"tt_integer"
#define TOKEN_TYPE_INJECTED_STRING				"tt_string_injected"
#define TOKEN_TYPE_INJECTED_STRING_ENCLOSING	"tt_string_injected_enclosing"

class tokenizer_rule {
public:
	tokenizer_rule(const std::string& ty, const std::string& val)
		:_ty{ ty }, _val{ val }, _len{ val.size() }{}
	~tokenizer_rule() {}

	std::string getType()const { return _ty; }
	std::string getValue() const { return _val; }
	unsigned int length() { return _len; }
	friend bool operator<(const tokenizer_rule& lhs, const tokenizer_rule& rhs) {
		return lhs._len < rhs._len;
	}

	friend bool operator>(const tokenizer_rule& lhs, const tokenizer_rule& rhs) {
		return lhs._len > rhs._len;
	}
private:
	std::string _ty;
	std::string _val;
	unsigned int _len{ 0 };
};

class token
{
public:
	token() {}
	token(std::string type, std::string lexeme, unsigned int row = 0, unsigned int column = 0)
		:m_szType{ type }, m_szLexeme{ lexeme }, m_nRow{ row }, m_nColumn{ column }{}
	token(const tokenizer_rule& rule, unsigned int row = 0, unsigned int column = 0)
		:m_szType{ rule.getType() }, m_szLexeme{ rule.getValue() }, m_nRow{ row }, m_nColumn{ column }{}
	~token() {}

	//	Setters
	inline void type(std::string type) {
		m_szType = type;
	}
	inline void lexeme(std::string lexeme) {
		m_szLexeme = lexeme;
	}
	inline void row(unsigned int row) {
		m_nRow = row;
	}
	inline void column(unsigned int column) {
		m_nColumn = column;
	}

	//	Getters

	inline std::string type() {
		return m_szType;
	}
	inline std::string lexeme() {
		return m_szLexeme;
	}
	inline unsigned int row() {
		return m_nRow;
	}
	inline unsigned int column() {
		return m_nColumn;
	}

	//	Methods

	inline std::string toStr() {
		std::string result = m_szType + "{ " + m_szLexeme + " }"
			+ "[" + std::to_string(m_nColumn) + "][" + std::to_string(m_nRow) + "]";
		return result;
	}

	inline token* combine(token* rhs) {
		row(rhs->row());
		column(rhs->column());
		type(type() + rhs->type());
		lexeme(lexeme() + rhs->lexeme());
		return this;
	}

	friend std::ostream& operator<<(std::ostream& out, token rhs)
	{
		out << "{[" << rhs.m_nRow << "][" << rhs.m_nColumn << "] " << rhs.m_szType << "|" << rhs.m_szLexeme << "}";
		return out;
	}


protected:
	std::string m_szLexeme{ "" }, m_szType{ "" };
	unsigned int m_nRow{ 0 }, m_nColumn{ 0 };
};



class tokenizer
{
public:
	tokenizer(const std::vector<tokenizer_rule>& rules)
	{
		m_rules = rules;
		std::sort(m_rules.begin(), m_rules.end(), std::greater<>());
	}
	~tokenizer() {}

	std::vector<token> tokenize(std::string text, bool debug = false, std::string delim = ",")
	{
		if (!init(text)) {
			return {};
		}
		token tok;
		while (!m_bAtEnd) {
			tok = next();
			if (debug) {
				std::cout << tok << delim;
			}
			m_tokens.push_back(tok);

		}
		auto tokens = m_tokens;
		reset();
		return tokens;
	}


private:

	//	Process helpers
	bool init(std::string text)
	{
		m_szText = text;
		reset();
		if (m_szText.size() == 0) {
			m_bAtEnd = true;
			return false;
		}
		m_cCurrent = m_szText.at(0);
		return true;
	}


	void reset()
	{
		m_nIndex = 0;
		m_nRow = 0;
		m_nColumn = 0;
		m_tokens.clear();
		m_bAtEnd = false;
	}


	void advance()
	{
		count();
		m_nIndex++;
		if (m_nIndex >= m_szText.size()) {
			m_bAtEnd = true;
			return;
		}
		m_cCurrent = m_szText.at(m_nIndex);
	}


	void advance(unsigned int next)
	{
		for (unsigned int i{ 0 }; i < next; i++) {
			advance();
		}
	}


	void count()
	{
		if (m_cCurrent == '\r' || m_cCurrent == '\n') {
			m_nRow++;
			m_nColumn = 0;
		}
		else if (m_cCurrent == '\t') {
			m_nColumn += 4;
		}
		else {
			m_nColumn++;
		}
	}


	token next()
	{
		bool commentFlag{ false };
		while (!m_bAtEnd) {

			if (m_cCurrent == '\0') {
				advance();
			}
			if (isspace(m_cCurrent)) {
				advance();
				continue;
			}

			if (isdigit(m_cCurrent)) {
				return number();
			}

			if (m_cCurrent == '_' || isalpha(m_cCurrent)) {
				return word();
			}

			if (next(2) == "&*") {
				advance(2);
				return tokenliteral();
			}

			for (unsigned int i{ 0 }; i < m_rules.size(); i++) {
				tokenizer_rule rule = m_rules.at(i);
				if (next(rule.length()) == rule.getValue()) {
					advance(rule.length());

					if (rule.getType() == TOKEN_TYPE_STRING_ENCLOSING) {
						return str(rule.getValue());
					}
					if (rule.getType() == TOKEN_TYPE_INJECTED_STRING_ENCLOSING) {
						token tok = str(rule.getValue(), true);
					}

					if (rule.getType() == TOKEN_TYPE_EOL_COMMENT) {
						commentFlag = true;
						break;
					}

					return token(rule, m_nRow, m_nColumn);
				}
			}

			if (commentFlag) {
				comment();
				continue;
			}

			token result = token(std::string(1, m_cCurrent), std::string(1, m_cCurrent), m_nRow, m_nColumn);
			advance();
			return result;

		}
		return token(TOKEN_TYPE_EOF, TOKEN_TYPE_EOF);
	}
	std::string next(unsigned int peek)
	{
		std::string result{ "" };

		for (unsigned int i{ 0 }; i < peek; i++) {
			if (m_nIndex + i < m_szText.size()) {
				result += m_szText.at(m_nIndex + i);
				continue;
			}
			break;
		}
		return result;
	}

	//	Multi-Token getters
	token str(std::string enclosing, bool injected_str = false)
	{
		std::string result{ "" };
		bool slash{ false };
		while (!m_bAtEnd && (next(enclosing.size()) != enclosing || slash)) {
			if (m_cCurrent == '\\' && !slash) {
				slash = true;
				advance();
				continue;
			}
			if (slash) {
				if (m_cCurrent == 'n') {
					result.push_back('\n');
					advance();
				}
				else if (m_cCurrent == 't') {
					result.push_back('\t');
					advance();
				}
				else if (m_cCurrent == 'r') {
					result.push_back('\r');
					advance();
				}
				else if (m_cCurrent == 'a') {
					result.push_back('\a');
					advance();
				}
				else if (m_cCurrent == 'b') {
					result.push_back('\b');
					advance();
				}
				else if (m_cCurrent == 'v') {
					result.push_back('\v');
					advance();
				}
				else if (m_cCurrent == 'f') {
					result.push_back('\f');
					advance();
				}
				else if (m_cCurrent == '"') {
					result.push_back('\"');
					advance();
				}
				else if (m_cCurrent == '\'') {
					result.push_back('\'');
					advance();
				}
				else if (m_cCurrent == '0') {
					result.push_back('\0');
					advance();
				}
				else if (m_cCurrent == '\\') {
					result.push_back('\\');
					advance();
				}
				else if (m_cCurrent == '?') {
					result.push_back('\?');
					advance();
				}
				else {
					result.push_back('\\');
					result.push_back(m_cCurrent);
					advance();
				}
				slash = false;
				continue;
			}
			else {
				result.push_back(m_cCurrent);
				advance();
				slash = false;
			}
		}

		if (!m_bAtEnd) {
			advance(enclosing.size());
		}
		return token(injected_str? TOKEN_TYPE_INJECTED_STRING : TOKEN_TYPE_STRING, result, m_nRow, m_nColumn);
	}
	token word()
	{
		std::string result{ "" };
		std::string type{ TOKEN_TYPE_WORD };
		while (!m_bAtEnd && (m_cCurrent == '_' || isalnum(m_cCurrent) || m_cCurrent == '\0')) {
			if (m_cCurrent != '\0') {
				result.push_back(m_cCurrent);
			}
			advance();
		}

		for (unsigned int i{ 0 }; i < m_rules.size(); i++) {
			if (result == m_rules.at(i).getValue()) {
				type = m_rules.at(i).getType();
			}
		}

		return token(type, result, m_nRow, m_nColumn);
	}

	token number()
	{
		std::string result{ "" };
		bool hadDecimal{ false };
		bool isFloat{ false };
		while (!m_bAtEnd && (isdigit(m_cCurrent) || (m_cCurrent == '.' && !hadDecimal) || (m_cCurrent == 'f' && hadDecimal))) {
			if (m_cCurrent == '.') {
				hadDecimal = true;
			}
			if (m_cCurrent == 'f') {
				isFloat = true;
				advance();
				break;
			}
			result.push_back(m_cCurrent);
			advance();
		}


		return token(hadDecimal ? (isFloat ? TOKEN_TYPE_FLOAT : TOKEN_TYPE_DOUBLE) : TOKEN_TYPE_INTEGER, result, m_nRow, m_nColumn);
	}


	token tokenliteral()
	{
		std::string result{ "" };
		while (!m_bAtEnd && m_cCurrent != '&') {
			result.push_back(m_cCurrent);
			advance();
		}

		if (!m_bAtEnd) {
			advance();
		}
		return token(TOKEN_TYPE_WORD, result, m_nRow, m_nColumn);
	}


	void comment()
	{
		while (!m_bAtEnd && m_cCurrent != '\n' && m_cCurrent != '\r') {
			advance();
		}
	}

private:
	std::vector<tokenizer_rule> m_rules;
	unsigned int m_nRow{ 0 }, m_nColumn{ 0 }, m_nIndex{ 0 };
	char m_cCurrent{ '`' };
	std::string m_szText;
	std::vector<token> m_tokens;
	bool m_bAtEnd{ false };

};

#endif // end #ifndef __INCLUDE_TOKENIZER_H