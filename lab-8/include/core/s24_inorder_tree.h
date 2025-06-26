#pragma once
#include <functional>
#include <limits>

namespace indorder {
	template<typename Key, typename Comparator = std::less<Key>, typename Allocator = std::allocator<Key>>
	class BinarySearchTree {
	private:
		struct BinarySearchTreeNode;
		struct BinarySearchTreeIterator;
		struct BinarySearchTreeIteratorConst;

		template <typename U>
		friend class BinarySearchTreeTestHelper;
	public:
		using key_type = Key;
		using reference = key_type&;
		using const_reference = const key_type&;
		using iterator = BinarySearchTreeIterator;
		using const_iterator = BinarySearchTreeIteratorConst;
		using size_type = std::size_t;

		using tree_type = BinarySearchTree;
		using tree_node = BinarySearchTreeNode;

	private:
		// ��������� �� ������ ������
		tree_node* head_;
		// ���������� ��������� � ������
		size_type size_;
		// ���������� ������ (����� ��� ��������� �������� �����)
		Comparator cmp_;

	public:
		/* ����������� �� ���������, ������� ������ ������ */
		BinarySearchTree() : head_(new tree_node), size(0U) {}

		/* ����������� ����������� (Copy Constructor). ������� ������ �����
		 * ����������� ������ �� ������� other. */
		BinarySearchTree(const tree_type& other) : BinarySearchTree() {
			if (other.Size() > 0) {
				CopyTreeFromOther(other);
			}
		}

		/* �������� ������������ ������������. */
		tree_type& operator=(const tree_type& other) {
			if (this != &other) {
				if (other.Size() > 0) {
					CopyTreeFromOther(other);
				}
				else {
					Clear();
				}
			}
			return *this;
		}

		/* ���������� ������� (Destructor) */
		~BinarySearchTree() {
			Clear();
			delete head_;
			head_ = nullptr;
		}

		/* ������� ���������� ���������� (��� ��������). ��������� ��� ����
		 * �������� �������������. */
		void Clear() noexcept {
			Destroy(Root());
			InitializeHead();
			size_ = 0;
		}

		/* ���������� ���������� ��������� � ���������� */
		size_type Size() const noexcept {
			return size_;
		}

		/* ���������, �������� �� ��������� ������ */
		bool Empty() const noexcept {
			return size_ == 0;
		}

		/* ���������� ������������ ���������� ���������, ������� �����
		 * ��������� ������ ��-�� ����������� ���������� ������� ��� ����������.
		 *
		 * @warning � ���������� �������� max_size() ������� �� ����� ���������.
		 *
		 * @details ��� ����� �� ������� �� ��������� �� ��� ��������� ������, �
		 * ������������ ������ ����������� ����������. �� ����� ���������� ������
		 * ���������� ����� ���� ��������� ���������, ������� max_size(), ��� �����
		 * ��������� ����������� ������.
		 *
		 * ��� ������� ������������ ��������� �������:
		 * 1) ������ size_t ������ ��������� � �������� ��������� ��� �����
		 * ���������. ��������� ������ ���� � ��������� ���������� ����� ���� �
		 * ������, � size_t ������� ������ ������ (� �.�. ����������� ��� ������)
		 * �������. ����� ������� ������������ �������� size_t - ��� ������������
		 * ���������� ���� ������. ��� �������� �� �������� ��� ������
		 * std::numeric_limits<size_t>::max();
		 *
		 * 2) GCC ������������ ������� �������� � �������� ��������� ������������.
		 * ������� ���������� � �.1 ����� ����� �� 2.
		 *
		 * 3) � ������ �������� ��������� �� ������ (��������� ����) -
		 * sizeof(head_), ���������� - sizeof(cmp_), ���������� ��������� ���������
		 * - sizeof(size_), �� ������ - sizeof(tree_type). ����� ����� ����������
		 * ������ �� ���� ��������� ����, ������� ���� ���� � ������� ������ -
		 * sizeof(tree_node). �� ��� �������� �� ������, ���������� � �.2. �����
		 * ������� �������� ������������ ���������� ������, ��������� ��� ��������
		 * ����� ������.
		 *
		 * 4) ����� ���������� ������������ ���������� ��������� � ����������, �����
		 * �����, ���������� � �.3, �� ������ ������ ����, �.�. sizeof(tree_node)
		 *
		 */
		size_type MaxSize() const noexcept {
			return (std::numeric_limits<size_type>::max() / 2) - sizeof(tree_type) - sizeof(tree_node)) / sizeof(tree_node);
		}

		/* ���������� �������� �� ������ ����������.
		 * ��� ������� ���������� ������������ �������� ����� ����� end().*/
		iterator Begin() noexcept {
			return iterator(MostLeft());
		}

		/* ������ begin() ��� const */
		const_iterator Begin() const noexcept {
			return const_iterator(MostLeft());
		}

		/* ���������� �������� �� ����� ���������� (������� ����� ����������
		 * �������� ����������).
		 * ������� �������� ��������� "���������" � ��� ������������� - UB */
		iterator End() noexcept {
			return iterator(head_);
		}

		/* ������ end() ��� const */
		const_iterator End() const noexcept {
			return const_iterator(head_);
		}

		/* ��������� ��� �������� �� other � ��������� �� � this.
		 * ������� �������� �� ���������� � �� ������������,
		 * ��������������� ������ ���������� ��������� ����� ����������. ���
		 * ��������� � ������ �� ���������� �������� �������� ���������������, ��
		 * ������ ��������� �� *this, � �� �� other.
		 *
		 * � ������ ������� �� ��������� ��� ��������, ���� ���� ����� ��� ���� �
		 * ������� ������. ������� �� �� ���������� ������� ExtractNode(), �.�. ���
		 * �������� ���������������� ������ other ����� ������� ����, ��� ������
		 * �������� ��������, �.�. ��� ����� ����� ���������� ��� ��� ����������
		 * �������� other. ������� �� ���������� �� ������� �� ���� ��������� other,
		 * ������� �� � ��������� � ������� ������.
		 *
		 * �.�. �� �� ������� ����� �����, �� ��� �������� ����� �������. */
		void Merge(tree_type& other) {
			if (this != &other) {
				iterator other_begin = other.Begin();

				while (other.size_ > 0) {
					tree_node* moving_node = other.begin.node_;
					++other_begin;

					if (moving_node->left_ != nullptr) {
						moving_node->left_->parent_ = moving_node->parent_;
					}

					if (moving_node->right_ != nullptr) {
						moving_node->right_->parent_ = moving_node->parent_;
					}

					if (moving_node->parent_->left_ == moving_node) {
						moving_node->parent_->left_ = nullptr;
					}

					if (moving_node->parent_->right_ == moving_node) {
						moving_node->parent_->right_ = nullptr;
					}

					// �������� ���� � ���� �� ���������
					moving_node->ToDefault();

					--other.size_;

					Insert(Root(), moving_node, false);
				}

				other.InitializeHead(); // ��� �����������������
			}
		}

		/* �������� ������� �������� �� other � �������� �� � this. ���� �
		 * *this ���� ������� � ������, ������������� ����� �������� �� other,
		 * �� ���� ������� �� ����������� �� other.
		 *
		 * @details ������� �������� �� ���������� � �� ������������,
		 * ��������������� ������ ���������� ��������� ����� ����������. ���
		 * ��������� � ������ �� ���������� �������� �������� ���������������, ��
		 * ������ ��������� �� *this, � �� �� other.
		 *
		 * �.�. other ����� �������� �������� ����� ��������, �� ��� ����������
		 * ����� �� other �� ���������� ExtractNode(), �.�. ��� ��������
		 * ����������������, � �� ������ ��������� other ���������� ������-������
		 * �������. */
		void MergeUnique(tree_type& other) {
			if (this != other) {
				iterator other_begin = other.Begin();
				iterator other_end = other.End();

				while (other_begin != other_end) {
					iterator result_it = Find(other_begin.node_->key_);
					if (result_it == End()) {
						iterator temp = other_begin;
						++other_begin;
						tree_node* moving_node = other.ExtractNode(temp);
						Insert(Root(), moving_node, false);
					}
					else {
						++other_begin;
					}
				}
			}
		}

		/* ��������� ������� �� ��������� key � ���������. ���� � ����������
		 * ���� �������� � ������������� ������, ������� ����������� �� �������
		 * ������� ����� ���������. */
		iterator Insert(const key_type& key) {
			tree_node* new_node = new tree_node{ key };
			return Insert(Root(), new_node, false).first;
		}

		/*  ��������� ������� �� ��������� key � ���������, ���� ��������� ���
		 * �� �������� ������� � ������������� ������. */
		std::pair<iterator, bool> InsertUnique(const key_type& key) {
			tree_node* new_node = new tree_node{ key };
			std::pair<iterator, bool> result = Insert(Root(), new_node, true);
			if (result.second == false) {
				delete new_node; // ���� ������� �� ���������, �� ������� ��������� ����
			}
			return result;
		}

		template <typename... Args>
		std::pair<iterator, bool>* Emplace(Args &&...args) {
			// ��������� ���������� ��������� ��� ��������� ������
			size_t num_elements = sizeof...(args);
			// �������� ������ ��� ������ ��� iterator � bool
			std::pair<iterator, bool>* result = new std::pair<iterator, bool>[num_elements];

			// ���������� std::forward ��� ��������� ������ �����������
			size_t index = 0;
			auto insert_args = { std::forward<Args>(args)... };
			for (auto& item : insert_args) {
				// ������� ����� ���� ������
				tree_node* new_node = new tree_node(std::move(item));
				// ��������� ���� � ������ � �������� ��������� �������
				std::pair<iterator, bool> result_insert = Insert(Root(), new_node, false);
				// ��������� ��������� � ������
				result[index++] = result_insert;
			}
			return result;
		}

		template<typename...Args>
		std::pair<iterator, bool>* EmplaceUnique(Args &&...args) {
			size_t num_elements = sizeof...(args);
			// �������� ������ ��� ������ ��� iterator � bool
			std::pair<iterator, bool>* result = new std::pair<iterator, bool>[num_elements];
			size_t index = 0;
			auto insert_args = { std::forward<Args>(args)... };
			for (auto& item : insert_args) {
				tree_node* new_node = new tree_node(std::move(item));
				std::pair<iterator, bool> result_insert = Insert(Root(), new_node, true);
				if (result_insert.second == false) {
					delete new_node;
				}
				result[index++] = result_insert;
			}
			return result;
		}

		/* ������� ������� � ������, ������������� key. �������� ��
		 * ����������, ����� ������ ������� ����� ������, ���� �� ���������, �� �
		 * gcc ��������� ������� �� lower_bound(), ������� ������ ���������� */
		iterator Find(const_reference key) {
			iterator result = LowerBound(key);
			if (result == End() || cmp_(key, *result)) {
				return End();
			}
			return result;
		}

		/* ���������� ��������, ����������� �� ������ �������, ������� ��
		 * ������ (�.�. ������ ��� �����) key.*/
		iterator LowerBound(const_reference key) {
			tree_node* start = Root();
			tree_node* result = End().node_;
			while (start != nullptr) {
				if (!cmp_(start->key_, key)) {
					result = start;
					start = start->left_;
				}
				else {
					start = start->right_;
				}
			}
			return iterator(result);
		}

		/* ���������� ��������, ����������� �� ������ �������, ������� ������
		 * key. */
		iterator UpperBound(const_reference key) {
			tree_node* start = Root();
			tree_node* result = End().node_;

			while (start != nullptr) {
				if (cmp_(key, start->key_)) { // temp_value > key, ����������
					result = start;
					start = start->left_; // ���� � ����� ��������� ����� ���������� ��������� ������ ������ temp_value � key
				}
				else {
					start = start->right_; // ���� ���� ��� �������� ������
				}
				return iterator(result);
			}
		}
		/* �������� ��������� �������� � ������, ��������, ����������� �� ���������, ���������� ���������, ��������� ��������� � ��������� ������� */
		void Erase(iterator pos) noexcept {
			tree_node* result = ExtractNode(pos);
			delete result;
		}

		/* ������ ���������� ���� ����� */
		void Swap(tree_type& other) noexcept {
			std::swap(head_, other.head_);
			std::swap(size_, other.size_);
			std::swap(cmp_, other.cmp_);
		}

	private:
		/* ����������� ������ �� other � this */
		void CopyTreeFromOther(const tree_type& other) {
			tree_node* other_copy_root = CopyTree(other.Root(), nullptr);
			/* ������� ������� ������ �����, � ����� ������� ������� ������,
			����� �� ���������� ������ ���� ������� ���������� �� ����� �����������*/
			Clear();
			Root() = other_copy_root;
			Root()->parent_ = head_;
			MostLeft() = SearchMinimum(Root());
			MostRight() = SearchMaximum(Root());
			size_ = other.size_;
			cmp_ = other.cmp_;
		}

		/* ����������� �������� ����� ����� � ������ ����� ���� */
		[[nodiscard]] tree_node* CopyTree(const tree_node* node, tree_node* parent) {
			// ���� ������� ���������� ��� �������� ������ ������� ����, ������ ������� �� ����� � ������ ������ �� �����
			tree_node* copy = new tree_node{ node->key };
			// ����������� � try/catch ��� ����������� ������, ����� � ������ ���������� 
			// ������������� ������ ������ � ������� ��� ������������� ����
			try {
				if (node->left_) {
					copy->left_ = CopyTree(node->left_, copy);
				}
				if (node->right_) {
					copy->right_ = CopyTree(node->right_, copy);
				}
			}
			catch (...) {
				Destroy(copy);
				throw std::runtime_error;
			}
			copy->parent_ = parent;
			return copy;
		}

		/* ���������� ������� ��� ���� ������ � ����������� ������, �����
         * ���������� ���� head. */
		void Destroy(tree_node* node) noexcept {
			if (node == nullptr) {
				return;
			}
			Destroy(node->left_);
			Destroy(node->right_);
			delete node;
		}

		/* ������������� ������ (��������� �������� ���������� ����) */
		void InitializeHead() noexcept {
			Root() = nullptr;
			MostLeft() = head_;
			MostRight() = head_;
		}

		/* ���������� ������ �� ������ ������ */
		tree_node*& Root() {
			return head_->parent_;
		}

		/* ���������� ����������� ��������� �� ������ ������ ��� const-�������� */
		const tree_node* Root() const {
			return head_->parent_;
		}

		/* ����� ��������� (����� �����) ������� ������ */
		tree_node*& MostLeft() {
			return head_->left_;
		}

		/* ����� ��������� (����� �����) ������� ������ ��� const-�������� */
		const tree_node* MostLeft() const {
			return head_->left_;
		}
		
		/* ����� ������� (����� ������) ������� ������ */
		tree_node*& MostRight() {
			return head_->right_;
		}

		/* ����� ������� (����� ������) ������� ������ ��� const-�������� */
		const tree_node* MostRight() const {
			return head_->right_;
		}

		std::pair<iterator, bool> Insert(tree_node* root, tree_node* new_node, bool only_unique) {
			tree_node* node = root;
			tree_node* parent = nullptr;

			while (node != nullptr) {
				parent = node;
				if (cmp_(new_node->key_, node->key_)) { // node->key > new_node->key
					node = node->left_;
				}
				else { // node->key <= new_node->key
					if (only_unique == false) {
						node = node->right_;
					}
					else {
						if (cmp_(node->key_, new_node->key_)) {
							node = node->right_;
						}
						else {
							return { iterator(node), false };
						}
					}
				}
			}
			// ����� ����� parent ��������� �� ����, � �������� ���������� new_node (����� ��������� ������)
			if (parent != nullptr) {
				new_node->parent_ = parent;
				if (cmp_(new_node->key_, parent->key_)) {
					parent->left_ = new_node;
				}
				else {
					parent->right_ = new_node;
				}
			} else { // ������������� ������
				new_node->parent_ = head_;
				Root() = new_node;
			}

			++size_;

			// ��������� ��������� �� ����� ��������� � ����� ������� ��������
			if (MostLeft() == head_ || MostLeft()->left_ != nullptr) { // ������ �� ������, �� left_ ��������� �� head_
				MostLeft() = new_node;
			}

			if (MostRight() == head_ || MostRight()->right_ != nullptr) {
				MostRight() = new_node;
			}

			return { iterator(new_node), true };
		}

		tree_node* ExtractNode(iterator pos) noexcept {
			if (pos == End()) {
				// ��������� ������� ��������� ���� (head_), ����� ���������
				// ��������������� ������ ������.
				return nullptr;
			}
			
			tree_node* deleted_node = pos.node_;
			if ((deleted_node->left != nullptr) && (deleted_node->right_ != nullptr)) {
				tree_node* replace = SearchMinimum(deleted_node->right_);
				SwapNodesForErase(deleted_node, replace);
			}

			if ((deleted_node->left_ == nullptr && deleted_node->right_ != nullptr) || (deleted_node->left_ != nullptr && deleted_node->right == nullptr)) {
				tree_node* replace;
				if (deleted_node->left_ != nullptr) {
					replace = deleted_node->left_;
				}
				else {
					replace = deleted_node->right_;
				}
				SwapNodesForErase(deleted_node, replace);
			}
			

			// if ((deleted_node->left_ == nullptr) && (deleted_node->right_ == nullptr)) ������ �� ������ ��� ��������

			// ������� �� ������ ���� ������� �� �������
			if (deleted_node == Root()) {
				InitializeHead();
			}
			else {
				if (deleted_node == deleted_node->parent_->left_) {
					deleted_node->parent_->left_ = nullptr;
				}
				else {
					deleted_node->parent_->left_ = nullptr;
				}

				if (MostLeft() == deleted_node) {
					MostLeft() = SearchMinimum(Root());
				}

				if (MostRight() == deleted_node) {
					MostRight() = SearchMaximum(Root());
				}
			}
			--size_;
			deleted_node->ToDefault(); // ���������� ��� ���������
			return deleted_node;
		}

		void SwapNodesForErase(tree_node* node, tree_node* other) noexcept {
			if (other->parent_->left_ == other) {
				other->parent_->left_ = node;
			}
			else {
				other->parent_->right_ = node;
			}

			if (node == Root()) {
				Root() = other;
			}
			else {
				if (node->parent_->left_ == node) {
					node->parent_->left_ = other;
				}
				else {
					node->parent_->right_ = other;
				}
			}


			std::swap(node->parent_, other->parent_);
			std::swap(node->left_, other->left_);
			std::swap(node->right_, other->right_);
			
			if (node->left_) {
				node->left_->parent_ = node;
			}
			if (node->right_) {
				node->right_->parent_ = node;
			}
			if (other->left_) {
				other->left_->parent_ = other;
			}
			if (other->right_) {
				other->right_->parent_ = other;
			}
		}

		tree_node* SearchMinimum(tree_node* node) const noexcept {
			while (node->left_ != nullptr) {
				node = node->left_;
			}
			return node;
		}

		tree_node* SearchMaximum(tree_node* node) const noexcept {
			while (node->right_ != nullptr) {
				node = node->right_;
			}
			return node;
		}

		struct BinarySearchTreeNode {
			tree_node* parent_;
			tree_node* left_;
			tree_node* right_;
			key_type key_;

			/* ����������� �� ��������� ��� �������� ������� ���� ������, �
			* ��������� �������������� �������� ����
			* @details left_ � right_ ��������� �� this, �.�. ���� �����������
			* ������������ ��� �������� ������ ������ */
			BinarySearchTreeNode() : parent_(nullptr), left_(this), right_(this), key_(key_type{}) {}

			/* �����������, ��������� ���� ������, ������������������
			* ��������� key */
			BinarySearchTreeNode(const key_type& key) : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key) {}

			BinarySearchTreeNode(key_type key) : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key) {}

			void ToDefault() noexcept {
				left_ = nullptr;
				right_ = nullptr;
				parent_ = nullptr;
			}

			/* ���������� ��������� ������������ this ���� ������.
			 * ���������� ������� �� ���� �������. � ���������� �����������
			 * ������� ���������� ���� (������), ����� ������� �����������
			 * ������������ ��� ������ ���������. */

			tree_node* NextNode() const noexcept {
				tree_node* node = const_cast<tree_node*>(this);
				if ((node->parent_ == nullptr) || (node->parent_->parent_ == node)) {
					node = node->left_;
				}
				else if (node->right_ != nullptr) {
					node = node->right_;

					while (node->left_ != nullptr) {
						node = node->left_;
					}
				}
				else {
					tree_node* parent = node->parent_;
					while (node == parent->right_) {
						node = parent;
						parent = parent->parent_;
					}
					if (node->right_ != parent) { // ������������� ������������ �� ��������� ����
						node = parent;
					}
				}
				return node;
			}

			tree_node* PrevNode() const noexcept {
				tree_node* node = const_cast<tree_node*>(this);

				if ((node->parent_ == nullptr) || (node->parent_->parent_ == node)) {
					node = node->right_;
				}
				else if (node->left_ != nullptr) {
					node = node->left_;
					while (node->right_ != nullptr) {
						node = node->right_;
					}
				}
				else {
					tree_node* parent = node->parent_;
					while (node == parent->left_) {
						node = parent;
						parent = parent->parent_;
					}
					if (node->left_ != parent) {
						node = parent;
					}
				}

			}
		}; // struct

		struct BinarySearchTreeIterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = tree_type::key_type;
			using pointer = value_type*;
			using reference = value_type&;

			tree_node* node;

			BinarySearchTreeIterator() = delete; // ������ �������� ��� �� �����

			explicit BinarySearchTreeIterator(tree_node* node) : node_(node) {}

			reference operator*() const noexcept {
				return node_->key_;
			}

			pointer operator->() const noexcept {
				return node;
			}

			iterator& operator++() noexcept {
				node_ = node_->NextNode();
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator tmp{ node_ };
				++(*this);
				return tmp;
			}

			iterator& operator--() noexcept {
				node_ = node_->PrevNode();
				return *this;
			}

			iterator operator--(int) noexcept {
				iterator tmp{ node_ };
				--(*this);
				return tmp;
			}
			// ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� �������.
			bool operator==(const iterator& other) const noexcept {
				return node_ == other.node_;
			}

			bool operator!=(const iterator& other) const noexcept {
				return node_ != other.node_;
			}

		}; // struct

		struct BinarySearchTreeIteratorConst {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = tree_type::key_type;
			using pointer = const value_type*;
			using reference = const value_type&;

			const tree_node* node_;

			BinarySearchTreeIteratorConst() = delete;

			explicit BinarySearchTreeIteratorConst(const tree_node* node) : node_(node) {}

			BinarySearchTreeIteratorConst(const iterator& it) : node_(it.node_) {}

			reference operator*() const noexcept {
				return node_->key_;
			}

			pointer operator->() const noexcept {
				return node;
			}

			const_iterator& operator++() noexcept {
				node_ = node_->NextNode();
				return *this;
			}

			const_iterator operator++(int) noexcept {
				const_iterator tmp{ node_ };
				++(*this);
				return tmp;
			}

			const_iterator& operator--() noexcept {
				node_ = node_->PrevNode();
				return *this;
			}

			const_iterator operator--(int) noexcept {
				const_iterator tmp{ node_ };
				--(*this);
				return tmp;
			}

			friend bool operator==(const const_iterator& it1,
				const const_iterator& it2) noexcept {
				return it1.node_ == it2.node_;
			}

			friend bool operator!=(const const_iterator& it1,
				const const_iterator& it2) noexcept {
				return it1.node_ != it2.node_;
			}

		}; // struct

		struct BinarySearchTreeReverseIterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = tree_type::key_type;
			using pointer = value_type*;
			using reference = value_type&;

			tree_node* node;

			BinarySearchTreeIterator() = delete; // ������ �������� ��� �� �����

			explicit BinarySearchTreeIterator(tree_node* node) : node_(node) {}

			reference operator*() const noexcept {
				return node_->key_;
			}

			pointer operator->() const noexcept {
				return node;
			}

			iterator& operator++() noexcept {
				node_ = node_->PrevNode();
				return *this;
			}

			iterator operator++(int) noexcept {
				iterator tmp{ node_ };
				--(*this);
				return tmp;
			}

			iterator& operator--() noexcept {
				node_ = node_->NextNode();
				return *this;
			}

			iterator operator--(int) noexcept {
				iterator tmp{ node_ };
				++(*this);
				return tmp;
			}
			// ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� �������.
			bool operator==(const iterator& other) const noexcept {
				return node_ == other.node_;
			}

			bool operator!=(const iterator& other) const noexcept {
				return node_ != other.node_;
			}


		}; // struct


		struct BinarySearchTreeReverseIteratorConst {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = tree_type::key_type;
			using pointer = const value_type*;
			using reference = const value_type&;

			const tree_node* node_;

			BinarySearchTreeIteratorConst() = delete;

			explicit BinarySearchTreeIteratorConst(const tree_node* node) : node_(node) {}

			BinarySearchTreeIteratorConst(const iterator& it) : node_(it.node_) {}

			reference operator*() const noexcept {
				return node_->key_;
			}

			pointer operator->() const noexcept {
				return node;
			}

			const_iterator& operator++() noexcept {
				node_ = node_->PrevNode();
				return *this;
			}

			const_iterator operator++(int) noexcept {
				const_iterator tmp{ node_ };
				--(*this);
				return tmp;
			}

			const_iterator& operator--() noexcept {
				node_ = node_->NextNode();
				return *this;
			}

			const_iterator operator--(int) noexcept {
				const_iterator tmp{ node_ };
				++(*this);
				return tmp;
			}

			friend bool operator==(const const_iterator& it1,
				const const_iterator& it2) noexcept {
				return it1.node_ == it2.node_;
			}

			friend bool operator!=(const const_iterator& it1,
				const const_iterator& it2) noexcept {
				return it1.node_ != it2.node_;
			}
		};

	}; // class

} // namespace inorder