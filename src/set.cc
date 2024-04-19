#include <set.h>
#include <iostream>
#include <queue>

node::node(int val, node* left, node* right) : data(val), left(left), right(right) {};

my_set::my_set() : _root(nullptr) {};

my_set::my_set(const int val) : _root(new node(val, nullptr, nullptr)) {};

my_set::my_set(std::initializer_list<int> values) {
	_root = nullptr;
	for (auto val : values) insert(val);
}

node* my_set::_copy(node* root) {
	if (!root) return nullptr;
	node* new_node = new node(root->data, root->left, root->right);
	new_node->left = _copy(root->left);
	new_node->right = _copy(root->right);
	return new_node;
}

my_set& my_set::operator=(my_set other) {
	swap(other);
	return *this;
}

void my_set::swap(my_set& other) noexcept {
	std::swap(_root, other._root);
}

my_set::my_set(const my_set& other) {
	_root = _copy(other._root);
}

void my_set::_clear(node* root) {
	if (!root) return;
	_clear(root->left);
	_clear(root->right);
	delete root;
}

bool my_set::_equal(const node* first, const node* second) const {
	if (!first && !second) return true;
	if (first && second) {
		return first->data == second->data && _equal(first->left, second->left) && _equal(first->right, second->right);
	}
	return false;
}

bool my_set::equal(const my_set& second) const {
	return _equal(_root, second._root);
}

bool my_set::operator==(const my_set& second) {
	return equal(second);
}

bool my_set::operator!=(const my_set& second) {
	return !(*this == second);
}

int my_set::get_root_value() const {
    //if root is null throw exception
	return _root ? _root->data : throw std::logic_error("root is nullptr");
}

bool my_set::contains(const int key) const {
	node* tmp = _root;
	while (tmp) {
		if (tmp->data == key) return true;
		if (tmp->data > key) {
			tmp = tmp->left;
		}
		else tmp = tmp->right;
	}
	return false;
}

bool my_set::insert(const int key) {
	auto new_node = new node(key, nullptr, nullptr);
	//replace head if head is null
	if (!_root) {
		_root = new_node;
		return true;
	}
	node* tmp = _root;
	 
	while (tmp) {
		//if there is such a node, then we dont insert
		if (tmp->data == key) return false;
		//else find position for insert
		if (tmp->data > key) {
			if (!tmp->left) {
				tmp->left = new_node;
				return true;
			}
			tmp = tmp->left;
		}
		else {
			if (!tmp->right) {
				tmp->right = new_node;
				return true;
			}
			tmp = tmp->right;
		}
	}
	return false;
}

bool my_set::erase(const int key) {
	return _erase(_root, key);
}

bool my_set::_erase(node*& node_, const int key) {
	if (!node_) {
		return false;
	}
	// If the key is less than current node's data, 
	//recursively call _erase on the left subtree.
	if (key < node_->data) {
		return _erase(node_->left, key);
	}
	// If the key is greater than current node's data,
	//recursively call _erase on the right subtree.
	else if (key > node_->data) {
		return _erase(node_->right, key);
	}
	else {
		if (!node_->left) {
			auto tmp = node_->right;
			delete node_;
			node_ = tmp;
			return true;
		}
		else if (!node_->right) {
			auto tmp = node_->left;
			delete node_;
			node_ = tmp;
			return true;
		}
		// If the current node has both left and right children.
		auto min_right = node_->right;
		while (min_right->left) {
			// Find the minimum node in the right subtree.
			min_right = min_right->left;
		}
		// Replace the current node's data with the data of the minimum node in the right subtree.
		node_->data = min_right->data;
		// Recursively call _erase to remove the minimum node from the right subtree.
		return _erase(node_->right, min_right->data);
	}
}

void my_set::walk(const node* other, std::vector<int>& vec) {
	if (!other) {
		return;
	}
	walk(other->left, vec);
	vec.push_back(other->data);
	walk(other->right, vec);
}

std::vector<int> my_set::to_vector() const {
	std::vector <int> convert;
	walk(_root, convert);
	convert.push_back(_root->data);
	return convert;
}

void my_set::_print(const node* root) const {
	//root -> left -> right
	if (!root) return;
	std::cout << root->data << " ";
	_print(root->left);
	_print(root->right);
}

void my_set::print() const {
	_print(_root);
}

//node* my_set::get_root() const {
//	return _root;
//}

my_set::~my_set() {
	_clear(_root);
	_root = nullptr;
}

my_set union_of_sets(const my_set& first, const my_set& second) {
	my_set result = first;  
	for (auto el : second.to_vector()) {
		result.insert(el);
	}
	return result;
}
my_set symmetrical_difference(const my_set& first, const my_set& second) {
	my_set result;
	auto vfirst = first.to_vector();
	auto vsecond = second.to_vector();

	for (auto el : vfirst) {
		if (!second.contains(el)) {
			result.insert(el);
		}
	}

	for (auto el : vsecond) {
		if (!first.contains(el)) {
			result.insert(el);
		}
	}
	return result;
}
//
//my_set symmetrical_difference(const my_set& first, const my_set& second) {
//	my_set result;
//	if (!first.get_root()) {
//		result = second;
//		return result;
//	}
//	if (!second.get_root()) {
//		result = first;
//		return result;
//	}
//	std::queue<node*> queue;;
//	queue.push(first.get_root());
//	queue.push(second.get_root());
//	//if the node is in both then go to the next one, else add it to the result
//	while (!queue.empty()) {
//		auto current = queue.front();
//		if (second.contains(current->data) && first.contains(current->data)) {
//			if (current->left) queue.push(current->left);
//			if (current->right)queue.push(current->right);
//			queue.pop();
//		}
//		else result.insert(current->data);
//		if (current->left) queue.push(current->left);
//		if (current->right)queue.push(current->right);
//		queue.pop();
//	}
//	return result;
//}