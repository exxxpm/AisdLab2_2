#include <initializer_list>
#include <vector>
struct node {
	int data;
	node* left;
	node* right;
	node(int val, node* left, node* right);
};

class my_set {
	node* _root;

	void _clear(node* root);
	node* _copy(node* root);
	bool _equal(const node* first, const node* second) const;
	bool _erase(node*& node_, const int key);
	void _print(const node* root) const;
public:
	my_set();
	my_set(const int val);
	my_set(const my_set& other);
	my_set(std::initializer_list<int> values);

	void swap(my_set& other) noexcept;
	my_set& operator=(my_set other);

	//node* get_root() const;
	std::vector<int> to_vector() const;
	static void walk(const node* other, std::vector<int>& vec);
	int get_root_value() const;
	bool equal(const my_set& second) const;
	bool operator==(const my_set& second);
	bool operator!=(const my_set& second);
	bool contains(const int key) const;
	bool insert(const int key);
	bool erase(const int key);
	void print() const;
	
	~my_set();
};

my_set union_of_sets(const my_set& first, const my_set& second);
my_set symmetrical_difference(const my_set& first, const my_set& second);