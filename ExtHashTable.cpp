
#include <memory>
#include <fstream>
#include "ExtHashTable.hpp"


ExtHashTable::ExtHashTable(const ExtHashTable &t)
    : HashTable(t), upper_bound_ratio(t.upper_bound_ratio), lower_bound_ratio(t.lower_bound_ratio) {}

// Constructor with custom parameters
ExtHashTable::ExtHashTable(double upper_bound_ratio, double lower_bound_ratio, int capacity)
    : HashTable(capacity), upper_bound_ratio(upper_bound_ratio), lower_bound_ratio(lower_bound_ratio) {}


// Rehash function to resize the hash table
void ExtHashTable::rehash() {
    if(size == 0){
        return;
    }
     double load_factor = static_cast<double>(size) / capacity;

    if (load_factor >= upper_bound_ratio) {
        int old_capacity = capacity;
        capacity *= 2;
        std::cout << "--> Size: " << size << ", New capacity: " << capacity << std::endl;
        rehash(old_capacity);
    } else if (load_factor <= lower_bound_ratio && capacity > 2) {
        int old_capacity = capacity;
        capacity /= 2;
        std::cout << "--> Size: " << size << ", New capacity: " << capacity << std::endl;
        rehash(old_capacity);
    }
}
// Add a string to the hash table
bool ExtHashTable::add(const char *s) {
    bool added = HashTable::add(s);
    if (added) {
        rehash();
    }
    return added;
}

// Add a string to the hash table (overloaded for std::string)
bool ExtHashTable::add(const string &s) {
    return ExtHashTable::add(s.c_str());
}

// Remove a string from the hash table
bool ExtHashTable::remove(const char *s) {
    bool removed = HashTable::remove(s);
    if (removed) {
        rehash();
    }
    return removed;
}
void ExtHashTable::rehash(int old_capacity) {
    string **new_table = new string*[capacity]();
    for (int i = 0; i < old_capacity; ++i) {
        if (table[i] && !isTomb(i)) {
            unsigned long hash = getHashCode(table[i]->c_str()) % capacity;
            while (new_table[hash]) {
                hash = (hash + 1) % capacity;
            }
            new_table[hash] = table[i];
            table[i] = nullptr;  // Prevent double deletion by setting to nullptr
        }
    }
    delete[] table;  // Free the old table
    table = new_table;
}


// Remove a string from the hash table (overloaded for std::string)
bool ExtHashTable::remove(const string &s) {
    return ExtHashTable::remove(s.c_str());
}

// Operator overloads
bool ExtHashTable::operator+=(const string &str) {
    return ExtHashTable::add(str);
}

bool ExtHashTable::operator+=(const char* s) {
    return ExtHashTable::add(s);
}

bool ExtHashTable::operator-=(const string &str) {
     unsigned long hash = getHashCode(str.c_str()) % capacity;

    for (int i = 0; i < capacity; ++i) {
        int pos = (hash + i) % capacity;

        if (table[pos] == nullptr) {
            // If we hit a null entry, the string is not in the table.
            return false;
        }

        if (table[pos] != TOMB && *table[pos] == str) {
            // We found the string, so we delete it and mark this slot with a tombstone.
            delete table[pos];
            table[pos] = TOMB;
            --size;

           return true;
        }
    }

    return false;
}

bool ExtHashTable::operator-=(const char *s) {
    return ExtHashTable::operator-=(s);
}

// Operator + to merge two ExtHashTable objects
ExtHashTable ExtHashTable::operator+(const ExtHashTable &t) const {
    ExtHashTable new_table(*this);
    for (int i = 0; i < t.capacity; ++i) {
        if (t.table[i] && !t.isTomb(i)) {
            new_table.add(*t.table[i]);
        }
    }
    return new_table;
}

// Operator += to add another ExtHashTable to this one
ExtHashTable& ExtHashTable::operator+=(const ExtHashTable &t) {
    for (int i = 0; i < t.capacity; ++i) {
        if (t.table[i] && !t.isTomb(i)) {
            add(*t.table[i]);
        }
    }
    return *this;
}

// Assignment operator
ExtHashTable& ExtHashTable::operator=(const ExtHashTable &t) {
    if (this != &t) {
        HashTable::operator=(t);
        upper_bound_ratio = t.upper_bound_ratio;
        lower_bound_ratio = t.lower_bound_ratio;
    }
    return *this;
}