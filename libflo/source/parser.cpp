#include "flo/dev/parser.h"
#include "flo/dev/token.h"

#include <iostream> //TMP!

namespace flo
{

struct ParseException : public std::exception
{
	CompileError error;
	ParseException(CompileError err) : error(err) {}
};

class TokenScanner
{
	std::vector<Token> tokens;
	unsigned int i = 0;

public:
	TokenScanner(const std::vector<Token> tokens)
		: tokens(tokens) {}

	bool match(Token::Type type)
	{
		if(check(type))
		{
		   ++i;
		   return true;
		}

		return false;
	}

	bool check(Token::Type type) { return !isEnd() && tokens[i].type == type; }
	bool isEnd() { return i >= tokens.size(); }
	Token previous() { return tokens[i-1]; }
	Token advance() { return tokens[i++]; }
	Token next() { return tokens[i]; }

	int index() { return i; }
	int size() { return tokens.size(); }
	bool isEmpty() { return tokens.empty(); }
};

class Parser
{
	TokenScanner& scn;
	CompileErrorListener* err;

public:
	Parser(TokenScanner& scn, CompileErrorListener* err) : scn(scn), err(err) {}

	std::vector<StmtPtr> parse()
	{
		std::vector<StmtPtr> statements;

		if(scn.isEmpty()) { return statements; }

		while(!scn.isEnd())
		{
			try
			{
				statements.push_back(statement());
			}
			catch(ParseException e)
			{
				if(err) { err->onCompileError(e.error); }
				synchronise();
			}
		}

		return statements;
	}

private:

	void synchronise()
	{
		if(scn.isEnd()) return;

		scn.advance();
		while(!scn.isEnd())
		{
			if(scn.previous().type == Token::Type::EndStmt) return;
			scn.advance();
		}
	}

	//TMP!?
	void ensurePrevious(CompileError::Type err)
	{
		if(scn.index() == 0)
		{
			throw ParseException(CompileError{err, scn.next()});
		}
	}

	StmtPtr statement()
	{
		if(scn.match(Token::Type::Out))
		{
			return flo::OutStmt::create(expression());
		}

		return flo::ExprStmt::create(expression());
	}

	ExprPtr expression()
	{
		return addition();
	}

	ExprPtr addition()
	{
		ExprPtr expr = multiplication();

		while(scn.match(Token::Type::Minus)
			  || scn.match(Token::Type::Plus))
		{
			ensurePrevious(CompileError::Type::ExpectExpression);

			Token op = scn.previous();
			ExprPtr right = multiplication();
			expr = BinaryExpr::create(expr, op, right);
		}

		return expr;
	}

	ExprPtr multiplication()
	{
		ExprPtr expr = unary();

		while(scn.match(Token::Type::Stroke)
			  || scn.match(Token::Type::Star))
		{
			ensurePrevious(CompileError::Type::ExpectExpression);

			Token op = scn.previous();
			ExprPtr right = unary();
			expr = BinaryExpr::create(expr, op, right);
		}

		return expr;
	}

	ExprPtr unary()
	{
		if(scn.match(Token::Type::Minus))
		{
			ensurePrevious(CompileError::Type::ExpectExpression);

			Token op = scn.previous();
			ExprPtr right = unary();
			return UnaryExpr::create(op, right);
		}

		return primary();
	}

	ExprPtr primary()
	{
		// Literal value
		if(scn.match(Token::Type::Number)
		   || scn.match(Token::Type::String))
		{
			return LiteralExpr::create(scn.previous());
		}

		// Group
		if(scn.match(Token::Type::OpenParen))
		{
			Token start = scn.previous();

			ExprPtr expr = expression();
			if(!scn.match(Token::Type::CloseParen))
			{
				throw ParseException(CompileError{CompileError::Type::UnterminatedToken, start});
			}

			return GroupExpr::create(expr);
		}

		// Failure D:
		Token failed = scn.advance();
		if(err)
		{
			err->onCompileError(CompileError{CompileError::Type::ParseFailure, failed});
		}
		scn.advance();
		return ErrorExpr::create(failed);
	}

	void ln(std::string s)
	{
		std::cout << s << std::endl;
	}
};

std::vector<StmtPtr> parse(std::vector<Token> tokens, CompileErrorListener* err)
{
	TokenScanner scn(tokens);
	Parser parser(scn, err);

	return parser.parse();
}

std::vector<StmtPtr> parse(std::vector<Token> tokens)
{
	return parse(tokens, nullptr);
}


}

