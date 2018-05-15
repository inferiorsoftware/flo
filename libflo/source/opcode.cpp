#include "flo/bytecode/opcode.h"

std::string flo::dbg::toString(flo::Opcode o)
{
	switch(o)
	{
	case Opcode::End:	return "end";
	case Opcode::Const:	return "const";

	case Opcode::Add:	return "add";
	case Opcode::Sub:	return "sub";
	case Opcode::Mul:	return "mul";
	case Opcode::Div:	return "div";

	case Opcode::Out:	return "out";

	default:			return "ERR";
	}
}
