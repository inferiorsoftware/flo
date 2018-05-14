#include "flo/dev/tokeniser.h"

#include <sstream>
#include <ctype.h>

class Scanner
{
	const std::string code;
	int i = 0;

public:
	Scanner(const std::string code) : code(code) {}

	/**
	 * Increment the current index & get the next character.
	 * @return the next character.
	 */
	const char advance()
	{
		return code.at(i++);
	}

	/**
	 * Advance if the next character in the code == the passed test char.
	 * @param test char to test.
	 * @return true if advance was called.
	 */
	bool match(char test)
	{
		if(!check(test)) return false;

		advance();
		return true;
	}

	bool match(char test1, char test2)
	{
		if(!check(test1, test2)) return false;

		advance();
		advance();
		return true;
	}

	/**
	 * Test if next character in the code == the passed test char.
	 * @param test
	 * @return
	 */
	bool check(char test)
	{
		return peek() == test;
	}

	bool check(char test1, char test2)
	{
		return check(test1)
				&& peekPeek() == test2;
	}

	/** Look-ahead one character. Returns '\0' if at the end. */
	char peek()
	{
		if(isEnd()) return '\0';
		return code.at(i);
	}

	/** Look-ahead two characters. Returns '\0' if either of those are at the end. */
	char peekPeek()
	{
		if(isLast()) return '\0';

		return code.at(i + 1);
	}

	int index()		{ return i; }
	bool isEnd()	{ return i >= code.size(); }
	bool isLast()	{ return i >= code.size()-1; }
};

class TokenList
{
	std::vector<flo::Token> tokens;
	Scanner& scn;

public:

	TokenList(Scanner& scn) : scn(scn) {}

	/** Append a token with a specific source index. */
	void append(flo::Token::Type ofType, std::string withLexeme, int atIndex)
	{
		tokens.push_back(flo::Token{ofType, withLexeme, atIndex});

		//std::cout << tokens.back().getName() << " " << withLexeme << std::endl;
	}

	/** Add a token of a type to the list at the Scanner's current source index. */
	void append(flo::Token::Type ofType, std::string withLexeme)
	{
		append(ofType, withLexeme, scn.index());
	}

	/** True if the last token in the list is of the passed type. */
	bool backEquals(flo::Token::Type type)
	{
		return tokens.size() != 0 && tokens.back().type == type;
	}

	/** Return the last token added to the list.*/
	flo::Token peep() { return tokens.back(); }

	std::vector<flo::Token> vec() { return tokens; };
};


std::vector<flo::Token> flo::tokenise(const std::string code, CompileErrorListener* err)
{
	#define TOKEN_ERROR_AT(ofType, onLexeme, atIndex) \
		tkns.append(Token::Type::Error, onLexeme, atIndex);\
		if(err) err->onCompileError(CompileError{ofType, tkns.peep()})

	#define TOKEN_ERROR(ofType, onLexeme) TOKEN_ERROR_AT(ofType, onLexeme, s.index())

	Scanner s(code);
	TokenList tkns(s);

	while(!s.isEnd())
	{
		const char ch = s.advance();

		switch(ch)
		{
		// Ignore whitespace
		case ' ':
		case '\t':
			break;

		// Semicolons & line-breaks end statements.
		case '\n':
		case ';':

			// Only ever need one in sequence.
			if(!tkns.backEquals(Token::Type::EndStmt))
			{
				tkns.append(Token::Type::EndStmt, ";");
			}
			break;

		case '+': tkns.append(Token::Type::Plus, "+"); break;
		case '-': tkns.append(Token::Type::Minus, "-"); break;
		case '*': tkns.append(Token::Type::Star, "*"); break;
		case '|': tkns.append(Token::Type::Pipe, "|"); break;


		case '/':
			// Line comment
			if(s.match('/'))
			{
				// Discard charactars until line-break or end.
				while(!s.check('\n') && !s.isEnd()) s.advance();

				// Discard the line-break
				if(!s.isEnd()) s.advance();
			}
			// Multi-line comment
			else if(s.match('*'))
			{
				// Number of /* comments opened.
				int depth = 1;

				std::vector<int> opens;
				opens.push_back(s.index());

				while(true)
				{
					if(s.match('/', '*'))
					{
						depth++;
						opens.push_back(s.index());
					}
					else if(s.match('*', '/'))
					{
						depth--;
					}

					// Discard characters until closing */ or end.
					if(depth == 0 || s.isEnd()) break;

					s.advance();
				}

				// If depth != 0, there are unterminated comments, which is an error.
				for(int i=0; i<depth; ++i)
				{
					TOKEN_ERROR_AT(CompileError::Type::UnterminatedToken,
								   "/*", opens[i]-1);
				}
			}
			else
			{
				tkns.append(Token::Type::Stroke, "/");
			}
			break;

		// String literal
		case '"':
			{
				const int start = s.index();
				std::stringstream ss;

				while(!s.isEnd())
				{
					if(s.match('"'))
					{
						tkns.append(flo::Token::Type::String, ss.str(), start);
						break;
					}

					const char next = s.advance();

					if(s.isEnd() || next == '\n')
					{
						TOKEN_ERROR_AT(CompileError::Type::UnterminatedToken,
									   "\"", start);
						break;
					}

					ss << next;
				}
			}
			break;

		default:

			// Number literal
			if(isdigit(ch))
			{
				const int start = s.index();
				std::stringstream ss;

				bool decimal;

				ss << ch;

				while(!s.isEnd())
				{
					const char next = s.peek();

					if(isdigit(next))
					{
						ss << s.advance();
					}
					else if(next == '.')
					{
						if(!decimal)
						{
							decimal = true;
							ss << s.advance();
						}
					}
					else
					{
						break;
					}
				}

				tkns.append(Token::Type::Number, ss.str(), start);

				break;
			}


			// Invalid token
			TOKEN_ERROR(CompileError::Type::InvalidToken, std::string(1, ch));
			break;
		}
	}

	#undef TOKEN_ERROR_AT
	#undef TOKEN_ERROR

	return tkns.vec();
}

std::vector<flo::Token> flo::tokenise(std::string code)
{
	return flo::tokenise(code, nullptr);
}
