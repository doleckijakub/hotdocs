#pragma once

#include <iostream>
#include <filesystem>
#include <string>

namespace hotdocs {

namespace fs = std::filesystem;
using namespace std::string_literals;

template<typename Arg, typename... Args>
static void error(Arg&& arg, Args&&... args) {
	std::cerr << "error: " << std::forward<Arg>(arg);
	((std::cerr << std::forward<Args>(args)), ...);
	std::cerr << std::endl;
}

template<typename Arg, typename... Args>
static void info(Arg&& arg, Args&&... args) {
	std::cerr << "info: " << std::forward<Arg>(arg);
	((std::cerr << std::forward<Args>(args)), ...);
	std::cerr << std::endl;
}

std::string errnoToString();

} // namespace hotdocs