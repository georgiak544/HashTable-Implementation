#include "HashTable.hpp"
#include <fstream>





HashTable::Iterator::Iterator(const HashTable *t) : ht(t), position(0) {
    curr = ht->table;
    while (position < ht->capacity && ht->isAvailable(position)) {
        position++;
        curr++;
    }
}

HashTable::Iterator::Iterator(const Iterator &it) : curr(it.curr), ht(it.ht), position(it.position) {}

HashTable::Iterator& HashTable::Iterator::operator=(const Iterator &it) {
    if (this == &it) {
        return *this;
    }
    curr = it.curr;
    ht = it.ht;
    position = it.position;
    return *this;
}

HashTable::Iterator HashTable::Iterator::operator++() {
    do {
        ++curr;
        ++position;
    } while (position < ht->capacity && ht->isAvailable(position));
    return *this;
}

HashTable::Iterator HashTable::Iterator::operator++(int a) {
    Iterator it(*this);
    ++(*this);
    return it;
}

bool HashTable::Iterator::operator==(const Iterator &it) const {
    return ht == it.ht && curr == it.curr;
}

bool HashTable::Iterator::operator!=(const Iterator &it) const {
    return curr != it.curr;
}

const string& HashTable::Iterator::operator*() {
    return **curr;
}

const string* HashTable::Iterator::operator->() {
    return *curr;
}

int HashTable::Iterator::pos() const {
    return position;
}

HashTable::Iterator HashTable::begin() const {
    return Iterator(this);
}

HashTable::Iterator HashTable::end() const {
    Iterator it(this);
    it.setPosition(capacity) ;
    return it;
}

void HashTable::Iterator::setPosition(int pos) {
    position = pos;
    curr = &ht->table[pos];
}