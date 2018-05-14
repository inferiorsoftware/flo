#include <flo/dev/source.h>
#include <flo/dev/token.h>
#include <flo/dev/tokeniser.h>

#include <iostream>
#include <iomanip>

using namespace std;

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
	flo::Source src = flo::Source("tmp", "5 + 5");

	ErrorPrinter erp;

	for(flo::Token tkn :flo::tokenise(src.code, &erp))
	{
		std::cout << std::setw(7) << tkn.getName() << " | " << tkn.lexeme << std::endl;
	}

	erp.print(src);


    return 0;
}
