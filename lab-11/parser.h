#pragma once

#include "BTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <map>
#include <algorithm>

namespace fs = std::filesystem;

class Parser {
public:
	static void AccessNode(PBTNode pnode, std::ofstream& outfile);
	static void ProcessDirectory(const fs::path& dir_path, BTree& bt, std::map<size_t, std::string>& doc_names);
	static void ProcessFile(const fs::path& file_path, BTree& bt, std::map<size_t, std::string>& doc_names);
	static std::vector<std::string> tokenize(const std::string& query);
	static std::vector<size_t> evaluate_boolean_query(const std::vector<std::string>& tokens, BTree& bt);
	static void process_user_query(BTree& bt, const std::map<size_t, std::string>& doc_names);
};