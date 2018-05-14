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
		UnterminatedToken,
		ParseFailure
	};

	/** The type of error this represents. */
	const CompileError::Type type;

	/** The token where this error occours. */
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

/**
 * Generates a useful error message about a CompileError.
 */
class CompileErrorMessageGenerator
{
public:
	std::string generateMessage(CompileError err);
};

}

