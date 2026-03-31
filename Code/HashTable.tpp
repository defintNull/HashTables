#include "HashTable.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

template <typename Tkey, typename Tvalue>
HashTable<Tkey, Tvalue>::HashTable(
    OVERFLOW overflow,
    HASH hash,
    RESIZE resize,
    std::size_t table_length,
    double max_load_factor
)
    : overflow(overflow),
      hash(hash),
      resize(resize),
      element_count(0),
      max_load_factor(max_load_factor)
{
    this->table_length = this->nextPrime(table_length);
    switch (this->overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING:
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING:
            table_open.resize(this->table_length);
            break;

        case OVERFLOW::CHAINING:
            table_chaining.resize(this->table_length);
            break;

        case OVERFLOW::CUCKOO:
            table_cuckoo_1.resize(this->table_length);
            table_cuckoo_2.resize(this->table_length);
            break;
    }
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::h1(const Tkey& key) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);
    // modulo table_length
    return h % table_length;
}
template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::h1(const Tkey& key, const std::size_t length) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);
    // modulo table_length
    return h % length;
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::h2(const Tkey& key) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);
    constexpr std::size_t a = 11400714819323198485ull;
    return (h * a) % table_length;
}
template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::h2(const Tkey& key, const std::size_t length) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);
    constexpr std::size_t a = 11400714819323198485ull;
    return (h * a) % length;
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::getHash(const Tkey& key) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);

    switch (hash) {
        case HASH::MODE_1:
            return h1(key);
        case HASH::MODE_2:
            return h2(key);
    }
    return 0;
}
template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::getHash(const Tkey& key, const std::size_t length) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);

    switch (hash) {
        case HASH::MODE_1:
            return h1(key, length);
        case HASH::MODE_2:
            return h2(key, length);
    }
    return 0;
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::h3(const Tkey& key) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);
    std::size_t R = previousPrime(table_length - 1);
    return R - (h % R); 
}
template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::h3(const Tkey& key, const std::size_t length) const {
    std::size_t h = KeyConvert<Tkey>::hash(key);
    std::size_t R = previousPrime(length - 1);
    return R - (h % R); 
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::isPrime(std::size_t n) const {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::nextPrime(std::size_t n) const {
    if (n <= 2) return 2;

    if (n % 2 == 0) n++;

    while (!isPrime(n)) {
        n += 2;
    }

    return n;
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::previousPrime(std::size_t n) const {
    if (n <= 2) return -1; // Non esistono primi minori di 2

    int candidate = n - 1;
    while (candidate >= 2) {
        if (isPrime(candidate)) {
            return candidate;
        }
        candidate--;
    }
    return -1;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::resizeTable() {
    switch(resize) {
        case RESIZE::DOUBLE : {
            table_length = this->nextPrime(table_length * 2);
            element_count = 0;

            switch (overflow) {
                case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING :
                case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
                    std::vector<Slot> table_open_old = std::move(table_open);

                    table_open.clear();
                    table_open.resize(table_length);

                    bool flag = false;
                    for(std::size_t i=0; i<table_open_old.size(); i++) {
                        if (table_open_old[i].state == SlotState::OCCUPIED) {
                            std::optional<std::size_t> payload = this->_insert(table_open_old[i].key, table_open_old[i].value);
                            if(!payload.has_value()) {
                                flag = true;
                                break;
                            }
                        }
                    }
                    while(flag) {
                        table_length = this->nextPrime(table_length * 2);
                        element_count = 0;
                        table_open.clear();
                        table_open.resize(table_length);

                        flag = false;
                        for(std::size_t i=0; i<table_open_old.size(); i++) {
                            if (table_open_old[i].state == SlotState::OCCUPIED) {
                                std::optional<std::size_t> payload = this->_insert(table_open_old[i].key, table_open_old[i].value);
                                if(!payload.has_value()) {
                                    flag = true;
                                    break;
                                }
                            }
                        }
                    }

                    return true;
                }
                case OVERFLOW::CHAINING : {
                    std::vector<std::vector<std::pair<Tkey, Tvalue>>> table_chaining_old = std::move(table_chaining);

                    table_chaining.clear();
                    table_chaining.resize(table_length);

                    for(std::size_t i=0; i<table_chaining_old.size(); i++) {
                        if (table_chaining_old[i].size() != 0) {
                            for(std::size_t j=0; j<table_chaining_old[i].size(); j++) {
                                this->_insert(table_chaining_old[i][j].first, table_chaining_old[i][j].second);
                            }
                        }
                    }
                    return true;
                }
                case OVERFLOW::CUCKOO : {
                    std::vector<Slot> table_cuckoo_1_old = std::move(table_cuckoo_1);
                    std::vector<Slot> table_cuckoo_2_old = std::move(table_cuckoo_2);

                    table_cuckoo_1.clear();
                    table_cuckoo_1.resize(table_length);
                    table_cuckoo_2.clear();
                    table_cuckoo_2.resize(table_length);

                    bool flag = false;
                    for(std::size_t i=0; i<table_cuckoo_1_old.size(); i++) {
                        std::optional<std::size_t> payload = std::nullopt;
                        if (table_cuckoo_1_old[i].state == SlotState::OCCUPIED) {
                            payload = this->_insert(table_cuckoo_1_old[i].key, table_cuckoo_1_old[i].value);
                            if(!payload.has_value()) {
                                flag = true;
                                break;
                            }
                        }
                        if (table_cuckoo_2_old[i].state == SlotState::OCCUPIED) {
                            payload = this->_insert(table_cuckoo_2_old[i].key, table_cuckoo_2_old[i].value);
                            if(!payload.has_value()) {
                                flag = true;
                                break;
                            }
                        }
                    }
                    while(flag) {
                        table_length = this->nextPrime(table_length * 2);
                        element_count = 0;
                        table_cuckoo_1.clear();
                        table_cuckoo_1.resize(table_length);
                        table_cuckoo_2.clear();
                        table_cuckoo_2.resize(table_length);

                        flag = false;
                        for(std::size_t i=0; i<table_cuckoo_1_old.size(); i++) {
                            std::optional<std::size_t> payload = std::nullopt;
                            if (table_cuckoo_1_old[i].state == SlotState::OCCUPIED) {
                                payload = this->_insert(table_cuckoo_1_old[i].key, table_cuckoo_1_old[i].value);
                                if(!payload.has_value()) {
                                    flag = true;
                                    break;
                                }
                            }
                            if (table_cuckoo_2_old[i].state == SlotState::OCCUPIED) {
                                payload = this->_insert(table_cuckoo_2_old[i].key, table_cuckoo_2_old[i].value);
                                if(!payload.has_value()) {
                                    flag = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                    return true;
                }
            }
        }
        case RESIZE::CONSTANT : {
            const int resize_const = 100000;
            table_length = this->nextPrime(table_length + resize_const);
            element_count = 0;

            switch (overflow) {
                case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING :
                case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
                    std::vector<Slot> table_open_old = std::move(table_open);

                    table_open.clear();
                    table_open.resize(table_length);

                    bool flag = false;
                    for(std::size_t i=0; i<table_open_old.size(); i++) {
                        if (table_open_old[i].state == SlotState::OCCUPIED) {
                            std::optional<std::size_t> payload = this->_insert(table_open_old[i].key, table_open_old[i].value);
                            if(!payload.has_value()) {
                                flag = true;
                                break;
                            }
                        }
                    }
                    while(flag) {
                        table_length = this->nextPrime(table_length + resize_const);
                        element_count = 0;
                        table_open.clear();
                        table_open.resize(table_length);

                        bool flag = false;
                        for(std::size_t i=0; i<table_open_old.size(); i++) {
                            if (table_open_old[i].state == SlotState::OCCUPIED) {
                                std::optional<std::size_t> payload = this->_insert(table_open_old[i].key, table_open_old[i].value);
                                if(!payload.has_value()) {
                                    flag = true;
                                    break;
                                }
                            }
                        }
                    }
                    return true;
                }
                case OVERFLOW::CHAINING : {
                    std::vector<std::vector<std::pair<Tkey, Tvalue>>> table_chaining_old = std::move(table_chaining);

                    table_chaining.clear();
                    table_chaining.resize(table_length);

                    for(std::size_t i=0; i<table_chaining_old.size(); i++) {
                        if (table_chaining_old[i].size() != 0) {
                            for(std::size_t j=0; j<table_chaining_old[i].size(); j++) {
                                this->_insert(table_chaining_old[i][j].first, table_chaining_old[i][j].second);
                            }
                        }
                    }
                    return true;
                }
                case OVERFLOW::CUCKOO : {
                    std::vector<Slot> table_cuckoo_1_old = std::move(table_cuckoo_1);
                    std::vector<Slot> table_cuckoo_2_old = std::move(table_cuckoo_2);

                    table_cuckoo_1.clear();
                    table_cuckoo_1.resize(table_length);
                    table_cuckoo_2.clear();
                    table_cuckoo_2.resize(table_length);

                    bool flag = false;
                    for(std::size_t i=0; i<table_cuckoo_1_old.size(); i++) {
                        std::optional<std::size_t> payload = std::nullopt;
                        if (table_cuckoo_1_old[i].state == SlotState::OCCUPIED) {
                            payload = this->_insert(table_cuckoo_1_old[i].key, table_cuckoo_1_old[i].value);
                            if(!payload.has_value()) {
                                flag = true;
                                break;
                            }
                        }
                        if (table_cuckoo_2_old[i].state == SlotState::OCCUPIED) {
                            payload = this->_insert(table_cuckoo_2_old[i].key, table_cuckoo_2_old[i].value);
                            if(!payload.has_value()) {
                                flag = true;
                                break;
                            }
                        }
                    }
                    while(flag) {
                        table_length = this->nextPrime(table_length + resize_const);
                        element_count = 0;
                        table_cuckoo_1.clear();
                        table_cuckoo_1.resize(table_length);
                        table_cuckoo_2.clear();
                        table_cuckoo_2.resize(table_length);

                        flag = false;
                        for(std::size_t i=0; i<table_cuckoo_1_old.size(); i++) {
                            std::optional<std::size_t> payload = std::nullopt;
                            if (table_cuckoo_1_old[i].state == SlotState::OCCUPIED) {
                                payload = this->_insert(table_cuckoo_1_old[i].key, table_cuckoo_1_old[i].value);
                                if(!payload.has_value()) {
                                    flag = true;
                                    break;
                                }
                            }
                            if (table_cuckoo_2_old[i].state == SlotState::OCCUPIED) {
                                payload = this->_insert(table_cuckoo_2_old[i].key, table_cuckoo_2_old[i].value);
                                if(!payload.has_value()) {
                                    flag = true;
                                    break;
                                }
                            }
                        }
                    }
                    return true;
                }
            }
        }
        case RESIZE::DYNAMIC : {
            if(this->dynamic == true) {
                switch (overflow) {
                    case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING :
                    case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
                        std::vector<Slot> table_open_old = std::move(table_open);
                        if(this->dynamic_index < this->dynamic_table_open.size() / 2) {
                            bool flag = true;
                            while(flag) {
                                table_length = this->nextPrime(table_length * 2);
                                table_open.clear();
                                table_open.resize(table_length);

                                flag = false;
                                for(std::size_t i=0; i<table_open_old.size(); i++) {
                                    if (table_open_old[i].state == SlotState::OCCUPIED) {
                                        std::optional<std::size_t> payload = this->_insert(table_open_old[i].key, table_open_old[i].value);
                                        if(!payload.has_value()) {
                                            flag = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        } else {
                            bool flag = true;
                            while(flag) {
                                table_length = this->nextPrime(table_length * 2);
                                table_open.clear();
                                table_open.resize(table_length);

                                flag = false;
                                for(std::size_t i=this->dynamic_index; i<dynamic_table_open.size(); i++) {
                                    if (dynamic_table_open[i].state == SlotState::OCCUPIED) {
                                        std::optional<std::size_t> payload = this->_insert(dynamic_table_open[i].key, dynamic_table_open[i].value);
                                        if(!payload.has_value()) {
                                            flag = true;
                                            break;
                                        }
                                    }
                                }
                            }

                            std::swap(table_open_old, dynamic_table_open);

                            this->dynamic_index = 0;
                        }
                        return true;
                    }
                    case OVERFLOW::CHAINING : {
                        std::vector<std::vector<std::pair<Tkey, Tvalue>>> table_chaining_old = std::move(table_chaining);
                        if(this->dynamic_index < this->dynamic_table_chaining.size() / 2) {
                            bool flag = true;
                            while(flag) {
                                table_length = this->nextPrime(table_length * 2);
                                table_chaining.clear();
                                table_chaining.resize(table_length);

                                flag = false;
                                for(std::size_t i=0; i<table_chaining_old.size(); i++) {
                                    if (table_chaining_old[i].size() != 0) {
                                        for(std::size_t j=0; j<table_chaining_old[i].size(); j++) {
                                            std::optional<std::size_t> payload = this->_insert(table_chaining_old[i][j].first, table_chaining_old[i][j].second);
                                            if(!payload.has_value()) {
                                                flag = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            bool flag = true;
                            while(flag) {
                                table_length = this->nextPrime(table_length * 2);
                                table_chaining.clear();
                                table_chaining.resize(table_length);

                                flag = false;
                                for(std::size_t i=dynamic_index; i<dynamic_table_chaining.size(); i++) {
                                    if (dynamic_table_chaining[i].size() != 0) {
                                        for(std::size_t j=0; j<dynamic_table_chaining[i].size(); j++) {
                                            std::optional<std::size_t> payload = this->_insert(dynamic_table_chaining[i][j].first, dynamic_table_chaining[i][j].second);
                                            if(!payload.has_value()) {
                                                flag = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            std::swap(table_chaining_old, dynamic_table_chaining);

                            this->dynamic_index = 0;
                        }
                        return true;
                    }
                    case OVERFLOW::CUCKOO : {
                        std::vector<Slot> table_cuckoo_1_old = std::move(table_cuckoo_1);
                        std::vector<Slot> table_cuckoo_2_old = std::move(table_cuckoo_2);
                        if(this->dynamic_index < this->dynamic_table_cuckoo_1.size()) {
                            bool flag = true;
                            while(flag) {
                                table_length = this->nextPrime(table_length * 2);
                                table_cuckoo_1.clear();
                                table_cuckoo_1.resize(table_length);
                                table_cuckoo_2.clear();
                                table_cuckoo_2.resize(table_length);

                                flag = false;
                                for(std::size_t i=0; i<(table_cuckoo_1_old.size()*2); i++) {
                                    if(i < table_cuckoo_1_old.size()) {
                                        if (table_cuckoo_1_old[i].state == SlotState::OCCUPIED) {
                                            std::optional<std::size_t> payload = this->_insert(table_cuckoo_1_old[i].key, table_cuckoo_1_old[i].value);
                                            if(!payload.has_value()) {
                                                flag = true;
                                                break;
                                            }
                                        }
                                    } else {
                                        std::size_t index = i - table_cuckoo_2_old.size();
                                        if (table_cuckoo_2_old[index].state == SlotState::OCCUPIED) {
                                            std::optional<std::size_t> payload = this->_insert(table_cuckoo_2_old[index].key, table_cuckoo_2_old[index].value);
                                            if(!payload.has_value()) {
                                                flag = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        } else {
                            bool flag = true;
                            while(flag) {
                                table_length = this->nextPrime(table_length * 2);
                                table_cuckoo_1.clear();
                                table_cuckoo_1.resize(table_length);
                                table_cuckoo_2.clear();
                                table_cuckoo_2.resize(table_length);

                                flag = false;
                                for(std::size_t i=this->dynamic_index; i<(dynamic_table_cuckoo_1.size()*2); i++) {
                                    if(i < dynamic_table_cuckoo_1.size()) {
                                        if (dynamic_table_cuckoo_1[i].state == SlotState::OCCUPIED) {
                                            std::optional<std::size_t> payload = this->_insert(dynamic_table_cuckoo_1[i].key, dynamic_table_cuckoo_1[i].value);
                                            if(!payload.has_value()) {
                                                flag = true;
                                                break;
                                            }
                                        }
                                    } else {
                                        std::size_t index = i - this->dynamic_table_cuckoo_2.size();
                                        if (dynamic_table_cuckoo_2[index].state == SlotState::OCCUPIED) {
                                            std::optional<std::size_t> payload = this->_insert(dynamic_table_cuckoo_2[index].key, dynamic_table_cuckoo_2[index].value);
                                            if(!payload.has_value()) {
                                                flag = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }

                            std::swap(table_cuckoo_1_old, dynamic_table_cuckoo_1);
                            std::swap(table_cuckoo_2_old, dynamic_table_cuckoo_2);

                            this->dynamic_index = 0;
                        }
                        return true;
                    }
                }
            } else {
                table_length = this->nextPrime(table_length * 2);
                this->dynamic = true;
                switch (overflow) {
                    case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING :
                    case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
                        this->dynamic_table_open = std::move(table_open);
                        
                        table_open.clear();
                        table_open.resize(table_length);
                        
                        return true;
                    }
                    case OVERFLOW::CHAINING : {
                        this->dynamic_table_chaining = std::move(table_chaining);
                        
                        table_chaining.clear();
                        table_chaining.resize(table_length);

                        return true;
                    }
                    case OVERFLOW::CUCKOO : {
                        this->dynamic_table_cuckoo_1 = std::move(table_cuckoo_1);
                        this->dynamic_table_cuckoo_2 = std::move(table_cuckoo_2);

                        table_cuckoo_1.clear();
                        table_cuckoo_1.resize(table_length);
                        table_cuckoo_2.clear();
                        table_cuckoo_2.resize(table_length);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

template <typename Tkey, typename Tvalue>
void HashTable<Tkey, Tvalue>::dynamicCopy() {
    const int batch = 8;
    switch (overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING :
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
            for(int i=0; i<batch; i++) {
                if(this->dynamic_table_open.size() > this->dynamic_index) {
                    Slot item = this->dynamic_table_open[this->dynamic_index];
                    this->dynamic_index += 1;
                    if(item.state == SlotState::OCCUPIED) {
                        this->element_count--;
                        std::optional<std::size_t> res = this->_insert(item.key, item.value);
                        while(!res.has_value()) {
                            this->resizeTable();
                            res = this->_insert(item.key, item.value);
                        }
                    }
                } else {
                    this->dynamic = false;
                    this->dynamic_index = 0;
                    this->dynamic_table_open.clear();
                    break;
                }
            }
            break;
        }
        case OVERFLOW::CHAINING : {
            for(int i=0; i<batch; i++) {
                if(this->dynamic_table_chaining.size() > this->dynamic_index) {
                    std::vector<std::pair<Tkey, Tvalue>>& item = dynamic_table_chaining[this->dynamic_index];
                    if(item.size() != 0) {
                        std::pair<Tkey, Tvalue> element = item.back();
                        item.pop_back();
                        this->element_count--;
                        std::optional<std::size_t> res = this->_insert(element.first, element.second);
                        while(!res.has_value()) {
                            this->resizeTable();
                            res = this->_insert(element.first, element.second);
                        }
                    } else {
                        this->dynamic_index += 1;
                    }
                } else {
                    this->dynamic = false;
                    this->dynamic_index = 0;
                    this->dynamic_table_chaining.clear();
                    break;
                }
            }
            break;
        }
        case OVERFLOW::CUCKOO : {
            for(int i=0; i<batch; i++) {
                if(this->dynamic_table_cuckoo_1.size() > this->dynamic_index) {
                    Slot item = dynamic_table_cuckoo_1[this->dynamic_index];
                    this->dynamic_index += 1;
                    if(item.state == SlotState::OCCUPIED) {
                        this->element_count--;
                        std::optional<std::size_t> res = this->_insert(item.key, item.value);
                        while(!res.has_value()) {
                            this->resizeTable();
                            res = this->_insert(item.key, item.value);
                        }
                    }
                } else if((2 * this->dynamic_table_cuckoo_2.size()) > this->dynamic_index) {
                    Slot item = dynamic_table_cuckoo_2[this->dynamic_index - this->dynamic_table_cuckoo_2.size()];
                    this->dynamic_index += 1;
                    if(item.state == SlotState::OCCUPIED) {
                        this->element_count--;
                        std::optional<std::size_t> res = this->_insert(item.key, item.value);
                        while(!res.has_value()) {
                            this->resizeTable();
                            res = this->_insert(item.key, item.value);
                        }
                    }
                } else {
                    this->dynamic = false;
                    this->dynamic_index = 0;
                    this->dynamic_table_cuckoo_1.clear();
                    this->dynamic_table_cuckoo_2.clear();
                    break;
                }
            }
            break;
        }
    }
}

template <typename Tkey, typename Tvalue>
std::optional<std::size_t> HashTable<Tkey, Tvalue>::find(const Tkey& key) {
    std::size_t index = this->getHash(key);
    bool result = false;
    std::size_t index_final;
    switch (overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING : {
            for (std::size_t i = 0; i < table_length; ++i) {

                std::size_t probeIndex = (index + i) % table_length;

                if (table_open[probeIndex].state == SlotState::EMPTY) {
                    result = false;
                    break;
                }
                if(table_open[probeIndex].state == SlotState::OCCUPIED && table_open[probeIndex].key == key) {
                    result = true;
                    index_final = probeIndex;
                    break;
                }
            }

            if(!result && this->dynamic) {
                index = this->getHash(key, dynamic_table_open.size());
                for (std::size_t i = 0; i < dynamic_table_open.size(); ++i) {

                    std::size_t probeIndex = (index + i) % dynamic_table_open.size();

                    if (dynamic_table_open[probeIndex].state == SlotState::EMPTY) {
                        result = false;
                        break;
                    }
                    if(dynamic_table_open[probeIndex].state == SlotState::OCCUPIED && dynamic_table_open[probeIndex].key == key) {
                        result = true;
                        this->element_count -= 1;
                        std::optional<std::size_t> payload = _insert(dynamic_table_open[probeIndex].key, dynamic_table_open[probeIndex].value);
                        while(!payload.has_value()) {
                            this->resizeTable();
                            payload = this->_insert(dynamic_table_open[probeIndex].key, dynamic_table_open[probeIndex].value);
                        }
                        index_final = payload.value();
                        this->dynamicCopy();
                        break;
                    }
                }
            }
            break;
        }
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
            std::size_t second_hash = this->h3(key);
            for (std::size_t i = 0; i < table_length; ++i) {

                std::size_t probeIndex = (index + i * second_hash) % table_length;

                if (table_open[probeIndex].state == SlotState::EMPTY) {
                    result = false;
                    break;
                }
                if(table_open[probeIndex].state == SlotState::OCCUPIED && table_open[probeIndex].key == key) {
                    result = true;
                    index_final = probeIndex;
                    break;
                }
            }

            if(!result && this->dynamic) {
                index = this->getHash(key, dynamic_table_open.size());
                second_hash = this->h3(key, dynamic_table_open.size());
                for (std::size_t i = 0; i < dynamic_table_open.size(); ++i) {

                    std::size_t probeIndex = (index + i * second_hash) % dynamic_table_open.size();

                    if (dynamic_table_open[probeIndex].state == SlotState::EMPTY) {
                        result = false;
                        break;
                    }
                    if(dynamic_table_open[probeIndex].state == SlotState::OCCUPIED && dynamic_table_open[probeIndex].key == key) {
                        result = true;
                        this->element_count -= 1;
                        std::optional<std::size_t> payload = _insert(dynamic_table_open[probeIndex].key, dynamic_table_open[probeIndex].value);
                        while(!payload.has_value()) {
                            this->resizeTable();
                            payload = this->_insert(dynamic_table_open[probeIndex].key, dynamic_table_open[probeIndex].value);
                        }
                        index_final = payload.value();
                        this->dynamicCopy();
                        break;
                    }
                }
            }
            break;
        }
        case OVERFLOW::CHAINING : {
            for(std::size_t i=0; i<this->table_chaining[index].size(); i++) {
                if(table_chaining[index][i].first == key) {
                    result = true;
                    index_final = i;
                    break;
                }
            }
            if(!result && this->dynamic) {
                index = this->getHash(key, dynamic_table_chaining.size());
                for(std::size_t i=0; i<this->dynamic_table_chaining[index].size(); i++) {
                    if(dynamic_table_chaining[index][i].first == key) {
                        result = true;
                        this->element_count -= 1;
                        std::optional<std::size_t> payload = _insert(dynamic_table_chaining[index][i].first, dynamic_table_chaining[index][i].second);

                        while(!payload.has_value()) {
                            this->resizeTable();
                            payload = this->_insert(dynamic_table_chaining[index][i].first, dynamic_table_chaining[index][i].second);
                        }
                        index_final = payload.value();
                        
                        this->dynamicCopy();
                        break;
                    }
                }
            }
            break;
        }
        case OVERFLOW::CUCKOO : {
            std::size_t second_index = 0;
            if(this->hash == HASH::MODE_1) {
                second_index = this->h2(key);
            } else {
                second_index = this->h1(key);
            }

            if(table_cuckoo_1[index].state == SlotState::OCCUPIED && table_cuckoo_1[index].key == key) {
                result = true;
                index_final = index;
            } else if(table_cuckoo_2[second_index].state == SlotState::OCCUPIED && table_cuckoo_2[second_index].key == key) {
                result = true;
                index_final = second_index;
            }

            if(!result && this->dynamic) {
                index = this->getHash(key, dynamic_table_cuckoo_1.size());
                if(this->hash == HASH::MODE_1) {
                    second_index = this->h2(key, dynamic_table_cuckoo_1.size());
                } else {
                    second_index = this->h1(key, dynamic_table_cuckoo_1.size());
                }
                if(dynamic_table_cuckoo_1[index].state == SlotState::OCCUPIED && dynamic_table_cuckoo_1[index].key == key) {
                    result = true;
                    this->element_count -= 1;
                    std::optional<std::size_t> payload = _insert(dynamic_table_cuckoo_1[index].key, dynamic_table_cuckoo_1[index].value);
                    while(!payload.has_value()) {
                        this->resizeTable();
                        payload = this->_insert(dynamic_table_cuckoo_1[index].key, dynamic_table_cuckoo_1[index].value);
                    }
                    index_final = payload.value();
                } else if(dynamic_table_cuckoo_2[second_index].state == SlotState::OCCUPIED && dynamic_table_cuckoo_2[second_index].key == key) {
                    result = true;
                    this->element_count -= 1;
                    std::optional<std::size_t> payload = _insert(dynamic_table_cuckoo_2[second_index].key, dynamic_table_cuckoo_2[second_index].value);
                    while(!payload.has_value()) {
                        this->resizeTable();
                        payload = this->_insert(dynamic_table_cuckoo_2[second_index].key, dynamic_table_cuckoo_2[second_index].value);
                    }
                    index_final = payload.value();
                }
                //this->dynamicCopy();
            }
            break;
        }
    }

    if(result) {
        return index_final;
    }
    return std::nullopt;
}

template <typename Tkey, typename Tvalue>
std::optional<std::size_t> HashTable<Tkey, Tvalue>::_insert(const Tkey& key, const Tvalue& value) {
    std::size_t index = this->getHash(key);
    std::optional<std::size_t> final_index = std::nullopt;
    switch (overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING : {
            // Exceding the load factor
            if(static_cast<double>(element_count) / table_length >= max_load_factor) {
                resizeTable();
                index = this->getHash(key);
            }

            for (std::size_t i = 0; i < table_length; ++i) {

                std::size_t probeIndex = (index + i) % table_length;

                // Slot vuoto → inserisci
                if (table_open[probeIndex].state != SlotState::OCCUPIED) {
                    table_open[probeIndex].key = key;
                    table_open[probeIndex].value = value;
                    table_open[probeIndex].state = SlotState::OCCUPIED;
                    element_count += 1;
                    final_index = probeIndex;
                    break;
                }

                // Chiave già esistente → aggiorna
                if (table_open[probeIndex].state == SlotState::OCCUPIED && table_open[probeIndex].key == key) {
                    table_open[probeIndex].value = value;
                    final_index = probeIndex;
                    break;
                }
            }
            break;
        }
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING : {
            // Exceding the load factor
            if(static_cast<double>(element_count) / table_length >= max_load_factor) {
                resizeTable();
                index = this->getHash(key);
            }

            std::size_t second_hash = this->h3(key);
            for (std::size_t i = 0; i < table_length; ++i) {

                std::size_t probeIndex = (index + i * second_hash) % table_length;

                // Slot vuoto → inserisci
                if (table_open[probeIndex].state != SlotState::OCCUPIED) {
                    table_open[probeIndex].key = key;
                    table_open[probeIndex].value = value;
                    table_open[probeIndex].state = SlotState::OCCUPIED;
                    element_count += 1;
                    final_index = probeIndex;
                    break;
                }

                // Chiave già esistente → aggiorna
                if (table_open[probeIndex].state == SlotState::OCCUPIED && table_open[probeIndex].key == key) {
                    table_open[probeIndex].value = value;
                    final_index = probeIndex;
                    break;
                }
            }
            break;
        }
        case OVERFLOW::CHAINING : {
            // Exceding the load factor
            if(static_cast<double>(element_count) / table_length >= max_load_factor) {
                resizeTable();
                index = this->getHash(key);
            }

            for (std::pair<Tkey, Tvalue>& kv : table_chaining[index]) {
                if (kv.first == key) {
                    kv.second = value;
                    return index;
                }
            }

            table_chaining[index].emplace_back(key, value);
            element_count += 1;
            final_index = table_chaining[index].size() - 1;

            break;
        }
        case OVERFLOW::CUCKOO : {
            // Controllo load factor
            if(static_cast<double>(element_count) / table_length >= max_load_factor) {
                resizeTable();
                index = this->getHash(key);
            }

            Slot new_slot{SlotState::OCCUPIED, key, value};
            std::size_t index_to_insert = index; // parte dalla prima tabella
            bool in_first_table = true;

            // Lista per tracciare i cambiamenti da annullare
            std::vector<std::pair<Slot*, Slot>> changes;
            bool flag = false;
            for(int i = 0; i < 500; ++i) {
                Slot& current_slot = in_first_table ? table_cuckoo_1[index_to_insert] : table_cuckoo_2[index_to_insert];

                // Memorizza lo stato precedente del current_slot prima di modificarlo
                changes.push_back({&current_slot, current_slot});

                // Vuoto
                if(current_slot.state != SlotState::OCCUPIED) {
                    current_slot = new_slot;
                    element_count += 1;
                    flag = true;
                    break;
                }

                // Chiave già esistente → aggiorna
                if(current_slot.key == new_slot.key) {
                    current_slot.value = value;
                    flag = true;
                    break;
                }

                // Eviction: scambia l'elemento corrente con il nuovo
                std::swap(new_slot, current_slot);

                // Passa all’altra tabella
                in_first_table = !in_first_table;
                if (in_first_table) {
                    if(this->hash == HASH::MODE_1) {
                        index_to_insert = this->h1(new_slot.key);
                    } else {
                        index_to_insert = this->h2(new_slot.key);
                    }
                } else {
                    if(this->hash == HASH::MODE_1) {
                        index_to_insert = this->h2(new_slot.key);
                    } else {
                        index_to_insert = this->h1(new_slot.key);
                    }
                }
            }
            if(flag) {
                final_index = index;
            }

            // Se non è stato possibile inserire, esegui il rollback
            if (!final_index.has_value()) {
                // Rollback dei cambiamenti
                for (int i = changes.size() - 1; i >= 0; --i) {
                    *changes[i].first = changes[i].second;
                }
            }

            break;
        }
    }
    
    return final_index;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::insert(const Tkey& key, const Tvalue& value) {
    std::optional<std::size_t> res = this->_insert(key, value);
    while(!res.has_value()) {
        this->resizeTable();
        res = this->_insert(key, value);
    }
    // Manage dynamic copy
    if(this->dynamic) {
        this->dynamicCopy();
    }
    return true;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::exist(const Tkey& key) {
    return this->find(key) != std::nullopt;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::update(const Tkey& key, const Tvalue& value) {
    std::size_t hash_index = this->getHash(key);
    std::optional<std::size_t> index = this->find(key);
    switch(overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING:
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING: {
            if(index != std::nullopt) {
                table_open[*index].value = value;
                return true;
            }
            return false;
        }
        case OVERFLOW::CHAINING: {
            if(index != std::nullopt) {
                table_chaining[hash_index][*index].second = value;
                return true;
            }
            return false;
        }
        case OVERFLOW::CUCKOO: {
            if(index != std::nullopt) {
                if(table_cuckoo_1[*index].key == key) {
                    table_cuckoo_1[*index].value = value;
                } else if(table_cuckoo_2[*index].key == key) {
                    table_cuckoo_2[*index].value = value;
                }
                return true;
            }
            return false;
        }   
    }
    return false;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey, Tvalue>::remove(const Tkey& key) {
    std::size_t hash_index = this->getHash(key);
    std::optional<std::size_t> index = this->find(key);
    switch(overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING:
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING: {
            if(index != std::nullopt) {
                table_open[*index].state = SlotState::DELETED;
                element_count -= 1;
                return true;
            }
            return false;
        }
        case OVERFLOW::CHAINING: {
            if(index != std::nullopt) {
                table_chaining[hash_index].erase(table_chaining[hash_index].begin() + *index);
                element_count -= 1;
                return true;
            }
            return false;
        }
        case OVERFLOW::CUCKOO: {
            if(index != std::nullopt) {
                if(table_cuckoo_1[*index].key == key) {
                    table_cuckoo_1[*index].state = SlotState::DELETED;
                } else if(table_cuckoo_2[*index].key == key) {
                    table_cuckoo_2[*index].state = SlotState::DELETED;
                }
                element_count -= 1;
                return true;
            }
            return false;
        }
    }
    return false;
}

template <typename Tkey, typename Tvalue>
std::optional<Tvalue> HashTable<Tkey, Tvalue>::get(const Tkey& key) {
    std::size_t hash_index = this->getHash(key);
    std::optional<std::size_t> index = this->find(key);
    switch(overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING:
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING: {
            if(index != std::nullopt) {
                return table_open[*index].value;
            }
            return std::nullopt;
        }
        case OVERFLOW::CHAINING: {
            if(index != std::nullopt) {
                return table_chaining[hash_index][*index].second;
            }
            return std::nullopt;
        }
        case OVERFLOW::CUCKOO: {
            if(index.has_value()) {
                if(table_cuckoo_1[*index].key == key) {
                    return table_cuckoo_1[*index].value;
                } else if(table_cuckoo_2[*index].key == key) {
                    return table_cuckoo_2[*index].value;
                }
            }
        }
    }
    return std::nullopt;
}

template <typename Tkey, typename Tvalue>
std::size_t HashTable<Tkey, Tvalue>::memoryUsage() {
    std::size_t mem = 0;

    switch(overflow) {

        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING:
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING:
            mem = (table_open.size() + dynamic_table_open.size()) * sizeof(table_open[0]);
            break;

        case OVERFLOW::CHAINING:
            mem = (table_chaining.size() + dynamic_table_chaining.size()) * sizeof(table_chaining[0]);
            break;

        case OVERFLOW::CUCKOO:
            mem =
                table_cuckoo_1.size() * sizeof(table_cuckoo_1[0]) +
                table_cuckoo_2.size() * sizeof(table_cuckoo_2[0]) +
                dynamic_table_cuckoo_1.size() * sizeof(dynamic_table_cuckoo_1[0]) +
                dynamic_table_cuckoo_2.size() * sizeof(dynamic_table_cuckoo_2[0]);
            break;
    }

    if(mem > this->max_memory_usage) {
        this->max_memory_usage = mem;
    }

    return mem;
}

template <typename Tkey, typename Tvalue>
double HashTable<Tkey, Tvalue>::getKeyDistribution() const {
    switch(overflow) {
        case OVERFLOW::OPEN_ADDRESSING_LINEAR_PROBING:
        case OVERFLOW::OPEN_ADDRESSING_DOUBLE_HASHING: {
            std::size_t current_cluster = 0;
            std::size_t sum_cluster = 0;
            std::size_t cluster_count = 0;

            for (auto &slot : table_open) {
                if (slot.state != SlotState::EMPTY) {
                    current_cluster++;
                } else if (current_cluster > 0) {
                    sum_cluster += current_cluster;
                    cluster_count++;
                    current_cluster = 0;
                }
            }

            if (current_cluster > 0) {
                sum_cluster += current_cluster;
                cluster_count++;
            }

            return cluster_count > 0 ? (double)sum_cluster / cluster_count : 0;
        }
        case OVERFLOW::CHAINING: {
            double mean = this->element_count / this->table_chaining.size();
            double variance = 0;

            for (std::vector<std::pair<Tkey, Tvalue>> c : this->table_chaining) {
                variance += (c.size() - mean) * (c.size() - mean);
            }

            variance /= this->table_chaining.size();
            return sqrt(variance);
        }
        case OVERFLOW::CUCKOO: {
            std::size_t current_cluster = 0;
            std::size_t sum_cluster = 0;
            std::size_t cluster_count = 0;

            for (auto &slot : table_cuckoo_1) {
                if (slot.state != SlotState::EMPTY) {
                    current_cluster++;
                } else if (current_cluster > 0) {
                    sum_cluster += current_cluster;
                    cluster_count++;
                    current_cluster = 0;
                }
            }
            for (auto &slot : table_cuckoo_2) {
                if (slot.state != SlotState::EMPTY) {
                    current_cluster++;
                } else if (current_cluster > 0) {
                    sum_cluster += current_cluster;
                    cluster_count++;
                    current_cluster = 0;
                }
            }

            if (current_cluster > 0) {
                sum_cluster += current_cluster;
                cluster_count++;
            }

            return cluster_count > 0 ? (double)sum_cluster / cluster_count : 0;
        }
    }
    return 0;
}