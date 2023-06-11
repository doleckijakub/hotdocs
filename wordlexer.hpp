#pragma once

#include "util.hpp"

namespace hotdocs {

class WordLexer {
	const std::string &content;
	size_t cur, len;

public:

	WordLexer(const std::string &content);

	std::string nextToken();

};

} // namespace hotdocs