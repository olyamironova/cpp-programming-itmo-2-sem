#include "parser.h"

int main(int argc, char* argv[]) {
	BTree bt;
	std::string path;
	std::map<size_t, std::string> doc_names;

	std::cout << "Enter the path (file or directory):" << std::endl;
	std::cin >> path;

	fs::path p(path);

	if (fs::is_directory(p)) {
		std::cout << "Document Scanning..." << std::endl;
		Parser::ProcessDirectory(path, bt, doc_names);
		std::cout << "Inverted indexing complete!" << std::endl;
	} else if (fs::is_regular_file(p) && p.filename().string().find("index.txt") == std::string::npos) {
		Parser::ProcessFile(p, bt, doc_names);
		std::cout << "Inverted indexing complete!" << std::endl;
	} else {
		std::cout << "Invalid input. Please enter a valid file or directory path." << std::endl;
		return 1;
	}

	std::ofstream outfile("index.txt", std::ios::out);
	std::cout << "Generating an inverted file..." << std::endl;

	Parser::AccessNode(bt.get_root(), outfile);

	outfile.close();

	std::cout << "Inverted file generation success!" << std::endl;

	std::string word;
	Record tmp_record;

	tmp_record.word = &word;

	Posting* curr_posting;
	Position* curr_pos;

	Parser::process_user_query(bt, doc_names);
}
