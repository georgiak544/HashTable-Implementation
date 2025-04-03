# 🧩 Dynamic Hash Table with Open Addressing (C++)

A C++ implementation of a dynamic hash table using open addressing with tombstones. Includes both a base hash table and an extended version that automatically resizes based on load factor. Also features full iterator support and operator overloading for intuitive usage.

---

## ✨ Features

- 🧠 **Open Addressing**: Collision handling with linear probing
- ⚰️ **Tombstones**: Safe deletion without breaking the probing chain
- ➕ **Dynamic Resizing**: `ExtHashTable` automatically resizes when thresholds are met
- ♻️ **Deep Copy & Assignment**: Proper memory management with copy constructor & operator=
- 🧮 **Iterator Support**: `begin()`, `end()`, `++`, `*`, `->` for STL-style iteration
- ➕/➖ **Operator Overloading**: `+=`, `-=`, `+`, `-` for intuitive hash table operations
- 📄 **Formatted Output**: `operator<<` prints the entire hash table with metadata
- 🧪 **Robustness**: Handles edge cases like empty strings, tombstones, rehashing

---

## 🔧 Classes & Responsibilities

- `HashTable`:  
  - Core logic: hashing, insertion, deletion, lookup  
  - Memory-safe deep copy  
  - Supports both `const char*` and `std::string`  
  - Operator overloads for manipulation  

- `ExtHashTable`:  
  - Inherits from `HashTable`  
  - Automatically resizes when load factor exceeds thresholds  
  - Merges tables via `+`, `+=`

- `HashTable::Iterator`:  
  - Enables C++-style iteration over valid elements  
  - Skips tombstones and null slots

- `HashTableException`:  
  - Custom exception thrown when table is full and insertion fails

---

## 🧠 Concepts Used

- Object-Oriented Design (Inheritance, Encapsulation)
- Dynamic Memory Management
- Hash Functions & Collision Resolution
- Iterators and Operator Overloading
- Deep Copy Semantics
- Exception Handling

---
