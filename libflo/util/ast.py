global header_output_path
global source_output_path
header_output_path = "../include/flo/dev/ast.h"
source_output_path = "../source/ast.cpp"


class Field:
    """ Defines a field in a class. """
    def __init__(self, typename, name, constructor=True, init=""):
        self.typename = typename
        self.name = name
        self.constructor = constructor
        self.init = (self.typename + " " + self.name
                     + (" " + init if init else "")
                     + ";")


class ClassDef:
    """ Defines an AST node class. """
    def __init__(self, superclass, name, fields, doc, abstract=False):
        self.superclass = superclass
        self.name = name
        self.fields = fields
        self.doc = doc
        self.abstract = abstract
        self.branch = self.abstract and self.superclass

        if not self.abstract and self.superclass:
            self.ptrName = superclass.name + "Ptr"

        self.constructor = ""
        for field in filter(lambda f: f.constructor, self.fields):
            self.constructor += field.typename + " " + field.name + ", "
        self.constructor = self.constructor[:-2]

base = ClassDef(None, "AstNode", [], "Base for AST nodes.", True)

stmt = ClassDef(base, "Stmt",
                [],
                "Base for statement nodes.",
                True)

expr = ClassDef(base, "Expr",
                [],
                "Base for expression nodes.",
                True)

nodes = [
    ClassDef(stmt, "ExprStmt",
             [Field("ExprPtr", "expression")],
             "Statement containing a single expression."
             ),
    ClassDef(stmt, "OutStmt",
             [Field("ExprPtr", "expression")],
             "Print the result of an expression."
             ),

    ClassDef(expr, "BinaryExpr",
             [Field("ExprPtr", "left"), Field("Token", "op"), Field("ExprPtr", "right")],
             "Expression which operates on two other expressions."
             ),
    ClassDef(expr, "UnaryExpr",
             [Field("Token", "op"), Field("ExprPtr", "right")],
             "Expression which operates on a single other expression."
             ),
    ClassDef(expr, "LiteralExpr",
             [Field("Token", "token", "Token defining the literal.")],
             "A literal value defined in source code."
             ),

    ClassDef(expr, "GroupExpr",
             [Field("ExprPtr", "expression", "Inner expression.")],
             "A parenthesised expression."
             ),

    ClassDef(expr, "ErrorExpr",
             [Field("Token", "token", "The unparsable token.")],
             "A parse error."
             )
]


class Source:
    """ Helper for building cpp source code. """
    def __init__(self):
        self.str = ""
        self.indent = 0

    def append(self, s):
        if self.str and self.str[-1] == "\n":
            # Indent if new line.
            self.str += "\t" * self.indent
        self.str += str(s)

    def ln(self, s):
        self.append(s + "\n")

    def doc(self, s):
        self.ln("/** " + s + " */")

    def br(self, num=1):
        self.append("\n" * num)

    def block_begin(self):
        self.ln("{")
        self.indent += 1

    def block_end(self, s=""):
        self.indent -= 1
        self.ln("}" + s)

    def access_mod(self, s):
        self.indent -= 1
        self.ln(s + ":")
        self.indent += 1

    def include(self, s):
        self.ln("#include " + s)

    def un_br(self):
        """Remove a line break if it was the last thing appended. """
        if self.str[-1] == "\n":
            self.str = self.str[:-1]


if __name__ == "__main__":
    def generate_header():
        src = Source()

        def generate_class(cls):
            src.br()
            src.doc(cls.doc)
            src.ln("struct " + cls.name + (" : public " + cls.superclass.name if cls.superclass else ""))
            src.block_begin()

            if cls.branch:
                src.access_mod("protected")

                if(not cls.superclass):
                    src.ln("AstNode* parent = nullptr;")

                src.ln(cls.name + "() {}")
                src.block_end(";")
                src.ln("typedef std::shared_ptr<" + cls.name + "> " + cls.name + "Ptr;")
                src.br()
                return

            # Expressions are friends with every other non-abstract node
            # to allow setting of their parent ptr.
            if cls.abstract:
                for other in nodes:
                    if not other.abstract and other.name != cls.name:
                        src.ln("friend struct " + other.name + ";")
                src.br()

            # Fields
            if cls.fields:
                src.un_br()
                if cls.fields:
                    src.br()
                    for field in cls.fields:
                        src.ln("const " + field.init)

                src.br()

            # Methods
            if not cls.abstract:
                src.ln("AstNode* getParent() final;")
                src.ln("void accept(AstVisitor& visitor) final;")
                src.ln("static " + cls.ptrName + " create(" + cls.constructor + ");")

            # Base AstNode
            elif not cls.superclass:
                src.ln("virtual ~" + cls.name + "() {}")
                src.ln("virtual AstNode* getParent() = 0;")
                src.ln("virtual void accept(AstVisitor& visitor) = 0;")
                src.access_mod("protected")
                src.ln("AstNode* parent;\n")


            # Constructor
            if cls.constructor:
                src.ln(cls.name + "(" + cls.constructor + ");")

            src.block_end(";")
            src.br()

        src.ln("#pragma once")
        src.br()
        src.include("\"flo/dev/token.h\"")
        src.include("<memory>")
        src.br(2)
        src.ln("namespace flo")
        src.ln("{")

        src.ln("class AstVisitor;")

        # Base classes
        generate_class(base)
        generate_class(stmt)
        generate_class(expr)

        # Node classes
        for cls in nodes:
            generate_class(cls)


        # Visitor
        src.ln("class AstVisitor");
        src.block_begin()
        src.access_mod("public")
        for cls in nodes:
            src.ln("virtual void visit(" + cls.name + "& " + cls.name[-4:].lower() + ");")

        src.block_end(";")

        src.br()
        src.ln("}")   # namespace

        return src.str


    def generate_cpp():
        src = Source()

        def generate_class(cls):
            # Constructor
            src.ln(cls.name + "::" + cls.name + "(" + cls.constructor + ")")
            src.append("\t: ")

            init = ""
            for field in cls.fields:
                init += field.name + "(" + field.name + "), "
            init = init[:-2] + " {}"
            src.ln(init)
            src.br()

            # Parent getter
            src.ln("AstNode* " + cls.name + "::getParent() { return parent; }\n")

            # Visitor
            src.ln("void " + cls.name + "::accept(AstVisitor& visitor) { visitor.visit(*this); }")

            # factory
            src.ln(cls.ptrName + " " + cls.name + "::create(" + cls.constructor + ")")
            src.block_begin()

            new = cls.name + "* n = new " + cls.name + "("
            for field in cls.fields:
                new += field.name + ", "
            new = new[:-2] + ");"
            src.ln(new)

            for field in cls.fields:
                if field.typename in ("ExprPtr", "StmtPtr"):
                    src.ln(field.name + "->parent = n;")

            src.ln("return std::shared_ptr<" + cls.name + ">(n);")
            src.block_end()
            src.br()

        src.include("\"flo/dev/ast.h\"")
        src.br()
        src.ln("using namespace flo;")
        src.br()

        # Classes
        for cls in nodes:
            generate_class(cls)

        # Visitor
        for cls in nodes:
            arg = cls.name[-4:].lower()
            src.ln("void flo::AstVisitor::visit(" + cls.name + "& " + arg + ")")
            src.block_begin()

            for field in cls.fields:
                if field.typename[-3:] == "Ptr":
                    src.ln(arg + "." + field.name + "->accept(*this);")

            src.block_end()


        return src.str

    def write(path, s):
        with open(path, "w") as outfile:
            outfile.write(s)

    write(header_output_path, generate_header())
    write(source_output_path, generate_cpp())


