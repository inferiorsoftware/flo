#pragma once

#include "flo/dev/token.h"

#include <vector>


namespace flo
{

struct CompileError
{
	enum class Type
	{
		InvalidToken,
		UnterminatedToken
	};

	const CompileError::Type type;
	const Token token;
};

class CompileErrorListener
{
public:
	virtual void onCompileError(CompileError err) = 0;
};

/**
 * CompileErrorListener which just stores compile-errors in a vector.
 */
class CompileErrorStore : public CompileErrorListener
{
public:
	std::vector<CompileError> errors;
	void onCompileError(CompileError err);
};

class CompileErrorMessageGenerator
{
public:
	std::string generateMessage(CompileError err);
};

}

