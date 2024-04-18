#include <iostream>
template<class Key,class Value>
struct pair {
	Key* key;
	Value* value;

public:
	pair() : key(nullptr), value(nullptr) {};
	pair(Key key, Value val) :key(new Key(key)), value(new Value(val)) {};
	pair(pair& other) {
		key = new Key(*(other.key));
		value = new Value(*(other.value));
	}

	pair& operator=(pair other) {
		swap(other);
		return *this;
	}

	void swap(pair& other) noexcept {
		std::swap(key, other.key);
		std::swap(value, other.value);
	}

	~pair() {
		delete key;
		delete value;
	};

	friend std::ostream& operator<<(std::ostream& stream, pair& pair) {
		if (!pair.key) {
			stream << "(" << pair.key << "," << pair.value << ")";
		}
		else {
			stream << "(" << *(pair.key) << "," << *(pair.value) << ")";
		}
		return stream;
	}
};