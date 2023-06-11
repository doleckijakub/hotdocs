#include "indexer.hpp"

#include "documentreader.hpp"
#include "wordlexer.hpp"

#include <unordered_map>
#include <cctype>

namespace hotdocs {

void Indexer::indexDirectory(const std::string &directory, std::ostream &sink) {
	std::unordered_map<std::string, uint32_t> df;
	std::unordered_map<std::string, std::pair<uint32_t, std::unordered_map<std::string, uint32_t>>> files;

	size_t filesTotal = 0;
	for (const auto &entry : fs::recursive_directory_iterator(directory))
		if(entry.is_regular_file())
			filesTotal++;

	size_t i = 0;
	for (const auto &entry : fs::recursive_directory_iterator(directory)) {
		fs::path path = entry.path();
		std::string genericPath = path.generic_string();

		std::string shortName = path.parent_path().stem().string() + "/"s + path.stem().string() + path.extension().string();

		if(!entry.is_regular_file()) {
			hotdocs::info("file ", genericPath, " is not a regular file, skipping");
			continue;
		}

		std::unordered_map<std::string, uint32_t> tf;

		hotdocs::info("indexing ", ++i, "/", filesTotal, " ", shortName);
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
			hotdocs::error("something went wrong when parsing: '", ex.what(), "', skipping");
			continue;
		}

		uint32_t total = 0;

		for(auto &[t, f] : tf) {
			df[t] += f;
			total += f;
		}

		files[genericPath] = std::make_pair(total, std::move(tf));
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
			for(auto &[filename, file] : files) {
				if(isfirst) {
					isfirst = false;
				} else {
					sink << ',';
				}
				sink << "\"" << filename << "\":{";

				auto &[total, tf] = file;

				sink << "\"total\":" << total << ",\"words\":{";

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

				sink << "}}";
			}
		}
		sink << "}";
	sink << "}";
}

} // namespace hotdocs