#include "documentreader.hpp"

#include "util.hpp"

#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <cctype>

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
	static std::unordered_map<std::string, std::string> escapeSequences = {
		{ "lt", "<" },
		{ "gt", ">" },
		{ "amp", "&" },
	};

	size_t n = fileContent.length();
	size_t i;

	auto isFollowedBySubstring = [&fileContent, &i](const std::string &str) -> bool {
		return std::strncmp(&fileContent[i + 1], str.c_str(), str.length()) == 0;
	};

	for(i = 0; i < n; ++i) {
		char c = fileContent[i];
		if(std::isprint(c)) {
			switch(c) {
				case '<': {
					if(isFollowedBySubstring("script")) {
						while(i < n && !isFollowedBySubstring("</script>")) i++;
					} else if(isFollowedBySubstring("style")) {
						while(i < n && !isFollowedBySubstring("</style>")) i++;
					} else {
						while(i < n && c != '>') c = fileContent[++i];
					}
					buffer += ' ';
				} break;
				case '&': {
					size_t j = i; std::string escape;
					while(j < n && c != ';' && j - i < 10) {
						escape += (c = fileContent[++j]);
					}
					escape.pop_back(); // remove the semicolon
					
					if(escapeSequences.find(escape) != escapeSequences.end()) {
						buffer += escapeSequences.at(escape);
						i += (escape.length() + 1);
					} else {
						throw std::runtime_error("unknown escape sequence: &"s + escape + ";"s);
					}
				} break;
				default: {
					buffer += c;
				}
			}
		}
	}
}

} // namespace hotdocs