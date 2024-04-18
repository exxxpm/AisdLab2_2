#include <iostream>
#include <vector>
#include <cmath>

#define MACHINE_WORD sizeof(size_t)//размер машинного слова
#define PARAM_A ((std::sqrt(5)-1)/2) // нут предложил использовать числа близкие к значению золотого сечени€
#define FIXED_INT (PARAM_A*std::pow(2,MACHINE_WORD)) //константа от 0 до 2^MACHINE_WORD
#define COUNT_ROME_NUMS 7

enum class rome_nums {I = 1, V = 5, X = 10, L = 50, C = 100, D = 500, M = 1000};

template<class Key,class Value>
struct pair {
	Key key;
	Value value;
	bool is_filled;

	pair() : key(NULL), value(NULL), is_filled(false) {};
	pair(Key key, Value val) :key(key), value(val), is_filled(true) {};

	friend std::ostream& operator<<(std::ostream& stream,const pair& pair) {
		stream << "(" << pair.key << "," << pair.value << ")";
		return stream;
	}
};

template <class Key, class Value>
class hash_table {
	std::vector<pair<Key, Value>> _data;
	size_t _size;//колличество зан€тых €чеек

	void _grow() {//увеличивает размер хеш таблицы, нужен когда коэф загруженности больше 0.6
		std::vector<pair<Key, Value>> tmp;
		tmp.resize(_data.size() * 1.7);
		std::swap(tmp, _data);
		_size = 0;
		for (auto& pair : tmp) {
			if (pair.is_filled) insert(pair.key, pair.value);
		}
	}
public:
	hash_table(size_t size): _size(0) {
		if (!size) {
			throw std::invalid_argument("invalid size");
		}
		_data.resize(size);
	}

	size_t get_size() { return _size; }

	void insert(const Key& key, const Value& value) {
		size_t ind = hash(key) % _data.size();
		auto load_factor = _size / (_data.size() + 0.0);//+0.0 это каст к даблу (считаем коэф загруженности хеш таблицы
		if (load_factor > 0.6) _grow();
		for (size_t i = ind; i < _data.size(); ++i)//линейное пробирование с шагом 1
		{
			if (_data[ind].key == key) throw std::logic_error("");
			if (!_data[i].is_filled) {
				_data[i] = pair(key, value);
				++_size;
				return;
			}
		}
	}

	size_t hash(Key key) {//метод многократного сдвига
		int first_part = static_cast<int>(std::fmod(static_cast<double>(key * FIXED_INT), std::pow(2, MACHINE_WORD)));
		int l = static_cast<int>(std::log2(_data.size()));
		return (first_part >> static_cast<int>(MACHINE_WORD - l));
	}
	size_t size() const noexcept { return _size; }
};