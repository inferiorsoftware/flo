#include "flo/dev/token.h"

std::string flo::Token::getName()
{
	switch(type)
	{
	case Type::EndStmt:		return "EndStmt";

	case Type::Plus:		return "Plus";
	case Type::Minus:		return "Minus";
	case Type::Star:		return "Start";
	case Type::SlashFwd:	return "SlashFwd";
	case Type::Pipe:		return "Pipe";

	case Type::String:		return "String";

	case Type::Error:		return "ERR";
	}
}
