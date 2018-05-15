#include "flo/dev/bytecode_generator.h"
#include "flo/dev/chunk_factory.h"
#include <string>
#include "flo/lang/number.h"

using namespace flo;

class BytecodeGenerator : public flo::AstVisitor
{
	flo::util::ChunkFactory f;

public:
	void visit(ExprStmt& stmt) override
	{
		AstVisitor::visit(stmt);
	}

	void visit(OutStmt& stmt) override
	{
		AstVisitor::visit(stmt);
		f.op(Opcode::Out);
	}

	void visit(BinaryExpr& expr) override
	{
		AstVisitor::visit(expr);

		switch(expr.op.type)
		{
		case Token::Type::Plus:		f.op(Opcode::Add); break;
		case Token::Type::Minus:	f.op(Opcode::Sub); break;
		case Token::Type::Star:		f.op(Opcode::Mul); break;
		case Token::Type::Stroke:	f.op(Opcode::Div); break;

		default:
			throw std::runtime_error(
						"Bad token for BinaryExpr: " + expr.op.lexeme);
		}
	}

	void visit(UnaryExpr& expr) override
	{
		AstVisitor::visit(expr);
	}

	void visit(LiteralExpr& expr) override
	{
		if(expr.token.type == Token::Type::Number)
		{
			f.constant(Number::create(std::stod(expr.token.lexeme)));
		}
	}

	void visit(ErrorExpr& expr) override
	{
		throw std::runtime_error(
					"Attempted to generate bytecode from broken syntax tree.");
	}


	Chunk* end()
	{
		return f.end();
	}
};

flo::Chunk* flo::generate(std::vector<flo::StmtPtr> tree)
{
	BytecodeGenerator g;

	for(StmtPtr stmt : tree)
	{
		stmt->accept(g);
	}

	return g.end();
}
