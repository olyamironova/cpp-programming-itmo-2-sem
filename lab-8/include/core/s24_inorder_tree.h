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
		// Указатель на голову дерева
		tree_node* head_;
		// Количество элементов в дереве
		size_type size_;
		// Компаратор дерева (класс для сравнения значений узлов)
		Comparator cmp_;

	public:
		/* Конструктор по умолчанию, создает пустое дерево */
		BinarySearchTree() : head_(new tree_node), size(0U) {}

		/* Конструктор копирования (Copy Constructor). Создает дерево путем
		 * копирования данных из объекта other. */
		BinarySearchTree(const tree_type& other) : BinarySearchTree() {
			if (other.Size() > 0) {
				CopyTreeFromOther(other);
			}
		}

		/* Оператор присваивания копированием. */
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

		/* Деструктор объекта (Destructor) */
		~BinarySearchTree() {
			Clear();
			delete head_;
			head_ = nullptr;
		}

		/* Удаляет содержимое контейнера (все элементы). Контейнер при этом
		 * остается консистентным. */
		void Clear() noexcept {
			Destroy(Root());
			InitializeHead();
			size_ = 0;
		}

		/* Возвращает количество элементов в контейнере */
		size_type Size() const noexcept {
			return size_;
		}

		/* Проверяет, является ли контейнер пустым */
		bool Empty() const noexcept {
			return size_ == 0;
		}

		/* Возвращает максимальное количество элементов, которое может
		 * содержать дерево из-за ограничений реализации системы или библиотеки.
		 *
		 * @warning В реальности значение max_size() никогда не будет достижимо.
		 *
		 * @details Это число не зависит от состояния ОС или доступной памяти, а
		 * определяется только реализацией контейнера. Во время выполнения размер
		 * контейнера может быть ограничен значением, меньшим max_size(), чем объем
		 * доступной оперативной памяти.
		 *
		 * Для расчета используются следующие вводные:
		 * 1) Размер size_t должен совпадать с размером указателя для любой
		 * платформы. Указатель должен быть в состоянии адресовать любой байт в
		 * памяти, а size_t хранить размер любого (в т.ч. занимающего всю память)
		 * объекта. Таким образом максимальное значение size_t - это максимальное
		 * количество байт памяти. Это значение мы получаем при помощи
		 * std::numeric_limits<size_t>::max();
		 *
		 * 2) GCC ограничивает объекты размером в половину адресного пространства.
		 * Поэтому полученное в п.1 число делим на 2.
		 *
		 * 3) В дереве хранится указатель на голову (служебный узел) -
		 * sizeof(head_), компаратор - sizeof(cmp_), количество созданных элементов
		 * - sizeof(size_), всё вместе - sizeof(tree_type). Кроме этого выделяется
		 * память на один служебный узел, который есть даже у пустого дерева -
		 * sizeof(tree_node). Всё это вычитаем из памяти, полученной в п.2. Таким
		 * образом получаем максимальное количество памяти, доступной для создания
		 * узлов дерева.
		 *
		 * 4) Чтобы определить максимальное количество элементов в контейнере, делим
		 * число, полученное в п.3, на размер одного узла, т.е. sizeof(tree_node)
		 *
		 */
		size_type MaxSize() const noexcept {
			return (std::numeric_limits<size_type>::max() / 2) - sizeof(tree_type) - sizeof(tree_node)) / sizeof(tree_node);
		}

		/* Возвращает итератор на начало контейнера.
		 * Для пустого контейнера возвращаемый итератор будет равен end().*/
		iterator Begin() noexcept {
			return iterator(MostLeft());
		}

		/* Версия begin() для const */
		const_iterator Begin() const noexcept {
			return const_iterator(MostLeft());
		}

		/* Возвращает итератор на конец контейнера (элемент после последнего
		 * элемента контейнера).
		 * Элемент является служебной "заглушкой" и его разыменование - UB */
		iterator End() noexcept {
			return iterator(head_);
		}

		/* Версия end() для const */
		const_iterator End() const noexcept {
			return const_iterator(head_);
		}

		/* Извлекает все элементы из other и вставляет их в this.
		 * Никакие элементы не копируются и не перемещаются,
		 * переназначаются только внутренние указатели узлов контейнера. Все
		 * указатели и ссылки на переданные элементы остаются действительными, но
		 * теперь ссылаются на *this, а не на other.
		 *
		 * В данной функции мы переносим все элементы, даже если такие уже есть в
		 * текущем дереве. Поэтому мы не используем функцию ExtractNode(), т.к. она
		 * вызывает перебалансировку дерева other после изъятия узла, что лишняя
		 * ненужная операция, т.к. все равно будут перенесены все без исключения
		 * элементы other. Поэтому мы проходимся по порядку по всем элементам other,
		 * изымаем их и вставляем в текущее дерево.
		 *
		 * Т.к. мы не создаем копии узлов, то это экономит много времени. */
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

					// Приводим узел к виду по умолчанию
					moving_node->ToDefault();

					--other.size_;

					Insert(Root(), moving_node, false);
				}

				other.InitializeHead(); // для переиспользования
			}
		}

		/* Пытается извлечь элементы из other и вставить их в this. Если в
		 * *this есть элемент с ключом, эквивалентным ключу элемента из other,
		 * то этот элемент не извлекается из other.
		 *
		 * @details Никакие элементы не копируются и не перемещаются,
		 * переназначаются только внутренние указатели узлов контейнера. Все
		 * указатели и ссылки на переданные элементы остаются действительными, но
		 * теперь ссылаются на *this, а не на other.
		 *
		 * Т.к. other может остаться непустым после операции, то для извлечения
		 * узлов из other мы используем ExtractNode(), т.к. она вызывает
		 * перебалансировку, а мы должны сохранить other корректным красно-чёрным
		 * деревом. */
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

		/* Вставляет элемент со значением key в контейнер. Если в контейнере
		 * есть элементы с эквивалентным ключом, вставка выполняется по верхней
		 * границе этого диапазона. */
		iterator Insert(const key_type& key) {
			tree_node* new_node = new tree_node{ key };
			return Insert(Root(), new_node, false).first;
		}

		/*  Вставляет элемент со значением key в контейнер, если контейнер еще
		 * не содержит элемент с эквивалентным ключом. */
		std::pair<iterator, bool> InsertUnique(const key_type& key) {
			tree_node* new_node = new tree_node{ key };
			std::pair<iterator, bool> result = Insert(Root(), new_node, true);
			if (result.second == false) {
				delete new_node; // Если вставка не произошла, то удаляем созданный узел
			}
			return result;
		}

		template <typename... Args>
		std::pair<iterator, bool>* Emplace(Args &&...args) {
			// Вычисляем количество элементов для выделения памяти
			size_t num_elements = sizeof...(args);
			// Выделяем память под массив пар iterator и bool
			std::pair<iterator, bool>* result = new std::pair<iterator, bool>[num_elements];

			// Используем std::forward для избежания лишних копирований
			size_t index = 0;
			auto insert_args = { std::forward<Args>(args)... };
			for (auto& item : insert_args) {
				// Создаем новый узел дерева
				tree_node* new_node = new tree_node(std::move(item));
				// Вставляем узел в дерево и получаем результат вставки
				std::pair<iterator, bool> result_insert = Insert(Root(), new_node, false);
				// Добавляем результат в массив
				result[index++] = result_insert;
			}
			return result;
		}

		template<typename...Args>
		std::pair<iterator, bool>* EmplaceUnique(Args &&...args) {
			size_t num_elements = sizeof...(args);
			// Выделяем память под массив пар iterator и bool
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

		/* Находит элемент с ключом, эквивалентным key. Стандарт не
		 * регулирует, какой именно элемент будет найден, если их несколько, но в
		 * gcc находится элемент из lower_bound(), поэтому делаем аналогично */
		iterator Find(const_reference key) {
			iterator result = LowerBound(key);
			if (result == End() || cmp_(key, *result)) {
				return End();
			}
			return result;
		}

		/* Возвращает итератор, указывающий на первый элемент, который не
		 * меньше (т.е. больше или равен) key.*/
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

		/* Возвращает итератор, указывающий на первый элемент, который больше
		 * key. */
		iterator UpperBound(const_reference key) {
			tree_node* start = Root();
			tree_node* result = End().node_;

			while (start != nullptr) {
				if (cmp_(key, start->key_)) { // temp_value > key, запоминаем
					result = start;
					start = start->left_; // идем в левое поддерево чтобы попытаться уменьшить дельту меньшу temp_value и key
				}
				else {
					start = start->right_; // идем туда где значения больше
				}
				return iterator(result);
			}
		}
		/* Удаление заданного элемента в дереве, итератор, указывающий на удаленный, становится невалиден, остальные итераторы и указатели валидны */
		void Erase(iterator pos) noexcept {
			tree_node* result = ExtractNode(pos);
			delete result;
		}

		/* Меняет содержимое двух сетов */
		void Swap(tree_type& other) noexcept {
			std::swap(head_, other.head_);
			std::swap(size_, other.size_);
			std::swap(cmp_, other.cmp_);
		}

	private:
		/* Копирование дерева из other в this */
		void CopyTreeFromOther(const tree_type& other) {
			tree_node* other_copy_root = CopyTree(other.Root(), nullptr);
			/* Сначала создаем полную копию, а потом очищаем текущее дерево,
			чтобы не уничтожить дерево если вылетит исключение на этапе копирования*/
			Clear();
			Root() = other_copy_root;
			Root()->parent_ = head_;
			MostLeft() = SearchMinimum(Root());
			MostRight() = SearchMaximum(Root());
			size_ = other.size_;
			cmp_ = other.cmp_;
		}

		/* Рекурсивное создание копий узлов и связей между ними */
		[[nodiscard]] tree_node* CopyTree(const tree_node* node, tree_node* parent) {
			// если вылетит исключение при создании самого первого узла, ничего создано не будет и утечек памяти не будет
			tree_node* copy = new tree_node{ node->key };
			// оборачиваем в try/catch все рекурсивные вызовы, чтобы в случае исключения 
			// предотвратить утечку памяти и удалить уже скопированные узлы
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

		/* Рекурсивно удаляет все узлы дерева и освобождает память, кроме
         * служебного узла head. */
		void Destroy(tree_node* node) noexcept {
			if (node == nullptr) {
				return;
			}
			Destroy(node->left_);
			Destroy(node->right_);
			delete node;
		}

		/* Инициализация дерева (дефолтные значения фиктивного узла) */
		void InitializeHead() noexcept {
			Root() = nullptr;
			MostLeft() = head_;
			MostRight() = head_;
		}

		/* Возвращает ссылку на корень дерева */
		tree_node*& Root() {
			return head_->parent_;
		}

		/* Возвращает константный указатель на корень дерева для const-объектов */
		const tree_node* Root() const {
			return head_->parent_;
		}

		/* самый маленький (самый левый) элемент дерева */
		tree_node*& MostLeft() {
			return head_->left_;
		}

		/* самый маленький (самый левый) элемент дерева для const-объектов */
		const tree_node* MostLeft() const {
			return head_->left_;
		}
		
		/* самый большой (самый правый) элемент дерева */
		tree_node*& MostRight() {
			return head_->right_;
		}

		/* самый большой (самый правый) элемент дерева для const-объектов */
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
			// после цикла parent указывает на узел, к которому привяжется new_node (может оказаться пустым)
			if (parent != nullptr) {
				new_node->parent_ = parent;
				if (cmp_(new_node->key_, parent->key_)) {
					parent->left_ = new_node;
				}
				else {
					parent->right_ = new_node;
				}
			} else { // инициализация дерева
				new_node->parent_ = head_;
				Root() = new_node;
			}

			++size_;

			// обновляем указатели на самый маленький и самый большой элементы
			if (MostLeft() == head_ || MostLeft()->left_ != nullptr) { // дерево не пустое, но left_ указывает на head_
				MostLeft() = new_node;
			}

			if (MostRight() == head_ || MostRight()->right_ != nullptr) {
				MostRight() = new_node;
			}

			return { iterator(new_node), true };
		}

		tree_node* ExtractNode(iterator pos) noexcept {
			if (pos == End()) {
				// Запрещаем удалять служебный узел (head_), чтобы сохранить
				// консистентность нашего класса.
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
			

			// if ((deleted_node->left_ == nullptr) && (deleted_node->right_ == nullptr)) ничего не делаем сам удалится

			// изымаем из дерева узел который мы удаляем
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
			deleted_node->ToDefault(); // сбрасываем все указатели
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

			/* Конструктор по умолчанию для создания пустого узла дерева, с
			* дефолтной инициализацией значения узла
			* @details left_ и right_ указывают на this, т.к. этот конструктор
			* используется для создания головы дерева */
			BinarySearchTreeNode() : parent_(nullptr), left_(this), right_(this), key_(key_type{}) {}

			/* Конструктор, создающий узел дерева, инициализированный
			* значением key */
			BinarySearchTreeNode(const key_type& key) : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key) {}

			BinarySearchTreeNode(key_type key) : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key) {}

			void ToDefault() noexcept {
				left_ = nullptr;
				right_ = nullptr;
				parent_ = nullptr;
			}

			/* Возвращает следующий относительно this узел дерева.
			 * Реализация описана по ходу функции. В реализации учитывается
			 * наличие служебного узла (головы), через которую реализуется
			 * зацикливание при обходе элементов. */

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
					if (node->right_ != parent) { // предотвращает зацикливание на служебном узле
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

			BinarySearchTreeIterator() = delete; // пустой итератор нам не нужен

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
			// два итератора равны, если они указывают на один и тот же элемент.
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

			BinarySearchTreeIterator() = delete; // пустой итератор нам не нужен

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
			// два итератора равны, если они указывают на один и тот же элемент.
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