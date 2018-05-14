#include "flo/dev/ast_printer.h"
#include "flo/dev/token.h"

#include <iostream>

void flo::dbg::AstPrinter::out(std::string str, std::string bullet)
{
	for(int i=0; i<indent.size(); ++i)
	{
		ss << indent[i];
	}

	ss << bullet << str << std::endl;
}

void flo::dbg::AstPrinter::beginBranch(std::string str, std::string bullet)
{
	out(str, bullet);
	indent.push_back("  ");
}

void flo::dbg::AstPrinter::beginStmt(std::string name)
{
	beginBranch(name, "■");
}

void flo::dbg::AstPrinter::beginExpr(std::string name)
{
	beginBranch("[" + name + "]");
}

void flo::dbg::AstPrinter::endBranch()
{
	depth--;
}


std::string flo::dbg::AstPrinter::print(flo::AstNode& root)
{
	ss.str("");
	root.accept(*this);
	return ss.str();
}


void flo::dbg::AstPrinter::visit(flo::ExprStmt& stmt)
{
	beginStmt("expr");
	AstVisitor::visit(stmt);
	endBranch();
}

void flo::dbg::AstPrinter::visit(flo::OutStmt& stmt)
{
	beginStmt("out");
	AstVisitor::visit(stmt);
	endBranch();
}

void flo::dbg::AstPrinter::visit(flo::BinaryExpr& expr)
{
	beginExpr(expr.op.lexeme);
	AstVisitor::visit(expr);
	endBranch();
}

void flo::dbg::AstPrinter::visit(flo::UnaryExpr& expr)
{
	beginExpr(expr.op.lexeme);
	AstVisitor::visit(expr);
	endBranch();
}

void flo::dbg::AstPrinter::visit(flo::LiteralExpr& expr)
{
	out("" + expr.token.lexeme + "");
}

void flo::dbg::AstPrinter::visit(flo::ErrorExpr& expr)
{
	AstVisitor::visit(expr);
}
