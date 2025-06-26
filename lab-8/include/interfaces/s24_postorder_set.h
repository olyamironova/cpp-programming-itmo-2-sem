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

		/* Конструктор по умолчанию, создает пустое множество */
		set() : tree_(new tree_type{}) {}

		/* Конструктор списка инициализаторов, создает множество,
		 * инициализированное с помощью std::initializer_list.*/

		set(std::initializer_list<value_type> const& items) : set() {
			for (auto item : items) {
				insert(item);
			}
		}

		/* Конструктор копирования (Copy Constructor). Создает множество
		 * путем копирования данных из объекта other.*/
		set(const set& other) : tree_(new tree_type(*other.tree_)) {}

		/* Оператор присваивания копированием.*/
		set& operator=(const set& other) {
			*tree_ = *other.tree_;
			return *this;
		}

		/* Деструктор объекта (Destructor) */
		~set() {
			delete tree_;
			// для запрета переиспользования и повторного освобождения указателей
			tree_ = nullptr;
		}

		/* Возвращает итератор на начало контейнера.
		 * Для пустого контейнера возвращаемый итератор будет равен end(). */
		iterator begin() {
			return tree_->Begin();
		}

		/* Версия begin() для const */
		const_iterator begin() const {
			return tree_->Begin();
		}

		/* Возвращает итератор на конец контейнера (элемент после последнего
		 * элемента контейнера).
		 * Элемент является служебной "заглушкой" и его разыменование - UB */
		iterator end() {
			return tree_->End();
		}

		/* Версия end() для const */
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


		/* Проверяет, является ли контейнер пустым */
		bool empty() const noexcept {
			return tree_->Empty();
		}

		/* Возвращает количество элементов в контейнере */
		size_type size() const noexcept {
			return tree_->Size();
		}

		/*  Возвращает максимальное количество элементов, которое может
		  * содержать контейнер из-за ограничений реализации системы или библиотеки.*/
		size_type max_size() const noexcept {
			return tree_->MaxSize();
		}

		/* Удаляет содержимое контейнера (все элементы), при этом узлы остаются связаны друг с другом */
		void clear() noexcept {
			tree_->Clear;
		}

		/* Вставляет элемент со значением value в контейнер, если контейнер
		   еще не содержит элемент с эквивалентным ключом.*/
		std::pair<iterator, bool> insert(const value_type& value) {
			return tree_->InsertUnique(value);
		}

		/* Удаляет элемент на позиции pos. Ссылки и итераторы на стертые
		 * элементы становятся недействительными. Другие ссылки и итераторы не
		 * затрагиваются.*/

		void erase(iterator pos) noexcept {
			tree_->Erase(pos);
		}

		/* Обменяет содержимое контейнера на содержимое other */
		void swap(inorder_set& other) noexcept {
			tree_->Swap(*other.tree_);
		}

		/* Пытается извлечь элементы из other и вставить их в this. Если в
		* *this есть элемент с ключом, эквивалентным ключу элемента из other,
		* то этот элемент не извлекается из other.*/
		void merge(set& other) noexcept {
			tree_->MergeUnique(*other.tree_);
		}

		/* Находит элемент с ключом, эквивалентным key.
		Если такой элемент не найден, возвращается end().*/
		iterator find(const key_type& key) noexcept {
			return tree_->Find(key);
		}

		/* Версия find() для константного объекта
		Если такой элемент не найден, возвращается end().*/
		const_iterator find(const key_type& key) const noexcept {
			return tree_->Find(key);
		}

		/* Проверяет, есть ли в контейнере элемент с ключом, эквивалентным key.*/
		bool contains(const key_type& key) const noexcept {
			return tree_->Find(key) != tree_->End();
		}

		/* Размещает новые элементы args в контейнер, если контейнер ещё не
		 * содержит элемент с эквивалентным ключом.*/
		template <typename... Args>
		<std::pair<iterator, bool>>* emplace(Args &&...args) {
			return tree_->EmplaceUnique(std::forward<Args>(args)...);
		}

	private:
		// Указатель на объект созданного дерева, используемого в контейнере
		tree_type* tree_;
	};
} // namespace postorder