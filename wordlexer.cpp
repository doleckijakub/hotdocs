#include "wordlexer.hpp"

#include <cctype>

namespace hotdocs {

WordLexer::WordLexer(const std::string &content) : content(content) {
	len = content.length();
	cur = 0;
}

typedef int (*charPredicate)(int);

std::string WordLexer::nextToken() {
	while(cur < len && std::isspace(content[cur])) cur++;

	if(cur == len) return "";

	auto chop = [&](size_t n) -> std::string {
		auto token = content.substr(cur, n);
		cur += n;
		return token;
	};

	auto chop_while = [&](charPredicate predicate) -> std::string {
		size_t n = 0;
		while(cur + n < len && predicate(content[cur + n])) {
			n++;
		}
		return chop(n);
	};

	if(std::isdigit(content[cur])) return chop_while(std::isdigit);
	if(std::isalpha(content[cur])) return chop_while(std::isalnum);

	return chop(1);
}

} // namespace hotdocs