#pragma once

#include "flo/lang/type.h"

#include <string>
#include <unordered_set>


namespace flo
{

class Environment
{
public:
	class Types
	{
		std::unordered_set<Type*> allTypes;

		Types()
		{

		}

		void initType(Type* type);

	};

	const Types types;
};

}
