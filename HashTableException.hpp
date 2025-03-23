 #ifndef __H_TABLE_EXCEPTION_H__
 #define __H_TABLE_EXCEPTION_H__

class HashTableException  {
 public:
    virtual const char* what() const noexcept {
        return "----- HashTableException-----\n";
    }  
 };
 #endif