#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

#define M 71 // BTree Order (odd)

struct Position {
	size_t pos_num;
	Position *next_position;

	Position() {
		next_position = NULL;
	}
};

struct Posting {
	size_t doc_id;
	Posting *next_posting;
	size_t term_frequency;
	Position *pos_info;

	Posting() {
		next_posting = NULL;
		term_frequency = 0;
		pos_info = NULL;
	}
};

struct Record {
	std::string *word;
	Posting *posting_list;

	Record() {
		word = NULL;
		posting_list = NULL;
	}
};

struct BTNode {
	size_t data_num;
	BTNode *parent;
	Record data[M - 1];
	BTNode *child[M];

	BTNode() {
		data_num = 0;

		parent = NULL;

		for(size_t i = 0; i < M; i++)
			child[i] = NULL;
	}
};
typedef BTNode *PBTNode;

class BTree {
private:
	PBTNode root;

public:
	BTree();
	bool insert(std::string *a_word, size_t doc_id, size_t pos_num);
	bool search(Record &a_record);
	PBTNode get_root();
};