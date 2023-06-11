#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdarg>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstring>

namespace hotdocs {

template<typename Arg, typename... Args>
static void error(Arg&& arg, Args&&... args) {
	std::cerr << "error: ";
	std::cerr << std::forward<Arg>(arg);
	((std::cerr << std::forward<Args>(args)), ...);
	std::cerr << std::endl;
}

template<typename Arg, typename... Args>
static void info(Arg&& arg, Args&&... args) {
	std::cerr << "info: ";
	std::cerr << std::forward<Arg>(arg);
	((std::cerr << std::forward<Args>(args)), ...);
	std::cerr << std::endl;
}

std::string errnoToString() {
	static const int bufferSize = 256;
	char buffer[bufferSize];
	strerror_s(buffer, bufferSize, errno);
	return buffer;
}

class WordLexer {
public:
	std::string lex(const std::string &content) {

	}
};

namespace fs = std::filesystem;
using namespace std::string_literals;

class DocumentReader {

	static void readFileToString(const fs::path& path, std::string &fileContent) {
		std::ifstream ifs(path, std::ios::in | std::ios::binary);
		if(ifs) {
			ifs.seekg(0, std::ios::end);
			fileContent.resize(ifs.tellg());
			ifs.seekg(0, std::ios::beg);
			ifs.read(&fileContent[0], fileContent.size());
			ifs.close();
		} else {
			throw std::runtime_error("Failed to open file: '"s + errnoToString() + "'");
		}
	}

	static void tryParseAsHTML(const std::string &fileContent, std::string &buffer) {
		info("length: ", fileContent.length());
	}

public:

	static std::string documentContents(const fs::path &path) {
		std::string fileContent, buffer;

		readFileToString(path, fileContent);

		auto extension = path.extension().string();
		if(extension == ".html") {
			tryParseAsHTML(fileContent, buffer);
			return buffer;
		}

		throw std::runtime_error("unknown file extension "s + extension);
	}
};

class Indexer {
public:
	static void indexDirectory(const std::string &directory) {
		for (const auto &entry : fs::recursive_directory_iterator(directory)) {
			fs::path path = entry.path();
			std::string genericPath = path.generic_string();

			if(!entry.is_regular_file()) {
				hotdocs::info("file ", genericPath, " is not a regular file, skipping");
				continue;
			}

			hotdocs::info("indexing ", genericPath);
			try {
				std::string content = DocumentReader::documentContents(entry.path());
				
			} catch(const std::runtime_error &ex) {
				hotdocs::error(ex.what());
			}


		}
	}
};

} // namespace hotdocs

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