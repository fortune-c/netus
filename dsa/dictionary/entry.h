//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_ENTRY_H
#define WEB_SERVER_ENTRY_H
// MARK: DATA TYPES

// Entries are the fundamental unit of a dictionary, containing a value to be stored and a key to identify it.
struct Entry
{
    void *key;
    void *value;
};

// MARK: CONSTRUCTORS
// The constructor for an entry requires the key, value, and sizes of each, allowing any data type to be stored.
struct Entry entry_constructor(void *key, unsigned long key_size, void *value, unsigned long value_size);
void entry_destructor(struct Entry *entry);

#endif //WEB_SERVER_ENTRY_H