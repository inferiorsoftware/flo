#include "flo/dev/compiler.h"

#include "flo/dev/tokeniser.h"
#include "flo/dev/parser.h"
#include "flo/dev/bytecode_generator.h"
#include "flo/dev/compile_error.h"
#include "flo/dev/ast_printer.h"

#include <iostream>
#include <vector>

using namespace flo;

class ErrHandler : public CompileErrorListener
{
	std::vector<CompileError> errors;

public:

	void onCompileError(CompileError err) override
	{
		errors.push_back(err);
	}

	bool process(Source src)
	{
		if(errors.empty()) return false;

		CompileErrorMessageGenerator msg;

		for(flo::CompileError err : errors)
		{
			int lineNum;
			int colNum;
			std::string line;
			src.getDebugInfoAtIndex(err.token.index, lineNum, colNum, line);

			std::cerr << "Compile error: (" << lineNum << " : " << colNum << ")" << std::endl;
			std::cerr << "> " <<  msg.generateMessage(err) <<std::endl;

			std::cerr << "> " << line << std::endl << "  ";

			for(int i=0; i<colNum-1; ++i)
			{
				std::cerr << " ";
			}
			std::cerr << "^" << std::endl;
		}

		return true;
	}
};


std::optional<flo::Chunk> flo::compile(const flo::Source& src)
{
	ErrHandler err;

	std::vector<Token> tokens = tokenise(src.code, &err);

	if(err.process(src))
	{
		return std::nullopt;
	}

	std::vector<StmtPtr> tree = parse(tokens, &err);

	flo::dbg::AstPrinter astpr;

	for(flo::StmtPtr stmt : tree)
	{
		std::cout << astpr.print(*stmt) << std::endl;
	}

	if(err.process(src))
	{
		return std::nullopt;
	}

	return std::optional(flo::generate(tree));
}

