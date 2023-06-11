#pragma once

#include "util.hpp"

namespace hotdocs {

class DocumentReader {

	static void readFileToString(const fs::path& path, std::string &fileContent);

	static void tryParseAsHTML(const std::string &fileContent, std::string &buffer);

public:

	static std::string documentContents(const fs::path &path);
};

} // namespace hotdocs