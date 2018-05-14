#pragma once

#include "flo/dev/token.h"

#include <string>

namespace flo
{

struct Token
{
	enum class Type
	{
		EndStmt,		/**< End of statement: semicolon or line-break. */

		Plus,			/**< + */
		Minus,			/**< - */
		Star,			/**< * */
		Stroke,			/**< / */
		Pipe,			/**< | */

		String,			/**< "..." */
		Number,			/**< x[.x] */

		Error			/**< Not a valid token. */
	};

	/** Type of token this represents. */
	const flo::Token::Type type;

	/** The "raw" value of the token. */
	const std::string lexeme;

	/** The character index where the token appears in source code. */
	const int index;

	/** Get the name of the token, suitable for debugging. */
	std::string getName();
};

}
