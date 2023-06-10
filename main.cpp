#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdarg>
#include <filesystem>

void error(const char *format, ...) {
	std::cerr << "error: ";
	va_list va;
	va_start(va, format);
	std::vfprintf(stderr, format, va);
	va_end(va);
	std::cerr << std::endl;
}

namespace fs = std::filesystem;

class Indexer {
public:
	static void indexDirectory(const std::string &directory) {
		for (const auto &entry : fs::recursive_directory_iterator(directory)) {
			std::cout << entry.path().generic_string() << std::endl;
		}
	}
};

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
					// fs::path directory = directory;
					std::string indexLocation = next_arg();
					Indexer::indexDirectory(directory);
				} else {
					error("no index location provided");
					return 1;
				}
			} else {
				error("no directory provided");
				return 1;
			}
		} else if(subcommand == "query") {
			// TODO
		} else {
			error("unknown subcommand '%s'", subcommand.c_str());
			return 1;
		}
	} else {
		error("no subcommand provided");
		return 1;
	}

	return 0;
}