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
		tmp.resize((_data.size() + 1)* 1.7);
		std::swap(tmp, _data);
		_size = 0;
		for (auto i = 0; i < tmp.size(); ++i) {
			if (tmp[i].is_filled) {
				insert(tmp[i].key, tmp[i].value);
			}
		}
	}
public:
	hash_table(size_t size): _size(0) {
		if (!size) {
			throw std::invalid_argument("invalid size");
		}
		_data.resize(size);
	}

	hash_table(size_t size, const Value& min, const Value& max): _size(0){
		if (!size) {
			throw std::invalid_argument("invalid size");
		}
		_data.resize(size);
		for (int i = 0; i < size; ++i) {
			Value value = rand() % (max - min + 1) + min;//рандомное значение
			_data[i] = pair(i+1, value);
			++_size;
			_data[i].is_filled = true;
		}
	}

	size_t get_size() { return _size; }

	void insert(const Key& key, const Value& value)
	{
		if (_size) {
			double load_factor = _size / (_data.size() + 0.0);//считаем коэф загруженности и увеличиваем размер таблицыЅ если он больше 0.6
			if (load_factor > 0.6) _grow();
		}
		for (size_t i = 0; i < _data.size(); ++i)
		{
			size_t index = (hash(key) + i * hash(key)) % _data.size();//пробирование с помощью двойного хешировани€
			if (!_data[index].is_filled)
			{
				_data[index] = pair(key, value);
				++_size;
				return;
			}
			if (_data[index].key == key) throw std::logic_error("key is exist");
		}
	}

	void insert_or_assign(const Key& key, const Value& value)
	{
		if (_size) {
			double load_factor = _size / (_data.size() + 0.0);
			if (load_factor > 0.6) _grow();
		}
		for (size_t i = 0; i < _data.size(); ++i)
		{
			size_t index = (hash(key) + i * hash(key)) % _data.size();//оп€ть считаем индекс методом двойного хешировани€
			if (!_data[index].is_filled)//если нет элемента вставл€ем, если есть обновл€ем
			{
				_data[index] = pair(key, value);
				++_size;
				return;
			}
			if (_data[index].key == key) {
				_data[index].value = value;
				return;
			}
		}
	}

	bool contains(Value value){//проверка на наличие значени€ в таблице
		for (auto& pair : _data){
			if (pair.is_filled && (pair.value == value)) return true;
		}
		return false;
	}

	size_t count(Key key){//т.к у нас ключи повтор€тьс€ не могут, то мы возвращаем либо 0 либо 1.если встретили такой ключ,то сразу вернем 1,т.к гарантируем, что ключ всего 1
		for (auto& pair : _data) {
			if (pair.is_filled && (pair.key == key)) return 1;
		}
		return 0;
	}

	Value* search(Key key){
		for (size_t i = 0; i < _data.size(); ++i){
			size_t index = (hash(key) + i * hash(key)) % _data.size();
			if (_data[index].is_filled && _data[index].key == key){
				return &(_data[index].value);
			}
		}
		return nullptr;
	}

	size_t hash(Key key) {//метод многократного сдвига
		int first_part = static_cast<int>(std::fmod(static_cast<double>(key * FIXED_INT), std::pow(2, MACHINE_WORD)));
		int l = static_cast<int>(std::log2(_data.size()));
		return (first_part >> static_cast<int>(MACHINE_WORD - l));
	}

	

	size_t size() const noexcept { return _size; }

	void print(){
		for (auto& pair : _data)
		{
			if (pair.is_filled) std::cout << pair<< std::endl;
		}
	}
};