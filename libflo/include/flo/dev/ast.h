#pragma once

#include "flo/dev/token.h"
#include <memory>


namespace flo
{
class AstVisitor;

/** Base for AST nodes. */
struct AstNode
{
	friend struct ExprStmt;
	friend struct OutStmt;
	friend struct BinaryExpr;
	friend struct UnaryExpr;
	friend struct LiteralExpr;
	friend struct GroupExpr;
	friend struct ErrorExpr;
	
	virtual ~AstNode() {}
	virtual AstNode* getParent() = 0;
	virtual void accept(AstVisitor& visitor) = 0;
protected:
	AstNode* parent;

};


/** Base for statement nodes. */
struct Stmt : public AstNode
{
protected:
	Stmt() {}
};
typedef std::shared_ptr<Stmt> StmtPtr;


/** Base for expression nodes. */
struct Expr : public AstNode
{
protected:
	Expr() {}
};
typedef std::shared_ptr<Expr> ExprPtr;


/** Statement containing a single expression. */
struct ExprStmt : public Stmt
{
	const ExprPtr expression;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static StmtPtr create(ExprPtr expression);
	ExprStmt(ExprPtr expression);
};


/** Print the result of an expression. */
struct OutStmt : public Stmt
{
	const ExprPtr expression;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static StmtPtr create(ExprPtr expression);
	OutStmt(ExprPtr expression);
};


/** Expression which operates on two other expressions. */
struct BinaryExpr : public Expr
{
	const ExprPtr left;
	const Token op;
	const ExprPtr right;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static ExprPtr create(ExprPtr left, Token op, ExprPtr right);
	BinaryExpr(ExprPtr left, Token op, ExprPtr right);
};


/** Expression which operates on a single other expression. */
struct UnaryExpr : public Expr
{
	const Token op;
	const ExprPtr right;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static ExprPtr create(Token op, ExprPtr right);
	UnaryExpr(Token op, ExprPtr right);
};


/** A literal value defined in source code. */
struct LiteralExpr : public Expr
{
	const Token token;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static ExprPtr create(Token token);
	LiteralExpr(Token token);
};


/** A parenthesised expression. */
struct GroupExpr : public Expr
{
	const ExprPtr expression;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static ExprPtr create(ExprPtr expression);
	GroupExpr(ExprPtr expression);
};


/** A parse error. */
struct ErrorExpr : public Expr
{
	const Token token;
	
	AstNode* getParent() final;
	void accept(AstVisitor& visitor) final;
	static ExprPtr create(Token token);
	ErrorExpr(Token token);
};

class AstVisitor
{
public:
	virtual void visit(ExprStmt& stmt);
	virtual void visit(OutStmt& stmt);
	virtual void visit(BinaryExpr& expr);
	virtual void visit(UnaryExpr& expr);
	virtual void visit(LiteralExpr& expr);
	virtual void visit(GroupExpr& expr);
	virtual void visit(ErrorExpr& expr);
};

}
