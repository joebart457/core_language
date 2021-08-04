#pragma once


#include <memory>
#include <string>
#include <vector>

#include "expression.hpp"

template <class _Ty>
class statement;
template <class _Ty>
class ifstmt;
template <class _Ty>
class whilestmt;
template <class _Ty>
class returnstmt;
template <class _Ty>
class functiondecl;
template <class _Ty>
class block;
template <class _Ty>
class exprstmt;
template <class _Ty>
class breakstmt;
template <class _Ty>
class vardecl;
template <class _Ty>
class classdecl;
template <class _Ty>
class importstmt;
template <class _Ty>
class directivestmt;

template <class _Ty>
class stmt_interpreter {
public:
	stmt_interpreter() {}
	~stmt_interpreter() {}

	virtual void acceptStatement(std::shared_ptr<statement<_Ty>>) = 0;
	virtual void acceptVariableDeclaration(std::shared_ptr<vardecl<_Ty>>) = 0;
	virtual void acceptIf(std::shared_ptr<ifstmt<_Ty>>) = 0;
	virtual void acceptWhile(std::shared_ptr<whilestmt<_Ty>>) = 0;
	virtual void acceptReturn(std::shared_ptr<returnstmt<_Ty>>) = 0;
	virtual void acceptFunctionDeclaration(std::shared_ptr<functiondecl<_Ty>>) = 0;
	virtual void acceptClassDeclaration(std::shared_ptr<classdecl<_Ty>>) = 0;
	virtual void acceptBlock(std::shared_ptr<block<_Ty>>) = 0;
	virtual void acceptExpressionStatement(std::shared_ptr<exprstmt<_Ty>>) = 0;
	virtual void acceptBreakStatement(std::shared_ptr<breakstmt<_Ty>>) = 0;
	virtual void acceptImportStatement(std::shared_ptr<importstmt<_Ty>>) = 0;
	virtual void acceptDirectiveStatement(std::shared_ptr<directivestmt<_Ty>>) = 0;
};


template <class _Ty>
class statement :
	public std::enable_shared_from_this<statement<_Ty>>
{
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	statement(unsigned int row, unsigned int column) {}
	~statement() {}
	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptStatement(this->shared_from_this());
	}
protected:
	unsigned int row{ 0 };
	unsigned int column{ 0 };
};


template <class _Ty>
class vardecl :
	public statement<_Ty> {
	friend class stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	vardecl(
		std::string szType,							// type of variable
		std::string szName,							// name of variable
		std::shared_ptr<expression<_Ty>> expr,		// initializer value of variable
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : statement<_Ty>(row, column), szType{ szType }, szName{ szName }, expr{ expr }{}
	~vardecl(){}


	virtual void visit(stmt_interpreter<_Ty>* i) {
		i->acceptVariableDeclaration(std::static_pointer_cast<vardecl<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string szType;
	std::string szName;
	std::shared_ptr<expression<_Ty>> expr;
};


template <class _Ty>
class ifstmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	ifstmt(
		std::shared_ptr<expression<_Ty>> condition,	// conditional expression
		std::shared_ptr<statement<_Ty>> then,		// statement to execute if condition is true
		std::shared_ptr<statement<_Ty>> elseDo,		// else block (nullable)
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : condition{ condition },
		then{ then },
		elseDo{ elseDo },
		statement<_Ty>(row, column){}
	~ifstmt() {}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptIf(std::static_pointer_cast<ifstmt<_Ty>>(this->shared_from_this()));
	}

private:
	std::shared_ptr<expression<_Ty>> condition;
	std::shared_ptr<statement<_Ty>> then;
	std::shared_ptr<statement<_Ty>> elseDo;
};


template <class _Ty>
class whilestmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	whilestmt(
		std::shared_ptr<expression<_Ty>> condition,	// conditional expression
		std::shared_ptr<statement<_Ty>> then,		// statement to execute if condition is true
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : condition{ condition },
		then{ then },
		statement<_Ty>(row, column) {}
	~whilestmt() {}
	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptWhile(std::static_pointer_cast<whilestmt<_Ty>>(this->shared_from_this()));
	}

private:
	std::shared_ptr<expression<_Ty>> condition;
	std::shared_ptr<statement<_Ty>> then;
};

template <class _Ty>
struct param
{
	std::string name;
	std::string type;
	std::shared_ptr<expression<_Ty>> default_value{ nullptr };
};

template <class _Ty>
class block :
	public statement<_Ty> {
	friend class interpreter;
public:
	block(
		std::vector<std::shared_ptr<statement<_Ty>>> body,   // list of statements to execute
		unsigned int row = 0,							// row location of statement
		unsigned int column = 0							// column location of statement
	) : m_body{ body }, statement<_Ty>(row, column) {}

	~block()
	{
		m_body.clear();
	}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptBlock(std::static_pointer_cast<block<_Ty>>(this->shared_from_this()));
	}

protected:
	std::vector<std::shared_ptr<statement<_Ty>>> m_body;
};


template <class _Ty>
class functiondecl :
	public statement<_Ty> {
	friend class interpreter;
public:
	functiondecl(
		std::string szName,								// name of function
		std::vector<param<_Ty>> params,					// list of parameters
		std::shared_ptr<block<_Ty>> body,					// block of code to execute
		unsigned int row = 0,							// row location of statement
		unsigned int column = 0							// column location of statement
	) : m_szName{ szName }, m_params{ params }, m_body{ body }, statement<_Ty>(row, column) {}

	~functiondecl()
	{
		m_params.clear();
	}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptFunctionDeclaration(std::static_pointer_cast<functiondecl<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string m_szName;
	std::vector<param<_Ty>> m_params;
	std::shared_ptr<block<_Ty>> m_body;
};


template <class _Ty>
class classdecl :
	public statement<_Ty> {
	friend class interpreter;
public:
	classdecl(
		std::string szName,								// name of function
		std::shared_ptr<block<_Ty>> body,				// block of code to execute
		unsigned int row = 0,							// row location of statement
		unsigned int column = 0							// column location of statement
	) : m_szName{ szName }, m_body{ body }, statement<_Ty>(row, column) {}

	~classdecl()
	{
		m_params.clear();
	}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptClassDeclaration(std::static_pointer_cast<classdecl<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string m_szName;
	std::vector<param<_Ty>> m_params;
	std::shared_ptr<block<_Ty>> m_body;
};



template <class _Ty>
class returnstmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	returnstmt(
		std::shared_ptr<expression<_Ty>> expr,		// expression to evaluate then return 
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : expr{ expr },
		statement<_Ty>(row, column) {}
	~returnstmt() {}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptReturn(std::static_pointer_cast<returnstmt<_Ty>>(this->shared_from_this()));
	}

private:
	std::shared_ptr<expression<_Ty>> expr;
};


template <class _Ty>
class exprstmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	exprstmt(
		std::shared_ptr<expression<_Ty>> expr,		// expression to evaluate 
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : expr{ expr },
		statement<_Ty>(row, column) {}
	~exprstmt() {}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptExpressionStatement(std::static_pointer_cast<exprstmt<_Ty>>(this->shared_from_this()));
	}

private:
	std::shared_ptr<expression<_Ty>> expr;
};


template <class _Ty>
class breakstmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	breakstmt(
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : statement<_Ty>(row, column) {}
	~breakstmt() {}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptBreakStatement(std::static_pointer_cast<breakstmt<_Ty>>(this->shared_from_this()));
	}

};


template <class _Ty>
class importstmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	importstmt(
		std::shared_ptr<expression<_Ty>> expr,		// expression that evaluates to filename
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : statement<_Ty>(row, column), expr{ expr } {}
	~importstmt() {}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptImportStatement(std::static_pointer_cast<importstmt<_Ty>>(this->shared_from_this()));
	}

private:
	std::shared_ptr<expression<_Ty>> expr;
};


template <class _Ty>
class directivestmt :
	public statement<_Ty> {
	friend stmt_interpreter<_Ty>;
	friend class interpreter;
public:
	directivestmt(
		std::string directive,						// token that represents directive to enable
		unsigned int row = 0,						// row location of statement
		unsigned int column = 0						// column location of statement
	) : statement<_Ty>(row, column), directive{ directive } {}
	~directivestmt() {}

	virtual void visit(stmt_interpreter<_Ty>* i)
	{
		i->acceptDirectiveStatement(std::static_pointer_cast<directivestmt<_Ty>>(this->shared_from_this()));
	}

private:
	std::string directive;
};
