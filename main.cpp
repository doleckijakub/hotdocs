#include "indexer.hpp"

#include <cassert>
#include <fstream>

void usage(const std::string &programName) {
	std::cerr << "Usage: " << programName << " index <directory to index> <path to store index.json>" << std::endl;
}

int main(int argc, char const *argv[]) {
	auto next_arg = [&]() -> const char *
	{
		assert(argc);
		const char *arg = *argv;
		--argc;
		++argv;
		return arg;
	};

	std::string programName = next_arg();

	if(argc) {
		std::string subcommand = next_arg();
		if(subcommand == "index") {
			if(argc) {
				std::string directory = next_arg();
				if(argc) {
					std::string indexLocation = (std::filesystem::path(next_arg()) / "index.json").generic_string();

					std::ofstream output(indexLocation);

					if(!output) {
						hotdocs::error("could not write to '", indexLocation, "'");
						return 1;
					}

					hotdocs::Indexer::indexDirectory(directory, output);
					return 0;
				} else {
					hotdocs::error("no index location provided");
					usage(programName);
					return 1;
				}
			} else {
				hotdocs::error("no directory provided");
				usage(programName);
				return 1;
			}
		} else {
			hotdocs::error("unknown subcommand '", subcommand, "'");
			usage(programName);
			return 1;
		}
	} else {
		hotdocs::error("no subcommand provided");
		usage(programName);
		return 1;
	}

	return 0;
}