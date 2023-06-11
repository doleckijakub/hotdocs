#include "indexer.hpp"

#include "documentreader.hpp"

namespace hotdocs {

void Indexer::indexDirectory(const std::string &directory) {
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

} // namespace hotdocs