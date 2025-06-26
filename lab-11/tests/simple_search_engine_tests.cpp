#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "parser.h"
#include "btree.h"

namespace fs = std::filesystem;

// Helper function to create a temporary file with given contentf
void create_temp_file(const std::string& filename, const std::string& content) {
    std::ofstream outfile(filename);
    outfile << content;
    outfile.close();
}

// Helper function to remove a file if it exists
void remove_temp_file(const std::string& filename) {
    if (fs::exists(filename)) {
        fs::remove(filename);
    }
}

// Test for AccessNode function
TEST(ParserTest, AccessNode) {
    BTree bt;
    std::map<size_t, std::string> doc_names;

    std::string filename = "testfile.txt";
    std::string content = "Hello world\n";
    create_temp_file(filename, content);

    Parser::ProcessFile(fs::path(filename), bt, doc_names);

    std::ofstream outfile("testindex.txt", std::ios::out);
    Parser::AccessNode(bt.get_root(), outfile);
    outfile.close();

    std::ifstream infile("testindex.txt");
    std::string line;
    ASSERT_TRUE(std::getline(infile, line));
    infile.close();

    remove_temp_file(filename);
    remove_temp_file("testindex.txt");
}

// Test for ProcessDirectory function
TEST(ParserTest, ProcessDirectory) {
    BTree bt;
    std::map<size_t, std::string> doc_names;

    fs::create_directory("testdir");
    create_temp_file("testdir/1.txt", "Hello world\n");
    create_temp_file("testdir/2.txt", "Hello again\n");

    Parser::ProcessDirectory(fs::path("testdir"), bt, doc_names);

    std::ofstream outfile("testdirindex.txt", std::ios::out);
    Parser::AccessNode(bt.get_root(), outfile);
    outfile.close();

    std::ifstream infile("testdirindex.txt");
    std::string line;
    ASSERT_TRUE(std::getline(infile, line));
    infile.close();

    remove_temp_file("testdir/1.txt");
    remove_temp_file("testdir/2.txt");
    remove_temp_file("testdirindex.txt");
    fs::remove_all("testdir");
}

// Test for ProcessFile function
TEST(ParserTest, ProcessFile) {
    BTree bt;
    std::map<size_t, std::string> doc_names;

    std::string filename = "testfile.txt";
    std::string content = "Hello world\n";
    create_temp_file(filename, content);

    Parser::ProcessFile(fs::path(filename), bt, doc_names);

    std::ofstream outfile("testfileindex.txt", std::ios::out);
    Parser::AccessNode(bt.get_root(), outfile);
    outfile.close();

    std::ifstream infile("testfileindex.txt");
    std::string line;
    ASSERT_TRUE(std::getline(infile, line));
    infile.close();

    remove_temp_file(filename);
    remove_temp_file("testfileindex.txt");
}

// Test for tokenize function
TEST(ParserTest, Tokenize) {
    std::string query = "hello world";
    std::vector<std::string> tokens = Parser::tokenize(query);
    ASSERT_EQ(tokens.size(), 2);
    ASSERT_EQ(tokens[0], "hello");
    ASSERT_EQ(tokens[1], "world");
}

// Test for evaluate_boolean_query function
TEST(ParserTest, EvaluateBooleanQuery) {
    BTree bt;
    std::map<size_t, std::string> doc_names;

    create_temp_file("1.txt", "hello world");
    create_temp_file("2.txt", "hello again");

    Parser::ProcessFile("1.txt", bt, doc_names);
    Parser::ProcessFile("2.txt", bt, doc_names);

    std::vector<std::string> tokens = { "hello" };
    std::vector<size_t> doc_ids = Parser::evaluate_boolean_query(tokens, bt);
    ASSERT_EQ(doc_ids.size(), 2);

    remove_temp_file("1.txt");
    remove_temp_file("2.txt");
}

// Test for process_user_query function
TEST(ParserTest, ProcessUserQuery) {
    BTree bt;
    std::map<size_t, std::string> doc_names;

    create_temp_file("1.txt", "hello world");
    create_temp_file("2.txt", "hello again");

    Parser::ProcessFile("1.txt", bt, doc_names);
    Parser::ProcessFile("2.txt", bt, doc_names);

    std::istringstream input("hello\nexit\n");
    std::streambuf* orig = std::cin.rdbuf();
    std::cin.rdbuf(input.rdbuf());

    std::ostringstream output;
    std::streambuf* orig_cout = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    Parser::process_user_query(bt, doc_names);

    std::cin.rdbuf(orig);
    std::cout.rdbuf(orig_cout);

    std::string output_str = output.str();
    ASSERT_NE(output_str.find("Matching documents:"), std::string::npos);

    remove_temp_file("1.txt");
    remove_temp_file("2.txt");
}