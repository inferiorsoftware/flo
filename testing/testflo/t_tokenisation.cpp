#include "gtest/gtest.h"
#include "flo/dev/token.h"
#include "flo/dev/tokeniser.h"
#include "flo/dev/compile_error.h"

using namespace flo;


TEST(TokenisationTest, EndStmt)
{
	EXPECT_EQ(flo::tokenise(";")[0].type, Token::Type::EndStmt)
			<< "Semicolon ends statement.";

	EXPECT_EQ(flo::tokenise("\n,")[0].type, Token::Type::EndStmt)
			<< "Line-break ends statement.";

	EXPECT_EQ(flo::tokenise(";;\n\n").size(), 1)
			<< "Ignore multiple end statements.";
}

TEST(TokenisationTest, Plus)
{
	EXPECT_EQ(flo::tokenise("+")[0].type, Token::Type::Plus);
}

TEST(TokenisationTest, Subtract)
{
	EXPECT_EQ(flo::tokenise("-")[0].type, Token::Type::Minus);
}

TEST(TokenisationTest, Star)
{
	EXPECT_EQ(flo::tokenise("*")[0].type, Token::Type::Star);
}

TEST(TokenisationTest, SlashFwd)
{
	EXPECT_EQ(flo::tokenise("/")[0].type, Token::Type::Stroke);
}

TEST(TokenisationTest, Pipe)
{
	EXPECT_EQ(flo::tokenise("|")[0].type, Token::Type::Pipe);
}

TEST(TokenisationTest, LineComment)
{
	EXPECT_EQ(flo::tokenise("// comment +-/dwqmk").size(), 0)
			<< "// Ignores line.";

	EXPECT_EQ(flo::tokenise("// comment\n+").size(), 1)
			<< "// ignores until end of line only.";
}

TEST(TokenisationTest, MultilineComment)
{
	EXPECT_EQ(flo::tokenise("/*\nfew\n*/").size(), 0)
			<< "/*...*/ Discards multiple lines.";

	EXPECT_EQ(flo::tokenise("/**/ + /**/").size(), 1)
			<< "Tokens allowed between /**/.../**/ on single lines.";

	EXPECT_EQ(flo::tokenise("/* /* */ + */").size(), 0)
			<< "/**/ comments nest.";

	flo::CompileErrorStore oneErrs;
	flo::tokenise("/*dwqhui", &oneErrs);
	EXPECT_EQ(oneErrs.errors.size(), 1)
			<< "One unterminated /* emmits 1 error.";
	EXPECT_EQ(oneErrs.errors[0].type, CompileError::Type::UnterminatedToken)
			<< "Unterminated /* emmits CompileError::Type::UnterminatedToken.";

	flo::CompileErrorStore twoErrs;
	flo::tokenise("/*/*few/*432*/", &twoErrs);
	EXPECT_EQ(twoErrs.errors.size(), 2)
			<< "Two unterminated /* emmits 2 errors.";
}

TEST(TokenisationTest, String)
{
	EXPECT_EQ(flo::tokenise("\"blabla\"")[0].type, Token::Type::String)
			<< "\"...\" is a string literal.";

	flo::CompileErrorStore untermErrs;
	flo::tokenise("\"hui", &untermErrs);
	EXPECT_EQ(untermErrs.errors.size(), 1)
			<< "Unterminated \" emmits 1 error.";
	EXPECT_EQ(untermErrs.errors[0].type, CompileError::Type::UnterminatedToken)
			<< "Unterminated \" emmits CompileError::Type::UnterminatedToken.";

}

TEST(TokenisationTest, DigitTest)
{
	std::vector<flo::Token> tkns = flo::tokenise("5");
	ASSERT_EQ(tkns.size(), 1)
			<< "Single digit produces single token;";

	EXPECT_EQ(tkns[0].type, Token::Type::Number)
			<< "Single digit produces number; got " +tkns[0].getName();
}

TEST(TokenisationTest, DigitSequenceTest)
{
	std::vector<flo::Token> tkns = flo::tokenise("55555");;
	ASSERT_EQ(tkns.size(), 1)
			<< "Digit sequence produces single token;";

	EXPECT_EQ(tkns[0].type, Token::Type::Number)
			<< "Digit sequence produces number; got " +tkns[0].getName();
}

TEST(TokenisationTest, DecimalTest)
{
	std::vector<flo::Token> tkns = flo::tokenise("5.5");

	ASSERT_EQ(tkns.size(), 1)
			<< "Decimal produces single token;";

	EXPECT_EQ(tkns[0].type, Token::Type::Number)
			<< "Decimal produces number; got " +tkns[0].getName();
}


