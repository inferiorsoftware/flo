#include "flo/dev/compile_error.h"
#include <sstream>

std::string flo::CompileErrorMessageGenerator::generateMessage(flo::CompileError err)
{
	std::stringstream ss;

	switch(err.type)
	{
	case CompileError::Type::InvalidToken:
		ss << "Invalid token: '" << err.token.lexeme << "'";
		break;

	case CompileError::Type::UnterminatedToken:
		if(err.token.lexeme == "/*")
		{
			ss << "Unterminated comment.";
		}
		else if(err.token.lexeme == "\"")
		{
			ss << "Unterminated string.";
		}
		else
		{
			ss << "Unterminated: " << err.token.lexeme;
		}

		break;
	}


	return ss.str();
}

void flo::CompileErrorStore::onCompileError(flo::CompileError err)
{
	errors.push_back(err);
}
