#pragma once

#include <string>

namespace flo
{

/**
 * Some source code with an identifier.
 */
struct Source
{
	/** A unique id for the source, e.g, a file name. */
	const std::string id;

	/** The body of the source code. */
	const std::string code;

	Source(const std::string id,
		   const std::string code);

	void getDebugInfoAtIndex(const int index, int& lineNum, int& colNum, std::string& line);

	static Source load(const std::string path);
};

}
