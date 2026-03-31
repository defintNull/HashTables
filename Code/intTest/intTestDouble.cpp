#include "../HashTable.h"
#include <cstddef>
#include <ctime>
#include <fstream>
#include <random>
#include <string>
#include <utility>

template <typename K, typename V>
void insert(HashTable<K, V>& hashTable, const std::pair<K, V>& arr, std::ofstream& file, int elements, bool print = true) {
    if(print) {
        struct timespec start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        // Insert
        hashTable.insert(arr.first, arr.second);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double cpu_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        csvOperationPrint(
            file,
            hashTable.getOverflow(),
            hashTable.getHash(),
            hashTable.getResize(),
            elements,
            hashTable.getMaxLoadFactor(),
            "INSERT",
            cpu_time,
            hashTable.getLoadFactor(),
            hashTable.memoryUsage(),
            hashTable.getKeyDistribution()
        );
    } else {
        // Insert
        hashTable.insert(arr.first, arr.second);
        hashTable.memoryUsage();
    }
}

template <typename K, typename V>
void exist(HashTable<K, V>& hashTable, const K& key, std::ofstream& file, int elements, bool print = true) {
    if(print) {
        struct timespec start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        // Exist
        hashTable.exist(key);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double cpu_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        csvOperationPrint(
            file,
            hashTable.getOverflow(),
            hashTable.getHash(),
            hashTable.getResize(),
            elements,
            hashTable.getMaxLoadFactor(),
            "EXIST",
            cpu_time,
            hashTable.getLoadFactor(),
            hashTable.memoryUsage(),
            hashTable.getKeyDistribution()
        );
    } else {
        // Exist
        hashTable.exist(key);
        hashTable.memoryUsage();
    }
}

template <typename K, typename V>
void get(HashTable<K, V>& hashTable, const K& key, std::ofstream& file, int elements, bool print = true) {
    if(print) {
        struct timespec start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        // Get
        hashTable.get(key);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double cpu_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        csvOperationPrint(
            file,
            hashTable.getOverflow(),
            hashTable.getHash(),
            hashTable.getResize(),
            elements,
            hashTable.getMaxLoadFactor(),
            "GET",
            cpu_time,
            hashTable.getLoadFactor(),
            hashTable.memoryUsage(),
            hashTable.getKeyDistribution()
        );
    } else {
        // Get
        hashTable.get(key);
        hashTable.memoryUsage();
    }
}

template <typename K, typename V>
void update(HashTable<K, V>& hashTable, const std::pair<K, V>& arr, std::ofstream& file, int elements, bool print = true) {
    if(print) {
        struct timespec start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        // Update
        hashTable.update(arr.first, arr.second);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double cpu_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        csvOperationPrint(
            file,
            hashTable.getOverflow(),
            hashTable.getHash(),
            hashTable.getResize(),
            elements,
            hashTable.getMaxLoadFactor(),
            "UPDATE",
            cpu_time,
            hashTable.getLoadFactor(),
            hashTable.memoryUsage(),
            hashTable.getKeyDistribution()
        );
    } else {
        // Update
        hashTable.update(arr.first, arr.second);
        hashTable.memoryUsage();    
    }
}

template <typename K, typename V>
void remove(HashTable<K, V>& hashTable, const K& key, std::ofstream& file, int elements, bool print = true) {
    if(print) {
        struct timespec start, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

        // Remove
        hashTable.remove(key);

        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double cpu_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        csvOperationPrint(
            file,
            hashTable.getOverflow(),
            hashTable.getHash(),
            hashTable.getResize(),
            elements,
            hashTable.getMaxLoadFactor(),
            "REMOVE",
            cpu_time,
            hashTable.getLoadFactor(),
            hashTable.memoryUsage(),
            hashTable.getKeyDistribution()
        );
    } else {
        // Remove
        hashTable.remove(key);
        hashTable.memoryUsage();
    }
}

std::pair<std::ofstream, std::ofstream> setup() {
    std::ofstream op_file("op_data_double.csv");
    std::ofstream hash_file("hash_data_double.csv");

    // Intestazione
    op_file << "OVERFLOW,HASH,RESIZE,Elements,Max_load_factor,Operazione,Latenza,Load_factor,Memoria,Key_distribution\n";
    hash_file << "OVERFLOW,HASH,RESIZE,Elements,Max_load_factor,Latenza,Memoria\n";

    return {std::move(hash_file), std::move(op_file)};
}

std::ostream& operator<<(std::ostream& os, OVERFLOW o) {
    switch(o) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING: return os << "OPEN_ADDRESSING_LINEAR_PROBING";
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING: return os << "OPEN_ADDRESSING_DOUBLE_HASHING";
        case OVERFLOW::CHAINING: return os << "CHAINING";
        case OVERFLOW::CUCKOO: return os << "CUCKOO";
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, HASH o) {
    switch(o) {
        case HASH::MODE_1: return os << "MODE_1";
        case HASH::MODE_2: return os << "MODE_2";
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, RESIZE o) {
    switch(o) {
        case RESIZE::CONSTANT: return os << "CONSTANT";
        case RESIZE::DOUBLE: return os << "DOUBLE";
        case RESIZE::DYNAMIC: return os << "DYNAMIC";
    }
    return os;
}

void csvOperationPrint(
    std::ofstream& file,
    OVERFLOW OVERFLOW,
    HASH HASH,
    RESIZE RESIZE,
    std::size_t elements,
    double max_load_factor,
    std::string Operazione,
    double Latenza,
    double Load_factor,
    std::size_t Memoria,
    double Key_distribution
) {
    file << OVERFLOW << ","
         << HASH << ","
         << RESIZE << ","
         << elements << ","
         << max_load_factor << ","
         << Operazione << ","
         << Latenza << ","
         << Load_factor << ","
         << Memoria << ","
         << Key_distribution << "\n";
}

void csvHashPrint(
    std::ofstream& file,
    OVERFLOW OVERFLOW,
    HASH HASH,
    RESIZE RESIZE,
    std::size_t elements,
    double max_load_factor,
    double Latenza,
    std::size_t Memoria
) {
    file << OVERFLOW << ","
         << HASH << ","
         << RESIZE << ","
         << elements << ","
         << max_load_factor << ","
         << Latenza << ","
         << Memoria << "\n";
}

template <typename K, typename V>
void intTestRoutine(HashTable<K, V>& HashTable, int elements, std::ofstream& file_hash, std::ofstream& file_op, bool mode = true) {
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

    struct timespec start, end;
    if(!mode) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    }

    std::size_t counter = 0;
    while(counter < numbers.size()) {
        std::uniform_int_distribution<> dist2(0, 9);
        switch(dist2(gen)) {
            case 0:
            case 1:
            case 2:
            case 3: {
                // Exist
                if(counter > 0) {
                    std::uniform_int_distribution<> dist_temp(0, numbers.size() - 1);
                    exist(HashTable, numbers[dist_temp(gen)].first, file_op, elements, mode);
                }
                break;
            }
            case 4:
            case 5:
            case 6: {
                // Insert
                insert(HashTable, numbers[counter], file_op, elements, mode);
                counter++;
                break;
            }
            case 7: {
                // Update
                if(counter > 0) {
                    std::uniform_int_distribution<> dist_temp(0, numbers.size() - 1);
                    update(HashTable, std::make_pair(numbers[dist_temp(gen)].first, dist(gen)), file_op, elements, mode);
                }
                break;
            }
            case 8: {
                // Get
                if(counter > 0) {
                    std::uniform_int_distribution<> dist_temp(0, numbers.size() - 1);
                    get(HashTable, numbers[dist_temp(gen)].first, file_op, elements, mode);
                }
                break;
            }
            case 9: {
                // Remove
                if(counter > 0) {
                    std::uniform_int_distribution<> dist_temp(0, numbers.size() - 1);
                    remove(HashTable, numbers[dist_temp(gen)].first, file_op, elements, mode);
                }
                break;
            }
        }
    }

    if(!mode) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        double cpu_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        csvHashPrint(
            file_hash,
            HashTable.getOverflow(),
            HashTable.getHash(),
            HashTable.getResize(),
            elements,
            HashTable.getMaxLoadFactor(),
            cpu_time,
            HashTable.maxMemoryUsage()
        );
    }
}

void intTest(int elements, std::pair<std::ofstream, std::ofstream>& files, int load_factor = 0) {
    int errors = 0;

    double linear_load_factor = 0.75;
    double chaining_load_factor = 2;
    double cuckoo_load_factor = 0.5;

    switch (load_factor) {
        case 1: {
            linear_load_factor = 0.5;
            chaining_load_factor = 1;
            cuckoo_load_factor = 0.3;
            break;
        }
        case 2: {
            linear_load_factor = 0.9;
            chaining_load_factor = 3;
            cuckoo_load_factor = 0.75;
            break;
        }
    }

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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::CONSTANT, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::CONSTANT, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DYNAMIC, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_1, RESIZE::DYNAMIC, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_2, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING, HASH::MODE_2, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::CONSTANT, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::CONSTANT, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DYNAMIC, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_1, RESIZE::DYNAMIC, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_2, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING, HASH::MODE_2, RESIZE::DOUBLE, 256, linear_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DOUBLE, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DOUBLE, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::CONSTANT, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::CONSTANT, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DYNAMIC, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_1, RESIZE::DYNAMIC, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_2, RESIZE::DOUBLE, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CHAINING" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CHAINING, HASH::MODE_2, RESIZE::DOUBLE, 256, chaining_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DOUBLE, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DOUBLE, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::CONSTANT, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::CONSTANT" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::CONSTANT, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DYNAMIC, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_1" << std::endl;
        std::cout << "  RESIZE::DYNAMIC" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_1, RESIZE::DYNAMIC, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
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

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_2, RESIZE::DOUBLE, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second);
    }
    {
        std::cout << "-----------------------------" <<std::endl;
        std::cout << "HashTable:" << std::endl;
        std::cout << "  OVERFLOW::CUCKOO" << std::endl;
        std::cout << "  HASH::MODE_2" << std::endl;
        std::cout << "  RESIZE::DOUBLE" << std::endl;
        std::cout << "  Key::INTEGER" << std::endl;
        std::cout << "  Value::INTEGER" << std::endl;
        std::cout << "-----------------------------" <<std::endl;

        HashTable<int, int> HashTable(OVERFLOW::CUCKOO, HASH::MODE_2, RESIZE::DOUBLE, 256, cuckoo_load_factor);
        intTestRoutine(HashTable, elements, files.first, files.second, false);
    }
}

int main() {
    std::pair<std::ofstream, std::ofstream> files = setup();
    std::size_t base_length = 1000;

    for(int x=0; x<5; x++) {
        // Standard load factor
        for(int i=0; i<5; i++) {
            std::cout << "-----------------------------" <<std::endl;
            std::cout << "ELEMENT COUNT: " << (base_length * std::pow(2, i)) <<std::endl;
            std::cout << "-----------------------------" <<std::endl;
            intTest(base_length * std::pow(2, i), files);
        }

        // Low load factor
        for(int i=0; i<5; i++) {
            std::cout << "-----------------------------" <<std::endl;
            std::cout << "ELEMENT COUNT: " << (base_length * std::pow(2, i)) <<std::endl;
            std::cout << "-----------------------------" <<std::endl;
            intTest(base_length * std::pow(2, i), files, 1);
        }

        // High load factor
        for(int i=0; i<5; i++) {
            std::cout << "-----------------------------" <<std::endl;
            std::cout << "ELEMENT COUNT: " << (base_length * std::pow(2, i)) <<std::endl;
            std::cout << "-----------------------------" <<std::endl;
            intTest(base_length * std::pow(2, i), files, 2);
        }
    }

    files.first.close();
    files.second.close();

    return 0;
}