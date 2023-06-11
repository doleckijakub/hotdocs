#include "documentreader.hpp"

#include "util.hpp"

#include <fstream>
#include <stdexcept>

namespace hotdocs {

void DocumentReader::readFileToString(const fs::path& path, std::string &fileContent) {
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

std::string DocumentReader::documentContents(const fs::path &path) {
	std::string fileContent, buffer;

	readFileToString(path, fileContent);

	auto extension = path.extension().string();
	if(extension == ".html") {
		tryParseAsHTML(fileContent, buffer);
		return buffer;
	}

	throw std::runtime_error("unknown file extension "s + extension);
}

void DocumentReader::tryParseAsHTML(const std::string &fileContent, std::string &buffer) {
	info("length: ", fileContent.length());
}

} // namespace hotdocs