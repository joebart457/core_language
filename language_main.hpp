#pragma once


#include "interpreter.hpp"
#include "parser.hpp"
#include "tokenizer.hpp"
#include "language_std_lib.hpp"
#include "language_sys_std_lib.h"
#include "execution_context.hpp"
#include "ufhndl.hpp"

const std::vector<tokenizer_rule> rules = {
	tokenizer_rule("value", "$"),
	tokenizer_rule("typeof", "|"),
	tokenizer_rule("cast", "->"),
	tokenizer_rule("lparen", "("),
	tokenizer_rule("rparen", ")"),
	tokenizer_rule("lcurly", "{"),
	tokenizer_rule("rcurly", "}"),
	tokenizer_rule("lbracket", "["),
	tokenizer_rule("rbracket", "]"),
	tokenizer_rule("comma", ","),
	tokenizer_rule("semi", ";"),
	tokenizer_rule("dot", "."),
	tokenizer_rule("equal", "="),
	tokenizer_rule("equalequal", "=="),
	tokenizer_rule("notequal", "!="),
	tokenizer_rule("not", "!"),
	tokenizer_rule("negate", "-"),
	tokenizer_rule("less", "<"),
	tokenizer_rule("lessequal", "<="),
	tokenizer_rule("greater", ">"),
	tokenizer_rule("greaterequal", ">="),
	tokenizer_rule("add", "+"),
	tokenizer_rule("multiply", "*"),
	tokenizer_rule("divide", "/"),
	tokenizer_rule("if", "if"),
	tokenizer_rule("else", "else"),
	tokenizer_rule("while", "while"),
	tokenizer_rule("return", "return"),
	tokenizer_rule("function", "function"),
	tokenizer_rule("group", "group"),
	tokenizer_rule("true", "true"),
	tokenizer_rule("false", "false"),
	tokenizer_rule("null", "null"),
	tokenizer_rule("uint", "uint"),
	tokenizer_rule("int", "int"),
	tokenizer_rule("float", "float"),
	tokenizer_rule("double", "double"),
	tokenizer_rule("char", "char"),
	tokenizer_rule("string", "string"),
	tokenizer_rule("fn_ptr", "fn_ptr"),
	tokenizer_rule("system_ptr", "system_ptr"),
	tokenizer_rule("group_ptr", "group_ptr"),
	tokenizer_rule("new", "new"),
	tokenizer_rule(TOKEN_TYPE_EOL_COMMENT, "//"),
	tokenizer_rule(TOKEN_TYPE_STRING_ENCLOSING, "\""),
	tokenizer_rule("any", "any"),
	tokenizer_rule("import", "import"),
	tokenizer_rule("directive", "@"),
	tokenizer_rule("xor", "^")
};


class language_main {
public:
	language_main(){}
	~language_main() {}

	int run_file(const std::string& szFilePath, std::vector<std::string> cl_arguments = {})
	{

		interpreter i(default_context(), default_developer_context(), tokenizer(rules));
		parser p;
		tokenizer t(rules);
		std::vector<serialized_integral> arguments;
		for (auto arg : cl_arguments) {
			arguments.push_back(serialized_integral(si_types().ANY(), arg));
		}
		try {
			serialized_integral rc = i.interpret_exec(p.parse_toplevel(t.tokenize(m_hFile.readFileAsString(szFilePath))), m_hFile.parent_path(szFilePath), "main", arguments);
			return 0;
		}
		catch (system_exception se) {
			std::cout << se.fullTrace();
			return -1;
		} catch(std::exception e) {
			std::cout << e.what();
			return -999;
		}
	}


	void repl() {
		bool bFinished = false;
		bool bShowTokens = false;
		std::string szText;
		interpreter i(default_context(), default_developer_context(), tokenizer(rules));
		parser p;
		tokenizer t(rules);

		while (!bFinished) {
			std::cout << (szText == ""? "" : "\n") << ">$ ";
			std::getline(std::cin, szText);
			if (szText == "-debug:show-tokens") {
				bShowTokens = !bShowTokens;
				std::cout << "show-tokens = " << std::boolalpha << bShowTokens;
				continue;
			}
			if (szText == "-debug:reset") {
				bShowTokens = false;
				std::cout << "show-tokens = " << std::boolalpha << bShowTokens;
				continue;
			}
			if (szText == "-debug") {
				std::cout << "show-tokens = " << std::boolalpha << bShowTokens;
				continue;
			}
			if (szText=="-debug:show_env") {
				i.output_context();
				continue;
			}
			if (szText == "exit") {
				break;
			}
			try {
				i.interpret(p.parse(t.tokenize(szText, bShowTokens)));
			}
			catch (return_exception<serialized_integral> ret) {
				std::cout << ret.what() << " :" << ret.value().toString();
			}
			catch (parsing_exception pe) {
				std::cout << pe.what();
			}
			catch (runtime_exception re) {
				std::cout << re.fullTrace();
			}
			catch (system_exception se) {
				std::cout << se.fullTrace();
			}
			catch (std::exception e) {
				std::cout << e.what();
			}
		}

	}
private:
	execution_context default_context()
	{
		activation_record ar;
		ar.id = 0;
		ar.callables = scope<std::shared_ptr<callable>>(get_std_operators(), "callables");
		return execution_context(std::make_shared<activation_record>(ar));
	}

	developer_context default_developer_context()
	{
		activation_record ar;
		ar.id = 0;
		ar.callables = scope<std::shared_ptr<callable>>(get_system_macros(), "system_callables");
		return developer_context(std::make_shared<activation_record>(ar));
	}

	FileHandle m_hFile;
};