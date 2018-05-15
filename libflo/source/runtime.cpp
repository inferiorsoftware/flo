#include "flo/runtime/runtime.h"
#include "flo/bytecode/opcode.h"
#include "flo/lang/number.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

void flo::Runtime::push(flo::ValuePtr v)
{
	stack.push_back(v);
}

flo::ValuePtr flo::Runtime::pop()
{
	ValuePtr p = stack.back();
	stack.pop_back();
	return p;
}

std::pair<flo::ValuePtr, flo::ValuePtr> flo::Runtime::pop2()
{
	ValuePtr b = pop();
	ValuePtr a = pop();
	return std::make_pair(a, b);
}

int flo::Runtime::run(flo::Chunk& chunk)
{

	auto ip = chunk.begin();

	bool running = true;
	int result = 0;
	while(running)
	{
		const uint8_t op = *ip;

		//TMP!
		{
			std::cout << std::setw(7) << flo::dbg::toString((Opcode) op);

			if(stack.empty())
			{
				std::cout << " [ ]" << std::endl;
			}
			else
			{
				std::stringstream ss;
				ss << " [";
				for(auto i = stack.rbegin(); i!=stack.rend(); ++i)
				{
					ss << " "; (*i)->toString(ss); ss << " |";
				}
				std::string str = ss.str();
				str = str.substr(0, str.size() - 1);
				std::cout << str << "]" << std::endl;
			}
		}


		switch((Opcode) op)
		{
		case Opcode::End:
			return 0;

		case Opcode::Const:
			push(chunk.constant(*(++ip)));
			break;

		case Opcode::Add:
			{
				auto v = pop2();
				push(Number::create(v.first->asNumber() + v.second->asNumber()));
			}
			break;

		case Opcode::Sub:
			{
				auto v = pop2();
				push(Number::create(v.first->asNumber() - v.second->asNumber()));
			}
			break;

		case Opcode::Mul:
			{
				auto v = pop2();
				push(Number::create(v.first->asNumber() * v.second->asNumber()));
			}
			break;

		case Opcode::Div:
			{
				auto v = pop2();
				push(Number::create(v.first->asNumber() / v.second->asNumber()));
			}
			break;

		case Opcode::Out:
			{
				std::stringstream ss;
				pop()->toString(ss);
				std::cout << ss.str() << std::endl;
			}
			break;

		default:
			std::cerr << "Bad opcode: " << op << std::endl;
			return 1;
		}

		ip++;
	}
}
