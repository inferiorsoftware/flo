#include "flo/dev/parser.h"

namespace flo
{


class TokenScanner
{
	std::vector<Token> tokens;
	int i = 0;

public:
	TokenScanner(const std::vector<Token> tokens)
		: tokens(tokens) {}

	bool match(Token::Type type)
	{
		if(peek(type))
		{
		   ++i;
		   return true;
		}

		return false;
	}

	bool peek(Token::Type type) { return tokens[i].type == type; }
	bool isEnd() { return i >= tokens.size(); }
	Token previous() { return tokens[i-1]; }
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

		while(!scn.isEnd())
		{
			statements.push_back(statement());
		}

		return statements;
	}

private:

	StmtPtr statement()
	{
		return flo::ExprStmt::create(addition());
	}

	ExprPtr addition()
	{
		ExprPtr expr = multiplication();

		while(scn.match(Token::Type::Minus)
			  || scn.match(Token::Type::Plus))
		{
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
			Token op = scn.previous();
			ExprPtr right = unary();
			return UnaryExpr::create(op, right);
		}

		return primary();
	}

	ExprPtr primary()
	{
		if(scn.match(Token::Type::Number)
		   || scn.match(Token::Type::String))
		{
			return LiteralExpr::create(scn.previous());
		}

		Token failed = scn.previous();
		if(err)
		{
			err->onCompileError(CompileError{CompileError::Type::ParseFailure, failed});
		}
		return ErrorExpr::create(failed);
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

