#include "indexer.hpp"

#include <cassert>

int main(int argc, char const *argv[]) {
	auto next_arg = [&]() -> const char *
	{
		assert(argc);
		const char *arg = *argv;
		--argc;
		++argv;
		return arg;
	};

	std::string program_name = next_arg();

	if(argc) {
		std::string subcommand = next_arg();
		if(subcommand == "index") {
			if(argc) {
				std::string directory = next_arg();
				if(argc) {
					std::string indexLocation = next_arg();
					hotdocs::Indexer::indexDirectory(directory);
				} else {
					hotdocs::error("no index location provided");
					return 1;
				}
			} else {
				hotdocs::error("no directory provided");
				return 1;
			}
		} else if(subcommand == "query") {
			// TODO
		} else {
			hotdocs::error("unknown subcommand '", subcommand, "'");
			return 1;
		}
	} else {
		hotdocs::error("no subcommand provided");
		return 1;
	}

	return 0;
}