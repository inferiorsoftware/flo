#include <flo/dev/source.h>
#include <flo/dev/token.h>
#include <flo/dev/tokeniser.h>
#include <flo/dev/ast.h>
#include <flo/dev/parser.h>
#include <flo/dev/ast_printer.h>

#include <flo/lang/number.h>

#include <flo/bytecode/chunk.h>
#include <flo/dev/chunk_factory.h>
#include <flo/bytecode/opcode.h>
#include <flo/runtime/runtime.h>

#include <iostream>
#include <iomanip>


class ErrorPrinter : public flo::CompileErrorListener
{
	std::vector<flo::CompileError> errors;

public:
	void onCompileError(flo::CompileError err)
	{
		errors.push_back(err);
	}

	void print(flo::Source& src)
	{
		flo::CompileErrorMessageGenerator m;

		for(flo::CompileError err : errors)
		{
			int lineNum;
			int colNum;
			std::string line;
			src.getDebugInfoAtIndex(err.token.index, lineNum, colNum, line);

			std::cerr << "Compile error: (" << lineNum << " : " << colNum << ")" << std::endl;
			std::cerr << "> " <<  m.generateMessage(err) <<std::endl;

			std::cerr << "> " << line << std::endl << "  ";

			for(int i=0; i<colNum-1; ++i)
			{
				std::cerr << " ";
			}
			std::cerr << "^" << std::endl;
		}
	}
};


int main()
{
	flo::Source src = flo::Source("tmp", "out 1 + 1 * 10");

	ErrorPrinter erp;


	std::vector<flo::Token> tokens = flo::tokenise(src.code, &erp);
	for(flo::Token tkn : tokens)
	{
		std::cout << std::setw(7) << tkn.getName() << " | " << tkn.lexeme << std::endl;
	}

	erp.print(src);
	std::cout << std::endl;

	std::vector<flo::StmtPtr> tree = flo::parse(tokens, &erp);

	flo::dbg::AstPrinter astpr;

	for(flo::StmtPtr stmt : tree)
	{
		std::cout << astpr.print(*stmt) << std::endl;
	}



	flo::Chunk* c = flo::util::ChunkFactory()
					.constant(flo::Number::create(10))
					.constant(flo::Number::create(1.5))
					.op(flo::Opcode::Add)
					.op(flo::Opcode::Out)
					.end();

	flo::Runtime rt;
	rt.run(*c);

	delete c;

    return 0;
}
