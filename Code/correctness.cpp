#include "HashTable.h"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>


template <typename K, typename V>
std::vector<std::pair<K, V>> insert(HashTable<K, V>& hashTable, const std::vector<std::pair<K, V>>& arr, bool print = true) {
    int op_failed = 0;
    std::vector<std::pair<K, V>> arr_failed;
    // Insert
    if(print) {
        std::cout << "Inserting " << arr.size() << " keys" << std::endl;
    }
    op_failed = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        bool result = hashTable.insert(arr[i].first, arr[i].second);

        if(!result) {
            op_failed++;
            arr_failed.push_back(arr[i]);
            if(print) {
                std::cout << "Failed Insert " << i << "/" << arr.size() << " % " << arr[i].first << std::endl;
            }
        }
    }
    if(print) {
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }
    return arr_failed;
}

template <typename K, typename V>
std::vector<std::pair<K, V>> exist(HashTable<K, V>& hashTable, const std::vector<std::pair<K, V>>& arr, bool print = true) {
    int op_failed = 0;
    std::vector<std::pair<K, V>> arr_failed;
    // Exist
    if(print) {
        std::cout << "Check existence of " << arr.size() << " keys" << std::endl;
    }
    op_failed = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        bool result = hashTable.exist(arr[i].first);

        if(!result) {
            op_failed++;
            arr_failed.push_back(arr[i]);
            if(print) {
                std::cout << "Failed Exists " << i << "/" << arr.size() << " % " << arr[i].first << std::endl;
            }
        }
    }
    if(print) {
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }
    return arr_failed;
}

template <typename K, typename V>
std::vector<std::pair<K, V>> get(HashTable<K, V>& hashTable, const std::vector<std::pair<K, V>>& arr, bool print = true) {
    int op_failed = 0;
    std::vector<std::pair<K, V>> arr_failed;
    // Get
    if(print) {
        std::cout << "Get values for " << arr.size() << " keys" << std::endl;
    }
    op_failed = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::optional<V> result = hashTable.get(arr[i].first);

        if(!result.has_value()) {
            op_failed++;
            arr_failed.push_back(arr[i]);
            if(print) {
                std::cout << "Failed Get " << i << "/" << arr.size() << " % " << arr[i].first << std::endl;
            }
        }
    }
    if(print) {
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }
    return arr_failed;
}

template <typename K, typename V>
std::vector<std::pair<K, V>> update(HashTable<K, V>& hashTable, const std::vector<std::pair<K, V>>& arr, bool print = true) {
    int op_failed = 0;
    std::vector<std::pair<K, V>> arr_failed;
    // Update
    if(print) {
        std::cout << "Updating " << arr.size() << " keys" << std::endl;
    }
    op_failed = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        bool result = hashTable.update(arr[i].first, arr[i].second);

        if(!result) {
            op_failed++;
            arr_failed.push_back(arr[i]);
            if(print) {
                std::cout << "Failed updating " << i << "/" << arr.size() << " % " << arr[i].first << std::endl;
            }
        }
    }
    if(print) {
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }
    return arr_failed;
}

template <typename K, typename V>
std::vector<std::pair<K, V>> remove(HashTable<K, V>& hashTable, const std::vector<std::pair<K, V>>& arr, bool print = true) {
    int op_failed = 0;
    std::vector<std::pair<K, V>> arr_failed;
    // Remove
    if(print) {
        std::cout << "Removing " << arr.size() << " keys" << std::endl;
    }
    op_failed = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        bool result = hashTable.remove(arr[i].first);

        if(!result) {
            op_failed++;
            arr_failed.push_back(arr[i]);
            if(print) {
                std::cout << "Failed removing " << i << "/" << arr.size() << " % " << arr[i].first << std::endl;
            }
        }
    }
    if(print) {
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }
    return arr_failed;
}

template <typename K, typename V>
int intTestRoutine(HashTable<K, V>& HashTable, int elements) {
    const int ELEMENTS = elements;
    int errors = 0;
    // Generatore di numeri casuali
    std::vector<std::pair<int, int>> numbers;
    numbers.reserve(ELEMENTS);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, ELEMENTS*10);

    int value = 0;
    for (int i = 0; i < ELEMENTS; ++i) {
        value = dist(gen);
        numbers.emplace_back(value, value);
    }

    // Inserts
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "INSERT" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<int, int>> failed;
        failed = insert(HashTable, numbers);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, numbers);
        errors += failed.size();
        // Get
        failed = get(HashTable, numbers);
        errors += failed.size();
    }
    
    // Update
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "UPDATE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<int, int>> failed;
        std::vector<std::pair<int,int>> update_copy = numbers;
        std::unordered_map<int, int*> key_map;
        for (auto& [key, value] : update_copy) {
            key_map[key] = &value;
        }

        for (std::size_t x = 0; x < ELEMENTS/10; x++) {
            int key_to_update = dist(gen);
            
            int new_value;
            do { new_value = dist(gen); } while (new_value == key_to_update);

            if (auto it = key_map.find(key_to_update); it != key_map.end()) {
                *(it->second) = new_value;
            }
        }
        failed = update(HashTable, update_copy);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, update_copy);
        errors += failed.size();
        // Get
        int op_failed = 0;
        for (std::size_t i = 0; i < ELEMENTS; ++i) {
            std::optional<int> result = HashTable.get(update_copy[i].first);
            if(!result.has_value()) {
                op_failed++;
                std::cout << "Failed Get Value " << i << "/" << ELEMENTS << " % " << update_copy[i].first << std::endl;
            }
        }
        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // Remove
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "REMOVE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<int, int>> failed;
        std::vector<std::pair<int, int>> removed;
        std::uniform_int_distribution<> removed_index_gen(0, ELEMENTS - 1);
        std::unordered_set<std::size_t> used_indices;
        std::unordered_set<int> used_keys;
        while (removed.size() < ELEMENTS / 10) {

            std::size_t index = removed_index_gen(gen);
            const auto& [key, value] = numbers[index];

            // Controllo duplicati solo su indice e chiave
            if (used_indices.count(index) == 0 && used_keys.count(key) == 0) {
                removed.emplace_back(key, value);
                used_indices.insert(index);
                used_keys.insert(key);
            }
        }
        failed = remove(HashTable, removed);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, numbers, false);
        std::unordered_set<int> removed_keys;
        for (const auto& [key, value] : removed) removed_keys.insert(key);

        int op_failed = 0;
        for (const auto& [key, value] : failed) {
            if (removed_keys.count(key) == 0) {
                op_failed++;
                std::cout << "Failed removing " << key << std::endl;
            }
        }

        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    return errors;
}

int intTest(int elements) {
    int errors = 0;

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::CONSTANT);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += intTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::INTEGER
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_2, RESIZE::DOUBLE);
        errors += intTestRoutine(HashTable, elements);
    }

    return errors;
}

std::string generateRandomString() {
    static const std::string letters = "abcdefghijklmnopqrstuvwxyz";
    static std::uniform_int_distribution<> dist(0, letters.size() - 1);
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> lenDist(3, 18);
    std::string word;
    int len = lenDist(rng);
    for (int i = 0; i < len; ++i) {
        word += letters[dist(rng)];
    }
    return word;
}

template <typename K, typename V>
int stringTestRoutine(HashTable<K, V>& HashTable, int elements) {
    const int ELEMENTS = elements;
    int errors = 0;
    // Generatore di stringhe casuali
    std::vector<std::pair<std::string, std::string>> words;

    std::string word;
    for (int i = 0; i < ELEMENTS; ++i) {
        word = generateRandomString();
        words.emplace_back(word, word);
    }

    // Inserts
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "INSERT" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, std::string>> failed;
        failed = insert(HashTable, words);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, words);
        errors += failed.size();
        // Get
        failed = get(HashTable, words);
        errors += failed.size();
    }
    
    // Update
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "UPDATE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, std::string>> failed;
        std::vector<std::pair<std::string, std::string>> update_copy = words;
        std::unordered_map<std::string, std::string*> key_map;
        for (auto& [key, value] : update_copy) {
            key_map[key] = &value;
        }

        for (std::size_t x = 0; x < ELEMENTS/10; x++) {
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<> dist(0, words.size() - 1);

            // Seleziona un elemento casuale
            int i = dist(rng);
            std::string key_to_update = words[i].first;
            
            std::string new_value;
            do {
                new_value = generateRandomString();
            } while (new_value == key_to_update);

            if (auto it = key_map.find(key_to_update); it != key_map.end()) {
                *(it->second) = new_value;
            }
        }
        failed = update(HashTable, update_copy);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, update_copy);
        errors += failed.size();
        // Get
        int op_failed = 0;
        for (std::size_t i = 0; i < ELEMENTS; ++i) {
            std::optional<V> result = HashTable.get(update_copy[i].first);
            if(!result.has_value()) {
                op_failed++;
                std::cout << "Failed Get Value " << i << "/" << ELEMENTS << " % " << update_copy[i].first << std::endl;
            }
        }
        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // Remove
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "REMOVE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, std::string>> failed;
        std::vector<std::pair<std::string, std::string>> removed;
        std::uniform_int_distribution<> removed_index_gen(0, ELEMENTS - 1);
        std::unordered_set<int> used_indices;
        std::unordered_set<std::string> used_keys;
        while (removed.size() < ELEMENTS / 10) {

            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<> dist(0, words.size() - 1);

            // Seleziona un elemento casuale
            int index = dist(rng);

            const auto& [key, value] = words[index];

            // Controllo duplicati solo su indice e chiave
            if (used_indices.count(index) == 0 && used_keys.count(key) == 0) {
                removed.emplace_back(key, value);
                used_indices.insert(index);
                used_keys.insert(key);
            }
        }
        failed = remove(HashTable, removed);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, words, false);
        std::unordered_set<std::string> removed_keys;
        for (const auto& [key, value] : removed) removed_keys.insert(key);

        int op_failed = 0;
        for (const auto& [key, value] : failed) {
            if (removed_keys.count(key) == 0) {
                op_failed++;
                std::cout << "Failed removing " << key << std::endl;
            }
        }

        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    return errors;
}

int stringTest(int elements) {
    int errors = 0;

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::CONSTANT);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += stringTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::STRING
    * Value::STRING
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::STRING" << std::endl;
        std::cout << "  Value::STRING" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_2, RESIZE::DOUBLE);
        errors += stringTestRoutine(HashTable, elements);
    }

    return errors;
}

template <typename K, typename V>
int realWordTestRoutine(HashTable<K, V>& HashTable, int elements) {
    const int ELEMENTS = elements;
    int errors = 0;
    
    std::vector<std::pair<std::string, std::string>> words;
    
    std::ifstream file("words.txt");
    std::vector<std::string> load_words;
    std::string line;

    // carica tutte le parole
    while (std::getline(file, line)) {
        load_words.push_back(line);
    }

    file.close();

    // generatore casuale
    std::random_device rd;
    std::mt19937 gen(rd());

    // mescola tutte le parole
    std::shuffle(load_words.begin(), load_words.end(), gen);

    // Prende una parola a caso dal file
    for (int i = 0; i < ELEMENTS; ++i) {
        words.emplace_back(load_words[i], load_words[i]);
    }

    // Inserts
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "INSERT" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, std::string>> failed;
        failed = insert(HashTable, words);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, words);
        errors += failed.size();
        // Get
        failed = get(HashTable, words);
        errors += failed.size();
    }
    
    // Update
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "UPDATE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, std::string>> failed;
        std::vector<std::pair<std::string, std::string>> update_copy = words;
        std::unordered_map<std::string, std::string*> key_map;
        for (auto& [key, value] : update_copy) {
            key_map[key] = &value;
        }

        for (std::size_t x = 0; x < ELEMENTS/10; x++) {
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<> dist(0, words.size() - 1);

            // Seleziona un elemento casuale
            int i = dist(rng);
            std::string key_to_update = words[i].first;

            if (auto it = key_map.find(key_to_update); it != key_map.end()) {
                *(it->second) = load_words[i];
            }
        }
        failed = update(HashTable, update_copy);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, update_copy);
        errors += failed.size();
        // Get
        int op_failed = 0;
        for (std::size_t i = 0; i < ELEMENTS; ++i) {
            std::optional<V> result = HashTable.get(update_copy[i].first);
            if(!result.has_value()) {
                op_failed++;
                std::cout << "Failed Get Value " << i << "/" << ELEMENTS << " % " << update_copy[i].first << std::endl;
            }
        }
        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // Remove
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "REMOVE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, std::string>> failed;
        std::vector<std::pair<std::string, std::string>> removed;
        std::uniform_int_distribution<> removed_index_gen(0, ELEMENTS - 1);
        std::unordered_set<int> used_indices;
        std::unordered_set<std::string> used_keys;
        while (removed.size() < ELEMENTS / 10) {

            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<> dist(0, words.size() - 1);

            // Seleziona un elemento casuale
            int index = dist(rng);

            const auto& [key, value] = words[index];

            // Controllo duplicati solo su indice e chiave
            if (used_indices.count(index) == 0 && used_keys.count(key) == 0) {
                removed.emplace_back(key, value);
                used_indices.insert(index);
                used_keys.insert(key);
            }
        }
        failed = remove(HashTable, removed);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, words, false);
        std::unordered_set<std::string> removed_keys;
        for (const auto& [key, value] : removed) removed_keys.insert(key);

        int op_failed = 0;
        for (const auto& [key, value] : failed) {
            if (removed_keys.count(key) == 0) {
                op_failed++;
                std::cout << "Failed removing " << key << std::endl;
            }
        }

        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    return errors;
}

int realWordTest(int elements) {
    int errors = 0;

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CHAINING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::CONSTANT);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += realWordTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::REAL WORD
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::REAL WORD" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, std::string> HashTable(OVERFLOW::CUCKOO, HASH::MODE_2, RESIZE::DOUBLE);
        errors += realWordTestRoutine(HashTable, elements);
    }

    return errors;
}

template <typename K, typename V>
int hybridTestRoutine(HashTable<K, V>& HashTable, int elements) {
    const int ELEMENTS = elements;
    int errors = 0;
    
    std::vector<std::pair<std::string, int>> words;
    
    std::ifstream file("words.txt");
    std::vector<std::string> load_words;
    std::string line;

    // carica tutte le parole
    while (std::getline(file, line)) {
        load_words.push_back(line);
    }

    file.close();

    // generatore casuale
    std::random_device rd;
    std::mt19937 gen(rd());

    // mescola tutte le parole
    std::shuffle(load_words.begin(), load_words.end(), gen);

    // Prende una parola a caso dal file
    std::uniform_int_distribution<> dist(1, ELEMENTS*10);
    for (int i = 0; i < ELEMENTS; ++i) {
        words.emplace_back(load_words[i], dist(gen));
    }

    // Inserts
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "INSERT" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, int>> failed;
        failed = insert(HashTable, words);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, words);
        errors += failed.size();
        // Get
        failed = get(HashTable, words);
        errors += failed.size();
    }
    
    // Update
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "UPDATE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, int>> failed;
        std::vector<std::pair<std::string, int>> update_copy = words;
        std::unordered_map<std::string, int*> key_map;
        for (auto& [key, value] : update_copy) {
            key_map[key] = &value;
        }

        for (std::size_t x = 0; x < ELEMENTS/10; x++) {
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<> dist2(0, words.size() - 1);

            // Seleziona un elemento casuale
            int i = dist2(rng);
            std::string key_to_update = words[i].first;

            if (auto it = key_map.find(key_to_update); it != key_map.end()) {
                *(it->second) = dist(gen);
            }
        }
        failed = update(HashTable, update_copy);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, update_copy);
        errors += failed.size();
        // Get
        int op_failed = 0;
        for (std::size_t i = 0; i < ELEMENTS; ++i) {
            std::optional<V> result = HashTable.get(update_copy[i].first);
            if(!result.has_value()) {
                op_failed++;
                std::cout << "Failed Get Value " << i << "/" << ELEMENTS << " % " << update_copy[i].first << std::endl;
            }
        }
        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // Remove
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "REMOVE" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::vector<std::pair<std::string, int>> failed;
        std::vector<std::pair<std::string, int>> removed;
        std::uniform_int_distribution<> removed_index_gen(0, ELEMENTS - 1);
        std::unordered_set<int> used_indices;
        std::unordered_set<std::string> used_keys;
        while (removed.size() < ELEMENTS / 10) {

            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<> dist(0, words.size() - 1);

            // Seleziona un elemento casuale
            int index = dist(rng);

            const auto& [key, value] = words[index];

            // Controllo duplicati solo su indice e chiave
            if (used_indices.count(index) == 0 && used_keys.count(key) == 0) {
                removed.emplace_back(key, value);
                used_indices.insert(index);
                used_keys.insert(key);
            }
        }
        failed = remove(HashTable, removed);
        errors += failed.size();
        // Exist
        failed = exist(HashTable, words, false);
        std::unordered_set<std::string> removed_keys;
        for (const auto& [key, value] : removed) removed_keys.insert(key);

        int op_failed = 0;
        for (const auto& [key, value] : failed) {
            if (removed_keys.count(key) == 0) {
                op_failed++;
                std::cout << "Failed removing " << key << std::endl;
            }
        }

        errors += op_failed;
        std::cout << "Operation Failed: " << op_failed << std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    return errors;
}

int hybridTest(int elements) {
    int errors = 0;

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::CONSTANT);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CHAINING
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_2, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }

    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::CONSTANT
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::CONSTANT);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_1
    *   RESIZE::DYNAMIC
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DYNAMIC);
        errors += hybridTestRoutine(HashTable, elements);
    }
    /*
    * HashTable:
    *   OVERFLOW::CUCKOO
    *   HASH::MODE_2
    *   RESIZE::DOUBLE
    * 
    * Key::REAL WORD
    * Value::INTEGER
    */
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::REAL WORD" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<std::string, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_2, RESIZE::DOUBLE);
        errors += hybridTestRoutine(HashTable, elements);
    }

    return errors;
}

void correctnessTest() {
    int arr_error[4] = {0, 0, 0, 0};

    // Integer
    {
        int errors = 0;
        for(int x=0; x<20; x++) {
            for(int i=0; i<6; i++) {
                int current_error = intTest(std::pow(10, i));
                std::cout << "Partial Operation Failed: " << current_error << std::endl;
                std::cout << "-----------------------------" <<std::endl;
                errors += current_error;
            }
        }
        arr_error[0] += errors;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "INTEGER ERRORS: " << errors << std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // String
    {
        int errors = 0;
        for(int x=0; x<20; x++) {
            for(int i=0; i<6; i++) {
                int current_error = stringTest(std::pow(10, i));
                std::cout << "Partial Operation Failed: " << current_error << std::endl;
                std::cout << "-----------------------------" <<std::endl;
                errors += current_error;
            }
        }
        arr_error[1] += errors;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "STRING ERRORS: " << errors << std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // Real word set
    {
        int errors = 0;
        for(int x=0; x<20; x++) {
            for(int i=0; i<6; i++) {
                int current_error = realWordTest(std::pow(10, i));
                std::cout << "Partial Operation Failed: " << current_error << std::endl;
                std::cout << "-----------------------------" <<std::endl;
                errors += current_error;
            }
        }
        arr_error[2] += errors;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "REAL WORD ERRORS: " << errors << std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    // Hybrid Test (Real Word / int)
    {
        int errors = 0;
        for(int x=0; x<20; x++) {
            for(int i=0; i<6; i++) {
                int current_error = hybridTest(std::pow(10, i));
                std::cout << "Partial Operation Failed: " << current_error << std::endl;
                std::cout << "-----------------------------" <<std::endl;
                errors += current_error;
            }
        }
        arr_error[3] += errors;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HYBRID ERRORS: " << errors << std::endl;
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "-----------------------------" <<std::endl;
    }

    int total = 0;
    for(int i=0; i<4; i++) {
        total += arr_error[i];
    }
    std::cout << "-----------------------------" <<std::endl;
    std::cout << "-----------------------------" <<std::endl;
    std::cout << "TOTAL ERRORS: " << total << std::endl;
    std::cout << "-----------------------------" <<std::endl;
    std::cout << "INTEGER ERRORS: " << arr_error[0] << std::endl;
    std::cout << "STRING ERRORS: " << arr_error[1] << std::endl;
    std::cout << "REAL WORD ERRORS: " << arr_error[2] << std::endl;
    std::cout << "HYBRID ERRORS: " << arr_error[3] << std::endl;
    std::cout << "-----------------------------" <<std::endl;
    std::cout << "-----------------------------" <<std::endl;
}

int main() {
    correctnessTest();
    return 0;
}