#include "indexer.hpp"

#include "documentreader.hpp"
#include "wordlexer.hpp"

#include <unordered_map>
#include <cctype>

namespace hotdocs {

void Indexer::indexDirectory(const std::string &directory, std::ostream &sink) {
	std::unordered_map<std::string, uint32_t> df;
	std::unordered_map<std::string, std::unordered_map<std::string, uint32_t>> files;

	for (const auto &entry : fs::recursive_directory_iterator(directory)) {
		fs::path path = entry.path();
		std::string genericPath = path.generic_string();

		if(!entry.is_regular_file()) {
			hotdocs::info("file ", genericPath, " is not a regular file, skipping");
			continue;
		}

		std::unordered_map<std::string, uint32_t> tf;

		hotdocs::info("indexing ", genericPath);
		try {
			std::string content = DocumentReader::documentContents(entry.path());

			for(auto &c : content) c = std::tolower(c);

			WordLexer lexer(content);
			std::string token;

			more:
			token = lexer.nextToken();
			if(token.length()) {
				// TODO: stem the token

				if(token.length() > 1) ++tf[token];
				
				goto more;
			}
		} catch(const std::runtime_error &ex) {
			hotdocs::error(ex.what());
		}

		for(auto &[t, f] : tf) {
			df[t] += f;
		}

		files[genericPath] = std::move(tf);
	}

	sink << "{";
		sink << "\"df\":{";
		{
			bool isfirst = true;
			for(auto &[t, f] : df) {
				if(isfirst) {
					isfirst = false;
				} else {
					sink << ',';
				}
				sink << '"' << t << "\":" << f;
			}
		}
		sink << "},\"files\":{";
		{
			bool isfirst = true;
			for(auto &[filename, tf] : files) {
				if(isfirst) {
					isfirst = false;
				} else {
					sink << ',';
				}
				sink << "\"" << filename << "\":{";

				{
					bool isfirst = true;
					for(auto &[t, f] : tf) {
						if(isfirst) {
							isfirst = false;
						} else {
							sink << ',';
						}
						sink << "\"" << t << "\":" << f;
					}
				}

				sink << "}";
			}
		}
		sink << "}";
	sink << "}";
}

} // namespace hotdocs