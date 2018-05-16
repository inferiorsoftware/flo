#include "flo/dev/compiler.h"
#include "flo/runtime/runtime.h"

#include <iostream>
#include <vector>

int repl()
{
	flo::Runtime rt;

	while(true)
	{
		std::cout << "\n>";

		std::string input;
		getline(std::cin, input);

		// Quit
		if(input == "\\quit" || input == "\\q") { return 0; }

		std::optional<flo::Chunk> ch = flo::compile(flo::Source("REPL", input));

		if(ch.has_value())
		{
			rt.run(ch.value());
		}
	}
}

int main(int argc, char *argv[])
{
	std::cout << "f l o" << std::endl;
	return repl();
}


