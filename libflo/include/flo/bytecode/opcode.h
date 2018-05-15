#pragma once

#include <cstdint>
#include <string>
namespace flo
{

enum class Opcode : uint8_t
{
	End		= 0x00,		/**< End of chunk. */
	Const	= 0x01,		/**< Push a constant from the chunk. */

	Add		= 0x10,		/**< Sum the top two values in the stack. */
	Sub		= 0x11,		/**< Subtract the top two values in the stack. */
	Mul		= 0x12,		/**< Multiply the top two values in the stack. */
	Div		= 0x13,		/**< Divide the top two values in the stack. */

	Out		= 0xf0,		/**< Pop & print. */
};


namespace dbg
{
std::string toString(Opcode o);
}


}
