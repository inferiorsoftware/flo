#pragma once

#include "flo/dev/ast.h"
#include <string>
#include <sstream>
#include <vector>

namespace flo
{
namespace dbg
{

class AstPrinter : AstVisitor
{
	std::stringstream ss;
	int depth = 0;
	std::vector<std::string> indent;

	void out(std::string str, std::string bullet = "├");
	void beginBranch(std::string str, std::string bullet = "├");
	void beginStmt(std::string name);
	void beginExpr(std::string name);
	void endBranch();


public:
	std::string print(AstNode& root);

private:
	void visit(ExprStmt& stmt) override;
	void visit(OutStmt& stmt) override;
	void visit(BinaryExpr& expr) override;
	void visit(UnaryExpr& expr) override;
	void visit(LiteralExpr& expr) override;
	void visit(GroupExpr& expr) override;
	void visit(ErrorExpr& expr) override;


	// AstVisitor interface
public:

};


}
}
