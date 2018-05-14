#include "flo/dev/ast.h"

using namespace flo;

ExprStmt::ExprStmt(ExprPtr expression)
	: expression(expression) {}

AstNode* ExprStmt::getParent() { return parent; }

StmtPtr ExprStmt::create(ExprPtr expression)
{
	ExprStmt* n = new ExprStmt(expression);
	expression->parent = n;
	return std::shared_ptr<ExprStmt>(n);
}

OutStmt::OutStmt(ExprPtr expression)
	: expression(expression) {}

AstNode* OutStmt::getParent() { return parent; }

StmtPtr OutStmt::create(ExprPtr expression)
{
	OutStmt* n = new OutStmt(expression);
	expression->parent = n;
	return std::shared_ptr<OutStmt>(n);
}

BinaryExpr::BinaryExpr(ExprPtr left, Token op, ExprPtr right)
	: left(left), op(op), right(right) {}

AstNode* BinaryExpr::getParent() { return parent; }

ExprPtr BinaryExpr::create(ExprPtr left, Token op, ExprPtr right)
{
	BinaryExpr* n = new BinaryExpr(left, op, right);
	left->parent = n;
	right->parent = n;
	return std::shared_ptr<BinaryExpr>(n);
}

UnaryExpr::UnaryExpr(Token op, ExprPtr right)
	: op(op), right(right) {}

AstNode* UnaryExpr::getParent() { return parent; }

ExprPtr UnaryExpr::create(Token op, ExprPtr right)
{
	UnaryExpr* n = new UnaryExpr(op, right);
	right->parent = n;
	return std::shared_ptr<UnaryExpr>(n);
}

LiteralExpr::LiteralExpr(Token token)
	: token(token) {}

AstNode* LiteralExpr::getParent() { return parent; }

ExprPtr LiteralExpr::create(Token token)
{
	LiteralExpr* n = new LiteralExpr(token);
	return std::shared_ptr<LiteralExpr>(n);
}

