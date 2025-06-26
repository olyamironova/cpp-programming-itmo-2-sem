#include "parser.h"

namespace fs = std::filesystem;

void Parser::AccessNode(PBTNode pnode, std::ofstream& outfile) {
	size_t i;
	for (i = 0; i < pnode->data_num; i++) {
		std::ostringstream posting_list_str;

		if (pnode->child[i])
			AccessNode(pnode->child[i], outfile);

		Posting* curr_posting = pnode->data[i].posting_list;
		Position* curr_pos;

		while (curr_posting) {
			posting_list_str << '<' << curr_posting->doc_id << ", "
				<< curr_posting->term_frequency << ", ";

			curr_pos = curr_posting->pos_info;

			while (curr_pos) {
				posting_list_str << curr_pos->pos_num;
				if (curr_pos->next_position)
					posting_list_str << ' ';
				curr_pos = curr_pos->next_position;
			}

			posting_list_str << '>';
			curr_posting = curr_posting->next_posting;
		}

		outfile << std::setiosflags(std::ios::left) << std::setw(20) << *pnode->data[i].word << posting_list_str.str() << std::endl;
	}

	if (pnode->child[i])
		AccessNode(pnode->child[i], outfile);
}

void Parser::ProcessDirectory(const fs::path& dir_path, BTree& bt, std::map<size_t, std::string>& doc_names) {

	for (const auto& entry : fs::directory_iterator(dir_path)) {
		if (entry.is_directory()) {
			ProcessDirectory(entry.path(), bt, doc_names);
		}
		else if (entry.is_regular_file() && entry.path().filename().string().find("index.txt") == std::string::npos) {
			std::ifstream infile(entry.path(), std::ios::in);
			if (!infile) {
				std::cout << "Open file error!" << std::endl;
				continue;
			}

			size_t doc_id = std::stoul(entry.path().filename().string().substr(0, entry.path().filename().string().find('.')));
			doc_names[doc_id] = entry.path().filename().string();
			size_t word_counter = 1;
			while (true) {
				std::string* word = new std::string();

				if (!(infile >> *word)) {
					delete word;
					break;
				}

				for (size_t i = 0; i < word->size(); i++) {
					if ((*word)[i] >= 'A' && (*word)[i] <= 'Z') {
						(*word)[i] += 32;
					}
				}

				bt.insert(word, doc_id, word_counter);

				word_counter++;
			}

			infile.close();
		}
	}
}

void Parser::ProcessFile(const fs::path& file_path, BTree& bt, std::map<size_t, std::string>& doc_names) {
	std::ifstream infile(file_path, std::ios::in);
	if (!infile) {
		std::cout << "Open file error!" << std::endl;
		return;
	}

	size_t doc_id = std::stoul(file_path.filename().string().substr(0, file_path.filename().string().find('.')));
	doc_names[doc_id] = file_path.filename().string();
	size_t word_counter = 1;

	while (true) {
		std::string* word = new std::string();

		if (!(infile >> *word)) {
			delete word;
			break;
		}

		for (size_t i = 0; i < word->size(); i++) {
			if ((*word)[i] >= 'A' && (*word)[i] <= 'Z') {
				(*word)[i] += 32;
			}
		}

		bt.insert(word, doc_id, word_counter);
		word_counter++;
	}

	infile.close();
}

std::vector<std::string> Parser::tokenize(const std::string& query) {
	std::vector<std::string> tokens;
	std::stringstream ss(query);
	std::string token;

	while (std::getline(ss, token, ' ')) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}

	return tokens;
}

std::vector<size_t> Parser::evaluate_boolean_query(const std::vector<std::string>& tokens, BTree& bt) {
	std::vector<size_t> doc_ids;
	std::string word;
	Record tmp_record;
	tmp_record.word = &word;
	Posting* curr_posting;
	size_t i = 0;

	while (i < tokens.size()) {
		word = tokens[i];

		if (word == "(") {
			i++;
			continue;
		}

		if (word == ")") {
			i++;
			break;
		}

		if (bt.search(tmp_record)) {
			std::vector<size_t> posting_doc_ids;
			curr_posting = tmp_record.posting_list;

			while (curr_posting) {
				posting_doc_ids.push_back(curr_posting->doc_id);
				curr_posting = curr_posting->next_posting;
			}

			if (i == 0 || (tokens[i - 1] != "AND" && tokens[i - 1] != "OR")) {
				for (auto& c : posting_doc_ids)
					doc_ids.push_back(c);
			}
			else if (tokens[i - 1] == "AND") {
				std::vector<size_t> intersection;
				std::set_intersection(doc_ids.begin(), doc_ids.end(),
					posting_doc_ids.begin(), posting_doc_ids.end(),
					std::back_inserter(intersection));
				doc_ids = intersection;
			}
			else if (tokens[i - 1] == "OR") {
				std::vector<size_t> union_vec;
				std::set_union(doc_ids.begin(), doc_ids.end(),
					posting_doc_ids.begin(), posting_doc_ids.end(),
					std::back_inserter(union_vec));
				doc_ids = union_vec;
			}
		}

		i++;
	}

	return doc_ids;
}

void Parser::process_user_query(BTree& bt, const std::map<size_t, std::string>& doc_names) {
	std::string query;
	std::string _;
	std::getline(std::cin, _);

	while (true) {
		std::cout << "Please enter a query keyword (or type 'exit' to quit): " << std::endl;
		std::getline(std::cin, query);

		if (query == "exit") {
			break;
		}

		std::vector<std::string> tokens = tokenize(query);
		std::vector<size_t> doc_ids = evaluate_boolean_query(tokens, bt);

		if (!doc_ids.empty()) {
			std::cout << "Matching documents:" << std::endl;

			for (size_t doc_id : doc_ids) {
				std::cout << "- " << doc_names.at(doc_id) << std::endl;
			}
		}
		else {
			std::cout << "Retrieval failed" << std::endl;
		}

		std::cout << std::endl;
	}
}