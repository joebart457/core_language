#pragma once


#include <memory>
#include <string>
#include <vector>

template <class _Ty>
class expression;
template <class _Ty>
class assignment;
template <class _Ty>
class logic_or;
template <class _Ty>
class logic_and;
template <class _Ty>
class equality;
template <class _Ty>
class comparison;
template <class _Ty>
class term;
template <class _Ty>
class factor;
template <class _Ty>
class unary;
template <class _Ty>
class call;
template <class _Ty>
class get;
template <class _Ty>
class group;
template <class _Ty>
class identifier;
template <class _Ty>
class primary;
template <class _Ty>
class cast;
template <class _Ty>
class valuemacro;
template <class _Ty>
class initializer;
template <class _Ty>
class listinitializer;


template <class _Ty>
class expr_interpreter :
	public std::enable_shared_from_this<expr_interpreter<_Ty>> {
public:
	expr_interpreter() {}
	~expr_interpreter() {}

	virtual _Ty acceptExpression(std::shared_ptr<expression<_Ty>>) = 0;
	virtual _Ty acceptAssignment(std::shared_ptr<assignment<_Ty>>) = 0;
	virtual _Ty acceptLogicOr(std::shared_ptr<logic_or<_Ty>>) = 0;
	virtual _Ty acceptLogicAnd(std::shared_ptr<logic_and<_Ty>>) = 0;
	virtual _Ty acceptEquality(std::shared_ptr<equality<_Ty>>) = 0;
	virtual _Ty acceptComparison(std::shared_ptr<comparison<_Ty>>) = 0;
	virtual _Ty acceptTerm(std::shared_ptr<term<_Ty>>) = 0;
	virtual _Ty acceptFactor(std::shared_ptr<factor<_Ty>>) = 0;
	virtual _Ty acceptUnary(std::shared_ptr<unary<_Ty>>) = 0;
	virtual _Ty acceptCall(std::shared_ptr<call<_Ty>>) = 0;
	virtual _Ty acceptGet(std::shared_ptr<get<_Ty>>) = 0;
	virtual _Ty acceptGroup(std::shared_ptr<group<_Ty>>) = 0;
	virtual _Ty acceptIdentifier(std::shared_ptr<identifier<_Ty>>) = 0;
	virtual _Ty acceptValueMacro(std::shared_ptr<valuemacro<_Ty>>) = 0;
	virtual _Ty acceptCast(std::shared_ptr<cast<_Ty>>) = 0;
	virtual _Ty acceptInitializer(std::shared_ptr<initializer<_Ty>>) = 0;
	virtual _Ty acceptPrimary(std::shared_ptr<primary<_Ty>>) = 0;
	virtual _Ty acceptListInitializer(std::shared_ptr<listinitializer<_Ty>>) = 0;
};

template <class _Ty>
class expression :
	public std::enable_shared_from_this<expression<_Ty>>
{
	friend class expr_interpreter<_Ty>;
	friend class interpreter;
public:
	expression() {}
	~expression() {}
	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptExpression(this->shared_from_this());
	}
};


template <class _Ty>
class assignment :
	public expression<_Ty> {
	friend class interpreter;
public:
	assignment(
		std::shared_ptr<expression<_Ty>> lhs, // parent object <nullable>
		std::string name,					  // name of assignment target variable
		std::shared_ptr<expression<_Ty>> val, // value to assign 
		bool dev_override = false,			  // whether or not assignment target is in dev scope
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, name{ name }, val{ val }, dev_override{ dev_override }, row{ row }, column{ column }{}
	~assignment() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptAssignment(std::static_pointer_cast<assignment<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> lhs{ nullptr };
	std::string name;
	std::shared_ptr<expression<_Ty>> val;
	bool dev_override;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class logic_or :
	public expression<_Ty> {
	friend class interpreter;
public:
	logic_or(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, rhs{ rhs }, row{ row }, column{ column }{}
	~logic_or() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptLogicOr(std::static_pointer_cast<logic_or<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class logic_and :
	public expression<_Ty> {
	friend class interpreter;
public:
	logic_and(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, rhs{ rhs }, row{ row }, column{ column }{}
	~logic_and() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptLogicAnd(std::static_pointer_cast<logic_and<_Ty>>(this->shared_from_this()));
	}


protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class equality :
	public expression<_Ty> {
	friend class interpreter;
public:
	equality(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::string op,						  // comparison to make ie "!="
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, op{ op }, rhs{ rhs }, row{ row }, column{ column }{}
	~equality() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptEquality(std::static_pointer_cast<equality<_Ty>>(this->shared_from_this()));
	}


protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::string op;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class comparison :
	public expression<_Ty> {
	friend class interpreter;
public:
	comparison(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::string op,						  // comparison to make ie "<="
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, op{ op }, rhs{ rhs }, row{ row }, column{ column }{}
	~comparison() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptComparison(std::static_pointer_cast<comparison<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::string op;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class term :
	public expression<_Ty> {
	friend class interpreter;
public:
	term(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::string op,						  // operation to perform ie "+"
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, op{ op }, rhs{ rhs }, row{ row }, column{ column }{}
	~term() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptTerm(std::static_pointer_cast<term<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::string op;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};

template <class _Ty>
class factor :
	public expression<_Ty> {
	friend class interpreter;
public:
	factor(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::string op,						  // operation to perform ie "*" or "/"
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, op{ op }, rhs{ rhs }, row{ row }, column{ column }{}
	~factor() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptFactor(std::static_pointer_cast<factor<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::string op;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class unary :
	public expression<_Ty>{
	friend class interpreter;
public:
	unary(
		std::string op,						  // operation to perform ie "-" or "!"
		std::shared_ptr<expression<_Ty>> rhs, // right hand side
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:op{ op }, rhs{ rhs }, row{ row }, column{ column }{}
	~unary() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptUnary(std::static_pointer_cast<unary<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string op;
	std::shared_ptr<expression<_Ty>> rhs;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class call :
	public expression<_Ty> {
	friend class interpreter;
public:
	call(
		std::shared_ptr<expression<_Ty>> lhs,						// left hand side
		std::vector<std::shared_ptr<expression<_Ty>>> arguments,	// arguments list
		unsigned int row = 0,										// row location of expression
		unsigned int column = 0										// column location of expression
	)
		:lhs{ lhs }, arguments{ arguments }, row{ row }, column{ column }{}
	~call() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptCall(std::static_pointer_cast<call<_Ty>>(this->shared_from_this()));
	}


protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::vector<std::shared_ptr<expression<_Ty>>> arguments;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class cast :
	public expression<_Ty> {
	friend class interpreter;
public:
	cast(
		std::shared_ptr<expression<_Ty>> lhs,						// left hand side
		std::string op,												// operation to execute
		std::string szTypeName,										// type to cast to
		unsigned int row = 0,										// row location of expression
		unsigned int column = 0										// column location of expression
	)
		:lhs{ lhs }, op{ op },  szTypeName{ szTypeName }, row{ row }, column{ column }{}
	~cast() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptCast(std::static_pointer_cast<cast<_Ty>>(this->shared_from_this()));
	}


protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::string op;
	std::string szTypeName;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class get :
	public expression<_Ty> {
	friend class parser;
	friend class interpreter;
public:
	get(
		std::shared_ptr<expression<_Ty>> lhs, // left hand side
		std::string identifier,				  // name of variable to retrieve
		bool dev_override = false,			  // whether or not variable exists in dev scope
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:lhs{ lhs }, identifier{ identifier }, dev_override{ dev_override }, row{ row }, column{ column }{}
	~get() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptGet(std::static_pointer_cast<get<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> lhs;
	std::string identifier;
	bool dev_override;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class group :
	public expression<_Ty> {
	friend class interpreter;
public:
	group(
		std::shared_ptr<expression<_Ty>> expr, // expression to evaluate
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:expr{ expr }, row{ row }, column{ column }{}
	~group() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptGroup(std::static_pointer_cast<group<_Ty>>(this->shared_from_this()));
	}

protected:
	std::shared_ptr<expression<_Ty>> expr;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class identifier :
	public expression<_Ty> {
	friend class parser;
	friend class interpreter;
public:
	identifier(
		std::string name,					  // name of identifier
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:name{ name }, row{ row }, column{ column }{}
	~identifier() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptIdentifier(std::static_pointer_cast<identifier<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string name;
	unsigned int row;
	unsigned int column;
};

template <class _Ty>
class valuemacro :
	public expression<_Ty> {
	friend class parser;
	friend class interpreter;
public:
	valuemacro(
		std::string name,					  // name of identifier
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:name{ name }, row{ row }, column{ column }{}
	~valuemacro() {}

	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptValueMacro(std::static_pointer_cast<valuemacro<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string name;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class initializer :
	public expression<_Ty> {
	friend class parser;
	friend class interpreter;
public:
	initializer(
		std::string szTypeName,											// typename to initialize
		std::vector<std::shared_ptr<expression<_Ty>>> arguments,		// arguments to pass to constructor
		unsigned int row = 0,											// row location of expression
		unsigned int column = 0											// column location of expression
	)
		:szTypeName{ szTypeName }, arguments{ arguments }, row{ row }, column{ column }{}
	~initializer() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptInitializer(std::static_pointer_cast<initializer<_Ty>>(this->shared_from_this()));
	}

protected:
	std::string szTypeName;
	std::vector<std::shared_ptr<expression<_Ty>>> arguments;
	unsigned int row;
	unsigned int column;
};


template <class _Ty>
class listinitializer :
	public expression<_Ty> {
	friend class interpreter;
public:
	listinitializer(
		std::vector<std::shared_ptr<expression<_Ty>>> elements, // list of elements to initialize
		unsigned int row = 0,									// row location of expression
		unsigned int column = 0									// column location of expression
	)
		:elements{ elements }, row{ row }, column{ column }{}
	~listinitializer() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptListInitializer(std::static_pointer_cast<listinitializer<_Ty>>(this->shared_from_this()));
	}

protected:
	std::vector<std::shared_ptr<expression<_Ty>>> elements;
	unsigned int row;
	unsigned int column;
};



template <class _Ty>
class primary :
	public expression<_Ty> {
	friend class interpreter;
public:
	primary(
		_Ty data,							  // Object
		unsigned int row = 0,				  // row location of expression
		unsigned int column = 0				  // column location of expression
	)
		:data{ data }, row{ row }, column{ column }{}
	~primary() {}


	virtual _Ty visit(expr_interpreter<_Ty>* i)
	{
		return i->acceptPrimary(std::static_pointer_cast<primary<_Ty>>(this->shared_from_this()));
	}

	_Ty getData()
	{
		return data;
	}

protected:
	_Ty data;
	unsigned int row;
	unsigned int column;
};


