#pragma once

#include "flo/dev/token.h"

#include <string>

namespace flo
{

struct Token
{
	enum class Type
	{
		EndStmt,		/**< End of statement; a semicolon or a line-break. */

		Plus,			/**< + */
		Minus,			/**< - */
		Star,			/**< * */
		SlashFwd,		/**< / */
		Pipe,			/**< | */

		String,			/**< "..." */

		Error			/**< Not a valid token.*/
	};

	/** Type of token this represents. */
	const flo::Token::Type type;

	/** The token as it appeared in the source code. */
	const std::string lexeme;

	/** The character index where the token appears in source code. */
	const int index;

	std::string getName();
};

}
