#include "flo/dev/ast.h"

using namespace flo;

ExprStmt::ExprStmt(ExprPtr expression)
	: expression(expression) {}

AstNode* ExprStmt::getParent() { return parent; }

void ExprStmt::accept(AstVisitor& visitor) { visitor.visit(*this); }
StmtPtr ExprStmt::create(ExprPtr expression)
{
	ExprStmt* n = new ExprStmt(expression);
	expression->parent = n;
	return std::shared_ptr<ExprStmt>(n);
}

OutStmt::OutStmt(ExprPtr expression)
	: expression(expression) {}

AstNode* OutStmt::getParent() { return parent; }

void OutStmt::accept(AstVisitor& visitor) { visitor.visit(*this); }
StmtPtr OutStmt::create(ExprPtr expression)
{
	OutStmt* n = new OutStmt(expression);
	expression->parent = n;
	return std::shared_ptr<OutStmt>(n);
}

BinaryExpr::BinaryExpr(ExprPtr left, Token op, ExprPtr right)
	: left(left), op(op), right(right) {}

AstNode* BinaryExpr::getParent() { return parent; }

void BinaryExpr::accept(AstVisitor& visitor) { visitor.visit(*this); }
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

void UnaryExpr::accept(AstVisitor& visitor) { visitor.visit(*this); }
ExprPtr UnaryExpr::create(Token op, ExprPtr right)
{
	UnaryExpr* n = new UnaryExpr(op, right);
	right->parent = n;
	return std::shared_ptr<UnaryExpr>(n);
}

LiteralExpr::LiteralExpr(Token token)
	: token(token) {}

AstNode* LiteralExpr::getParent() { return parent; }

void LiteralExpr::accept(AstVisitor& visitor) { visitor.visit(*this); }
ExprPtr LiteralExpr::create(Token token)
{
	LiteralExpr* n = new LiteralExpr(token);
	return std::shared_ptr<LiteralExpr>(n);
}

ErrorExpr::ErrorExpr(Token token)
	: token(token) {}

AstNode* ErrorExpr::getParent() { return parent; }

void ErrorExpr::accept(AstVisitor& visitor) { visitor.visit(*this); }
ExprPtr ErrorExpr::create(Token token)
{
	ErrorExpr* n = new ErrorExpr(token);
	return std::shared_ptr<ErrorExpr>(n);
}

void flo::AstVisitor::visit(ExprStmt& stmt)
{
	stmt.expression->accept(*this);
}
void flo::AstVisitor::visit(OutStmt& stmt)
{
	stmt.expression->accept(*this);
}
void flo::AstVisitor::visit(BinaryExpr& expr)
{
	expr.left->accept(*this);
	expr.right->accept(*this);
}
void flo::AstVisitor::visit(UnaryExpr& expr)
{
	expr.right->accept(*this);
}
void flo::AstVisitor::visit(LiteralExpr& expr)
{
}
void flo::AstVisitor::visit(ErrorExpr& expr)
{
}
