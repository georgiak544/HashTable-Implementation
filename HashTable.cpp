#include "HashTable.hpp"


#include <fstream>
#include <cstring>
#include <stdlib.h>



string HashTable:: print() const{
    string str;
    char buf[128];

    for(int i = 0; i < capacity; i++){
        if(!isAvailable(i)){
            sprintf(buf, "%2d. -%s-\n", i, (*table[i]).c_str());
            str.append(buf);
        }
    }
    sprintf(buf, " --- CAPACITY: %d, SIZE: %d ---\n", capacity, size);
    str.append(buf);
    return str;
}

unsigned long HashTable::getHashCode(const char *str){
    unsigned long hash = 97;
    int c;

    while((c = *(str++)) != '\0')
        hash = ((hash << 5) + hash) + c;

    return hash;
    
}

HashTable::HashTable(int capacity): size(0), capacity(capacity){
    if (capacity < 0) 
        throw std::bad_alloc();

    table = new (nothrow) string*[capacity];

    if(table == nullptr)
        throw std::bad_alloc();

    for(int i = 0; i < capacity; i++){
        table[i] = nullptr;
    }

}
HashTable::~HashTable() {
    if(size == 0){
        delete[] table;
        return;
    }
    if (table) {
        for (int i = 0; i < capacity; ++i) {
            if (!isAvailable(i)) {
                delete table[i];  // Free each string in the table
                table[i] = nullptr;
            }
        }
        delete[] table;  // Free the array of pointers
        table = nullptr;
    }
}
HashTable::HashTable(const HashTable &ht) : size(ht.size), capacity(ht.capacity) {
    if(ht.table == nullptr) {
        table = nullptr;
    } else {
        table = new std::string*[capacity];
        for (int i = 0; i < capacity; ++i) {
            if (ht.table[i] != nullptr) {
                if(ht.table[i] == TOMB)
                    table[i] = TOMB;  // Copy tombstones correctly
                else
                    table[i] = new std::string(*ht.table[i]);  // Deep copy the string
            } else {
                table[i] = nullptr;
            }
        }
    }
}

int HashTable::getSize() const{
    return size;
}

int HashTable::getCapacity() const{
    return capacity;
}

bool HashTable::isEmpty(int pos) const{
    return pos < capacity && table[pos] == nullptr;
}

bool HashTable::isTomb(int index) const {
    // Ensure index is within bounds
    if (index < 0 || index >= capacity) {
        return false; // or handle this error as needed
    }

    // Safely handle the case where the table or table[index] is null
    if (table == nullptr || table[index] == nullptr) {
        return false;
    }

    // Check if the entry at the index is a tombstone
    return table[index] == TOMB;
}

bool HashTable::isAvailable(int index) const {
    // Ensure index is within bounds
    if (index < 0 || index >= capacity) {
        return false; // or handle this error as needed
    }

    // Safely handle the case where the table is null or the entry is null
    if (table == nullptr || table[index] == nullptr) {
        return true;  // Available if there's no entry at this index
    }

    // Check if the entry is a tombstone
    return isTomb(index);
}

bool HashTable::contains(const char *s) const{
    if(strcmp(s,"") == 0){
        return false;
    }
    int pos = 0;
    unsigned long hash = getHashCode(s);
    for (int n = 0; n < capacity && pos < capacity; ++n) {
        pos = (hash + n) % capacity;
        if (isEmpty(pos)) {
            return false;
        }
        if (table[pos] != nullptr && table[pos] != TOMB && *table[pos] == s) {
            return true;
        }
    }
    return false;
}

bool HashTable::contains(const string &s) const{

    return contains(s.c_str());
}


bool HashTable::add(const char *s){

    if (contains(s)) {
        return false;
    }
    if(strcmp(s,"") == 0){
        return false;
    }
    unsigned long hash = getHashCode(s);
    for (int n = 0; n < capacity; ++n) {
        int pos = (hash + n) % capacity;
        if (isAvailable(pos)) {
            if (!isTomb(pos)) {
                delete table[pos];
            }
            table[pos] = new string(s);
            ++size;
            return true;
        }
    }
    throw HashTableException();
    
}

bool HashTable::add(const string &s){

    return add(s.c_str());
}
bool HashTable::remove(const char *s){
    
    if(!contains(s)){
        return false;
    }
    unsigned long hash = getHashCode(s);

    for (int n = 0; n < capacity; ++n) {
        int pos = (hash + n) % capacity;
        if (!(isAvailable(pos)) && *table[pos] == s) {
            delete table[pos];
            table[pos] = TOMB;
            size--;
            return true;
        }
    }
    return false;
}

bool HashTable::remove(const string &s){
    return remove(s.c_str());
}

HashTable& HashTable::operator = (const HashTable &ht){
    if(table != NULL)
        this->~HashTable();

    capacity = ht.capacity;
    size = ht.size;
    table = new (nothrow) string*[capacity];

    for(int i = 0; i < capacity; i++){
        if (ht.table[i] != nullptr) {
            if(ht.isTomb(i))
                table[i] = TOMB;
            else
                table[i] = new string(*ht.table[i]);
        } 
        else {
            table[i] = nullptr;
        }
    }   
   return *this;     
    
}
HashTable& HashTable::operator += (const string& str){
    add(str);
    return *this;
}

HashTable& HashTable::operator += (const char* s){
    add(s);
    return *this;
}

HashTable& HashTable::operator -= (const string& str){
    remove(str);
    return *this;
}
HashTable& HashTable::operator -= (const char* s){
    remove(s);
    return *this;
}
HashTable HashTable::operator + (const string& str) const{
    HashTable ht(*this);
    ht.add(str);

    return ht;
}    

HashTable HashTable::operator + (const char* s) const{
    HashTable ht(*this);
    ht.add(s);

    return ht;
}

HashTable HashTable::operator - (const string& str) const{
    HashTable ht(*this);
    ht.remove(str);

    return ht;
}

HashTable HashTable::operator - (const char* s) const{
    HashTable ht(*this);
    ht.remove(s);

    return ht;
}

std::ostream& operator<<(std::ostream &stream, const HashTable &t) {
    return stream << t.print();
}

/*HashTable::Iterator:: Iterator(const HashTable *t) : ht(t), position(0){
    curr = t->table;
    while (position < t->capacity && t->isAvailable(position)) {
        ++position;
        ++curr;
    }
} 

HashTable::Iterator::Iterator(const Iterator &it) : curr(it.curr), ht(it.ht), position(it.position){}

HashTable::Iterator HashTable::begin() const{
    return Iterator(this);
        
}

HashTable::Iterator HashTable::end() const{
    return Iterator(this + capacity);
}*/


