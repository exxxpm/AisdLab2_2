#include <iostream>
#include <vector>
#include <cmath>

#define MACHINE_WORD sizeof(size_t)//размер машинного слова
#define PARAM_A ((std::sqrt(5)-1)/2) //Кнут предложил использовать числа близкие к значению золотого сечения
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

	bool operator==(const pair& other){
		return key == other.key && value == other.value && is_filled == other.is_filled;
	}

	bool operator!=(const pair& other) {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& stream,const pair& pair) {
		stream << "(" << pair.key << "," << pair.value << ")";
		return stream;
	}
};

template <class Key, class Value>
class hash_table {
	std::vector<pair<Key, Value>> _data;
	size_t _size;//колличество занятых ячеек

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

	hash_table(size_t size, const Value& min, const Value& max): _size(0){//макс и мин в основном нужны чтобы вызывался именно тот коструктор который мы хотим. иначе у них сигнатура одинаковая
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

	hash_table(const hash_table& other) : _size(other._size)
	{
		if (other == *this) return;
		for (auto& pair : other._data)
		{
			if (pair.is_filled) insert(pair.key, pair.value);
		}
	}

	hash_table& operator=(hash_table other) {//передаем копию специяально! не по конст ссылке, чтоб не делать копию самостоятельно
		swap(other);
		return *this;
	}

	void swap(hash_table& other) noexcept {//для оператора =
		std::swap(_data, other._data);
		std::swap(_size, other._size);
	}

	size_t get_size() const noexcept { return _size; }

	bool operator==(const hash_table& other){
		if (_size != other._size) return false;
		for (size_t i = 0; i < std::min(this->_data.size(), other._data.size()); ++i){//берем минимальный размер вектора из двух,чтоб не делать лишних итераций
			if (this->_data[i] != other._data[i]) return false;
		}
		return true;
	}

	void insert(const Key& key, const Value& value){
		if (_size) {
			double load_factor = _size / (_data.size() + 0.0);//считаем коэф загруженности и увеличиваем размер таблицыБ если он больше 0.6
			if (load_factor > 0.6) _grow();
		}
		for (size_t i = 0; i < _data.size(); ++i)
		{
			size_t index = (hash(key) + i * hash(key)) % _data.size();//пробирование с помощью двойного хеширования
			if (!_data[index].is_filled)
			{
				_data[index] = pair(key, value);
				++_size;
				return;
			}
			if (_data[index].key == key) throw std::logic_error("key is exist");
		}
	}

	void insert_or_assign(const Key& key, const Value& value){
		if (_size) {
			double load_factor = _size / (_data.size() + 0.0);
			if (load_factor > 0.6) _grow();
		}
		for (size_t i = 0; i < _data.size(); ++i){
			size_t index = (hash(key) + i * hash(key)) % _data.size();//опять считаем индекс методом двойного хеширования
			if (!_data[index].is_filled){//если нет элемента вставляем, если есть обновляем
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

	bool contains(Value value){//проверка на наличие значения в таблице
		for (auto& pair : _data){
			if (pair.is_filled && (pair.value == value)) return true;
		}
		return false;
	}

	size_t count(Key key){//т.к у нас ключи повторяться не могут, то мы возвращаем либо 0 либо 1.если встретили такой ключ,то сразу вернем 1,т.к гарантируем, что ключ всего 1
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

	bool erase(Key key){
		for (size_t i = 0; i < _data.size(); ++i){
			size_t index = (hash(key) + i * hash(key)) % _data.size();
			if (_data[index].is_filled && _data[index].key == key){
				_data[index].is_filled = false;//фактически мы его не удаляем, просто говорим, что его нет (нам же не надо менять размер таблицы, просто будем считать, что удалили)
				--_size;
				return true;
			}
		}
		return false;
	}

	size_t hash(Key key) {//метод многократного сдвига
		int first_part = static_cast<int>(std::fmod(static_cast<double>(key * FIXED_INT), std::pow(2, MACHINE_WORD)));
		int l = static_cast<int>(std::log2(_data.size()));
		return (first_part >> static_cast<int>(MACHINE_WORD - l));
	}

	void print(){
		for (auto& pair : _data)
		{
			if (pair.is_filled) std::cout << pair<< std::endl;
		}
	}
};