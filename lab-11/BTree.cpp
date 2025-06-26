#include "btree.h"

BTree::BTree() {
	root = new BTNode();
}

bool BTree::insert(std::string* a_word, size_t doc_id, size_t pos_num) {
	PBTNode current_pnode = root;

	while (true) {
		if (current_pnode->child[0] == NULL) {
			for (size_t i = 0; i < current_pnode->data_num; i++) {
				if (*(current_pnode->data[i].word) == *a_word) {
					Posting* current_posting = current_pnode->data[i].posting_list;

					while (true) {
						if (current_posting->doc_id == doc_id) {
							current_posting->term_frequency++;

							Position* tmp_position = new Position();

							tmp_position->pos_num = pos_num;

							Position* current_position = current_posting->pos_info;
							while (current_position->next_position) {
								current_position = current_position->next_position;
							}

							current_position->next_position = tmp_position;
							break;
						} else if (current_posting->next_posting == NULL) {
							Posting* tmp_new_posting = new Posting();

							tmp_new_posting->doc_id = doc_id;
							tmp_new_posting->term_frequency = 1;
							tmp_new_posting->pos_info = new Position();
							tmp_new_posting->pos_info->pos_num = pos_num;

							current_posting->next_posting = tmp_new_posting;
							break;
						} else {
							current_posting = current_posting->next_posting;
						}
					}

					delete a_word;

					return true;
				}
			}
			break;
		} else {
			size_t i;
			for (i = 0; i < current_pnode->data_num; i++) {
				if (*(current_pnode->data[i].word) == *a_word) {
					Posting* current_posting = current_pnode->data[i].posting_list;
					while (true) {
						if (current_posting->doc_id == doc_id) {
							current_posting->term_frequency++;

							Position* tmp_position = new Position();
							tmp_position->pos_num = pos_num;

							Position* current_position = current_posting->pos_info;
							while (current_position->next_position) {
								current_position = current_position->next_position;
							}

							current_position->next_position = tmp_position;
							break;
						} else if (current_posting->next_posting == NULL) {
							Posting* tmp_new_posting = new Posting();

							tmp_new_posting->doc_id = doc_id;
							tmp_new_posting->term_frequency = 1;
							tmp_new_posting->pos_info = new Position();
							tmp_new_posting->pos_info->pos_num = pos_num;

							current_posting->next_posting = tmp_new_posting;
							break;
						} else {
							current_posting = current_posting->next_posting;
						}
					}

					delete a_word;
					return true;
				}

				if (*(current_pnode->data[i].word) > *a_word) {
					break;
				}
			}

			current_pnode = current_pnode->child[i];
		}
	}

	Record tmp_record;

	tmp_record.word = a_word;
	tmp_record.posting_list = new Posting();
	tmp_record.posting_list->doc_id = doc_id;
	tmp_record.posting_list->term_frequency = 1;
	tmp_record.posting_list->pos_info = new Position();
	tmp_record.posting_list->pos_info->pos_num = pos_num;

	PBTNode tmp_right_pointer = NULL;
	while (true)
	{
		if (current_pnode->data_num != M - 1) {
			int i;
			for (i = current_pnode->data_num - 1; i >= 0; i--) {
				if (*(current_pnode->data[i].word) > *(tmp_record.word)) {
					current_pnode->data[i + 1] = current_pnode->data[i];
					current_pnode->child[i + 2] = current_pnode->child[i + 1];
				} else {
					break;
				}
			}

			current_pnode->data[i + 1] = tmp_record;
			current_pnode->data_num++;
			current_pnode->child[i + 2] = tmp_right_pointer;

			if (tmp_right_pointer) {
				tmp_right_pointer->parent = current_pnode;
			}

			return true;
		}
		else {
			PBTNode new_node = new BTNode();

			size_t i = M - 2;
			while (true) {
				if (i == M - 2) {
					if (*(current_pnode->data[i].word) > *(tmp_record.word)) {
						new_node->data[0] = current_pnode->data[i];
						new_node->child[1] = current_pnode->child[i + 1];
					} else {
						new_node->data[0] = tmp_record;
						new_node->child[1] = tmp_right_pointer;

						break;
					}
				} else {
					if (*(current_pnode->data[i].word) > *(tmp_record.word)) {
						current_pnode->data[i + 1] = current_pnode->data[i];
						current_pnode->child[i + 2] = current_pnode->child[i + 1];
					} else {
						current_pnode->data[i + 1] = tmp_record;
						current_pnode->child[i + 2] = tmp_right_pointer;

						if (tmp_right_pointer)
							tmp_right_pointer->parent = current_pnode;

						break;
					}
				}

				i--;
				if (i == -1) {
					current_pnode->data[i + 1] = tmp_record;
					current_pnode->child[i + 2] = tmp_right_pointer;
					if (tmp_right_pointer != NULL)
						tmp_right_pointer->parent = current_pnode;
					break;
				}
			}

			int j = M / 2 - 1;

			new_node->data[j] = new_node->data[0];
			new_node->child[j + 1] = new_node->child[1];

			if (new_node->child[j + 1])
				new_node->child[j + 1]->parent = new_node;

			i = M - 2;
			j--;

			while (j >= 0) {
				new_node->data[j] = current_pnode->data[i];
				new_node->child[j + 1] = current_pnode->child[i + 1];

				if (new_node->child[j + 1])
					new_node->child[j + 1]->parent = new_node;

				j--;
				i--;
			}

			tmp_record = current_pnode->data[i];
			new_node->child[0] = current_pnode->child[i + 1];
			if (new_node->child[0])
				new_node->child[0]->parent = new_node;

			current_pnode->data_num = M / 2;
			new_node->data_num = M / 2;
			tmp_right_pointer = new_node;

			if (current_pnode->parent == NULL) {
				PBTNode new_root_node = new BTNode();

				new_root_node->data_num = 1;
				new_root_node->data[0] = tmp_record;

				current_pnode->parent = new_root_node;
				tmp_right_pointer->parent = new_root_node;

				new_root_node->child[0] = current_pnode;
				new_root_node->child[1] = tmp_right_pointer;

				root = new_root_node;

				return true;                            
			} else {
				current_pnode = current_pnode->parent;
			}
		}
	}
}

bool BTree::search(Record& a_record)
{
	PBTNode current_pnode = root;

	while (true) {
		if (current_pnode->child[0] == NULL) {
			for (size_t i = 0; i < current_pnode->data_num; i++) {
				if (*(current_pnode->data[i].word) == *(a_record.word)) {
					a_record.posting_list = current_pnode->data[i].posting_list;

					return true;
				}
			}

			return false;
		} else {
			size_t i;
			for (i = 0; i < current_pnode->data_num; i++)
			{
				if (*(current_pnode->data[i].word) == *(a_record.word)) {
					a_record.posting_list = current_pnode->data[i].posting_list;
					return true;
				}

				if (*(current_pnode->data[i].word) > *(a_record.word)) {
					break;
				}
			}

			current_pnode = current_pnode->child[i];
		}
	}

}

PBTNode BTree::get_root()
{
	return root;
}