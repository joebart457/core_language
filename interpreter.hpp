#pragma once

#include <map>
#include <string>
#include <vector>

#include "statement.hpp"
#include "expression.hpp"
#include "serialized_integral.hpp"
#include "exceptions.hpp"
#include "language_callable.hpp"

#include "scope.hpp"
#include "container.hpp"
#include "execution_context.hpp"
#include "activation_record.hpp"
#include "developer_context.hpp"

#include "ufhndl.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "db_helper.hpp"

class interpreter;

class ud_fn :
	public callable {
public:
	ud_fn(execution_context context, std::vector<param<serialized_integral>> params, std::shared_ptr<block<serialized_integral>> body, unsigned int row = 0, unsigned int column = 0)
		:m_context{ context }, m_params{ params }, m_body{ body }, m_row{ row }, m_column{ column }{}
	~ud_fn() {}


	virtual serialized_integral call(interpreter* i, std::vector<serialized_integral> args);

	execution_context getContext() { return m_context; }
	std::vector<param<serialized_integral>> getParameters() { return m_params; }
	std::shared_ptr<block<serialized_integral>> getBody() { return m_body; }

	unsigned int getRow() { return m_row; }
	unsigned int getColumn() { return m_column; }

protected:
	execution_context m_context;
	std::vector<param<serialized_integral>> m_params;
	std::shared_ptr<block<serialized_integral>> m_body;
	unsigned int m_row = 0;
	unsigned int m_column = 0;

};


#define DEV_FLAG_VERSION							"1.2.3"
#define DEV_FLAG_STRICT_MODE						   true
#define DEV_FLAG_OVERWRITE_EXISTING					  false
#define DEV_FLAG_POINTER_PER_TYPE_LIMIT				   1000
#define DEV_FLAG_PROTO_PREFIX					"__proto__"
#define DEV_FLAG_LIST_PREFIX					 "__list__"
#define DEV_FLAG_QUERY_RESULTS_PREFIX	"__query_results__"
#define DEV_FLAG_WD										 ""
#define DEV_FLAG_MAX_CALLSTACK_INDEX					100
#define DEV_FLAG_UNSAFE_MODE							false

#define DEV_FLAG_EXE_PATH				"C:\\Program Files\\jmy core\\bin\\core.exe"
#define DEV_FLAG_INCLUDE_DIR			"C:\\Program Files\\jmy core\\includes"

class interpreter :
	public stmt_interpreter<serialized_integral>,
	public expr_interpreter<serialized_integral>
{
public:
	interpreter(execution_context context, developer_context dcontext, tokenizer tokenizer, std::string szExecPath = "")
		:m_context{ context }, m_dcontext{ dcontext }, m_tokenizer(tokenizer)
	{
		// Setup dev macros
		m_dcontext.define("version", serialized_integral(DEV_FLAG_VERSION, false), true);
		m_dcontext.define("strict_mode", serialized_integral(DEV_FLAG_STRICT_MODE), true);
		m_dcontext.define("overwrite_existing", serialized_integral(DEV_FLAG_OVERWRITE_EXISTING), true);
		m_dcontext.define("pointer_per_type_limit", serialized_integral(DEV_FLAG_POINTER_PER_TYPE_LIMIT), true);
		m_dcontext.define("proto_prefix", serialized_integral(DEV_FLAG_PROTO_PREFIX, false), true);
		m_dcontext.define("list_prefix", serialized_integral(DEV_FLAG_LIST_PREFIX, false), true);
		m_dcontext.define("query_results_prefix", serialized_integral(DEV_FLAG_QUERY_RESULTS_PREFIX, false), true);
		m_dcontext.define("wd", serialized_integral(m_hFile.current_path(), false), true);
		m_dcontext.define("max_callstack_index", serialized_integral(DEV_FLAG_MAX_CALLSTACK_INDEX), true);
		m_dcontext.define("callstack_index", serialized_integral(static_cast<unsigned int>(0)), true);
		m_dcontext.define("unsafe_mode", serialized_integral(DEV_FLAG_UNSAFE_MODE), true);

		/* types */
		m_dcontext.define("ty_uint", serialized_integral(si_types().UINT(), false), true);
		m_dcontext.define("ty_int", serialized_integral(si_types().INT(), false), true);
		m_dcontext.define("ty_float", serialized_integral(si_types().FLOAT(), false), true);
		m_dcontext.define("ty_double", serialized_integral(si_types().DOUBLE(), false), true);
		m_dcontext.define("ty_char", serialized_integral(si_types().CHAR(), false), true);
		m_dcontext.define("ty_string", serialized_integral(si_types().STRING(), false), true);
		m_dcontext.define("ty_any", serialized_integral(si_types().ANY(), false), true);
		m_dcontext.define("ty_null", serialized_integral(si_types().NULLTYPE(), false), true);
		m_dcontext.define("ty_group_ptr", serialized_integral("group_ptr", false), true);
		m_dcontext.define("ty_fn_ptr", serialized_integral("fn_ptr", false), true);
		m_dcontext.define("ty_system_ptr", serialized_integral("system_ptr", false), true);


		m_dcontext.define("exec_path", serialized_integral(DEV_FLAG_EXE_PATH, false), true);
		m_dcontext.define("include_dir", serialized_integral(DEV_FLAG_INCLUDE_DIR, false), true);
	}

	~interpreter() {}

	serialized_integral interpret_exec(std::vector<std::shared_ptr<statement<serialized_integral>>> stmts, 
		const std::string& szFilePath, 
		const std::string& szEntry, 
		std::vector<serialized_integral> args)
	{
		m_dcontext.define("wd", serialized_integral(szFilePath, false), true);
		for (auto stmt : stmts) {
			acceptStatement(stmt);
		}
		if (m_context.exists_callable(szEntry)) {
			return m_context.get_callable(szEntry)->call(this, args);
		}
		throw system_exception("entry point '" + szEntry + "' not found");
	}


	void interpret(std::vector<std::shared_ptr<statement<serialized_integral>>> stmts)
	{
		for (auto stmt : stmts) {
			acceptStatement(stmt);
		}
	}

	virtual void acceptStatement(std::shared_ptr<statement<serialized_integral>> stmt)
	{
		if (stmt != nullptr) {
			stmt->visit(this);
			return;
		}
		throw system_exception("unable to execute null statement");
	}

	virtual void acceptDirectiveStatement(std::shared_ptr<directivestmt<serialized_integral>> stmt_directive)
	{
		if (stmt_directive->directive == "enablesql") {
			m_db.open();
		}
		else if (stmt_directive->directive == "unsafe_mode") {
			set_unsafe_mode(true);
		}
		else if (stmt_directive->directive == "safe_mode") {
			set_unsafe_mode(false);
		}
		else {
			throw runtime_exception("unknown directive " + stmt_directive->directive, stmt_directive->row, stmt_directive->column);
		}
	}

	virtual void acceptImportStatement(std::shared_ptr<importstmt<serialized_integral>> stmt_import)
	{
		serialized_integral previous_wd = m_dcontext.get_primitive("wd");
		std::string inFile = acceptExpression(stmt_import->expr).getValue();
		m_dcontext.define("wd", serialized_integral(m_hFile.parent_path(inFile), false), true);
		try {
			interpret(m_parser.parse_toplevel(m_tokenizer.tokenize(m_hFile.readFileAsString(inFile))));
			m_dcontext.define("wd", previous_wd, true); // Won't throw runtime_error 
		}
		catch (runtime_exception re) {
			m_dcontext.define("wd", previous_wd, true);
			re.addStackTrace("import " + inFile, stmt_import->row, stmt_import->column);
			throw re;
		}
		
	}


	virtual void acceptVariableDeclaration(std::shared_ptr<vardecl<serialized_integral>> stmt_decl)
	{
		serialized_integral initializer;
		if (stmt_decl->expr == nullptr) {
			initializer = m_context.get_primitive(m_dcontext.as_string_or_default("proto_prefix", DEV_FLAG_PROTO_PREFIX) + stmt_decl->szType);
		}
		else {
			initializer = acceptExpression(stmt_decl->expr);
		}
		m_context.define(stmt_decl->szName, assert_or_convert_type(stmt_decl->szType, initializer), 
			overwrite_existing(),stmt_decl->row, stmt_decl->column);
	}


	virtual void acceptIf(std::shared_ptr<ifstmt<serialized_integral>> stmt_if)
	{
		serialized_integral condition = acceptExpression(stmt_if->condition);
		std::shared_ptr<statement<serialized_integral>> stmt_to_exec = nullptr;
		if (isTruthy(condition)){
			stmt_to_exec = stmt_if->then;
		}
		else {
			stmt_to_exec = stmt_if->elseDo;
		}
		if (stmt_to_exec != nullptr) {
			acceptStatement(stmt_to_exec);
		}
	}


	virtual void acceptWhile(std::shared_ptr<whilestmt<serialized_integral>> stmt_while)
	{
		while (isTruthy(acceptExpression(stmt_while->condition))) {
			try {
				acceptStatement(stmt_while->then);
			}
			catch (break_exception) {
				break;
			}
			catch (return_exception<serialized_integral> ret) {
				throw ret;
			}
			catch(runtime_exception re) {
				re.addStackTrace("whilestmt", stmt_while->row, stmt_while->column);
				throw re;
			}	
		}
	}

	virtual void acceptReturn(std::shared_ptr<returnstmt<serialized_integral>> stmt_return)
	{
		throw return_exception<serialized_integral>(acceptExpression(stmt_return->expr), stmt_return->row, stmt_return->column);
	}


	virtual void acceptFunctionDeclaration(std::shared_ptr<functiondecl<serialized_integral>> decl_fn)
	{
		std::string szSignature = decl_fn->m_szName;
		m_context.define(szSignature, std::make_shared<ud_fn>(m_context, decl_fn->m_params, decl_fn->m_body, decl_fn->row, decl_fn->column), overwrite_existing(), decl_fn->row, decl_fn->column);
	}

	virtual void acceptClassDeclaration(std::shared_ptr<classdecl<serialized_integral>> decl_class)
	{
		std::string szSignature = decl_class->m_szName;
		std::shared_ptr<container> init = std::make_shared<container>(execute_block_keep_scope(decl_class->m_body));
		m_context.define(szSignature, init, strict_mode(), decl_class->row, decl_class->column);
	}

	virtual scope<serialized_integral> execute_block_keep_scope(std::shared_ptr<block<serialized_integral>> stmt_block)
	{
		m_context.push_ar();
		try {
			std::vector<std::shared_ptr<statement<serialized_integral>>> stmts = stmt_block->m_body;
			for (unsigned int i{ 0 }; i < stmts.size(); i++) {
				acceptStatement(stmts.at(i));
			}
			return m_context.pop_ar()->primitives;
		}
		catch (return_exception<serialized_integral> ret) {
			m_context.pop_ar();
			throw ret;
		}
		catch (runtime_exception re) {
			m_context.pop_ar();
			re.addStackTrace("execute block", stmt_block->row, stmt_block->column);
			throw re;
		}
	}


	virtual void acceptBlock(std::shared_ptr<block<serialized_integral>> stmt_block)
	{
		m_context.push_ar();
		try {
			std::vector<std::shared_ptr<statement<serialized_integral>>> stmts = stmt_block->m_body;
			for (unsigned int i{ 0 }; i < stmts.size(); i++) {
				acceptStatement(stmts.at(i));
			}
			m_context.pop_ar();
		}
		catch (break_exception b) {
			m_context.pop_ar();
			throw b;
		}
		catch (return_exception<serialized_integral> ret) {
			m_context.pop_ar();
			throw ret;
		}
		catch (runtime_exception re) {
			m_context.pop_ar();
			re.addStackTrace("execute block", stmt_block->row, stmt_block->column);
			throw re;
		}
	}

	virtual void acceptExpressionStatement(std::shared_ptr<exprstmt<serialized_integral>> stmt_expr)
	{
		acceptExpression(stmt_expr->expr);
	}

	virtual void acceptBreakStatement(std::shared_ptr<breakstmt<serialized_integral>> stmt_break)
	{
		throw break_exception(stmt_break->row, stmt_break->column);
	}

	virtual serialized_integral acceptExpression(std::shared_ptr<expression<serialized_integral>> expr) 
	{
		if (expr != nullptr) {
			return expr->visit(this);
		}
		throw system_exception("expression to evaluate was null");
	}


	virtual serialized_integral acceptAssignment(std::shared_ptr<assignment<serialized_integral>> expr_assignment)
	{
		serialized_integral val = acceptExpression(expr_assignment->val);
		if (expr_assignment->lhs != nullptr) {
			serialized_integral lhs = acceptExpression(expr_assignment->lhs);
			if (lhs.getType() != "group_ptr") {
				throw system_exception("expect group_ptr type in lhs of get", expr_assignment->row, expr_assignment->column);
			}
			std::string szSignature = lhs.getValue();
			if (m_context.exists_container(szSignature)) {
				m_context.get_container(szSignature)->assign(expr_assignment->name, val, strict_mode());
				return val;
			}
			else {
				throw runtime_exception("container with key '" + szSignature + "' not found in current scope", expr_assignment->row, expr_assignment->column);
			}
		}
		if (expr_assignment->dev_override) {
			if (unsafe_mode()) {
				m_dcontext.assign(expr_assignment->name, val, true); // Strict mode always on for dev scope
				return val;
			}
			throw runtime_exception("unable to assign to system variable in safe mode", expr_assignment->row, expr_assignment->column);
		}
		m_context.assign(expr_assignment->name, val, strict_mode());
		return val;
	}

	virtual serialized_integral acceptLogicOr(std::shared_ptr<logic_or<serialized_integral>> expr_or)
	{
		if (isTruthy(acceptExpression(expr_or->lhs)) || isTruthy(acceptExpression(expr_or->rhs))) {
			return serialized_integral(1);
		}
		return serialized_integral(0);
	}

	virtual serialized_integral acceptLogicAnd(std::shared_ptr<logic_and<serialized_integral>> expr_and)
	{
		if (isTruthy(acceptExpression(expr_and->lhs)) && isTruthy(acceptExpression(expr_and->rhs))) {
			return serialized_integral(1);
		}
		return serialized_integral(0);
	}

	virtual serialized_integral acceptEquality(std::shared_ptr<equality<serialized_integral>> expr_equality)
	{
		serialized_integral lhs = acceptExpression(expr_equality->lhs);
		serialized_integral rhs = acceptExpression(expr_equality->rhs);

		std::string szSignature = create_operator_signature(expr_equality->op, { lhs, rhs });
		return m_context.get_callable(szSignature, expr_equality->row, expr_equality->column)->call(this, { lhs, rhs });
	}

	virtual serialized_integral acceptComparison(std::shared_ptr<comparison<serialized_integral>> expr_comparison)
	{
		serialized_integral lhs = acceptExpression(expr_comparison->lhs);
		serialized_integral rhs = acceptExpression(expr_comparison->rhs);
		
		std::string szSignature = create_operator_signature(expr_comparison->op, { lhs, rhs });
		return m_context.get_callable(szSignature, expr_comparison->row, expr_comparison->column)->call(this, { lhs, rhs });
	}

	virtual serialized_integral acceptTerm(std::shared_ptr<term<serialized_integral>> expr_term)
	{
		serialized_integral lhs = acceptExpression(expr_term->lhs);
		serialized_integral rhs = acceptExpression(expr_term->rhs);

		std::string szSignature = create_operator_signature(expr_term->op, { lhs, rhs });
		return m_context.get_callable(szSignature, expr_term->row, expr_term->column)->call(this, { lhs, rhs });
	}


	virtual serialized_integral acceptFactor(std::shared_ptr<factor<serialized_integral>> expr_factor)
	{
		serialized_integral lhs = acceptExpression(expr_factor->lhs);
		serialized_integral rhs = acceptExpression(expr_factor->rhs);

		std::string szSignature = create_operator_signature(expr_factor->op, { lhs, rhs });
		return m_context.get_callable(szSignature, expr_factor->row, expr_factor->column)->call(this, { lhs, rhs });
	}

	virtual serialized_integral acceptValueMacro(std::shared_ptr<valuemacro<serialized_integral>> expr_valuemacro)
	{
		std::string szName = expr_valuemacro->name;
		if (m_dcontext.exists_callable(szName)) {
			return serialized_integral("system_ptr", szName);
		}
		return m_dcontext.get_primitive(szName, expr_valuemacro->row, expr_valuemacro->column);
	}
	
	virtual serialized_integral acceptCast(std::shared_ptr<cast<serialized_integral>> expr_cast)
	{
		serialized_integral lhs = acceptExpression(expr_cast->lhs);

		std::string szSignature = create_cast_operator_signature(expr_cast->op, lhs, expr_cast->szTypeName);
		return m_context.get_callable(szSignature, expr_cast->row, expr_cast->column)->call(this, { lhs });
	}


	virtual serialized_integral acceptInitializer(std::shared_ptr<initializer<serialized_integral>> expr_initializer)
	{
		std::string szTypeName = expr_initializer->szTypeName;
		if (m_context.top().exists_container(szTypeName)) {
			unsigned int pptl = pointer_per_type_limit();
			std::string try_name;
			for (unsigned int i{ 0 }; i < pptl; i++) {
				try_name = szTypeName + "." + std::to_string(i);
				if (!m_context.top().exists_container(try_name)) {

					scope<serialized_integral> s = m_context.top().get_container(szTypeName)->get_scope();
					m_context.top().define(try_name, std::make_shared<container>(s), overwrite_existing(), expr_initializer->row, expr_initializer->column);
					return serialized_integral("group_ptr", try_name);
				}
			}
			throw runtime_exception("unable to define new pointer to type '" + szTypeName + "'; maximum number of pointers to type reached (limit " + std::to_string(pptl) + ")", 
				expr_initializer->row, expr_initializer->column);
		}
		throw runtime_exception("container with typename '" + szTypeName + "' not found in scope", expr_initializer->row, expr_initializer->column);
	}


	virtual serialized_integral acceptUnary(std::shared_ptr<unary<serialized_integral>> expr_unary)
	{
		serialized_integral rhs = acceptExpression(expr_unary->rhs);
	
		std::string szSignature = create_operator_signature(expr_unary->op, { rhs });
		if (m_context.exists_callable(szSignature)) {
			return m_context.get_callable(szSignature, expr_unary->row, expr_unary->column)->call(this, { rhs });
		}
		else {
			szSignature = create_operator_signature(expr_unary->op, { serialized_integral("any", std::string("")) });
			return m_context.get_callable(szSignature, expr_unary->row, expr_unary->column)->call(this, { rhs });
		}
	}

	virtual serialized_integral acceptCall(std::shared_ptr<call<serialized_integral>> expr_call)
	{
		serialized_integral lhs = acceptExpression(expr_call->lhs);
		update_callstack_index(m_nCallStackIndex++);
		if (callstack_index() <= max_callstack_index()) {
			try {
				std::vector<std::shared_ptr<expression<serialized_integral>>> arg_exprs = expr_call->arguments;
				std::vector<serialized_integral> args;
				for (unsigned int i{ 0 }; i < arg_exprs.size(); i++) {
					args.push_back(acceptExpression(arg_exprs.at(i)));
				}
				if (lhs.getType() == "system_ptr") {
					if (m_dcontext.exists_callable(lhs.getValue())) {
						serialized_integral ret = m_dcontext.get_callable(lhs.getValue())->call(this, args);
						update_callstack_index(m_nCallStackIndex--);
						return ret;
					}
					throw system_exception("system_ptr was null: " + lhs.getValue(), expr_call->row, expr_call->column);
				}
				else if (lhs.getType() != "fn_ptr") {
					throw runtime_exception("unable to perform call on non-callable object: " + lhs.toString(), expr_call->row, expr_call->column);
				}
				else {
					serialized_integral ret = m_context.get_callable(lhs.getValue(), expr_call->row, expr_call->column)->call(this, args);
					update_callstack_index(m_nCallStackIndex--);
					return ret;
				}
			}
			catch (runtime_exception re) {
				update_callstack_index(m_nCallStackIndex--);
				re.addStackTrace("call "+lhs.toString(), expr_call->row, expr_call->column);
				throw re;
			}
			catch (system_exception se) {
				update_callstack_index(m_nCallStackIndex--);
				se.addStackTrace("call " + lhs.toString(), expr_call->row, expr_call->column);
				throw se;
			}
		}
		throw system_exception("call stack index " + std::to_string(callstack_index()) + " exceeds maximum of " + std::to_string(max_callstack_index()),
			expr_call->row, expr_call->column);
	}


	virtual serialized_integral acceptGet(std::shared_ptr<get<serialized_integral>> expr_get)
	{
		serialized_integral lhs = acceptExpression(expr_get->lhs);
		if (lhs.getType() != "group_ptr") {
			throw system_exception("expect group_ptr type in lhs of get", expr_get->row, expr_get->column);
		}
		std::string szSignature = lhs.getValue();
		return m_context.get_container(lhs.getValue(), expr_get->row, expr_get->column)->get(expr_get->identifier, expr_get->row, expr_get->column);
	}

	virtual serialized_integral acceptGroup(std::shared_ptr<group<serialized_integral>> expr_group)
	{
		return acceptExpression(expr_group->expr);
	}

	virtual serialized_integral acceptIdentifier(std::shared_ptr<identifier<serialized_integral>> expr_identifier)
	{
		const std::string szName = expr_identifier->name;
		if (m_context.exists_primitive(expr_identifier->name)) {
			return m_context.get_primitive(expr_identifier->name);
		}
		if (m_context.exists_callable(szName)) {
			return serialized_integral("fn_ptr", szName);
		}
		if (m_context.exists_container(szName)) {
			return serialized_integral("group_ptr", szName);
		}
		throw runtime_exception("identifier '" + expr_identifier->name + "' does not exist", expr_identifier->row, expr_identifier->column);
	}

	virtual serialized_integral acceptListInitializer(std::shared_ptr<listinitializer<serialized_integral>> expr_list_init)
	{
		std::vector<serialized_integral> elements;
		for (unsigned int i{0}; i< expr_list_init->elements.size(); i++) {
			elements.push_back(acceptExpression(expr_list_init->elements.at(i)));
		}
		std::string szSignature;
		unsigned int pptl = pointer_per_type_limit();
		for (unsigned int i{ 0 }; i < pptl; i++) {
			szSignature = get_list_type_macro() + std::to_string(i);
			if (!m_context.top().exists_container(szSignature)) {
				m_context.top().define(szSignature, std::make_shared<container>(elements), overwrite_existing(), expr_list_init->row, expr_list_init->column);
				return serialized_integral("group_ptr", szSignature);
			}
		}
		throw runtime_exception("maximum pointer per type limit reached for type list", expr_list_init->row, expr_list_init->column);
	}

	virtual serialized_integral acceptPrimary(std::shared_ptr<primary<serialized_integral>> expr_primary)
	{
		return expr_primary->data;
	}

	
	virtual serialized_integral call_ud_fn(std::shared_ptr<ud_fn> fn, std::vector<serialized_integral> arguments)
	{
		std::vector<param<serialized_integral>> parameters = fn->getParameters();
		if (arguments.size() != parameters.size()) {
			throw runtime_exception("invalid number of arguments: expected " + std::to_string(parameters.size()) + " but got " + std::to_string(arguments.size()), fn->getRow(), fn->getColumn());
		}

		execution_context previous = m_context;
		m_context = fn->getContext();
		m_context.push_ar("fn_scope");
		try {
			for (unsigned int i{ 0 }; i < parameters.size(); i++) {
				m_context.define(parameters.at(i).name, 
					assert_or_convert_type(parameters.at(i).type, arguments.at(i), fn->getRow(), fn->getColumn()),overwrite_existing(),fn->getRow(), fn->getColumn());
			}
			auto stmts = fn->getBody()->m_body;
			for (auto stmt : stmts) {
				acceptStatement(stmt);
			}
			m_context.pop_ar();
			m_context = previous;
			return serialized_integral();
		}
		catch (return_exception<serialized_integral> ret) {
			m_context.pop_ar();
			m_context = previous;
			return ret.value();
		}
		catch (runtime_exception re) {
			m_context.pop_ar();
			m_context = previous;
			re.addStackTrace("call ud function", fn->getRow(), fn->getColumn());
			throw re;
		}
		catch (system_exception se) {
			m_context.pop_ar();
			m_context = previous;
			se.addStackTrace("call userdefined function", fn->getRow(), fn->getColumn());
			throw se;
		}
	}

	serialized_integral get_from_container(serialized_integral lhs, std::string szKey)
	{
		return m_context.get_container(lhs.getValue())->get(szKey);
	}

	execution_context* context()
	{
		return &m_context;
	}

	developer_context* dcontext()
	{
		return &m_dcontext;
	}

	void output_context(const std::string& exclude = "")
	{
		m_context.output(exclude);
	}

	void output_dcontext(const std::string& exclude = "")
	{
		m_dcontext.output(exclude);
	}

	void set_callstack_index(const unsigned int& val)
	{
		m_nCallStackIndex = val;
		update_callstack_index(val);
	}


	serialized_integral run_map_query(const std::string& szQuery, serialized_integral mapper_ptr)
	{
		std::shared_ptr<container> mPtr = m_context.top().get_container(mapper_ptr.getValue());
		if (mPtr == nullptr) {
			throw system_exception("sql error: mapper was nullptr");
		}
		container mapper = *mPtr;
		std::vector<container> results = m_db.get(szQuery, mapper);
		std::string resultAlias = m_dcontext.as_string_or_default("query_results_prefix", DEV_FLAG_QUERY_RESULTS_PREFIX)
			+ std::to_string(m_db.query_count());
	
		std::vector<serialized_integral> resultPtrs;
		std::string szResultSignature;
		for (unsigned int i{ 0 }; i < results.size(); i++) {
			szResultSignature = resultAlias + "." + std::to_string(i);
			m_context.top().define(szResultSignature, std::make_shared<container>(results.at(i).get_scope()), overwrite_existing());
			resultPtrs.push_back(serialized_integral("group_ptr", szResultSignature));
		}
		m_context.top().define(resultAlias, std::make_shared<container>(resultPtrs), overwrite_existing());
		return serialized_integral("group_ptr", resultAlias);
	}

	serialized_integral run_query(const std::string& szQuery)
	{
		container mapper;
		std::vector<container> results = m_db.get(szQuery, mapper);
		return serialized_integral();
	}

	serialized_integral run_map_prepared_query(const std::string& szQuery, serialized_integral mapper_ptr, std::vector<serialized_integral> args, bool include_unexpected)
	{
		/* @TODO: if include_unexpected we also have to accept a null mapper_ptr*/
		container mapper;
		if (!include_unexpected) {
			std::shared_ptr<container> mPtr = m_context.top().get_container(mapper_ptr.getValue());
			if (mPtr == nullptr) {
				throw system_exception("sql error: mapper was nullptr");
			}
			mapper = *mPtr;
		}

		std::vector<container> results = m_db.run_prepared(szQuery, args, mapper, include_unexpected, 1);
		std::string resultAlias = m_dcontext.as_string_or_default("query_results_prefix", DEV_FLAG_QUERY_RESULTS_PREFIX)
			+ std::to_string(m_db.query_count());

		std::vector<serialized_integral> resultPtrs;
		std::string szResultSignature;
		for (unsigned int i{ 0 }; i < results.size(); i++) {
			szResultSignature = resultAlias + "." + std::to_string(i);
			m_context.top().define(szResultSignature, std::make_shared<container>(results.at(i).get_scope()), overwrite_existing());
			resultPtrs.push_back(serialized_integral("group_ptr", szResultSignature));
		}
		m_context.top().define(resultAlias, std::make_shared<container>(resultPtrs), overwrite_existing());
		return serialized_integral("group_ptr", resultAlias);
	}

	serialized_integral run_prepared_query(const std::string& szQuery, std::vector<serialized_integral> args)
	{
		std::shared_ptr<container> mPtr = std::make_shared<container>();

		if (mPtr == nullptr) {
			throw system_exception("sql error: mapper was nullptr");
		}
		container mapper = *mPtr;
		std::vector<container> results = m_db.run_prepared(szQuery, args, mapper, true, 1);
		return serialized_integral();
	}


private:

		serialized_integral assert_or_convert_type( std::string szType, serialized_integral val, unsigned int row = 0, unsigned int column = 0)
		{
			if (val.getType() == szType) {
				return val;
			}
			std::string coversionName = "operator->(" + val.getType() + "," + szType + ")";
			if (m_context.exists_callable(coversionName)) {
				serialized_integral converted_value = m_context.get_callable(coversionName)->call(this, { val });
				return converted_value;
			}
			throw runtime_exception("no suitable conversion exists; conversion " + coversionName + " does not exist", row, column);
		}

		bool isTruthy(serialized_integral si)
		{
			if (si.getType() == si_types().STRING()) {
				return si.getValue() != "";
			}

			if (si.getType() == si_types().CHAR()) {
				return si.getValue() != std::string(1, static_cast<char>(0));
			}

			if (is_numeric_type(si)) {
				auto pr = si.tryParseDouble();
				if (!pr.error) {
					return pr.value;
				}
				throw system_exception("unable to parse value to numeric " + si.toString());
			}

			if (si.getType() == "fn_ptr") {
				if (m_context.exists_callable(si.getValue())) {
					return true;
				}
				return false;
			}

			throw runtime_exception("unable to determine truthiness of object " + si.toString());
		}

		bool is_numeric_type(const serialized_integral& val)
		{
			return val.getType() == si_types().INT() || val.getType() == si_types().FLOAT() 
				|| val.getType() == si_types().DOUBLE() || val.getType() == si_types().UINT();
		}

		std::string create_cast_operator_signature(std::string op, serialized_integral arg, std::string szTargetType)
		{
			std::ostringstream oss;
			oss << "operator" << op << "(" << arg.getType() << "," << szTargetType << ")";
			return oss.str();
		}


		std::string create_operator_signature(std::string op, std::vector<serialized_integral> args)
		{
			std::ostringstream oss;
			oss << "operator" << op << "(";
			if (args.size() == 0) {
				oss << ")";
				return oss.str();
			}
			oss << args.at(0).getType();
			for (unsigned int i{ 1 }; i < args.size(); i++) {
				oss << "," << args.at(i).getType();
			}
			oss << ")";
			return oss.str();
		}

		void update_callstack_index(unsigned int idx)
		{
			m_dcontext.define("callstack_index", serialized_integral(idx), true);
		}

		bool overwrite_existing() {
			return m_dcontext.as_int_or_default("overwrite_existing", DEV_FLAG_OVERWRITE_EXISTING);
		}

		bool strict_mode()
		{
			return m_dcontext.as_int_or_default("strict_mode", DEV_FLAG_STRICT_MODE);
		}

		unsigned int pointer_per_type_limit()
		{
			return m_dcontext.as_int_or_default("pointer_per_type_limit", DEV_FLAG_POINTER_PER_TYPE_LIMIT);
		}

		std::string get_list_type_macro()
		{
			return m_dcontext.as_string_or_default("list_prefix", DEV_FLAG_LIST_PREFIX);
		}

		unsigned int max_callstack_index()
		{
			return static_cast<unsigned int>(m_dcontext.as_int_or_default("max_callstack_index", DEV_FLAG_MAX_CALLSTACK_INDEX));
		}

		unsigned int callstack_index()
		{
			return m_dcontext.as_uint_or_default("callstack_index", m_nCallStackIndex);
		}

		bool unsafe_mode() 
		{
			return m_dcontext.as_int_or_default("unsafe_mode", DEV_FLAG_UNSAFE_MODE);
		}

		void set_unsafe_mode(bool enabled)
		{
			m_dcontext.define("unsafe_mode", serialized_integral(enabled), true);
		}

		developer_context m_dcontext;
		execution_context m_context;
		FileHandle m_hFile;
		tokenizer m_tokenizer;
		parser m_parser;
		unsigned int m_nCallStackIndex{ 0 };

		db_helper m_db;
};

serialized_integral ud_fn::call(interpreter* i, std::vector<serialized_integral> args)
{
	if (i != nullptr) {
		return i->call_ud_fn(std::static_pointer_cast<ud_fn>(this->shared_from_this()), args);
	}
	throw system_exception("interp is null", m_row, m_column);
}
