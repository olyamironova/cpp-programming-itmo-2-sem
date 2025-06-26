#pragma once
#pragma once

#include "s24_postorder_tree.h"

namespace postorder {
	template<class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
	class set {
	public:
		using key_type = key;
		using value_type = key_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using tree_type = BinarySearchTree<value_type>;
		using iterator = typename tree_type::iterator;
		using const_iterator = typename tree_type::const_iterator;
		using size_type = std::size_t;

		/* ����������� �� ���������, ������� ������ ��������� */
		set() : tree_(new tree_type{}) {}

		/* ����������� ������ ���������������, ������� ���������,
		 * ������������������ � ������� std::initializer_list.*/

		set(std::initializer_list<value_type> const& items) : set() {
			for (auto item : items) {
				insert(item);
			}
		}

		/* ����������� ����������� (Copy Constructor). ������� ���������
		 * ����� ����������� ������ �� ������� other.*/
		set(const set& other) : tree_(new tree_type(*other.tree_)) {}

		/* �������� ������������ ������������.*/
		set& operator=(const set& other) {
			*tree_ = *other.tree_;
			return *this;
		}

		/* ���������� ������� (Destructor) */
		~set() {
			delete tree_;
			// ��� ������� ����������������� � ���������� ������������ ����������
			tree_ = nullptr;
		}

		/* ���������� �������� �� ������ ����������.
		 * ��� ������� ���������� ������������ �������� ����� ����� end(). */
		iterator begin() {
			return tree_->Begin();
		}

		/* ������ begin() ��� const */
		const_iterator begin() const {
			return tree_->Begin();
		}

		/* ���������� �������� �� ����� ���������� (������� ����� ����������
		 * �������� ����������).
		 * ������� �������� ��������� "���������" � ��� ������������� - UB */
		iterator end() {
			return tree_->End();
		}

		/* ������ end() ��� const */
		const_iterator end() const {
			return tree_->End();
		}

		iterator rbegin() {
			return --(tree_->End());
		}

		const_iterator rbegin() const {
			return --(tree_->End());
		}

		iterator rend() {
			return ++(tree_->Begin());
		}

		const_iterator rend() const {
			return ++(tree_->Begin());
		}

		const_iterator cbegin() const {
			return const_cast<const_iterator>(tree_->Begin());
		}

		const_iterator cend() const {
			return const_cast<const_iterator>(tree_->End());;
		}

		const_iterator crbegin() const {
			return const_cast<const_iterator>(--(tree->End()));
		}

		const_iterator crend() const {
			return const_cast<const_iterator>(++(tree->Begin()));
		}


		/* ���������, �������� �� ��������� ������ */
		bool empty() const noexcept {
			return tree_->Empty();
		}

		/* ���������� ���������� ��������� � ���������� */
		size_type size() const noexcept {
			return tree_->Size();
		}

		/*  ���������� ������������ ���������� ���������, ������� �����
		  * ��������� ��������� ��-�� ����������� ���������� ������� ��� ����������.*/
		size_type max_size() const noexcept {
			return tree_->MaxSize();
		}

		/* ������� ���������� ���������� (��� ��������), ��� ���� ���� �������� ������� ���� � ������ */
		void clear() noexcept {
			tree_->Clear;
		}

		/* ��������� ������� �� ��������� value � ���������, ���� ���������
		   ��� �� �������� ������� � ������������� ������.*/
		std::pair<iterator, bool> insert(const value_type& value) {
			return tree_->InsertUnique(value);
		}

		/* ������� ������� �� ������� pos. ������ � ��������� �� �������
		 * �������� ���������� �����������������. ������ ������ � ��������� ��
		 * �������������.*/

		void erase(iterator pos) noexcept {
			tree_->Erase(pos);
		}

		/* �������� ���������� ���������� �� ���������� other */
		void swap(inorder_set& other) noexcept {
			tree_->Swap(*other.tree_);
		}

		/* �������� ������� �������� �� other � �������� �� � this. ���� �
		* *this ���� ������� � ������, ������������� ����� �������� �� other,
		* �� ���� ������� �� ����������� �� other.*/
		void merge(set& other) noexcept {
			tree_->MergeUnique(*other.tree_);
		}

		/* ������� ������� � ������, ������������� key.
		���� ����� ������� �� ������, ������������ end().*/
		iterator find(const key_type& key) noexcept {
			return tree_->Find(key);
		}

		/* ������ find() ��� ������������ �������
		���� ����� ������� �� ������, ������������ end().*/
		const_iterator find(const key_type& key) const noexcept {
			return tree_->Find(key);
		}

		/* ���������, ���� �� � ���������� ������� � ������, ������������� key.*/
		bool contains(const key_type& key) const noexcept {
			return tree_->Find(key) != tree_->End();
		}

		/* ��������� ����� �������� args � ���������, ���� ��������� ��� ��
		 * �������� ������� � ������������� ������.*/
		template <typename... Args>
		<std::pair<iterator, bool>>* emplace(Args &&...args) {
			return tree_->EmplaceUnique(std::forward<Args>(args)...);
		}

	private:
		// ��������� �� ������ ���������� ������, ������������� � ����������
		tree_type* tree_;
	};
} // namespace postorder