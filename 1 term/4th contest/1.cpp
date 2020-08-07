/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Хеш-функцию строки реализуйте с помощью вычисления значения многочлена методом Горнера.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
Для разрешения коллизий используйте двойное хеширование
*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class HashTable {
public:
	HashTable(size_t init_size = 8) {
		body.resize(init_size);
		inserted = 0;
	}

	~HashTable() = default;

	bool insert(string const& key) {
		if (4 * inserted >= 3 * body.size()) rehash(); // if fill_factor >= 0.75 rehash the table
    	if (find(key)) return false;
		for (size_t hash = hash1(key), step = hash2(key); true; hash = (hash + step) % body.size()) {
			if (body[hash].key == key && body[hash].tag == item::BUSY) return false;
			else if (body[hash].tag != item::BUSY) {
				body[hash] = { key, item::BUSY };
				++inserted;
				return true;
			}
		}
	}

	bool remove(string const& key) {
		for (size_t hash = hash1(key), step = hash2(key), i = 0; i < body.size(); hash = (hash + step) % body.size()) {
			if (body[hash].tag == item::EMPTY) return false;
			else if (body[hash].key == key  && body[hash].tag == item::BUSY) {
				body[hash].tag = item::DELETED;
				--inserted;
				return true;
			}
			++i;
		}
		return false;
	}

	bool find(string const& key) const {
		for (size_t hash = hash1(key), step = hash2(key), i = 0; i < body.size(); hash = (hash + step) % body.size()) {
			if (body[hash].tag == item::EMPTY) return false;
			else if (body[hash].key == key && body[hash].tag == item::BUSY) return true;
			++i;
		}
		return false;
	}

private:
	struct item {
		item(string const& key = "", int tag = item::EMPTY) : key(key), tag(tag) {};
		string key;
		int tag;
		enum {
			EMPTY, // there is no item
			BUSY, // there is an item
			DELETED // it's a deleted item
		};
	};

	vector<item> body;
	size_t inserted;

	size_t hash1(string const& key) const {
		size_t hash = 0;
		for (size_t i = 0; i < key.size(); ++i) hash = (hash * 29 + key[i] - 'a' + 1) % body.size();
		return hash;
	}

	size_t hash2(string const& key) const {
		size_t hash = 0;
		for (size_t i = 0; i < key.size(); ++i) hash = (hash * 31 + key[i] - 'a' + 1) % body.size();
		return hash % 2 == 0 ? (hash + 1) % body.size() : hash; // to make hash2 mutually simple to body.size() = 2^n
	}

	void rehash() {
		vector<item> buf;
		swap(buf, body);
		body.resize(2 * buf.size());
		inserted = 0;
		for (size_t i = 0; i < buf.size(); ++i)
			if (buf[i].tag == item::BUSY) insert(buf[i].key);
	}
};

int main() {
	HashTable table;
	string line;
	while (getline(cin, line)) {
		char command = line[0];
		line.erase(0, 2);
		string word = line;

		bool answer;
		if (command == '+') answer = table.insert(word); // insert
		else if (command == '-') answer = table.remove(word); // erase
		else answer = table.find(word); // find
		cout << (answer ? "OK" : "FAIL") << endl;
	}
	return 0;
}