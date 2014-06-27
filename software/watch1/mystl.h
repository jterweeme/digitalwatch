#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stddef.h>

namespace Utility
{
    int sprintf(char *str, const char *format, ...);
    static void strcpy(char *d, const char *s) { while ((*d++ = *s++)); }
};

namespace mstd
{
class string
{
    char p_str[255];
    char *p_pstr;
public:
    string() { }
    string(const char *s) { Utility::strcpy(p_str, s); }
    const char *c_str() { return p_str; }
};

template <class T> class vector
{
protected:
    size_t capacity;
    size_t size;
    T *buffer;
public:
    typedef T *iterator;
    size_t getSize() const { return size; }
    vector(size_t capacity) : capacity(capacity), size(0) { buffer = new T[capacity]; }
    void push_back(const T &value) { buffer[size++] = value; }
    iterator begin() { return buffer; }
    iterator end() { return buffer + getSize(); }
    T & operator [] (unsigned index) { return buffer[index]; }
};

};
#endif


