#include "flo/lang/environment.h"


void flo::Environment::Types::initType(flo::Type* type)
{
	allTypes.insert(type);
}
