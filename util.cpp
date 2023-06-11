#include "util.hpp"

#include <cstring>

namespace hotdocs {

std::string errnoToString() {
	static const int bufferSize = 256;
	char buffer[bufferSize];
	strerror_s(buffer, bufferSize, errno);
	return buffer;
}

} // namespace hotdocs