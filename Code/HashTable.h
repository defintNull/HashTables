#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <functional>

enum class OVERFLOW {
    OPEN_ADDRESSING_LINEAR_PROBING,
    OPEN_ADDRESSING_DOUBLE_HASHING,
    CHAINING,
    CUCKOO
};

enum class HASH {
    MODE_1,
    MODE_2
};

enum class RESIZE {
    DOUBLE,
    CONSTANT,
    DYNAMIC
};

template <typename Tkey, typename Tvalue>
class HashTable {
    public:
        HashTable(
            OVERFLOW overflow = OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING,
            HASH hash = HASH::MODE_1,
            RESIZE resize = RESIZE::DOUBLE,
            std::size_t table_length = 256,
            double max_load_factor = 0.75
        );

        bool insert(const Tkey& key, const Tvalue& value);
        bool update(const Tkey& key, const Tvalue& value);
        bool remove(const Tkey& key);
        bool exist(const Tkey& key);
        std::optional<Tvalue> get(const Tkey& key);

        std::size_t memoryUsage();
        std::size_t maxMemoryUsage() const {
            return this->max_memory_usage;
        };
        OVERFLOW getOverflow() const {
            return this->overflow;
        };
        HASH getHash() const {
            return this->hash;
        };
        RESIZE getResize() const {
            return this->resize;
        };
        std::size_t getElementCount() const {
            return this->element_count;
        };
        double getLoadFactor() const {
            return static_cast<double>(this->element_count) / this->table_length;
        };
        double getMaxLoadFactor() const {
            return this->max_load_factor;
        };
        double getKeyDistribution() const;
    
    private:
        OVERFLOW overflow;
        HASH hash;
        RESIZE resize;
        std::size_t table_length;
        std::size_t element_count;
        double max_load_factor;
        bool dynamic = false;
        std::size_t dynamic_index = 0;
        std::size_t max_memory_usage = 0;

        enum class SlotState { EMPTY, OCCUPIED, DELETED };
        struct Slot {
            SlotState state = SlotState::EMPTY;
            Tkey key;
            Tvalue value;
        };
        std::vector<Slot> table_open;
        std::vector<std::vector<std::pair<Tkey, Tvalue>>> table_chaining;
        std::vector<Slot> table_cuckoo_1;
        std::vector<Slot> table_cuckoo_2;
        std::vector<Slot> dynamic_table_open;
        std::vector<std::vector<std::pair<Tkey, Tvalue>>> dynamic_table_chaining;
        std::vector<Slot> dynamic_table_cuckoo_1;
        std::vector<Slot> dynamic_table_cuckoo_2;

        template<typename K>
        struct KeyConvert {
            static std::size_t hash(const K& key) {
                if constexpr (std::is_integral_v<K> || std::is_same_v<K, char>) {
                    // int, long, size_t, char ecc → hash = valore così com’è
                    return static_cast<std::size_t>(key);
                } else if constexpr (std::is_same_v<K, std::string>) {
                    // stringhe
                    std::size_t h = 0;
                    for (char c : key) h = h * 31 + static_cast<unsigned char>(c);
                    return h;
                } else {
                    static_assert(sizeof(K) == 0, "Tipo non supportato");
                }
            }
        };
        std::size_t h1(const Tkey& key) const;
        std::size_t h1(const Tkey& key, const std::size_t length) const;
        std::size_t h2(const Tkey& key) const;
        std::size_t h2(const Tkey& key, const std::size_t length) const;
        std::size_t getHash(const Tkey& key) const;
        std::size_t getHash(const Tkey& key, const std::size_t length) const;
        std::size_t h3(const Tkey& key) const;
        std::size_t h3(const Tkey& key, const std::size_t length) const;

        bool isPrime(std::size_t n) const;
        std::size_t nextPrime(std::size_t n) const;
        std::size_t previousPrime(std::size_t n) const;

        bool resizeTable();
        void dynamicCopy();

        std::optional<std::size_t> find(const Tkey& key);
        std::optional<std::size_t> _insert(const Tkey& key, const Tvalue& value);
};

#include "HashTable.tpp"