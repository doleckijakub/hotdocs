#pragma once

#include "util.hpp"

#include <ostream>

namespace hotdocs {

class Indexer {
public:

	static void indexDirectory(const std::string &directory, std::ostream &sink);
	
};

} // namespace hotdocs